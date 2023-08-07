// GB2312
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdbool>
#include <climits>

//有关函数返回值的宏定义
#define ERROR 0
#define OK 1
#define COLOR_DEFAULT "\033[0m"
#define COLOR_Blue "\033[0;34m"
#define COLOR_Green "\033[0;32m"
#define COLOR_Red "\033[0;31m"

//二维链表数据结构的定义,同一个链不同结点之间是并运算,不同链之间是交运算
typedef struct Node
{
    int data;   //链表结点
    Node *next; //下一个链表结点
} Node;
typedef struct Linklist
{
    int cnt;
    Node *head;     //指向第一个表结点的指针
    Linklist *next; //下一个表头结点
} Linklist;

//全局变量
char Filename[100];
Linklist *L0;                         //全局表头
int boolnum;                          //布尔变元数
int sentencenum;                      //子句数
clock_t begintime;                    //程序运行开始时间
clock_t endtime;                      //程序运行结束时间
bool ans;                             // Cnf范式是否可满足
int arr1[9][9], arr2[9][9];           //两个棋盘
int arr1ans[9][9], arr2ans[9][9];     //保存两个棋盘的答案
int mapboard1[9][9], mapboard2[9][9]; //标记两个棋盘中的点位是否为初始点位,0为初始,1为可填入
int oo1, oo2, oo3;                    //位置标识符
int sumOfAns;                         //记录答案数
int b[9][9];                          //递归过程量
//函数声明
// SAT
void Showmenu(int type);                               //显示菜单
int RD(FILE *fp);                                      //用文件指针快速读取数字
int ReadCnf(Linklist *&L);                             //读取Cnf文件
void PrintCnf(Linklist *L);                            //打印Cnf文件
void PrintSolution(bool *s);                           //输出Cnf文件的解
int Destory2DLinklist(Linklist *&L);                   //销毁二维链表
int Removelist(Linklist *&L, Linklist *&p0);           //在整个表中删除p0为表头所在的一条链
int RemoveNode(Linklist *&sentence, Node *&p0);        //在sentence子句所在链中删除p0结点
int Addlist(Linklist *&L, Linklist *&p0);              //在整个表中加入表头结点p0
bool issinglesentence(Linklist *p0);                   //判断是否为单子句
int Removesinglesentence(Linklist *&L, Linklist *p0);  //在整个表中删除含有单子句p0的句元结点,此操作将p0也删除
bool isemptysentence(Linklist *L);                     //判断是否有空的子句(链)
int Getnextelem_firsthead(Linklist *L);                //选元策略:选择第一个出现的数
int Getnextelem_mostfrequently(Linklist *L);           //选元策略:优先选择出现次数最多
int Getnextelem_JW(Linklist *L);                       //选元策略:JW,给每一个文字赋上权重2^(-n),正负合并考虑
bool DPLL(Linklist *&L, bool *s, int mode);            //解Cnf文件并保存到数组s中
int Copy2DLinklist(Linklist *L, Linklist *&Copy_List); //将L复制到Copy_List
int SaveCnf(bool res, bool *s, double t);              //打印并保存Cnf到res文件
// SuDoKu
void PrintBoard(int a1[][9], int b1[][9], int mode);        //输出棋盘,mode1:游戏模式,mode2:输出答案
void InitBoard();                                           //初始化棋盘,使其全部被填满
void DigHole(int holes);                                    //挖洞,返回挖洞数量
bool CheckBoard();                                          //检查是否满足数独要求
int TransBoardtoCnf(int a1[][9], int a2[][9], int holes);   //转换为Cnf文件
void PlayGame();                                            //游戏交互
int CreateOtherPosition(int row, int col, int chess);       //填满其他位置,chess第几个棋盘
int Transnumber(int oak1, int oak2, int oak3, int oak4);    //将布尔变元编号并返回
int Transback(int oak);                                     //将解出来的编号返回成原编号
void calSolution(int a[][9], int row, int col, int mark[]); //计算解的数量
bool issingleans(int a1[][9], int a2[][9], int holes);      //计算是否是唯一解

int main()
{
    int op = 1, Sat_op = 1, SuDoKu_op = 1, isinput = 0, issolved = 0, create_flag = 0, solve_flag = 0, holes;
    int a1[9][9], a2[9][9];
    bool *s; //存储解Cnf的结果
    while (op)
    {
        Showmenu(1);
        scanf("%d", &op);
        if (op == 1) // SAT Part
        {
            system("cls");
            while (Sat_op)
            {
                Showmenu(2);
                scanf("%d", &Sat_op);
                if (Sat_op == 1) // read cnf
                {
                    printf("Please input filename:\n");
                    scanf("%s", &Filename);
                    if (ReadCnf(L0) == OK)
                    {
                        isinput = 1; //已经输入文件
                        printf("Read file successfully!\n");
                    }
                    else
                        printf("Read file error\n");
                    getchar();
                    getchar();
                }
                else if (Sat_op == 2) // print cnf
                {
                    if (isinput == 0)
                        printf("You need to input file first!\n");
                    else
                    {
                        PrintCnf(L0);
                    }
                    getchar();
                    getchar();
                }
                else if (Sat_op == 3) // solve cnf
                {
                    if (isinput == 0)
                        printf("You need to input file first!\n");
                    else
                    {
                        issolved = 1;
                        s = (bool *)malloc(sizeof(bool) * (boolnum + 1));
                        PrintSolution(s);
                    }
                    getchar();
                    getchar();
                }
                else if (Sat_op == 4) // save cnf
                {
                    if (isinput == 0)
                        printf("You need to input file first!\n");
                    else if (issolved == 0)
                        printf("You need to solve it first!\n");
                    else
                    {
                        if (SaveCnf(ans, s, (double)(endtime - begintime) / CLK_TCK * 1000) == OK)
                            printf("Save Cnf successfully\n");
                        else
                            printf("Save Cnf Error!\n");
                    }
                    getchar();
                    getchar();
                }
                else if (Sat_op == 0) // return back
                {
                    Sat_op = 1;
                    system("cls");
                    break;
                }
                else // wrong input
                {
                    printf("There is something wrong with your input.\n");
                    printf("Please enter again.\n");
                    getchar();
                    getchar();
                }
                system("cls");
            }
        }
        else if (op == 2) // SuDoKu Part
        {
            system("cls");
            while (SuDoKu_op)
            {
                Showmenu(3);
                scanf("%d", &SuDoKu_op);
                if (SuDoKu_op == 1) // create SuDoKu
                {
                    InitBoard();
                    while (1)
                    {
                        // printf("Please input how many holes you want to dig(10-30)\n");
                        printf("choose:1.easy 2.nomral 3.hard\n");
                        scanf("%d", &holes);
                        if (holes == 1)
                            holes = 10;
                        else if (holes == 2)
                            holes = 18;
                        else if (holes == 3)
                            holes = 26;
                        if (holes >= 10 && holes <= 30)
                            break;
                        printf("Wrong input! Please input agiain.\n");
                    }
                    DigHole(holes);
                    PrintBoard(arr1, arr2, 1);
                    create_flag = 1;
                    getchar();
                    getchar();
                }
                else if (SuDoKu_op == 2) // solve SuDoKu
                {
                    printf("Please input filename\n");
                    scanf("%s", &Filename);
                    if (ReadCnf(L0) == ERROR)
                    {
                        printf("File read error!\n");
                        getchar();
                        getchar();
                        continue;
                    }
                    s = (bool *)malloc(sizeof(bool) * (boolnum + 1));
                    begintime = clock();
                    ans = DPLL(L0, s, 3);
                    endtime = clock();
                    printf("Answer: \n");
                    for (int i = 1; i <= boolnum; i++)
                    {
                        if (s[i])
                            printf("%d ", i);
                    }
                    printf("\n");
                    int array1ans[9][9], array2ans[9][9];
                    if (ans)
                    {
                        for (int i = 1; i <= boolnum; i++)
                        {
                            if (s[i] > 0)
                            {
                                Transback(i);
                                if (i <= 729 && oo1 <= 9 && oo1 >= 1 && oo2 >= 1 && oo2 <= 9)
                                    array1ans[oo1][oo2] = oo3;
                                else if (i >= 729 && oo1 <= 9 && oo1 >= 1 && oo2 >= 1 && oo2 <= 9)
                                    array2ans[oo1][oo2] = oo3;
                            }
                        }
                    }
                    if (SaveCnf(ans, s, (double)(endtime - begintime) / CLK_TCK * 1000) == OK)
                        printf("Solve successfully! The answer has been saved.\n");
                    else
                        printf("There is someting wrong...\n");
                    PrintBoard(array1ans, array2ans, 2);
                    getchar();
                    getchar();
                }
                else if (SuDoKu_op == 3) // save as cnf
                {
                    if (create_flag == 0)
                        printf("You need to create SuDoKu first!\n");
                    else
                    {
                        printf("Please input filename\n");
                        scanf("%s", &Filename);
                        if (TransBoardtoCnf(arr1, arr2, holes) == OK)
                            printf("Save successfully\n");
                        else
                            printf("File save error\n");
                    }
                    getchar();
                    getchar();
                }
                else if (SuDoKu_op == 4) // play game
                {
                    PlayGame();
                    getchar();
                    getchar();
                }
                else if (SuDoKu_op == 0) // return back
                {
                    SuDoKu_op = 1;
                    system("cls");
                    break;
                }
                else // wrong input
                {
                    printf("There is something wrong with your input.\n");
                    printf("Please enter again.\n");
                    getchar();
                    getchar();
                }
                system("cls");
            }
        }
        else if (op == 0)
        {
            printf("Thank you for using! ^_^ \n");
            system("pause");
            return 0;
        }
        else // wrong input
        {
            printf("There is something wrong with your input.\n");
            printf("Please enter again.\n");
            getchar();
            getchar();
        }
    }
    return 0;
}
//显示菜单
void Showmenu(int type)
{
    if (type == 1)
    {
        printf("-------Menu for SAT & SuDoKu!-------\n");
        printf("|  1.SAT Solve   |  2.SuDoKu Game  |\n");
        printf("------------------------------------\n");
        printf("|  0.Exit        |                 |\n");
        printf("------------------------------------\n");
        printf("You can choose now! Please enter(0~2)\n");
    }
    else if (type == 2)
    {
        printf("---------Menu for SAT Solve!--------\n");
        printf("|   1.Read Cnf    |   2.Print Cnf  |\n");
        printf("------------------------------------\n");
        printf("|   3.Sovle Cnf   |   4.Save Cnf   |\n");
        printf("------------------------------------\n");
        printf("|   0.Return      |                |\n");
        printf("------------------------------------\n");
        printf("You can choose now! Please enter(0~4)\n");
    }
    else if (type == 3)
    {
        printf("------------Menu for SuDoKu!-----------\n");
        printf("|  1.Create SuDoKu  |  2.Sovle SuDoKu |\n");
        printf("---------------------------------------\n");
        printf("|  3.Save as Cnf    |  4.Play SuDoKu  |\n");
        printf("---------------------------------------\n");
        printf("|  0.Return         |                 |\n");
        printf("---------------------------------------\n");
        printf("You can choose now! Please enter(0~4)\n");
    }
}
//用文件指针快速读取数字
int RD(FILE *fp)
{
    int x = 0, flag = 1, f = 1; // f确定正负号
    char ch = fgetc(fp);
    while (ch < '0' || ch > '9') //非数字
    {
        if (ch == '-')
        {
            f = -1;
            ch = fgetc(fp);
        }
        else
            return INT_MAX; //输入出错防止混淆,使用INT_MAX表示
    }
    while (ch >= '0' && ch <= '9') //快速读取数字
    {
        x = (x << 3) + (x << 1) + (ch - '0');
        ch = fgetc(fp);
        if (ch == -1)
            flag = 0; //读取失败说明读到结尾(该情况只会出现一次)
    }
    if (flag && ch != '\n' && ch != ' ') //最后一个字符未到该数所应该到的结尾就异常停止
        return INT_MAX;
    return x * f;
}
//读取Cnf文件
int ReadCnf(Linklist *&L)
{
    FILE *fp;        //文件指针
    Linklist *linkp; //链表结点指针
    Node *nodep;     //表头结点指针
    char ch;         //读取的字符
    int elem;
    if (!(fp = fopen(Filename, "r")))
        return ERROR;               //读取失败
    while ((ch = fgetc(fp)) == 'c') //每一行开头都是c,将c开头的行信息全部读完
        while ((ch = fgetc(fp)) != '\n')
            continue;
    //此时读取到p字符的那一行
    if (ch != 'p')
        return ERROR;            //如果没有读到p
    for (int i = 1; i <= 5; i++) //接下来5个字符必定为' cnf '
    {
        ch = fgetc(fp);
        if (i == 1 && ch != ' ')
            return ERROR;
        else if (i == 2 && ch != 'c')
            return ERROR;
        else if (i == 3 && ch != 'n')
            return ERROR;
        else if (i == 4 && ch != 'f')
            return ERROR;
        else if (i == 5 && ch != ' ')
            return ERROR;
    }
    if ((boolnum = RD(fp)) == INT_MAX)
        return ERROR;
    if ((sentencenum = RD(fp)) == INT_MAX)
        return ERROR;
    //初始化二维链表
    L = (Linklist *)malloc(sizeof(Linklist));
    L->next = NULL;
    L->head = (Node *)malloc(sizeof(Node));
    nodep = L->head;
    nodep->next = NULL;
    L->cnt = 0;
    linkp = L;
    //建立二维链表
    for (int i = 1; i <= sentencenum; i++)
    {
        if ((elem = RD(fp)) == INT_MAX)
            return ERROR; //读取失败
        while (elem != 0) //读到0为止
        {
            linkp->cnt++;
            nodep->data = elem;
            if ((elem = RD(fp)) == INT_MAX)
                return ERROR; //读取失败
            if (elem == 0)
                nodep->next = NULL;
            else
                nodep->next = (Node *)malloc(sizeof(Node));
            nodep = nodep->next;
        }
        //初始化下一个表头
        if (i == sentencenum)
        {
            linkp->next = NULL;
            continue;
        }
        linkp->next = (Linklist *)malloc(sizeof(Linklist));
        linkp = linkp->next;
        linkp->cnt = 0;
        linkp->head = (Node *)malloc(sizeof(Node));
        nodep = linkp->head;
    }
    fclose(fp);
    return OK;
}

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
    int *fre_map = (int *)malloc(sizeof(int) * (2 * boolnum + 1));
    // int fre_map[2 * boolnum + 1];
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
    double *w = (double *)malloc(sizeof(double) * (2 * boolnum + 1));
    double mw = 0;
    // double w[2 * boolnum + 1], mw = 0;
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

//将L复制到Copy_List
int Copy2DLinklist(Linklist *L, Linklist *&Copy_List)
{
    if (L == NULL)
    {
        Copy_List = NULL;
        return OK;
    }
    Copy_List = (Linklist *)malloc(sizeof(Linklist));
    Copy_List->next = NULL;
    Copy_List->head = (Node *)malloc(sizeof(Node));
    Copy_List->head->next = NULL;
    Copy_List->cnt = 0;
    Linklist *l1 = L, *l2 = Copy_List;
    Node *n1, *n2;
    while (l1 != NULL)
    {
        n1 = l1->head;
        n2 = l2->head;
        while (n1 != NULL)
        {
            l2->cnt++;
            n2->data = n1->data;
            n2->next = (Node *)malloc(sizeof(Node));
            n2->next->next = NULL;
            if (n1->next == NULL)
                free(n2->next), n2->next = NULL;
            n1 = n1->next;
            n2 = n2->next;
        }
        if (l1->next == NULL) // l1已经没有后续,不用继续开辟空间
        {
            l2->next = NULL;
        }
        else
        {
            l2->next = (Linklist *)malloc(sizeof(Linklist));
            l2->next->head = (Node *)malloc(sizeof(Node));
            l2->next->next = NULL;
            l2->next->head->next = NULL;
            l2->next->cnt = 0;
        }
        l1 = l1->next;
        l2 = l2->next;
    }
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
            if (mode < 1)
                return false;
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

//打印并保存Cnf到res文件,res是否可满足,s数组保存结果,t记录运行时间
int SaveCnf(bool res, bool *s, double t)
{
    char Newfilename[100];
    int i;
    FILE *fp;
    strcpy(Newfilename, Filename);
    for (i = 0; i < 100; i++)
    {
        if (Newfilename[i] == '.')
            break;
    }
    Newfilename[i + 1] = 'r';
    Newfilename[i + 2] = 'e';
    Newfilename[i + 3] = 's';
    Newfilename[i + 4] = '\0';
    if (!(fp = fopen(Newfilename, "w")))
        return ERROR;
    fprintf(fp, "s %d\n", (res == true ? 1 : 0));
    fprintf(fp, "v ");
    if (res) //可满足
    {
        for (i = 1; i <= boolnum; i++)
        {
            if (s[i] == true)
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    }
    fprintf(fp, "\n");
    fprintf(fp, "t %f", t);
    fclose(fp);
    return OK;
}

//打印Cnf文件
void PrintCnf(Linklist *L)
{
    Linklist *p1 = L;
    Node *p2;
    while (p1 != NULL)
    {
        p2 = p1->head;
        while (p2 != NULL)
        {
            printf("%d ", p2->data);
            p2 = p2->next;
        }
        printf("\n");
        p1 = p1->next;
    }
}

void PrintSolution(bool *s)
{
    for (int i = 0; i < boolnum + 1; i++)
        s[i] = 1;
    int choice1 = 1, mode;
    while (choice1)
    {
        printf("Please choose a mode: 1.firstdata  2.mostfrequently  3.JW\n");
        scanf("%d", &choice1);
        if (choice1 == 1)
        {
            mode = 1;
            break;
        }
        else if (choice1 == 2)
        {
            mode = 2;
            break;
        }
        else if (choice1 == 3)
        {
            mode = 3;
            break;
        }
        else
        {
            choice1 = 1;
            printf("Your input is wrong! Please input again\n");
        }
    }
    begintime = clock();
    ans = DPLL(L0, s, mode);
    endtime = clock();
    if (ans == false)
        printf("The problem is unsatisfied!\n");
    else
    {
        printf("Answer: \n");
        for (int i = 1; i <= boolnum; i++)
        {
            if (s[i])
                printf("%d ", i);
            else
                printf("%d ", -i);
        }
        printf("\n");
    }
    printf("Time: %f ms", (double)(endtime - begintime) / CLK_TCK * 1000);
}

void PrintBoard(int a1[][9], int b1[][9], int mode) //输出棋盘,mode1:游戏模式,mode2:输出答案
{
    int i, j, k;
    if (mode == 1)
    {
        for (i = 0; i < 6; i++)
        {
            if (i % 3 == 0)
                printf("-------------------\n");
            printf("|");
            for (j = 0; j < 9; j++)
            {
                if ((j + 1) % 3 == 0)
                {
                    if (arr1[i][j] == 0)
                    {
                        printf(COLOR_Green "_" COLOR_DEFAULT);
                        printf("|");
                    }
                    else
                    {
                        if (mapboard1[i][j])
                            printf(COLOR_Green "%d" COLOR_DEFAULT, arr1[i][j]);
                        else
                            printf("%d", arr1[i][j]);
                        printf("|");
                    }
                }
                else
                {
                    if (arr1[i][j] == 0)
                        printf(COLOR_Green "_ " COLOR_DEFAULT);
                    else
                    {
                        if (mapboard1[i][j])
                            printf(COLOR_Green "%d " COLOR_DEFAULT, arr1[i][j]);
                        else
                            printf("%d ", arr1[i][j]);
                    }
                }
            }
            printf("\n");
        }
        for (i = 6; i < 9; i++)
        {
            if (i % 3 == 0)
                printf("-------------------------------\n");
            printf("|");
            for (j = 0; j < 15; j++)
            {
                if (j < 9)
                {
                    if ((j + 1) % 3 == 0)
                    {
                        if (arr1[i][j] == 0)
                        {
                            printf(COLOR_Green "_" COLOR_DEFAULT);
                            printf("|");
                        }
                        else
                        {
                            if (mapboard1[i][j])
                                printf(COLOR_Green "%d" COLOR_DEFAULT, arr1[i][j]);
                            else
                                printf("%d", arr1[i][j]);
                            printf("|");
                        }
                    }
                    else
                    {
                        if (arr1[i][j] == 0)
                            printf(COLOR_Green "_ " COLOR_DEFAULT);
                        else
                        {
                            if (mapboard1[i][j])
                                printf(COLOR_Green "%d " COLOR_DEFAULT, arr1[i][j]);
                            else
                                printf("%d ", arr1[i][j]);
                        }
                    }
                }
                else
                {
                    if ((j + 1) % 3 == 0)
                    {
                        if (arr2[i - 6][j - 6] == 0)
                        {
                            printf(COLOR_Green "_" COLOR_DEFAULT);
                            printf("|");
                        }

                        else
                        {
                            if (mapboard2[i - 6][j - 6])
                                printf(COLOR_Green "%d" COLOR_DEFAULT, arr2[i - 6][j - 6]);
                            else
                                printf("%d", arr2[i - 6][j - 6]);
                            printf("|");
                        }
                    }
                    else
                    {
                        if (arr2[i - 6][j - 6] == 0)
                            printf(COLOR_Green "_ " COLOR_DEFAULT);
                        else
                        {
                            if (mapboard2[i - 6][j - 6])
                                printf(COLOR_Green "%d " COLOR_DEFAULT, arr2[i - 6][j - 6]);
                            else
                                printf("%d ", arr2[i - 6][j - 6]);
                        }
                    }
                }
            }
            printf("\n");
        }
        for (i = 9; i < 15; i++)
        {
            if (i == 9)
                printf("-------------------------------\n");
            else if (i % 3 == 0)
                printf("            -------------------\n");
            printf("            |");
            for (j = 0; j < 9; j++)
            {
                if ((j + 1) % 3 == 0)
                {
                    if (arr2[i - 6][j] == 0)
                    {
                        printf(COLOR_Green "_" COLOR_DEFAULT);
                        printf("|");
                    }
                    else
                    {
                        if (mapboard2[i - 6][j])
                            printf(COLOR_Green "%d" COLOR_DEFAULT, arr2[i - 6][j]);
                        else
                            printf("%d", arr2[i - 6][j]);
                        printf("|");
                    }
                }
                else
                {
                    if (arr2[i - 6][j] == 0)
                    {
                        printf(COLOR_Green "_ " COLOR_DEFAULT);
                    }
                    else
                    {
                        if (mapboard2[i - 6][j])
                            printf(COLOR_Green "%d " COLOR_DEFAULT, arr2[i - 6][j]);
                        else
                            printf("%d ", arr2[i - 6][j]);
                    }
                }
            }
            printf("\n");
        }
        printf("            -------------------\n");
    }
    else if (mode == 2)
    {
        for (i = 0; i < 6; i++)
        {
            if (i % 3 == 0)
                printf("-------------------\n");
            printf("|");
            for (j = 0; j < 9; j++)
            {
                if ((j + 1) % 3 == 0)
                {
                    if (mapboard1[i][j])
                        printf(COLOR_Green "%d" COLOR_DEFAULT, arr1ans[i][j]);
                    else
                        printf("%d", arr1ans[i][j]);
                    printf("|");
                }
                else
                {
                    if (mapboard1[i][j])
                        printf(COLOR_Green "%d " COLOR_DEFAULT, arr1ans[i][j]);
                    else
                        printf("%d ", arr1ans[i][j]);
                }
            }
            printf("\n");
        }
        for (i = 6; i < 9; i++)
        {
            if (i % 3 == 0)
                printf("-------------------------------\n");
            printf("|");
            for (j = 0; j < 15; j++)
            {
                if (j < 9)
                {
                    if ((j + 1) % 3 == 0)
                    {
                        if (mapboard1[i][j])
                            printf(COLOR_Green "%d" COLOR_DEFAULT, arr1ans[i][j]);
                        else
                            printf("%d", arr1ans[i][j]);
                        printf("|");
                    }
                    else
                    {
                        if (mapboard1[i][j])
                            printf(COLOR_Green "%d " COLOR_DEFAULT, arr1ans[i][j]);
                        else
                            printf("%d ", arr1ans[i][j]);
                    }
                }
                else
                {
                    if ((j + 1) % 3 == 0)
                    {
                        if (mapboard2[i - 6][j - 6])
                            printf(COLOR_Green "%d" COLOR_DEFAULT, arr2ans[i - 6][j - 6]);
                        else
                            printf("%d", arr2ans[i - 6][j - 6]);
                        printf("|");
                    }
                    else
                    {
                        if (mapboard2[i - 6][j - 6])
                            printf(COLOR_Green "%d " COLOR_DEFAULT, arr2ans[i - 6][j - 6]);
                        else
                            printf("%d ", arr2ans[i - 6][j - 6]);
                    }
                }
            }
            printf("\n");
        }
        for (i = 9; i < 15; i++)
        {
            if (i == 9)
                printf("-------------------------------\n");
            else if (i % 3 == 0)
                printf("            -------------------\n");
            printf("            |");
            for (j = 0; j < 9; j++)
            {
                if ((j + 1) % 3 == 0)
                {
                    if (mapboard2[i - 6][j])
                        printf(COLOR_Green "%d" COLOR_DEFAULT, arr2ans[i - 6][j]);
                    else
                        printf("%d", arr2ans[i - 6][j]);
                    printf("|");
                }
                else
                {
                    if (mapboard2[i - 6][j])
                        printf(COLOR_Green "%d " COLOR_DEFAULT, arr2ans[i - 6][j]);
                    else
                        printf("%d ", arr2ans[i - 6][j]);
                }
            }
            printf("\n");
        }
        printf("            -------------------\n");
    }
}

//初始化棋盘,使其全部被填满
void InitBoard()
{
    //初始化
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            arr1[i][j] = 0;
            arr2[i][j] = 0;
            mapboard1[i][j] = 0;
            mapboard2[i][j] = 0;
        }
    }
    srand(time(NULL));
    //填好arr1
    //填充第一行
    for (int i = 0; i < 9; i++)
    {
        arr1[0][i] = rand() % 9 + 1;
        for (int j = 0; j < i; j++)
        {
            if (arr1[0][j] == arr1[0][i])
            {
                arr1[0][i] = rand() % 9 + 1;
                j = -1; //回到原位置
            }
        }
    }
    //填充其他位置
    CreateOtherPosition(1, 0, 1);
    //填好arr2
    // arr2左上角九宫格已经被arr1确定
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr2[i][j] = arr1[i + 6][j + 6];
        }
    }
    //填好arr2第一行剩余部分
    for (int i = 3; i < 9; i++)
    {
        arr2[0][i] = rand() % 9 + 1;
        for (int j = 0; j < i; j++)
        {
            if (arr2[0][j] == arr2[0][i])
            {
                arr2[0][i] = rand() % 9 + 1;
                j = -1; //回到原位置
            }
        }
    }
    CreateOtherPosition(1, 3, 2);
}

//填满其他位置
int CreateOtherPosition(int row, int col, int chess)
{
    if (row >= 9 || col >= 9)
        return OK;
    int numbermark[10], x, y, val = 0;
    for (int i = 0; i < 10; i++)
        numbermark[i] = 0;
    if (chess == 1)
    {
        //同一行已使用的数标记为1
        for (y = 0; y < col; y++)
            numbermark[arr1[row][y]] = 1;
        //同一列已使用的数标记为1
        for (x = 0; x < row; x++)
            numbermark[arr1[x][col]] = 1;
        //同一九宫格已使用的数标记为1
        for (x = row / 3 * 3; x <= row; x++)
        {
            if (x < row) //此时y还未与col在同一列,将所在九宫格的row行上侧全部移动到
            {
                for (y = col / 3 * 3; y < col / 3 * 3 + 3; y++)
                    numbermark[arr1[x][y]] = 1;
            }
            else if (x == row) //处理和col同一列的情况,最多移动到当前位置上方
            {
                for (y = col / 3 * 3; y < col; y++)
                    numbermark[arr1[x][y]] = 1;
            }
        }
        //从左往右,从上往下
        for (int i = 1; i <= 9 && !val; i++) //填入1-9枚举
        {
            if (numbermark[i] == 1)
                continue;
            val = 1;
            arr1[row][col] = i;       //填入数字
            if (col == 8 && row != 8) //到最后一列且没到自后一行,此时需要换行
            {
                if (CreateOtherPosition(row + 1, 0, 1) == OK)
                    return OK;
                else
                    val = 0;
            }
            else if (col < 8) //不是最后一列
            {
                if (CreateOtherPosition(row, col + 1, 1) == OK)
                    return OK;
                else
                    val = 0;
            }
        }
        if (val == 0) //如果尝试所有数都不满足当前位置,说明之前填入有误,返回ERROR回溯
        {
            arr1[row][col] = 0;
            return ERROR;
        }
    }
    else if (chess == 2)
    {
        //同一行已使用的数标记为1
        for (y = 0; y < col; y++)
            numbermark[arr2[row][y]] = 1;
        //同一列已使用的数标记为1
        for (x = 0; x < row; x++)
            numbermark[arr2[x][col]] = 1;
        //同一九宫格已使用的数标记为1
        for (x = row / 3 * 3; x <= row; x++)
        {
            if (x < row) //此时y还未与col在同一列,将所在九宫格的row行上侧全部移动到
            {
                for (y = col / 3 * 3; y < col / 3 * 3 + 3; y++)
                    numbermark[arr2[x][y]] = 1;
            }
            else if (x == row) //处理和col同一列的情况,最多移动到当前位置上方
            {
                for (y = col / 3 * 3; y < col; y++)
                    numbermark[arr2[x][y]] = 1;
            }
        }
        //从左往右,从上往下
        for (int i = 1; i <= 9 && !val; i++) //填入1-9枚举
        {
            if (numbermark[i] == 1)
                continue;
            val = 1;
            arr2[row][col] = i;       //填入数字
            if (col == 8 && row != 8) //到最后一列且没到自后一行,此时需要换行
            {
                if (row < 2)
                {
                    if (CreateOtherPosition(row + 1, 3, 2) == OK)
                        return OK;
                    else
                        val = 0;
                }
                else
                {
                    if (CreateOtherPosition(row + 1, 0, 2) == OK)
                        return OK;
                    else
                        val = 0;
                }
            }
            else if (col < 8) //不是最后一列
            {
                if (CreateOtherPosition(row, col + 1, 2) == OK)
                    return OK;
                else
                    val = 0;
            }
        }
        if (val == 0) //如果尝试所有数都不满足当前位置,说明之前填入有误,返回ERROR回溯
        {
            arr2[row][col] = 0;
            return ERROR;
        }
    }
}

//挖洞,返回挖洞数量
void DigHole(int holes)
{
    //先复制一份答案棋盘
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            arr1ans[i][j] = arr1[i][j];
            arr2ans[i][j] = arr2[i][j];
        }
    }
    srand((unsigned)time(NULL));
    int remainholes = holes, x, y, chess, a1[9][9], a2[9][9];
    while (remainholes)
    {
        chess = rand() % 2 + 1;
        x = rand() % 9;
        y = rand() % 9;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                a1[i][j] = arr1[i][j];
                a2[i][j] = arr2[i][j];
            }
        }
        int cntnum = 0;
        for (int k = 1; k <= 9; k++)
        {
            if (chess == 1)
            {
                a1[x][y] = k;
                if (x > 6 && y > 6)
                    a2[x][y] = k;
            }
            else
            {
                a2[x][y] = k;
                if (x < 3 && y < 3)
                    a1[x][y] = k;
            }
            if (issingleans(a1, a2, holes) == true)
                cntnum++;
        }
        if (cntnum < 2)
        {
            if (chess == 1 && !mapboard1[x][y]) //棋盘1
            {
                arr1[x][y] = 0;
                mapboard1[x][y] = 1;
                if (x >= 6 && y >= 6) //棋盘1右下影响到棋盘2
                    mapboard2[x - 6][y - 6] = 1;
                remainholes--;
            }
            else if (chess == 2 && !mapboard2[x][y] && !(x <= 2 && y <= 2)) //棋盘2
            {
                arr2[x][y] = 0;
                mapboard2[x][y] = 1;
                remainholes--;
            }
        }
    }
    // int cnt = 0, m, n, flag, chess;
    // int mark[81][2];
    // for (int i = 0; i < holes; i++)
    // {
    //     chess = rand() % 2 + 1;
    //     m = rand() % 9;
    //     n = rand() % 9;
    //     flag = 0;
    //     if (chess == 1)
    //     {
    //         for (int i = 0; i < 9; i++)
    //             for (int j = 0; j < 9; j++)
    //                 b[i][j] = arr1[i][j];
    //         for (int j = 0; j < cnt; j++)
    //             if (m == mark[j][0] && n == mark[j][1])
    //                 flag = 1;
    //         sumOfAns = 0;
    //         int markNUM[10] = {0}, copyOfb[9][9];
    //         for (int x = 0; x < 9; x++)
    //             for (int y = 0; y < 9; y++)
    //                 copyOfb[x][y] = b[x][y];
    //         copyOfb[m][n] = 0;
    //         calSolution(copyOfb, 0, 0, markNUM);
    //         if (!flag && sumOfAns == 1) //可以挖洞
    //         {
    //             b[m][n] = 0, mark[cnt][0] = m, mark[cnt][1] = n, cnt++;
    //             arr1[m][n] = 0;
    //         }
    //         else
    //             i--;
    //     }
    //     else if (chess == 2)
    //     {
    //         for (int i = 0; i < 9; i++)
    //             for (int j = 0; j < 9; j++)
    //                 b[i][j] = arr1[i][j];
    //         for (int j = 0; j < cnt; j++)
    //             if (m == mark[j][0] && n == mark[j][1])
    //                 flag = 1;
    //         sumOfAns = 0;
    //         int markNUM[10] = {0}, copyOfb[9][9];
    //         for (int x = 0; x < 9; x++)
    //             for (int y = 0; y < 9; y++)
    //                 copyOfb[x][y] = b[x][y];
    //         copyOfb[m][n] = 0;
    //         calSolution(copyOfb, 0, 0, markNUM);
    //         if (!flag && sumOfAns == 1) //可以挖洞
    //         {
    //             b[m][n] = 0, mark[cnt][0] = m, mark[cnt][1] = n, cnt++;
    //             arr2[m][n] = 0;
    //         }
    //         else
    //             i--;
    //     }
    // }
}

//检查是否满足双数独要求
bool CheckBoard()
{
    int numcnt1[10];
    int numcnt2[10];
    //行
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (arr1[i][j] < 1 || arr1[i][j] > 9 || arr2[i][j] < 1 || arr2[i][j] > 9)
                return false;
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
        //检查
        for (int j = 1; j <= 9; j++)
        {
            if (numcnt1[j] == 0 || numcnt2[j] == 0)
                return false;
        }
    }
    //列
    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
        //检查
        for (int i = 1; i <= 9; i++)
        {
            if (numcnt1[i] == 0 || numcnt2[i] == 0)
                return false;
        }
    }
    //小九宫格
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 3; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 6; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 3; j < 6; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 3; i < 6; i++)
    {
        for (int j = 3; j < 6; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 6; i < 9; i++)
    {
        for (int j = 3; j < 6; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 6; j < 9; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 3; i < 6; i++)
    {
        for (int j = 6; j < 9; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }

    for (int i = 6; i < 9; i++)
    {
        for (int j = 6; j < 9; j++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
    }
    for (int i = 1; i <= 9; i++)
    {
        if (numcnt1[i] == 0 || numcnt2[i] == 0)
            return false;
    }
    //重合部分再次检验
    for (int i = 6; i < 9; i++)
    {
        for (int j = 6; j < 9; j++)
        {
            if (arr1[i][j] != arr2[i - 6][j - 6])
                return false;
        }
    }
    return true;
}

//转换为Cnf文件
int TransBoardtoCnf(int a1[][9], int a2[][9], int holes) //写入文件区分了重合位置
{
    FILE *fp;
    if (!(fp = fopen(Filename, "w")))
        return ERROR;
    fprintf(fp, "c SuDoKu\n");
    fprintf(fp, "p cnf 1458 %d\n", 13608 - holes);
    // fprintf(fp, "p cnf 1458 %d\n",13446-holes);
    int writenumber1, writenumber2;
    //约束1:单子句表明该位置已确定出现该数
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (arr1[i][j] >= 1 && arr1[i][j] <= 9)
            {
                writenumber1 = Transnumber(1, i + 1, j + 1, arr1[i][j]);
                // fprintf(fp, "%d 0\n", 1110 + i * 100 + j * 10 + arr1[i][j]);
                fprintf(fp, "%d 0\n", writenumber1);
            }

            if (arr2[i][j] >= 1 && arr2[i][j] <= 9)
            {
                writenumber1 = Transnumber(2, i + 1, j + 1, arr2[i][j]);
                // fprintf(fp, "%d 0\n", 2110 + i * 100 + j * 10 + arr1[i][j]);
                fprintf(fp, "%d 0\n", writenumber1);
            }
        }
    }
    // 162
    //约束2:每个位置需要出现每一个数
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++) //左上数独
            {
                writenumber1 = Transnumber(1, i + 1, j + 1, k + 1);
                fprintf(fp, "%d ", writenumber1);
                // fprintf(fp, "%d ", 1111 + i * 100 + j * 10 + k);
            }
            fprintf(fp, "0\n");         //该位置对应子句的结束
            for (int k = 0; k < 9; k++) //右下数独
            {
                writenumber1 = Transnumber(2, i + 1, j + 1, k + 1);
                fprintf(fp, "%d ", writenumber1);
                // fprintf(fp, "%d ", 2111 + i * 100 + j * 10 + k);
            }
            fprintf(fp, "0\n"); //该位置对应子句的结束
        }
    }
    // 324
    //约束3:每一列数字不重复
    for (int i = 1; i <= 9; i++) //对于每一行
    {
        for (int k = 1; k <= 9; k++) //对于每个数字
        {
            for (int j = 1; j < 9; j++) //对于每一列
            {
                for (int p = j + 1; p <= 9; p++) //对于该列右侧的任取一列,则两列不能填入相同的数
                {
                    //左上数独
                    writenumber1 = Transnumber(1, i, j, k);
                    writenumber2 = Transnumber(1, i, p, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(1000 + i * 100 + j * 10 + k), -(1000 + i * 100 + p * 10 + k));
                    //右下数独
                    writenumber1 = Transnumber(2, i, j, k);
                    writenumber2 = Transnumber(2, i, p, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(2000 + i * 100 + j * 10 + k), -(2000 + i * 100 + p * 10 + k));
                }
            }
        }
    }
    // 6156
    //约束4:每一行数字不重复
    for (int j = 1; j <= 9; j++) //对于每一列
    {
        for (int k = 1; k <= 9; k++) //对于每个数字
        {
            for (int i = 1; i < 9; i++) //对于每一行
            {
                for (int p = i + 1; p <= 9; p++) //对于该列右侧的任取一行,则两行不能填入相同的数
                {
                    //左上数独
                    writenumber1 = Transnumber(1, i, j, k);
                    writenumber2 = Transnumber(1, p, j, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(1000 + i * 100 + j * 10 + k), -(1000 + p * 100 + j * 10 + k));
                    //右下数独
                    writenumber1 = Transnumber(2, i, j, k);
                    writenumber2 = Transnumber(2, p, j, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(2000 + i * 100 + j * 10 + k), -(2000 + p * 100 + j * 10 + k));
                }
            }
        }
    }
    // 11988
    //约束5:每一个小九宫格数字不重复
    for (int num = 1; num <= 9; num++) //对于每一个数字
    {
        for (int i = 0; i < 3; i++) //第i+1个小九宫格的起始行为3*i
        {
            for (int j = 0; j < 3; j++) //第j+1个小九宫格的起始列为3*j
            {
                for (int p = 1; p <= 3; p++) //小九宫格内的行标
                {
                    for (int q = 1; q < 3; q++) //小九宫格内的列标
                    {
                        for (int r = q + 1; r < 3; r++) //小九宫格内q列之后任取一列,两列不相同
                        {
                            writenumber1 = Transnumber(1, 3 * i + p, 3 * j + q, num);
                            writenumber2 = Transnumber(1, 3 * i + p, 3 * j + r, num);
                            fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                            writenumber1 = Transnumber(2, 3 * i + p, 3 * j + q, num);
                            writenumber2 = Transnumber(2, 3 * i + p, 3 * j + r, num);
                            fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                            // fprintf(fp, "%d %d 0\n", -(1000 + (3 * i + p) * 100 + (3 * j + q) * 10 + num), -(1000 + (3 * i + p) * 100 + (3 * j + r) * 10 + num));
                            // fprintf(fp, "%d %d 0\n", -(2000 + (3 * i + p) * 100 + (3 * j + q) * 10 + num), -(2000 + (3 * i + p) * 100 + (3 * j + r) * 10 + num));
                        }
                    }
                }
            }
        }
    }
    // 12474
    for (int num = 1; num <= 9; num++) //对于每一个数字
    {
        for (int i = 0; i < 3; i++) //第i+1个小九宫格的起始行为3*i
        {
            for (int j = 0; j < 3; j++) //第j+1个小九宫格的起始列为3*j
            {
                for (int p = 1; p <= 3; p++) //小九宫格内的行标
                {
                    for (int q = 1; q < 3; q++) //小九宫格内的列标
                    {
                        for (int r = p + 1; r < 3; r++) //小九宫格内p行之后任取一行,两行不相同
                        {
                            for (int t = 1; t <= 3; t++)
                            {
                                writenumber1 = Transnumber(1, 3 * i + p, 3 * j + q, num);
                                writenumber2 = Transnumber(1, 3 * i + r, 3 * j + t, num);
                                fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                                writenumber1 = Transnumber(2, 3 * i + p, 3 * j + q, num);
                                writenumber2 = Transnumber(2, 3 * i + r, 3 * j + t, num);
                                fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                                // fprintf(fp, "%d %d 0\n", -(1000 + (3 * i + p) * 100 + (3 * j + q) * 10 + num), -(1000 + (3 * i + r) * 100 + (3 * j + t) * 10 + num));
                                // fprintf(fp, "%d %d 0\n", -(2000 + (3 * i + p) * 100 + (3 * j + q) * 10 + num), -(2000 + (3 * i + r) * 100 + (3 * j + t) * 10 + num));
                            }
                        }
                    }
                }
            }
        }
    }
    // 13446
    //约束6:重合部分布尔等值
    for (int i = 7; i <= 9; i++)
    {
        for (int j = 7; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                writenumber1 = Transnumber(1, i, j, k);
                writenumber2 = Transnumber(2, i - 6, j - 6, k);
                fprintf(fp, "%d %d 0\n", -writenumber1, writenumber2);
                writenumber1 = Transnumber(1, i, j, k);
                writenumber2 = Transnumber(2, i - 6, j - 6, k);
                fprintf(fp, "%d %d 0\n", writenumber1, -writenumber2);
                // fprintf(fp,"%d %d 0\n",-(1000+i*100+j*10+k),(2000+(i-6)*100+(j-6)*10+k));
                // fprintf(fp,"%d %d 0\n",(1000+i*100+j*10+k),-(2000+(i-6)*100+(j-6)*10+k));
            }
        }
    }
    // 13608
    fclose(fp);
    return OK;
}

//计算是否是唯一解
bool issingleans(int a1[][9], int a2[][9], int holes)
{
    bool *s1 = (bool *)malloc(sizeof(bool) * (2000));
    strcpy(Filename, "temp.cnf");
    TransBoardtoCnf(a1, a2, holes);
    ReadCnf(L0);
    bool ans1 = DPLL(L0, s1, 0);
    return ans1;
}

//游戏交互
void PlayGame()
{
    printf("Game start now!\n");
    getchar();
    getchar();
    int choice, n, x, y, k;
    while (1)
    {
        system("cls");
        PrintBoard(arr1, arr2, 1);
        printf("Please choose: 1.modify 2.verify 3.Need hint 4.get answer\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("Please enter the position you want to modify and the value.(n[1-2] x[1-9] y[1-9] k[1-9])\n");
            scanf("%d %d %d %d", &n, &x, &y, &k);
            if ((n == 1 && !mapboard1[x - 1][y - 1]) || (n == 2 && !mapboard2[x - 1][y - 1]))
            {
                printf("This position can not be modified!\n");
                getchar();
                getchar();
                continue;
            }
            if ((n != 1 && n != 2) || (x < 1 || x > 9) || (y < 1 || y > 9) || (k < 1 || k > 9))
            {
                printf("Wrong input! Please input again\n");
                getchar();
                getchar();
                continue;
            }
            if (n == 1)
            {
                arr1[x - 1][y - 1] = k;
                if (x > 6 && y > 6)
                    arr2[x - 1][y - 1] = k;
            }
            else if (n == 2)
            {
                arr2[x - 1][y - 1] = k;
                if (x < 3 && y < 3)
                    arr1[x - 1][y - 1] = k;
            }
        }
        else if (choice == 2)
        {
            if (CheckBoard() == OK)
                printf("You win!\n");
            else
                printf("You lose it\n");
            getchar();
            getchar();
            break;
        }
        else if (choice == 3)
        {
            printf("Please enter the position you want to get a hint(n x y).\n");
            scanf("%d %d %d", &n, &x, &y);
            if ((n != 1 && n != 2) || (x < 1 || x > 9) || (y < 1 || y > 9))
            {
                printf("Wrong input! Please input again\n");
                getchar();
                getchar();
                continue;
            }
            k = n == 1 ? arr1ans[x - 1][y - 1] : arr2ans[x - 1][y - 1];
            printf("The number at %dth (%d,%d) may be %d.\n", n, x, y, k);
            getchar();
            getchar();
        }
        else if (choice == 4)
        {
            bool *s = (bool *)malloc(sizeof(bool) * (boolnum + 1));
            ReadCnf(L0);
            ans = DPLL(L0, s, 3);
            int array1ans[9][9], array2ans[9][9];
            if (ans)
            {
                for (int i = 1; i <= boolnum; i++)
                {
                    if (s[i] > 0)
                    {
                        Transback(i);
                        if (i <= 729 && oo1 <= 9 && oo1 >= 1 && oo2 >= 1 && oo2 <= 9)
                            array1ans[oo1][oo2] = oo3;
                        else if (i >= 729 && oo1 <= 9 && oo1 >= 1 && oo2 >= 1 && oo2 <= 9)
                            array2ans[oo1][oo2] = oo3;
                    }
                }
            }
            PrintBoard(array1ans, array2ans, 2);
            getchar();
            getchar();
            break;
        }
        else
        {
            printf("Wrong input!\n");
            getchar();
            getchar();
        }
    }
}

//将布尔变元编号并返回
int Transnumber(int oak1, int oak2, int oak3, int oak4)
{
    if (oak1 == 1) //在第一个数独中
    {
        return (oak2 - 1) * 81 + (oak3 - 1) * 9 + oak4;
    }
    else if (oak1 == 2)
    {
        return 729 + (oak2 - 1) * 81 + (oak3 - 1) * 9 + oak4;
    }
}

//将解出来的编号返回成原编号
int Transback(int oak)
{
    if (oak % 81 == 0)
        oo1 = oak / 81;
    else
        oo1 = oak / 81 + 1;
    int temp = oak - 81 * (oo1 - 1);
    if (temp % 9 == 0)
        oo2 = temp / 9;
    else
        oo3 = temp / 9 + 1;
    oo3 = oak - 81 * (oo1 - 1) - 9 * (oo2 - 1);
}
