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
