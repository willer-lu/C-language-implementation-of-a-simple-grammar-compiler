#include"标头.h"
enum KIND { I, C, K, P, Ch, St, default };//TOKEN种类
static char *KT[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return" ,"cout","cin"};//关键字表
static char *PT[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };//界符表
char variate[16][15] = {};//变量表
enum TYP{in,real,ch,b,default1};//类型，包括int，float,char,bool型
enum CAT{f,con,t,d,v,vn,vf,default2};//种类，包括f函数，c常量，t类型，d域名，v变量，vn换名形参，vf，赋值形参
enum ADDR{PFINFL,LENL,VALL,default3};//地址表，包括函数表，活动记录表
int idlocate[16];//记录标识符在代码中首次出现的位置
int hg = 0;

struct TOKEN {//TOKEN数据结构
	 KIND kind;
	char value1[25];
	int value2;
};
TOKEN token[1000];//词法序列

struct symbol {//符号表
	char name[15];//名字
	TYP type;//类型
	CAT kind;//种类
	ADDR addr;//地址
};
symbol environment[100];//符号表
struct pfinfl {//函数表
	int level;//层级
	int off;//区距
	int fn;//参数个数
	symbol para[5];//参数表
	int entry;//入口
};

struct vall {//活动记录表(链表结构)
	char name[15];//名字
	char name1[15];//内容
	int low;//下界
	int up;//上界
	struct vall *next;
};
vall *firstnode=new vall;

void valladd(vall* head, int t1, int t2, char*id,char*id1) {//添加活动记录
	vall *a = new vall;
	a->low = t1;
	a->up = t2;
	strcpy_s(a->name, id);
	strcpy_s(a->name1, id1);
	a->next = NULL;
	vall *b = head;
	while (b->next != NULL)b = b->next;
	b->next = a;
	return;
}

void initvall(vall* head) {//初始化活动记录
	head->low = 0;
	head->up = 0;
	strcpy_s(head->name1, "");
	strcpy_s(head->name, "old sp");
	head->next = NULL;
	valladd(head, 1, 1, "返回地址","");
	valladd(head, 2, 2, "全局display","");
	return;
}

void vallprint(vall*head) {//打印活动记录表
	vall *pr = head;
	ofstream valladd;
	valladd.open("vall.txt", ios::out);//打开活动记录文件
	cout << "活动记录表如下:" << endl << "名字"<<'\t'<<"        "<<"标识符"<<'\t'<<"      "<<"下界存储"<<'\t'<<"上界存储" << endl;
	while (pr!= NULL) {
		cout << pr->name;
		valladd << pr->name<<endl;
		int c = strlen(pr->name);
		while (c < 16) {
			cout << " ";
			c++;
		}
		cout << pr->name1 << '\t' << "      ";
		valladd << pr->name1 << endl;
		cout << pr->low << "      " << '\t';
		valladd << pr->low << endl;
		cout << pr->up  << endl;
		valladd << pr->up << endl;
		pr = pr->next;
	}
	valladd.close();
	return;
}



void initsymbol(symbol*environment,int length) {//初始化符号表
	int i = 0;
	while (i<length) {
		strcpy_s(environment[i].name, "#");
		environment[i].type = default1;
		environment[i].kind = default2;
		environment[i++].addr = default3;
	}
	return;
}

int getlen(symbol k) {//获取长度
	switch (k.type) {
	case in:return 4;
	case real:return 4;
	case ch:return 1;
	}
	return 0;
}

void funcpro(pfinfl*function) {
	initsymbol(function->para, 5);
	function->fn = 0;
	function->level = 0;
	function->off = 3;
	function->entry = -1;
	int c = 0;
	if (token[idlocate[0] + 1].kind != P) {
		cout << "函数有问题...";
		getchar();
		exit(1);
	}
	int end=0;
	for (int i = idlocate[0] + 2;; i++) {
		if ((token[i].kind == P) && (token[i].value2 == 16)) {
			break;
			end = i;
		}
		else if ((token[i].kind == P) && (token[i].value2 == 13))continue;
		function->fn++;
		strcpy_s(function->para[c].name, token[i + 1].value1);
		switch (token[i].value2) {
		case 1:function->para[c].type = in; break;
		case 2:function->para[c].type = real; break;
		case 3:function->para[c].type = ch; break;
		}
		function->para[c++].kind = v;
		i++;
	}
	c = 0;
	char add[10];
	_itoa_s(function->fn, add, 10);
	valladd(firstnode, 3, 3, "参数个数", add);
	int st = 4;
	for (int i = function->fn; i > 0; i--) {
		if (i == function->fn) {
			valladd(firstnode, st, st - 1 + getlen(function->para[c]), "形式单元", function->para[c].name);
			st = st - 1+ getlen(function->para[c]);
			c++;
			continue;
		}
		valladd(firstnode, st, st - 1 + getlen(function->para[c]), "", function->para[c].name);
		st = st + getlen(function->para[c]);
		c++;
	}
	valladd(firstnode, st, st++, "display表", "");
	for (int i = 0;; i++) {
		if (idlocate[i] > end) {
			end = i;
			break;
		}
		if (idlocate[i] == -1)return;
	}
	for (int i = end;; i++) {
		if (idlocate[i] == -1)break;
		int ge;
		switch (token[idlocate[i]-1].value2) {
		case 1:
		case 2:ge = 4; break;
		case 3:ge = 1;
		}
		if (i == end) {
			valladd(firstnode, st,st+ ge-1 , "局部变量", token[idlocate[i]].value1);
			st = st + ge - 1;
			continue;
		}
		valladd(firstnode, st, st + ge - 1, "", token[idlocate[i]].value1);
		st = st + ge;
	}
	return;
}

static void init_token(TOKEN*token) {//初始化二元式，同时将变量表初始化
	for (int i = 0; i < 1000; i++) {
		token[i].kind = default;
		strcpy_s(token[i].value1, "#");
		token[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
	return;
}

void tableadd(char idtable[16][15],char*cmp,int loc) {//标识符表元素添加
	int i = 0;
	for (;; i++) {
		if (strcmp(idtable[i], "#") == 0)break;
		if (strcmp(idtable[i], cmp) == 0)return;
	}
	strcpy_s(idtable[i], cmp);
	idlocate[hg++] = loc;
	return;
}

void finshanalysis() {//完成符号表
	for (int g = 0;; g++) {
		if (strcmp(variate[g], "#") == 0)break;
		if (g == 0)environment[g].kind = f;
		else environment[g].kind = v;
		strcpy_s(environment[g].name, variate[g]);
		if (strcmp(KT[token[idlocate[g] - 1].value2-1],"int")==0) {
			environment[g].type = in;
		}
		else if (strcmp(KT[token[idlocate[g] - 1].value2 - 1], "float") == 0) {
			environment[g].type = real;
		}
		else if (strcmp(KT[token[idlocate[g] - 1].value2 - 1], "char") == 0){
			environment[g].type = ch;
		}
		else if (strcmp(KT[token[idlocate[g] - 1].value2 - 1], "bool") == 0){
			environment[g].type = b;
		}
		else {
			cout << "变量定义有问题...";
			getchar();
			exit(1);
		}
	}
	return;
}

void print(symbol*out,int pr) {//打印表的内容 将符号表输出到文件中
	if (pr == 1) {
		ofstream symboltable;
		symboltable.open("symboltable.txt", ios::out);
		for (int g = 0;; g++) {
			if (out[g].type == default1)break;
			cout << out[g].name;
			symboltable << out[g].name << endl;
			int j = strlen(out[g].name);
			while (j < 10) {
				cout << " ";
				j++;
			}
			switch (out[g].type) {
			case in:cout << "int        "; symboltable << "int" << endl; break;
			case real:cout << "float      "; symboltable << "float" << endl;  break;
			case ch:cout << "char       "; symboltable << "char" << endl; break;
			}
			switch (out[g].kind) {
			case f:cout << "function  "; symboltable << "function" << endl; break;
			case v:cout << "var       "; symboltable << "var" << endl; break;
			}
			switch (out[g].addr) {
			case PFINFL:cout << "PFINFL    "; symboltable << "PFINFL" << endl; break;
			case LENL:cout << "LENL      "; symboltable << "LENL" << endl; break;
			case VALL:cout << "VALL      "; symboltable << "VALL" << endl;  break;
			default:cout << "      "; symboltable << " " << endl;
			}
			cout << endl;
		}
		symboltable.close();
	}
	else {
		for (int g = 0;; g++) {
			if (out[g].type == default1)break;
			cout << out[g].name;
			int j = strlen(out[g].name);
			while (j < 10) {
				cout << " ";
				j++;
			}
			switch (out[g].type) {
			case in:cout << "int        ";break;
			case real:cout << "float      ";   break;
			case ch:cout << "char       ";  break;
			}
			switch (out[g].kind) {
			case f:cout << "function  "; break;
			case v:cout << "var       ";  break;
			}
			switch (out[g].addr) {
			case PFINFL:cout << "PFINFL    "; break;
			case LENL:cout << "LENL      "; break;
			case VALL:cout << "VALL      ";  break;
			}
			cout << endl;
		}
	}
}

void tabprint(pfinfl*function) {//打印符号表、函数表、活动记录表、长度表
	cout << "符号表组织如下" << endl;
	cout << "名字      类型       种类        地址" << endl;
	print(environment,1);
	cout << "函数表如下(包含参数表）:" << endl;
	cout << "层次号    区距离     参数个数    入口地址" << endl;
	cout << function->level << "         " << function->off << "          " << function->fn << "           " << "未定义" << endl;
	cout << "名字      类型       种类        地址" << endl;
	print(function->para,0);
	vallprint(firstnode);
	return;
}

void analysistable() {
	pfinfl *func=new pfinfl;
	ifstream outfile;
	outfile.open("wordsequence.txt", ios::in);
	for (int i = 0; i < 16; i++) {//初始化标识符位置记录
		idlocate[i] = -1;
	}
	int i = 0;
	if (!outfile) {
		cout << "open error!";
		getchar();
		getchar();
		exit(1);
	}
	init_token(token);
	while (!outfile.eof()) {
		int sty;
		outfile >> sty;
		token[i].kind = (KIND)sty;
		switch (token[i].kind) {
		case P:
		case K:outfile >> token[i++].value2; break;
		case I: {
			outfile >> token[i++].value1;
			tableadd(variate, token[i-1].value1,i-1);
			break;
		}
		case Ch:
		case St:
		case C:outfile >> token[i++].value1; break;
		}
	}
	token[i - 1].kind = default;
	initsymbol(environment,100);
	initvall(firstnode);
	finshanalysis();
	funcpro(func);
	tabprint(func);
	return;
}