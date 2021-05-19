#include <bits/stdc++.h>
#include <conio.h>
#include <ctime>
#include <windows.h>
#ifndef consider_step
#define consider_step 5 //考虑步数
#define total_num 10    //棋手人数
#define size 15         //棋盘大小
#define generations 10  //进化代数
#define mod (1 + total_num / 2) * total_num / 4
using namespace std;
int consider_range; // 考虑的范围
int previousx, previousy;
#endif