#include"标头.h"
#include"windows.h"
char pro[100][200];//储存产生式
int table[100][100];//储存分析表
char row[100][15];
char line[100][15];
char note[1000][15];//记录节点动作信息
int y = 0;
int x;
int v[10];
enum style { I, C, K, P, Ch, St, default };
enum type { vnk, vtk, nothing, def };//vn为非终结符 vt为终结符 nothing表示空
char* keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };//关键字表
char* delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };//界符表
int gng;//当前产生式序号

struct pronode {//产生式节点
	type it;
	char symbol[15];
};
struct product {//产生式数据结构
	char vn[15];
	pronode itself[10];
};
product c[100];//产生式序列
struct twoele {//Token数据结构
	style kind;
	char value1[25];
	int value2;
};
twoele TOKEN[1000];//Token序列
void init_twoele(twoele* tok) {//初始化二元式
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = default;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	return;
}
bool traverse(char tra[100][15], char cmp[15]) {//遍历终结符和非终结符表，查看是否存在要加入元素
	int i = 0;
	while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0) {
			return false;
		}
		if (strcmp("$", cmp) == 0) {
			return false;
		}//无需加入元素
	}
	return true;
}
bool special(char cmp[10]) {//查看是否存在特殊字符
	int i = 0;
	while (strcmp(row[i], "#") != 0) {
		if (strcmp(row[i++], cmp) == 0) {
			return false;
		}
		if (strcmp("$", cmp) == 0) {
			return false;
		}
	}
	return true;
}
int tableprep(char tra[100][15], char cmp[15]) {//准备填表
	int i = 0;
	while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0)break;
	}
	i--;
	if (strcmp(tra[i], "#") == 0) {
		cout << "语法有问题...";
		getchar();
		exit(0);
	}
	return (i);//完成定位
}
int tableget(char tra[100][15], char cmp[15]) {//准备填表
	int i = 0;
	if (strcmp(cmp, "#") == 0) {
		return y;
	}
	else while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0) {
			i--;
			break;
		}
	}
	if (strcmp(tra[i], "#") == 0) {
		return -1;
	}
	return (i);
}
int special1(char cmp[10]) {
	int i = 0;
	while (strcmp(line[i], "#") != 0) {
		if (strcmp(line[i++], cmp) == 0)break;
	}
	i--;
	if (strcmp(line[i], "#") == 0) {
		cout << "语法有问题...";
		getchar();
		exit(0);
	}
	return (i);
}
void inittable() {//分析表的初始化
	int hang;
	char cmp[15];
	for (int i = 0; i < 100; i++) {
		row[i][0] = '#';
		line[i][0] = '#';
	}
	for (int i = 0;; i++) {
		if (strcmp(pro[i], "#") == 0) {//#代表一个产生式末尾
			break;
			hang = i;
		}
		int j = 0;
		while (pro[i][j++] != '%') {//%代表产生式箭头
		}
		strncpy_s(row[i], pro[i], j - 1);
	}
	for (int i = 0;; i++) {
		if (strcmp(pro[i], "#") == 0) {
			break;
			hang = i;
		}
		int j = 0;
		while (pro[i][j++] != '%') {//找到产生式箭头
		}
		while (pro[i][j] != '#') {
			int length = 0;
			int length1 = j;
			while ((pro[i][j] != '&') && (pro[i][j] != '|')) {//且与或
				j++;
				length++;
			}
			strncpy_s(cmp, pro[i] + j - length, length);
			j++;
			if (traverse(line, cmp) && traverse(row, cmp)) {
				strcpy_s(line[y++], cmp);
			}
		}
		x = i;
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			table[i][j] = -1;
		}
	}
	return;
}
void nor(int cmp) {//填表
	int i = 0;
	for (; i < 10; i++) {//判断表中是否有cmp 没有则填入
		if (v[i] == -1)break;
		else if (v[i] == cmp)return;
	}
	v[i] = cmp;
	return;
}
void cyclefirst(int i) {//查first集合
	int j = 0;
	if (c[i].itself[0].it != vtk) {
		for (; j < gng; j++) {
			if (strcmp(c[i].itself[0].symbol, c[j].vn) == 0)
				cyclefirst(j);//查首字符
		}
	}
	if (j == gng)return;
	int cmp = tableprep(line, c[i].itself[0].symbol);
	nor(cmp);//填表
	return;
}
void follow(int i, int x1, int local);//提前声明follow函数
void first(int x1, int i, int local) {//根据规则求first集合 
	if (c[i].itself[0].it == nothing) {//右部为空则求左部follow集
		follow(i, x1, local);
		return;
	}
	for (int i = 0; i < 10; i++) {
		v[i] = -1;
	}
	cyclefirst(i);
	for (int i = 0; i < 10; i++) {
		if (v[i] == -1)break;
		table[x1][v[i]] = local;
	}
	return;
}
void follow(int i, int x1, int local) {//求follow集合 即按照相应规则
	char cmp[15];
	for (int i = 0; i < 10; i++) {
		v[i] = -1;
	}
	strcpy_s(cmp, c[i].vn);
	for (int j = 0; j < gng; j++) {
		int s = -1;
		if (strcmp(c[j].vn, "#") == 0) {
			break;
		}
		for (int k = 0; k < 10; k++) {
			if (strcmp(c[j].itself[k].symbol, "#") == 0)break;
			if (strcmp(c[j].itself[k].symbol, cmp) == 0) {
				s = k + 1;
				break;
			}
		}
		if (s != -1) {
			if (c[j].itself[s].it == vtk) {//终结符直接填
				s = special1(c[j].itself[s].symbol);
				nor(s);
			}
			else if (c[j].itself[s].it == vnk) {//非终结符继续按规则求
				for (int n = 0; n < gng; n++) {
					if (strcmp(c[j].itself[s].symbol, c[n].vn) == 0)
						first(x1, n, local);
				}
			}
			else {
				if (strcmp(cmp, c[j].vn) == 0) {
					continue;
				}
				else follow(j, x1, local);//为空继续求左部follow集
			}
		}
	}
	if (strcmp(cmp, row[0]) == 0) {
		int s = y;
		nor(y);
	}
	for (int i = 0; i < 10; i++) {
		if (v[i] == -1)break;
		table[x1][v[i]] = local;
	}
	return;
}
void initproduct() {//初始化产生式结构
	for (int i = 0; i < 100; i++) {
		strcpy_s(c[i].vn, "#");
		for (int j = 0; j < 10; j++) {
			c[i].itself[j].it = def;
			strcpy_s(c[i].itself[j].symbol, "#");
		}
	}
	return;
}
void getselect(int n) {//判断为求first或follow集合
	for (int i = 0; i < n; i++) {
		int x1 = tableprep(row, c[i].vn);
		first(x1, i, i);
	}
	return;
}
void tableend() {//完成分析表
	int o;
	int m;
	int n = 0;
	for (int i = 0; i <= x; i++) {
		int j = 0;
		o = 0;
		while (pro[i][j++] != '%') {
		}
		strncpy_s(c[n].vn, pro[i], j - 1);//非终结符
		m = j;
		for (;; j++) {//遍历每一条产生式  根据不同情况填写分析表
			if (pro[i][j] == '&') {
				strncpy_s(c[n].itself[o].symbol, pro[i] + m, j - m);
				m = j + 1;
				if (!traverse(row, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vnk;
				}
				else if (!traverse(line, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vtk;
				}
				if (strcmp("$", c[n].itself[o].symbol) == 0)  c[n].itself[o].it = nothing;
				o++;
			}
			if (pro[i][j] == '|') {
				strncpy_s(c[n].itself[o].symbol, pro[i] + m, j - m);
				m = j + 1;
				if (!traverse(row, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vnk;
				}
				else if (!traverse(line, c[n].itself[o].symbol)) {
					c[n].itself[o].it = vtk;
				}
				if (strcmp("$", c[n].itself[o].symbol) == 0)  c[n].itself[o].it = nothing;
				n++;
				o = 0;
				strcpy_s(c[n].vn, c[n - 1].vn);
			}
			if (pro[i][j] == '#') {
				break;
			}
		}
		n++;
	}
	gng = n;
	getselect(n);
	return;
}
typedef struct Node//栈中元素节点
{
	char element[200];
	struct Node* pNext;
}NODE, * PNODE;
typedef struct Stack//栈 包含栈顶和栈底元素
{
	PNODE pTop;
	PNODE pBottom;
}STACK, * PSTACK;
void push(PSTACK out, char* in) {//非终结符入分析栈
	NODE* k = new NODE;
	strcpy_s(k->element, in);
	k->pNext = out->pTop;
	out->pTop = k;
	return;
}
bool empty(PSTACK out) {//检查栈是否为空
	if ((out->pBottom == out->pTop) && (out->pBottom == NULL))
		return true;
	else
		return false;
}
char* pop(PSTACK out) {//获取栈顶元素用于比较
	if (empty(out)) {
		cout << "error!语法有问题" << endl;
		getchar();
		exit(1);
	}
	return out->pTop->element;
}
void freepop(PSTACK out) {//栈顶元素出栈
	PNODE clean = out->pTop;
	out->pTop = out->pTop->pNext;
	free(clean);
	return;
}
void inistack(PSTACK begin) {//初始化分析栈(#+开始符号)
	begin->pBottom = new NODE;
	strcpy_s(begin->pBottom->element, "#");
	begin->pBottom->pNext = NULL;
	begin->pTop = new NODE;
	begin->pTop->pNext = begin->pBottom;
	strcpy_s(begin->pTop->element, row[0]);
	return;
}

int read(twoele gg) {//语法匹配过程
	int length;
	switch (gg.kind) {
	case P:length = strlen(delimiters[gg.value2 - 1]); break;
	case K:length = strlen(keyword[gg.value2 - 1]); break;
	case I:if (strcmp(gg.value1, "") == 0) {
		length = 1;
		break;
	}
	case Ch:
	case St:
	case C:length = strlen(gg.value1); break;
	case default:length = 1; break;
	}
	return length;
}
void analysis(int m, int n, PSTACK out) {//按分析表进行分析栈匹配 即LL(1)分析过程
	freepop(out);
	int k = table[m][n];
	int end;
	for (int i = 0;; i++) {
		if (strcmp(c[k].itself[i].symbol, "#") == 0) {
			end = i;
			break;
		}
	}
	for (int i = end - 1; i >= 0; i--) {
		if (c[k].itself[i].it == nothing)
		{
			break;
		}
		push(out, c[k].itself[i].symbol);
	}
	return;
}
void gra(PSTACK begin) {//语法分析主体程序
	int i = 0;
	int gh = 1;
	while ((TOKEN[i].kind != default) || (strcmp(begin->pTop->element, "#") != 0)) {//当分析栈与待分析序列都为结束符时结束语法分析
		char* cmp = pop(begin);
		if ((strcmp(begin->pTop->element, "if") == 0) || (strcmp(begin->pTop->element, "while") == 0) || (strcmp(begin->pTop->element, "else") == 0) || (strcmp(begin->pTop->element, "=") == 0) || (strcmp(begin->pTop->element, "+") == 0) || (strcmp(begin->pTop->element, "-") == 0) || (strcmp(begin->pTop->element, "*") == 0) || (strcmp(begin->pTop->element, "/") == 0) || (strcmp(begin->pTop->element, "id") == 0) || (strcmp(begin->pTop->element, "number") == 0) || (strcmp(begin->pTop->element, "ch") == 0) || (strcmp(begin->pTop->element, "we") == 0) || (strcmp(begin->pTop->element, "do") == 0) || (strcmp(begin->pTop->element, "ie") == 0) || (strcmp(begin->pTop->element, ">") == 0) || (strcmp(begin->pTop->element, "<") == 0) || (strcmp(begin->pTop->element, "==") == 0) || (strcmp(begin->pTop->element, ">=") == 0) || (strcmp(begin->pTop->element, "<=") == 0) || (strcmp(begin->pTop->element, "return") == 0) || (strcmp(begin->pTop->element, "cout") == 0) || (strcmp(begin->pTop->element, "cin") == 0)) {
			strcpy_s(note[gh++], begin->pTop->element);//终结符情况
		}
		char cc[15];
		int m = tableget(row, begin->pTop->element);
		int n;
		switch (TOKEN[i].kind) {//根据TOKEN式类型查找分析表
		case I: {
			if (strcmp(TOKEN[i].value1, "") == 0) {
				n = y;
				break;
			}
			n = tableget(line, "id"); break;
		}
		case P:strcpy_s(cc, delimiters[TOKEN[i].value2 - 1]); n = tableget(line, cc); break;
		case K:strcpy_s(cc, keyword[TOKEN[i].value2 - 1]); n = tableget(line, cc); break;
		case Ch:n = tableget(line, "ch"); break;
		case St:n = tableget(line, "string"); break;
		case C:n = tableget(line, "number"); break;
		case default:n = tableget(line, "#"); break;
		}
		if (m >= 0 && m <= x && n >= 0 && n <= y)
		{
			analysis(m, n, begin);//分析过程
		}
		else if (strcmp(begin->pTop->element, line[n]) == 0) {//匹配成功
			int rab = read(TOKEN[i++]);
			freepop(begin);
		}
		else {
			cout << "出现错误，错误在语法分析出入栈过程中" << endl;
			getchar();
			exit(1);
		}
		read(TOKEN[i]);
		if ((strcmp(begin->pTop->element, "#") == 0) && TOKEN[i].kind == default) {
			cout << "#匹配成功" << endl;
			return;
		}
	}
	return;
}
void tableprint() {//将LL(1)分析表输入到文件中
	ofstream selectout;
	selectout.open("analysistable.txt", ios::out);
	int i = 0;
	if (!selectout) {
		cout << "open error!";
		exit(1);
	}
	selectout << "vn/vt ";
	for (int i = 0; i <= y; i++) 
		selectout << line[i] << " ";
	selectout << endl;
	for (int i = 0; i <= x; i++) {
		selectout << row[i] << " ";
		int k = strlen(row[i]);
		for (int j = 0; j <= y; j++) {
			if (table[i][j] + 1 >= 10) 
				selectout << table[i][j] + 1 << " ";
			else
				selectout << table[i][j] + 1 << " ";
		}
		selectout << endl;
	}
	selectout.close();
	return;
}
bool val(char cmp[15]) {//跳过界符
	if (strcmp(cmp, "+") == 0) {
		return false;
	}
	else if (strcmp(cmp, "-") == 0) {
		return false;
	}
	else if (strcmp(cmp, "*") == 0) {
		return false;
	}
	else if (strcmp(cmp, "/") == 0) {
		return false;
	}
	else if (strcmp(cmp, ">") == 0) {
		return false;
	}
	else if (strcmp(cmp, "<") == 0) {
		return false;
	}
	else if (strcmp(cmp, "==") == 0) {
		return false;
	}
	else if (strcmp(cmp, ">=") == 0) {
		return false;
	}
	else if (strcmp(cmp, "<=") == 0) {
		return false;
	}
	else if (strcmp(cmp, "=") == 0) {
		return false;
	}
	return true;
}
void initnote() {//初始化节点
	for (int i = 0; i < 1000; i++) {
		strcpy_s(note[i], "#");
	}
	return;
}
void yufa() {
	tokenaly();//先进行词法分析得到TOKEN序列
	PSTACK alystack = new STACK;//分析栈
	int i = 0;
	ifstream outfile;
	outfile.open("wordsequence.txt", ios::in);//将文件中的TOKEN序列写入到TOKEN数组中
	if (!outfile) {
		cout << "open error!";
		exit(1);
	}
	init_twoele(TOKEN);
	while (!outfile.eof()) {
		int sty;
		outfile >> sty;
		TOKEN[i].kind = (style)sty;
		switch (TOKEN[i].kind) {
		case P:
		case K:outfile >> TOKEN[i++].value2; break;
		case I:
		case Ch:
		case St:
		case C:outfile >> TOKEN[i++].value1; break;
		}
	}
	TOKEN[i - 1].kind = default;
	ifstream gram;
	gram.open("grammar.txt", ios::in);//打开语法分析所需的文法文件
	for (int i = 0;; i++) {//读取产生式
		gram >> pro[i];
		if (strcmp(pro[i], "#") == 0) {
			break;
		}
	}
	initnote();//节点初始化
	gram.close();
	outfile.close();
	inittable();//分析表初始化
	initproduct();//产生式初始化
	tableend();//构造分析表
	tableprint();//将分析表输入到文件中
	inistack(alystack);//初始化分析栈
	cout << "进行语法分析判断句子是否正确" << endl;
	gra(alystack);//语法分析过程
	ofstream act;//将动作序列写入文件
	act.open("action.txt", ios::out);
	for (int i = 1;; i++) {
		if (strcmp(note[i], "#") == 0) {
			act << note[i] << endl;
			break;
		}
		act << note[i] << endl;
		if (strcmp(note[i], "cout") == 0)i = i + 2;
		if (strcmp(note[i], "cin") == 0)i = i + 2;
	}
	cout << endl;
	act.close();
}



