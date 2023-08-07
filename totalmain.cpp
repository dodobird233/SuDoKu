// GB2312
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdbool>
#include <climits>

//�йغ�������ֵ�ĺ궨��
#define ERROR 0
#define OK 1
#define COLOR_DEFAULT "\033[0m"
#define COLOR_Blue "\033[0;34m"
#define COLOR_Green "\033[0;32m"
#define COLOR_Red "\033[0;31m"

//��ά�������ݽṹ�Ķ���,ͬһ������ͬ���֮���ǲ�����,��ͬ��֮���ǽ�����
typedef struct Node
{
    int data;   //������
    Node *next; //��һ��������
} Node;
typedef struct Linklist
{
    int cnt;
    Node *head;     //ָ���һ�������ָ��
    Linklist *next; //��һ����ͷ���
} Linklist;

//ȫ�ֱ���
char Filename[100];
Linklist *L0;                         //ȫ�ֱ�ͷ
int boolnum;                          //������Ԫ��
int sentencenum;                      //�Ӿ���
clock_t begintime;                    //�������п�ʼʱ��
clock_t endtime;                      //�������н���ʱ��
bool ans;                             // Cnf��ʽ�Ƿ������
int arr1[9][9], arr2[9][9];           //��������
int arr1ans[9][9], arr2ans[9][9];     //�����������̵Ĵ�
int mapboard1[9][9], mapboard2[9][9]; //������������еĵ�λ�Ƿ�Ϊ��ʼ��λ,0Ϊ��ʼ,1Ϊ������
int oo1, oo2, oo3;                    //λ�ñ�ʶ��
int sumOfAns;                         //��¼����
int b[9][9];                          //�ݹ������
//��������
// SAT
void Showmenu(int type);                               //��ʾ�˵�
int RD(FILE *fp);                                      //���ļ�ָ����ٶ�ȡ����
int ReadCnf(Linklist *&L);                             //��ȡCnf�ļ�
void PrintCnf(Linklist *L);                            //��ӡCnf�ļ�
void PrintSolution(bool *s);                           //���Cnf�ļ��Ľ�
int Destory2DLinklist(Linklist *&L);                   //���ٶ�ά����
int Removelist(Linklist *&L, Linklist *&p0);           //����������ɾ��p0Ϊ��ͷ���ڵ�һ����
int RemoveNode(Linklist *&sentence, Node *&p0);        //��sentence�Ӿ���������ɾ��p0���
int Addlist(Linklist *&L, Linklist *&p0);              //���������м����ͷ���p0
bool issinglesentence(Linklist *p0);                   //�ж��Ƿ�Ϊ���Ӿ�
int Removesinglesentence(Linklist *&L, Linklist *p0);  //����������ɾ�����е��Ӿ�p0�ľ�Ԫ���,�˲�����p0Ҳɾ��
bool isemptysentence(Linklist *L);                     //�ж��Ƿ��пյ��Ӿ�(��)
int Getnextelem_firsthead(Linklist *L);                //ѡԪ����:ѡ���һ�����ֵ���
int Getnextelem_mostfrequently(Linklist *L);           //ѡԪ����:����ѡ����ִ������
int Getnextelem_JW(Linklist *L);                       //ѡԪ����:JW,��ÿһ�����ָ���Ȩ��2^(-n),�����ϲ�����
bool DPLL(Linklist *&L, bool *s, int mode);            //��Cnf�ļ������浽����s��
int Copy2DLinklist(Linklist *L, Linklist *&Copy_List); //��L���Ƶ�Copy_List
int SaveCnf(bool res, bool *s, double t);              //��ӡ������Cnf��res�ļ�
// SuDoKu
void PrintBoard(int a1[][9], int b1[][9], int mode);        //�������,mode1:��Ϸģʽ,mode2:�����
void InitBoard();                                           //��ʼ������,ʹ��ȫ��������
void DigHole(int holes);                                    //�ڶ�,�����ڶ�����
bool CheckBoard();                                          //����Ƿ���������Ҫ��
int TransBoardtoCnf(int a1[][9], int a2[][9], int holes);   //ת��ΪCnf�ļ�
void PlayGame();                                            //��Ϸ����
int CreateOtherPosition(int row, int col, int chess);       //��������λ��,chess�ڼ�������
int Transnumber(int oak1, int oak2, int oak3, int oak4);    //��������Ԫ��Ų�����
int Transback(int oak);                                     //��������ı�ŷ��س�ԭ���
void calSolution(int a[][9], int row, int col, int mark[]); //����������
bool issingleans(int a1[][9], int a2[][9], int holes);      //�����Ƿ���Ψһ��

int main()
{
    int op = 1, Sat_op = 1, SuDoKu_op = 1, isinput = 0, issolved = 0, create_flag = 0, solve_flag = 0, holes;
    int a1[9][9], a2[9][9];
    bool *s; //�洢��Cnf�Ľ��
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
                        isinput = 1; //�Ѿ������ļ�
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
//��ʾ�˵�
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
//���ļ�ָ����ٶ�ȡ����
int RD(FILE *fp)
{
    int x = 0, flag = 1, f = 1; // fȷ��������
    char ch = fgetc(fp);
    while (ch < '0' || ch > '9') //������
    {
        if (ch == '-')
        {
            f = -1;
            ch = fgetc(fp);
        }
        else
            return INT_MAX; //��������ֹ����,ʹ��INT_MAX��ʾ
    }
    while (ch >= '0' && ch <= '9') //���ٶ�ȡ����
    {
        x = (x << 3) + (x << 1) + (ch - '0');
        ch = fgetc(fp);
        if (ch == -1)
            flag = 0; //��ȡʧ��˵��������β(�����ֻ�����һ��)
    }
    if (flag && ch != '\n' && ch != ' ') //���һ���ַ�δ��������Ӧ�õ��Ľ�β���쳣ֹͣ
        return INT_MAX;
    return x * f;
}
//��ȡCnf�ļ�
int ReadCnf(Linklist *&L)
{
    FILE *fp;        //�ļ�ָ��
    Linklist *linkp; //������ָ��
    Node *nodep;     //��ͷ���ָ��
    char ch;         //��ȡ���ַ�
    int elem;
    if (!(fp = fopen(Filename, "r")))
        return ERROR;               //��ȡʧ��
    while ((ch = fgetc(fp)) == 'c') //ÿһ�п�ͷ����c,��c��ͷ������Ϣȫ������
        while ((ch = fgetc(fp)) != '\n')
            continue;
    //��ʱ��ȡ��p�ַ�����һ��
    if (ch != 'p')
        return ERROR;            //���û�ж���p
    for (int i = 1; i <= 5; i++) //������5���ַ��ض�Ϊ' cnf '
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
    //��ʼ����ά����
    L = (Linklist *)malloc(sizeof(Linklist));
    L->next = NULL;
    L->head = (Node *)malloc(sizeof(Node));
    nodep = L->head;
    nodep->next = NULL;
    L->cnt = 0;
    linkp = L;
    //������ά����
    for (int i = 1; i <= sentencenum; i++)
    {
        if ((elem = RD(fp)) == INT_MAX)
            return ERROR; //��ȡʧ��
        while (elem != 0) //����0Ϊֹ
        {
            linkp->cnt++;
            nodep->data = elem;
            if ((elem = RD(fp)) == INT_MAX)
                return ERROR; //��ȡʧ��
            if (elem == 0)
                nodep->next = NULL;
            else
                nodep->next = (Node *)malloc(sizeof(Node));
            nodep = nodep->next;
        }
        //��ʼ����һ����ͷ
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

//���ٶ�ά����
int Destory2DLinklist(Linklist *&L)
{
    if (L == NULL)
        return ERROR;
    Linklist *linkp1 = L, *linkp2;
    Node *p1, *p2;
    while (linkp1 != NULL)
    {
        p1 = linkp1->head;
        while (p1 != NULL) // p1Ϊ��ָ��,p2Ϊǰָ��
        {
            p2 = p1->next;
            free(p1);
            p1 = p2;
        }
        linkp2 = linkp1->next; // linkp2Ϊǰָ��,linkp1Ϊ��ָ��
        free(linkp1);          //�ͷź�ָ���ͷ���
        linkp1 = linkp2;
    }
    L = NULL;
    return OK;
}

//����������ɾ��p0Ϊ��ͷ���ڵ�һ����
int Removelist(Linklist *&L, Linklist *&p0)
{
    Linklist *p;
    Node *p1, *p2;
    if (L == p0) //�����ǵ�һ����ͷ���
    {
        L = L->next;
        //����p������
    }
    else
    {
        p = L;
        while (p != NULL && p->next != p0)
            p = p->next; //��p0,ʹ��p��һ��Ϊp0
        if (p == NULL)
            return ERROR; //�Ҳ���p0
        if (p0->next != NULL)
            p->next = p0->next; // p0����ĩβ,��p����p0�ĺ���
        else
            p->next = NULL; // p0��ĩβ
    }
    //����p������
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

//��sentence�Ӿ���������ɾ��p0���
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

//����������ͷ�巨�����ͷ���p0,�������ͷ��Ϊp0,p0�ռ���Ҫ��ǰ����
int Addlist(Linklist *&L, Linklist *&p0)
{
    if (p0 == NULL)
        return ERROR;
    p0->next = L;
    L = p0;
    return OK;
}

//�ж��Ƿ�Ϊ���Ӿ�
bool issinglesentence(Linklist *p0)
{
    if (p0->head != NULL && p0->head->next == NULL)
        return true;
    else
        return false;
}

//����������ɾ�����е��Ӿ�p0�ľ�Ԫ���,�˲�����p0Ҳɾ��
int Removesinglesentence(Linklist *&L, Linklist *p0)
{
    int num1 = p0->head->data;
    Linklist *p1, *pp;
    Node *p2;
    p1 = L;
    while (p1 != NULL)
    {
        pp = p1->next; // p1���ܱ�ɾ��,��Ҫ��ʱ����pp�ݴ�
        p2 = p1->head;
        while (p2 != NULL)
        {
            if (p2->data == num1) //��ֵʼ���뵥�Ӿ�p0��ͬ,����ֱ���Ƴ�������
            {
                Removelist(L, p1);
                break;
            }
            else if (p2->data == -num1) //��ֵ�뵥�Ӿ�p0�෴,�����ж��б�Ȼ�Խ��û������,ɾ�����
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

//�ж��Ƿ��пյ��Ӿ�(��)
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

//ѡԪ����:ѡ���һ�����ֵ���
int Getnextelem_firsthead(Linklist *L)
{
    return L->head->data;
}

//ѡԪ����:����ѡ����ִ������
int Getnextelem_mostfrequently(Linklist *L)
{
    //������Ƶ��,����Ԫ1...boolnum,����Ԫboolnum+1...2*boolnum
    int *fre_map = (int *)malloc(sizeof(int) * (2 * boolnum + 1));
    // int fre_map[2 * boolnum + 1];
    for (int i = 0; i < 2 * boolnum + 1; i++)
        fre_map[i] = 0; //��ʼ��Ϊ0
    Linklist *p1 = L;
    Node *p2;
    int mostfre;
    int maxtimes = 0;
    while (p1 != NULL)
    {
        p2 = p1->head;
        while (p2 != NULL)
        {
            if (p2->data > 0) //����Ԫ
                fre_map[p2->data]++;
            else //����Ԫ��������boolnum֮��
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

int Getnextelem_JW(Linklist *L) //ѡԪ����:JW,��ÿһ�����ָ���Ȩ��2^(-n),�����ϲ�����
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

//��L���Ƶ�Copy_List
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
        if (l1->next == NULL) // l1�Ѿ�û�к���,���ü������ٿռ�
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

//��Cnf�ļ������浽����s��
bool DPLL(Linklist *&L, bool *s, int mode)
{
    Linklist *p;
    while (true)
    {
        p = L;
        while (p && !issinglesentence(p))
            p = p->next;
        if (p != NULL) //���е��Ӿ�
        {
            //���Ӿ�ֱ���ж���ֵ
            if (p->head->data > 0)
                s[p->head->data] = true;
            else
                s[-p->head->data] = false;
            Removesinglesentence(L, p);
            if (mode < 1)
                return false;
            if (L == NULL) //��������Ϊ��
                return true;
            else if (isemptysentence(L)) //����п��Ӿ�
                return false;
        }
        else
            break;
    }
    //ѡԪ
    int elem;
    if (mode == 1)
        elem = Getnextelem_firsthead(L);
    else if (mode == 2)
        elem = Getnextelem_mostfrequently(L);
    else if (mode == 3)
        elem = Getnextelem_JW(L);
    //Ϊ�˵ݹ��ܻص���̬��Ҫ�ȿ���һ��,����ԭ���ı�
    Linklist *Copy_List;
    Linklist *newlist = (Linklist *)malloc(sizeof(Linklist));
    newlist->head = (Node *)malloc(sizeof(Linklist));
    newlist->head->data = elem;
    newlist->head->next = NULL;
    newlist->cnt = 1;
    newlist->next = NULL;
    //�����µı�Ԫ
    Copy2DLinklist(L, Copy_List);
    Addlist(Copy_List, newlist);
    //�ڵ�һ��֧������
    if (DPLL(Copy_List, s, mode))
        return true;
    //���ݵ���elemִ�з�֧���Եĳ�̬������һ��֧
    //��ʱ��Ҫɾ���ؽ�,��Ϊ�ݹ���ô���ָ�����,���Ѿ��ı�
    Destory2DLinklist(Copy_List); //ɾ����ʱ���̱�
    newlist = (Linklist *)malloc(sizeof(Linklist));
    newlist->head = (Node *)malloc(sizeof(Linklist));
    newlist->head->data = -elem;
    newlist->head->next = NULL;
    newlist->cnt = 1;
    newlist->next = NULL;
    Addlist(L, newlist);
    bool res = DPLL(L, s, mode);
    Destory2DLinklist(L); //����֮ǰ��ɾ�����ͷ��ڴ�
    return res;
}

//��ӡ������Cnf��res�ļ�,res�Ƿ������,s���鱣����,t��¼����ʱ��
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
    if (res) //������
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

//��ӡCnf�ļ�
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

void PrintBoard(int a1[][9], int b1[][9], int mode) //�������,mode1:��Ϸģʽ,mode2:�����
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

//��ʼ������,ʹ��ȫ��������
void InitBoard()
{
    //��ʼ��
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
    //���arr1
    //����һ��
    for (int i = 0; i < 9; i++)
    {
        arr1[0][i] = rand() % 9 + 1;
        for (int j = 0; j < i; j++)
        {
            if (arr1[0][j] == arr1[0][i])
            {
                arr1[0][i] = rand() % 9 + 1;
                j = -1; //�ص�ԭλ��
            }
        }
    }
    //�������λ��
    CreateOtherPosition(1, 0, 1);
    //���arr2
    // arr2���ϽǾŹ����Ѿ���arr1ȷ��
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr2[i][j] = arr1[i + 6][j + 6];
        }
    }
    //���arr2��һ��ʣ�ಿ��
    for (int i = 3; i < 9; i++)
    {
        arr2[0][i] = rand() % 9 + 1;
        for (int j = 0; j < i; j++)
        {
            if (arr2[0][j] == arr2[0][i])
            {
                arr2[0][i] = rand() % 9 + 1;
                j = -1; //�ص�ԭλ��
            }
        }
    }
    CreateOtherPosition(1, 3, 2);
}

//��������λ��
int CreateOtherPosition(int row, int col, int chess)
{
    if (row >= 9 || col >= 9)
        return OK;
    int numbermark[10], x, y, val = 0;
    for (int i = 0; i < 10; i++)
        numbermark[i] = 0;
    if (chess == 1)
    {
        //ͬһ����ʹ�õ������Ϊ1
        for (y = 0; y < col; y++)
            numbermark[arr1[row][y]] = 1;
        //ͬһ����ʹ�õ������Ϊ1
        for (x = 0; x < row; x++)
            numbermark[arr1[x][col]] = 1;
        //ͬһ�Ź�����ʹ�õ������Ϊ1
        for (x = row / 3 * 3; x <= row; x++)
        {
            if (x < row) //��ʱy��δ��col��ͬһ��,�����ھŹ����row���ϲ�ȫ���ƶ���
            {
                for (y = col / 3 * 3; y < col / 3 * 3 + 3; y++)
                    numbermark[arr1[x][y]] = 1;
            }
            else if (x == row) //�����colͬһ�е����,����ƶ�����ǰλ���Ϸ�
            {
                for (y = col / 3 * 3; y < col; y++)
                    numbermark[arr1[x][y]] = 1;
            }
        }
        //��������,��������
        for (int i = 1; i <= 9 && !val; i++) //����1-9ö��
        {
            if (numbermark[i] == 1)
                continue;
            val = 1;
            arr1[row][col] = i;       //��������
            if (col == 8 && row != 8) //�����һ����û���Ժ�һ��,��ʱ��Ҫ����
            {
                if (CreateOtherPosition(row + 1, 0, 1) == OK)
                    return OK;
                else
                    val = 0;
            }
            else if (col < 8) //�������һ��
            {
                if (CreateOtherPosition(row, col + 1, 1) == OK)
                    return OK;
                else
                    val = 0;
            }
        }
        if (val == 0) //��������������������㵱ǰλ��,˵��֮ǰ��������,����ERROR����
        {
            arr1[row][col] = 0;
            return ERROR;
        }
    }
    else if (chess == 2)
    {
        //ͬһ����ʹ�õ������Ϊ1
        for (y = 0; y < col; y++)
            numbermark[arr2[row][y]] = 1;
        //ͬһ����ʹ�õ������Ϊ1
        for (x = 0; x < row; x++)
            numbermark[arr2[x][col]] = 1;
        //ͬһ�Ź�����ʹ�õ������Ϊ1
        for (x = row / 3 * 3; x <= row; x++)
        {
            if (x < row) //��ʱy��δ��col��ͬһ��,�����ھŹ����row���ϲ�ȫ���ƶ���
            {
                for (y = col / 3 * 3; y < col / 3 * 3 + 3; y++)
                    numbermark[arr2[x][y]] = 1;
            }
            else if (x == row) //�����colͬһ�е����,����ƶ�����ǰλ���Ϸ�
            {
                for (y = col / 3 * 3; y < col; y++)
                    numbermark[arr2[x][y]] = 1;
            }
        }
        //��������,��������
        for (int i = 1; i <= 9 && !val; i++) //����1-9ö��
        {
            if (numbermark[i] == 1)
                continue;
            val = 1;
            arr2[row][col] = i;       //��������
            if (col == 8 && row != 8) //�����һ����û���Ժ�һ��,��ʱ��Ҫ����
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
            else if (col < 8) //�������һ��
            {
                if (CreateOtherPosition(row, col + 1, 2) == OK)
                    return OK;
                else
                    val = 0;
            }
        }
        if (val == 0) //��������������������㵱ǰλ��,˵��֮ǰ��������,����ERROR����
        {
            arr2[row][col] = 0;
            return ERROR;
        }
    }
}

//�ڶ�,�����ڶ�����
void DigHole(int holes)
{
    //�ȸ���һ�ݴ�����
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
            if (chess == 1 && !mapboard1[x][y]) //����1
            {
                arr1[x][y] = 0;
                mapboard1[x][y] = 1;
                if (x >= 6 && y >= 6) //����1����Ӱ�쵽����2
                    mapboard2[x - 6][y - 6] = 1;
                remainholes--;
            }
            else if (chess == 2 && !mapboard2[x][y] && !(x <= 2 && y <= 2)) //����2
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
    //         if (!flag && sumOfAns == 1) //�����ڶ�
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
    //         if (!flag && sumOfAns == 1) //�����ڶ�
    //         {
    //             b[m][n] = 0, mark[cnt][0] = m, mark[cnt][1] = n, cnt++;
    //             arr2[m][n] = 0;
    //         }
    //         else
    //             i--;
    //     }
    // }
}

//����Ƿ�����˫����Ҫ��
bool CheckBoard()
{
    int numcnt1[10];
    int numcnt2[10];
    //��
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (arr1[i][j] < 1 || arr1[i][j] > 9 || arr2[i][j] < 1 || arr2[i][j] > 9)
                return false;
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
        //���
        for (int j = 1; j <= 9; j++)
        {
            if (numcnt1[j] == 0 || numcnt2[j] == 0)
                return false;
        }
    }
    //��
    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            numcnt1[arr1[i][j]]++;
            numcnt2[arr2[i][j]]++;
        }
        //���
        for (int i = 1; i <= 9; i++)
        {
            if (numcnt1[i] == 0 || numcnt2[i] == 0)
                return false;
        }
    }
    //С�Ź���
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
    //�غϲ����ٴμ���
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

//ת��ΪCnf�ļ�
int TransBoardtoCnf(int a1[][9], int a2[][9], int holes) //д���ļ��������غ�λ��
{
    FILE *fp;
    if (!(fp = fopen(Filename, "w")))
        return ERROR;
    fprintf(fp, "c SuDoKu\n");
    fprintf(fp, "p cnf 1458 %d\n", 13608 - holes);
    // fprintf(fp, "p cnf 1458 %d\n",13446-holes);
    int writenumber1, writenumber2;
    //Լ��1:���Ӿ������λ����ȷ�����ָ���
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
    //Լ��2:ÿ��λ����Ҫ����ÿһ����
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++) //��������
            {
                writenumber1 = Transnumber(1, i + 1, j + 1, k + 1);
                fprintf(fp, "%d ", writenumber1);
                // fprintf(fp, "%d ", 1111 + i * 100 + j * 10 + k);
            }
            fprintf(fp, "0\n");         //��λ�ö�Ӧ�Ӿ�Ľ���
            for (int k = 0; k < 9; k++) //��������
            {
                writenumber1 = Transnumber(2, i + 1, j + 1, k + 1);
                fprintf(fp, "%d ", writenumber1);
                // fprintf(fp, "%d ", 2111 + i * 100 + j * 10 + k);
            }
            fprintf(fp, "0\n"); //��λ�ö�Ӧ�Ӿ�Ľ���
        }
    }
    // 324
    //Լ��3:ÿһ�����ֲ��ظ�
    for (int i = 1; i <= 9; i++) //����ÿһ��
    {
        for (int k = 1; k <= 9; k++) //����ÿ������
        {
            for (int j = 1; j < 9; j++) //����ÿһ��
            {
                for (int p = j + 1; p <= 9; p++) //���ڸ����Ҳ����ȡһ��,�����в���������ͬ����
                {
                    //��������
                    writenumber1 = Transnumber(1, i, j, k);
                    writenumber2 = Transnumber(1, i, p, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(1000 + i * 100 + j * 10 + k), -(1000 + i * 100 + p * 10 + k));
                    //��������
                    writenumber1 = Transnumber(2, i, j, k);
                    writenumber2 = Transnumber(2, i, p, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(2000 + i * 100 + j * 10 + k), -(2000 + i * 100 + p * 10 + k));
                }
            }
        }
    }
    // 6156
    //Լ��4:ÿһ�����ֲ��ظ�
    for (int j = 1; j <= 9; j++) //����ÿһ��
    {
        for (int k = 1; k <= 9; k++) //����ÿ������
        {
            for (int i = 1; i < 9; i++) //����ÿһ��
            {
                for (int p = i + 1; p <= 9; p++) //���ڸ����Ҳ����ȡһ��,�����в���������ͬ����
                {
                    //��������
                    writenumber1 = Transnumber(1, i, j, k);
                    writenumber2 = Transnumber(1, p, j, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(1000 + i * 100 + j * 10 + k), -(1000 + p * 100 + j * 10 + k));
                    //��������
                    writenumber1 = Transnumber(2, i, j, k);
                    writenumber2 = Transnumber(2, p, j, k);
                    fprintf(fp, "%d %d 0\n", -writenumber1, -writenumber2);
                    // fprintf(fp, "%d %d 0\n", -(2000 + i * 100 + j * 10 + k), -(2000 + p * 100 + j * 10 + k));
                }
            }
        }
    }
    // 11988
    //Լ��5:ÿһ��С�Ź������ֲ��ظ�
    for (int num = 1; num <= 9; num++) //����ÿһ������
    {
        for (int i = 0; i < 3; i++) //��i+1��С�Ź������ʼ��Ϊ3*i
        {
            for (int j = 0; j < 3; j++) //��j+1��С�Ź������ʼ��Ϊ3*j
            {
                for (int p = 1; p <= 3; p++) //С�Ź����ڵ��б�
                {
                    for (int q = 1; q < 3; q++) //С�Ź����ڵ��б�
                    {
                        for (int r = q + 1; r < 3; r++) //С�Ź�����q��֮����ȡһ��,���в���ͬ
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
    for (int num = 1; num <= 9; num++) //����ÿһ������
    {
        for (int i = 0; i < 3; i++) //��i+1��С�Ź������ʼ��Ϊ3*i
        {
            for (int j = 0; j < 3; j++) //��j+1��С�Ź������ʼ��Ϊ3*j
            {
                for (int p = 1; p <= 3; p++) //С�Ź����ڵ��б�
                {
                    for (int q = 1; q < 3; q++) //С�Ź����ڵ��б�
                    {
                        for (int r = p + 1; r < 3; r++) //С�Ź�����p��֮����ȡһ��,���в���ͬ
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
    //Լ��6:�غϲ��ֲ�����ֵ
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

//�����Ƿ���Ψһ��
bool issingleans(int a1[][9], int a2[][9], int holes)
{
    bool *s1 = (bool *)malloc(sizeof(bool) * (2000));
    strcpy(Filename, "temp.cnf");
    TransBoardtoCnf(a1, a2, holes);
    ReadCnf(L0);
    bool ans1 = DPLL(L0, s1, 0);
    return ans1;
}

//��Ϸ����
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

//��������Ԫ��Ų�����
int Transnumber(int oak1, int oak2, int oak3, int oak4)
{
    if (oak1 == 1) //�ڵ�һ��������
    {
        return (oak2 - 1) * 81 + (oak3 - 1) * 9 + oak4;
    }
    else if (oak1 == 2)
    {
        return 729 + (oak2 - 1) * 81 + (oak3 - 1) * 9 + oak4;
    }
}

//��������ı�ŷ��س�ԭ���
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
