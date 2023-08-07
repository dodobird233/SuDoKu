#include"sat_sudoku.h"

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
