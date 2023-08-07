#include"sat_sudoku.h"
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
    int choice1=1, mode;
    while (choice1)
    {
        printf("Please choose a mode: 1.mostfrequently  2.JW\n");
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
        else
        {
            choice1=1;
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