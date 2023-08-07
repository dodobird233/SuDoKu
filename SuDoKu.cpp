#include"sat_sudoku.h"

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
                if (row < 2) //???此处<3不对为何捏
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

void calSolution(int a[][9], int row, int col, int mark[])
{
    int possible, minRow, minCol, minPos = 10, minMark[10] = {0};
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (!a[i][j])
            {
                for (int k = 0; k <= 9; k++)
                    mark[k] = 0;
                for (int k = 0; k < 3; k++)
                    for (int l = 0; l < 3; l++)
                        mark[a[i / 3 * 3 + k][j / 3 * 3 + l]]++;
                for (int k = 0; k < 9; k++)
                    mark[a[k][j]]++;
                for (int k = 0; k < 9; k++)
                    mark[a[i][k]]++;
                possible = 0;
                for (int k = 0; k <= 9; k++)
                    if (!mark[k])
                        possible++;
                if (!possible)
                {
                    a[row][col] = 0;
                    return;
                }
                if (possible < minPos)
                {
                    minPos = possible;
                    minRow = i;
                    minCol = j;
                    for (int k = 1; k <= 9; k++)
                        minMark[k] = mark[k];
                }
            }
    if (minPos == 10)
    {
        sumOfAns++;
        a[row][col] = 0;
        return;
    }
    for (int i = 1; i <= 9; i++)
        if (!minMark[i])
        {
            a[minRow][minCol] = i;
            calSolution(a, minRow, minCol, mark);
        }
    a[row][col] = 0;
    return;
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
