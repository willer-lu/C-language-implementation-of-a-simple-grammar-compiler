#include<fstream>
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<math.h>
using namespace std;
void tokenaly();//�ʷ���������Token����
void yufa();//�﷨����
void analysistable();//���ű���
void translate1();//��Ԫʽ����
void optimize_s();//�Ż�
int tableget(char tra[100][15], char cmp[15]);//׼�����
bool traverse(char tra[100][15], char cmp[15]);//�����ս���ͷ��ս�����鿴�Ƿ����Ҫ����Ԫ��
void inittable();//������ĳ�ʼ��
void tableend();//��ɷ�����
void initproduct();//��ʼ������ʽ�ṹ
void targetcode();//Ŀ���������
void range(char c[30]);//����
