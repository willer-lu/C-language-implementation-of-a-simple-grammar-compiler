#include<fstream>
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<math.h>
using namespace std;
void tokenaly();//词法分析生成Token序列
void yufa();//语法分析
void analysistable();//符号表构造
void translate1();//四元式翻译
void optimize_s();//优化
int tableget(char tra[100][15], char cmp[15]);//准备填表
bool traverse(char tra[100][15], char cmp[15]);//遍历终结符和非终结符表，查看是否存在要加入元素
void inittable();//分析表的初始化
void tableend();//完成分析表
void initproduct();//初始化产生式结构
void targetcode();//目标代码生成
void range(char c[30]);//排序
