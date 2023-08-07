#include"sat_sudoku.h"
//销毁二维链表
int Destory2DLinklist(Linklist *&L)
{
    if (L == NULL)
        return ERROR;
    Linklist *linkp1 = L, *linkp2;
    Node *p1, *p2;
    while (linkp1 != NULL)
    {
        p1 = linkp1->head;
        while (p1 != NULL) // p1为后指针,p2为前指针
        {
            p2 = p1->next;
            free(p1);
            p1 = p2;
        }
        linkp2 = linkp1->next; // linkp2为前指针,linkp1为后指针
        free(linkp1);          //释放后指针表头结点
        linkp1 = linkp2;
    }
    L = NULL;
    return OK;
}

//在整个表中删除p0为表头所在的一条链
int Removelist(Linklist *&L, Linklist *&p0)
{
    Linklist *p;
    Node *p1, *p2;
    if (L == p0) //正好是第一个表头结点
    {
        L = L->next;
        //销毁p所在链
    }
    else
    {
        p = L;
        while (p != NULL && p->next != p0)
            p = p->next; //找p0,使得p下一个为p0
        if (p == NULL)
            return ERROR; //找不到p0
        if (p0->next != NULL)
            p->next = p0->next; // p0不是末尾,将p连上p0的后续
        else
            p->next = NULL; // p0是末尾
    }
    //销毁p所在链
    p1 = p0->head;
    while (p1 != NULL)
    {
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
    free(p0);
    p0 = NULL;
    return OK;
}

//在sentence子句所在链中删除p0结点
int RemoveNode(Linklist *&sentence, Node *&p0)
{
    Node *p1;
    p1 = sentence->head;
    if (p1 == p0)
        sentence->head = sentence->head->next;
    else
    {
        while (p1 != NULL && p1->next != p0)
            p1 = p1->next;
        if (p1 == NULL)
            return ERROR;
        p1->next = p0->next;
    }
    free(p0);
    p0 = NULL;
    sentence->cnt--;
    return OK;
}

//在整个表中头插法加入表头结点p0,整个表的头变为p0,p0空间需要提前开辟
int Addlist(Linklist *&L, Linklist *&p0)
{
    if (p0 == NULL)
        return ERROR;
    p0->next = L;
    L = p0;
    return OK;
}

//判断是否为单子句
bool issinglesentence(Linklist *p0)
{
    if (p0->head != NULL && p0->head->next == NULL)
        return true;
    else
        return false;
}

//在整个表中删除含有单子句p0的句元结点,此操作将p0也删除
int Removesinglesentence(Linklist *&L, Linklist *p0)
{
    int num1 = p0->head->data;
    Linklist *p1, *pp;
    Node *p2;
    p1 = L;
    while (p1 != NULL)
    {
        pp = p1->next; // p1可能被删除,需要临时变量pp暂存
        p2 = p1->head;
        while (p2 != NULL)
        {
            if (p2->data == num1) //真值始终与单子句p0相同,可以直接移除整条链
            {
                Removelist(L, p1);
                break;
            }
            else if (p2->data == -num1) //真值与单子句p0相反,最后的判定中必然对结果没有意义,删除结点
            {
                RemoveNode(p1, p2);
                break;
            }
            p2 = p2->next;
        }
        p1 = pp;
    }
    return OK;
}

//判断是否有空的子句(链)
bool isemptysentence(Linklist *L)
{
    Linklist *p = L;
    while (p != NULL)
    {
        if (p->head == NULL)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

//选元策略:选择第一个出现的数
int Getnextelem_firsthead(Linklist *L)
{
    return L->head->data;
}

//选元策略:优先选择出现次数最多
int Getnextelem_mostfrequently(Linklist *L)
{
    //创建词频表,正变元1...boolnum,负变元boolnum+1...2*boolnum
    int* fre_map = (int*)malloc(sizeof(int) * (2 * boolnum + 1));
    //int fre_map[2 * boolnum + 1];
    for (int i = 0; i < 2 * boolnum + 1; i++)
        fre_map[i] = 0; //初始化为0
    Linklist *p1 = L;
    Node *p2;
    int mostfre;
    int maxtimes = 0;
    while (p1 != NULL)
    {
        p2 = p1->head;
        while (p2 != NULL)
        {
            if (p2->data > 0) //正变元
                fre_map[p2->data]++;
            else //负变元索引放在boolnum之后
                fre_map[boolnum - (p2->data)]++;
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    for (int i = 1; i < 2 * boolnum + 1; i++)
    {
        if (fre_map[i] > maxtimes)
        {
            maxtimes = fre_map[i];
            mostfre = (i <= boolnum ? i : i - boolnum);
        }
    }
    return mostfre;
}

int Getnextelem_JW(Linklist *L) //选元策略:JW,给每一个文字赋上权重2^(-n),正负合并考虑
{
    double* w = (double*)malloc(sizeof(double) * (2 * boolnum + 1));
    double mw = 0;
    //double w[2 * boolnum + 1], mw = 0;
    int temp, bestchoice;
    for (int i = 0; i < 2 * boolnum + 1; i++)
        w[i] = 0;
    Linklist *p1 = L;
    Node *p2;
    while (p1 != NULL)
    {
        p2 = p1->head;
        while (p2 != NULL)
        {
            temp = p2->data > 0 ? p2->data : boolnum - p2->data;
            w[temp] += pow((double)2, -(double)(p1->cnt));
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    for (int i = 1; i < boolnum + 1; i++)
    {
        if (w[i] + w[boolnum + i] > mw)
        {
            mw = w[i] + w[boolnum + i];
            bestchoice = i;
        }
    }
    return w[bestchoice] >= w[boolnum + bestchoice] ? bestchoice : -bestchoice;
}

//解Cnf文件并保存到数组s中
bool DPLL(Linklist *&L, bool *s, int mode)
{
    Linklist *p;
    while (true)
    {
        p = L;
        while (p && !issinglesentence(p))
            p = p->next;
        if (p != NULL) //还有单子句
        {
            //单子句直接判断真值
            if (p->head->data > 0)
                s[p->head->data] = true;
            else
                s[-p->head->data] = false;
            Removesinglesentence(L, p);
            if (L == NULL) //如果化简后为空
                return true;
            else if (isemptysentence(L)) //如果有空子句
                return false;
        }
        else
            break;
    }
    //选元
    int elem;
    if (mode == 1)
        elem = Getnextelem_firsthead(L);
    else if (mode == 2)
        elem = Getnextelem_mostfrequently(L);
    else if (mode == 3)
        elem = Getnextelem_JW(L);
    else 
        return false;
    //为了递归能回到初态需要先拷贝一份,否则原表被改变
    Linklist *Copy_List;
    Linklist *newlist = (Linklist *)malloc(sizeof(Linklist));
    newlist->head = (Node *)malloc(sizeof(Linklist));
    newlist->head->data = elem;
    newlist->head->next = NULL;
    newlist->cnt = 1;
    newlist->next = NULL;
    //加入新的变元
    Copy2DLinklist(L, Copy_List);
    Addlist(Copy_List, newlist);
    //在第一分支中搜索
    if (DPLL(Copy_List, s, mode))
        return true;
    //回溯到对elem执行分支策略的初态进入另一分支
    //此时需要删除重建,因为递归调用传入指针参数,表已经改变
    Destory2DLinklist(Copy_List); //删除临时复刻表
    newlist = (Linklist *)malloc(sizeof(Linklist));
    newlist->head = (Node *)malloc(sizeof(Linklist));
    newlist->head->data = -elem;
    newlist->head->next = NULL;
    newlist->cnt = 1;
    newlist->next = NULL;
    Addlist(L, newlist);
    bool res = DPLL(L, s, mode);
    Destory2DLinklist(L); //返回之前先删除表释放内存
    return res;
}
