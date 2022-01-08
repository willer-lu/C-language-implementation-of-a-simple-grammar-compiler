#include"标头.h"
static int state = 1;
int location1 = 0;//扫描到的位置
char str[10000];//储存源码
char ch;//借助ch扫描
char token[25];//暂存单词值
enum style{I,C,K,P,Ch,St,def};//标识符 常数 关键字 界符 字符 字符串 初始定义
static char *KT[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };//关键字表
static char *PT[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]"};//界符表
char variate[16][10] = {};//变量表
int serial = 0;
int count1 = 0, count2 = 0, count3 = 0, count4 = 0,flag=0;
char  CT[10][20], ST[10][20];//分别暂存标识符 常数 字符串
char cT[20];//储存字符

static struct twoele {//Token序列的数据结构
	style kind;//类型
	char value1[25];//单词名
	int value2;//序号
};

static twoele TOKEN[1000];//Token序列

static void init_twoele(twoele*tok) {//初始化Token序列和变量表
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = def;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
}


int tra(char*cmp) {//判断获得单词是否为关键字，是则返回关键字序号
	for (int i = 0; i < 18; i++) {
		if (strcmp(KT[i], cmp) == 0)return i + 1;
	}
	return 0;
}

void printf(twoele*tok) {//输出Token序列
	float ck = 10;
	for (int i = 0; i < 1000; i++) {
		if (tok[i].kind == def) break;//单词序列为空则停止输出
		float number=0;
		switch (tok[i].kind) {//根据单词的种类
		case I:cout <<"(IT," <<tok[i].value2+1<< ")" << endl;  break;
		case C:cout <<"(CT," << tok[i].value2<<")" << endl;
			for (int f = 0;; f++) {
				if (tok[i].value1[f] == '\0')break;
				if (tok[i].value1[f] == '0' && tok[i].value1[f + 1] == '0' && tok[i].value1[f + 2] == '0')break;
				if (tok[i].value1[f] == '.' && tok[i].value1[f + 1] == '0' && tok[i].value1[f + 2] == '0')break;
			}
			break;
		case K:cout <<"(KT," << tok[i].value2 << ")" << endl; break;
		case P:cout <<"(PT," << tok[i].value2+1 << ")" << endl; break;
		case Ch:cout << "(cT,"<< tok[i].value2 << ")"  <<endl;  break;
		case St:cout << "(ST," << tok[i].value2<< ")" << endl;  break;
		}

	}
}
int addvar(char* cmp) {//加入变量表
	int i = 0;
	for (; i < 16; i++) {
		if (strcmp(variate[i], "#") == 0)break;//已到结尾
		if (strcmp(variate[i], cmp) == 0)return i;//已有声明或定义
	}
	strcpy_s(variate[i], cmp);
	return i;
}
void prep(char ch) {//跳过源代码中的注释
	int state = 1;//state表示各种情况
	int cycle = 0;//cycle代指整个注释 
	int back;
	if (ch == '/') {//出现'/'则准备进行过滤注释
		state = 2;
		back = location1 - 1;
	}
	else {
		location1--;
		return;
	}
	ch = str[location1++];
	while (ch != EOF) {
		switch (state) {
		case 2:
			if (ch == '*')//出现'*'则认为已进入注释字段
				state = 3;
			else {
				state = 1;
				cycle = 1;
			}
			break;
		case 3:if (ch == '*')//再一次出现'*'则准备跳出注释字段
			state = 4; break;
		case 4:if (ch == '/')//再一次出现'/'则跳出注释字段，否则继续进行注释过滤
			state = 5;
			  else
			state = 4; break;
		case 5:cycle = 1;
		}
		if (cycle == 1)
			break;//过滤完成
		ch = str[location1++];
	}
	if (state == 1) {
		location1 = back;
	}
	return;
}
static void scanner() {//扫描器
	int i = 0;
	ch = str[location1++];
	for (int i = 0; i < 25; i++)
	{
		token[i] = NULL;//将单词比较窗口清空
	}
	
	prep(ch);//过滤掉程序中的注释段
	ch = str[location1++];

	while (ch == ' ') {//跳过空白
		ch = str[location1++];
	}

	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {//判断为关键字或标识符
		token[i++] = ch;
		ch = str[location1++];
		while ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9')||(ch=='_'))
		{
			token[i++] = ch;
			ch = str[location1++];
		}
		token[i] = '\0';
		location1--;
		TOKEN[serial].kind = I;
		if (tra(token)) {//已获取单词为关键字
			TOKEN[serial].kind = K;
			TOKEN[serial].value2 = tra(token);
		}
		else {//单词为标识符
			strcpy_s(TOKEN[serial].value1, token);
			TOKEN[serial].value2 = addvar(token);
		}
		serial++;//继续判断下一个单词 
	}
	else if (ch >= '0'&&ch <= '9') {//出现数字
		int num = ch - '0';
		bool k = false;
		float dot = 1;
		ch = str[location1++];
		while ((ch >= '0'&&ch <= '9') || (ch == '.')) {
			if (ch == '.') {
				ch = str[location1++];
				k = true;
				continue;
			}
			if (k == true) {
				dot /= 10;
			}
			num = num * 10 + ch - '0';//循环乘10并加入后入位
			ch = str[location1++];
		}
		float numget = (float)num;
		numget *= dot;
		location1--;
		TOKEN[serial].kind = C;
		sprintf_s(TOKEN[serial++].value1, "%f", numget);
		for (int i = 0; i < 20; i++)
		if (strcmp(CT[i], TOKEN[serial-1].value1) == 0) { flag = 2; TOKEN[serial-1].value2 = i + 1; break; }
		if (flag != 2) { strcpy_s(CT[count2], TOKEN[serial-1].value1); count2++; TOKEN[serial-1].value2 = count2; }
	}
	else {
		TOKEN[serial].kind = P;
		switch (ch) {//界符判定
		case '>':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 1;
				location1++;
			}
			else
				TOKEN[serial].value2 = 5;
			serial++;
			break;
		case'<':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 2;
				location1++;
			}
			else
				TOKEN[serial].value2 = 6;
			serial++;
			break;
		case'=':
			if (str[location1] == '=') {
				TOKEN[serial].value2 = 3;
				location1++;
			}
			else
				TOKEN[serial].value2 = 4;
			serial++;
			break;
		case'+':TOKEN[serial++].value2 = 7; break;
		case'-':TOKEN[serial++].value2 = 8; break;
		case'*':TOKEN[serial++].value2 = 9; break;
		case'/':TOKEN[serial++].value2 = 10; break;
		case'{':TOKEN[serial++].value2 = 11; break;
		case'}':TOKEN[serial++].value2 = 12; break;
		case',':TOKEN[serial++].value2 = 13; break;
		case';':TOKEN[serial++].value2 = 14; break;
		case'(':TOKEN[serial++].value2 = 15; break;
		case')':TOKEN[serial++].value2 = 16;  break;
		case'[':TOKEN[serial++].value2 = 17;  break;
		case']':TOKEN[serial++].value2 = 18;  break;
		case'\'': {//字符获取
			TOKEN[serial].kind = Ch;
			token[i++] = str[location1++];
			token[i] = '\0';
			strcpy_s(TOKEN[serial++].value1, token);
			if (str[location1++] != '\'') {
				cout << "单词有问题，字符应为单个存在..." << endl;
				getchar();
				exit(1);
			}
			for (int i = 0; i < 10; i++)
			if (strcmp(CT[i], TOKEN[serial-1].value1) == 0) { flag = 3; TOKEN[serial-1].value2 = i + 1; break; }
			if (flag != 3) { strcpy_s(CT[count3], TOKEN[serial-1].value1); count3++; TOKEN[serial-1].value2 = count3; }
		}break;
		case'"': {//字符串获取
			TOKEN[serial].kind = St;
			for (; i < 25; ) {
				if (str[location1] != '"') {
					token[i++] = str[location1++];
				}
				else break;
			}
			token[i] = '\0';
			if (str[location1++] != '"') {
				cout << "单词有问题，字符串应以”结束" << endl;
				getchar();
				exit(1);
			}
			strcpy_s(TOKEN[serial++].value1, token);
			for (int i = 0; i < 10; i++)
			if (strcmp(CT[i], TOKEN[serial-1].value1) == 0) { flag = 4; TOKEN[serial-1].value2 = i + 1; break; }
			if (flag != 4) { strcpy_s(CT[count4], TOKEN[serial-1].value1); count4++; TOKEN[serial-1].value2 = count4; }
		}break;
		}
	}
}

void tokenaly() {
	system("cls");
	fstream outfile;
	outfile.open("text.txt", ios::out | ios::in);
	for (int i = 0; i < 10; i++) {
		str[i] = NULL;
	}
	if (!outfile) {
		cout << "open error!";
		exit(1);
	}
	outfile.get(str, 10000, EOF);//将代码内容复制进去待分析字符串
	outfile.close();
	init_twoele(TOKEN);//初始化TOKEN序列
	cout << "程序代码如下：" << endl << str << endl << "分析词法序号如下:" << endl;
	while (str[location1] != NULL) {//未结束时进行扫描
		scanner();
	}
	printf(TOKEN);//打印TOKEN串
	ofstream fout;
	fout.open("wordsequence.txt", ios::out);
	for (int i = 0; i < serial; i++) {//输出类型值和单词序列到文件中
		fout << TOKEN[i].kind << endl;
		switch (TOKEN[i].kind)
		{
		case P:
		case K:fout << TOKEN[i].value2<<endl; break;
		case I:
		case Ch:
		case St:
		case C:fout << TOKEN[i].value1<<endl;
		}
	}
	cout << "输出完毕！下面进行语法分析";
	fout.close();
}
