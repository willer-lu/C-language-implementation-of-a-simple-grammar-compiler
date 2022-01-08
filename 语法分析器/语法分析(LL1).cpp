#include"��ͷ.h"
#include"windows.h"
char pro[100][200];//�������ʽ
int table[100][100];//���������
char row[100][15];
char line[100][15];
char note[1000][15];//��¼�ڵ㶯����Ϣ
int y = 0;
int x;
int v[10];
enum style { I, C, K, P, Ch, St, default };
enum type { vnk, vtk, nothing, def };//vnΪ���ս�� vtΪ�ս�� nothing��ʾ��
char* keyword[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };//�ؼ��ֱ�
char* delimiters[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]" };//�����
int gng;//��ǰ����ʽ���

struct pronode {//����ʽ�ڵ�
	type it;
	char symbol[15];
};
struct product {//����ʽ���ݽṹ
	char vn[15];
	pronode itself[10];
};
product c[100];//����ʽ����
struct twoele {//Token���ݽṹ
	style kind;
	char value1[25];
	int value2;
};
twoele TOKEN[1000];//Token����
void init_twoele(twoele* tok) {//��ʼ����Ԫʽ
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = default;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	return;
}
bool traverse(char tra[100][15], char cmp[15]) {//�����ս���ͷ��ս�����鿴�Ƿ����Ҫ����Ԫ��
	int i = 0;
	while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0) {
			return false;
		}
		if (strcmp("$", cmp) == 0) {
			return false;
		}//�������Ԫ��
	}
	return true;
}
bool special(char cmp[10]) {//�鿴�Ƿ���������ַ�
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
int tableprep(char tra[100][15], char cmp[15]) {//׼�����
	int i = 0;
	while (strcmp(tra[i], "#") != 0) {
		if (strcmp(tra[i++], cmp) == 0)break;
	}
	i--;
	if (strcmp(tra[i], "#") == 0) {
		cout << "�﷨������...";
		getchar();
		exit(0);
	}
	return (i);//��ɶ�λ
}
int tableget(char tra[100][15], char cmp[15]) {//׼�����
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
		cout << "�﷨������...";
		getchar();
		exit(0);
	}
	return (i);
}
void inittable() {//������ĳ�ʼ��
	int hang;
	char cmp[15];
	for (int i = 0; i < 100; i++) {
		row[i][0] = '#';
		line[i][0] = '#';
	}
	for (int i = 0;; i++) {
		if (strcmp(pro[i], "#") == 0) {//#����һ������ʽĩβ
			break;
			hang = i;
		}
		int j = 0;
		while (pro[i][j++] != '%') {//%�������ʽ��ͷ
		}
		strncpy_s(row[i], pro[i], j - 1);
	}
	for (int i = 0;; i++) {
		if (strcmp(pro[i], "#") == 0) {
			break;
			hang = i;
		}
		int j = 0;
		while (pro[i][j++] != '%') {//�ҵ�����ʽ��ͷ
		}
		while (pro[i][j] != '#') {
			int length = 0;
			int length1 = j;
			while ((pro[i][j] != '&') && (pro[i][j] != '|')) {//�����
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
void nor(int cmp) {//���
	int i = 0;
	for (; i < 10; i++) {//�жϱ����Ƿ���cmp û��������
		if (v[i] == -1)break;
		else if (v[i] == cmp)return;
	}
	v[i] = cmp;
	return;
}
void cyclefirst(int i) {//��first����
	int j = 0;
	if (c[i].itself[0].it != vtk) {
		for (; j < gng; j++) {
			if (strcmp(c[i].itself[0].symbol, c[j].vn) == 0)
				cyclefirst(j);//�����ַ�
		}
	}
	if (j == gng)return;
	int cmp = tableprep(line, c[i].itself[0].symbol);
	nor(cmp);//���
	return;
}
void follow(int i, int x1, int local);//��ǰ����follow����
void first(int x1, int i, int local) {//���ݹ�����first���� 
	if (c[i].itself[0].it == nothing) {//�Ҳ�Ϊ��������follow��
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
void follow(int i, int x1, int local) {//��follow���� ��������Ӧ����
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
			if (c[j].itself[s].it == vtk) {//�ս��ֱ����
				s = special1(c[j].itself[s].symbol);
				nor(s);
			}
			else if (c[j].itself[s].it == vnk) {//���ս��������������
				for (int n = 0; n < gng; n++) {
					if (strcmp(c[j].itself[s].symbol, c[n].vn) == 0)
						first(x1, n, local);
				}
			}
			else {
				if (strcmp(cmp, c[j].vn) == 0) {
					continue;
				}
				else follow(j, x1, local);//Ϊ�ռ�������follow��
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
void initproduct() {//��ʼ������ʽ�ṹ
	for (int i = 0; i < 100; i++) {
		strcpy_s(c[i].vn, "#");
		for (int j = 0; j < 10; j++) {
			c[i].itself[j].it = def;
			strcpy_s(c[i].itself[j].symbol, "#");
		}
	}
	return;
}
void getselect(int n) {//�ж�Ϊ��first��follow����
	for (int i = 0; i < n; i++) {
		int x1 = tableprep(row, c[i].vn);
		first(x1, i, i);
	}
	return;
}
void tableend() {//��ɷ�����
	int o;
	int m;
	int n = 0;
	for (int i = 0; i <= x; i++) {
		int j = 0;
		o = 0;
		while (pro[i][j++] != '%') {
		}
		strncpy_s(c[n].vn, pro[i], j - 1);//���ս��
		m = j;
		for (;; j++) {//����ÿһ������ʽ  ���ݲ�ͬ�����д������
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
typedef struct Node//ջ��Ԫ�ؽڵ�
{
	char element[200];
	struct Node* pNext;
}NODE, * PNODE;
typedef struct Stack//ջ ����ջ����ջ��Ԫ��
{
	PNODE pTop;
	PNODE pBottom;
}STACK, * PSTACK;
void push(PSTACK out, char* in) {//���ս�������ջ
	NODE* k = new NODE;
	strcpy_s(k->element, in);
	k->pNext = out->pTop;
	out->pTop = k;
	return;
}
bool empty(PSTACK out) {//���ջ�Ƿ�Ϊ��
	if ((out->pBottom == out->pTop) && (out->pBottom == NULL))
		return true;
	else
		return false;
}
char* pop(PSTACK out) {//��ȡջ��Ԫ�����ڱȽ�
	if (empty(out)) {
		cout << "error!�﷨������" << endl;
		getchar();
		exit(1);
	}
	return out->pTop->element;
}
void freepop(PSTACK out) {//ջ��Ԫ�س�ջ
	PNODE clean = out->pTop;
	out->pTop = out->pTop->pNext;
	free(clean);
	return;
}
void inistack(PSTACK begin) {//��ʼ������ջ(#+��ʼ����)
	begin->pBottom = new NODE;
	strcpy_s(begin->pBottom->element, "#");
	begin->pBottom->pNext = NULL;
	begin->pTop = new NODE;
	begin->pTop->pNext = begin->pBottom;
	strcpy_s(begin->pTop->element, row[0]);
	return;
}

int read(twoele gg) {//�﷨ƥ�����
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
void analysis(int m, int n, PSTACK out) {//����������з���ջƥ�� ��LL(1)��������
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
void gra(PSTACK begin) {//�﷨�����������
	int i = 0;
	int gh = 1;
	while ((TOKEN[i].kind != default) || (strcmp(begin->pTop->element, "#") != 0)) {//������ջ����������ж�Ϊ������ʱ�����﷨����
		char* cmp = pop(begin);
		if ((strcmp(begin->pTop->element, "if") == 0) || (strcmp(begin->pTop->element, "while") == 0) || (strcmp(begin->pTop->element, "else") == 0) || (strcmp(begin->pTop->element, "=") == 0) || (strcmp(begin->pTop->element, "+") == 0) || (strcmp(begin->pTop->element, "-") == 0) || (strcmp(begin->pTop->element, "*") == 0) || (strcmp(begin->pTop->element, "/") == 0) || (strcmp(begin->pTop->element, "id") == 0) || (strcmp(begin->pTop->element, "number") == 0) || (strcmp(begin->pTop->element, "ch") == 0) || (strcmp(begin->pTop->element, "we") == 0) || (strcmp(begin->pTop->element, "do") == 0) || (strcmp(begin->pTop->element, "ie") == 0) || (strcmp(begin->pTop->element, ">") == 0) || (strcmp(begin->pTop->element, "<") == 0) || (strcmp(begin->pTop->element, "==") == 0) || (strcmp(begin->pTop->element, ">=") == 0) || (strcmp(begin->pTop->element, "<=") == 0) || (strcmp(begin->pTop->element, "return") == 0) || (strcmp(begin->pTop->element, "cout") == 0) || (strcmp(begin->pTop->element, "cin") == 0)) {
			strcpy_s(note[gh++], begin->pTop->element);//�ս�����
		}
		char cc[15];
		int m = tableget(row, begin->pTop->element);
		int n;
		switch (TOKEN[i].kind) {//����TOKENʽ���Ͳ��ҷ�����
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
			analysis(m, n, begin);//��������
		}
		else if (strcmp(begin->pTop->element, line[n]) == 0) {//ƥ��ɹ�
			int rab = read(TOKEN[i++]);
			freepop(begin);
		}
		else {
			cout << "���ִ��󣬴������﷨��������ջ������" << endl;
			getchar();
			exit(1);
		}
		read(TOKEN[i]);
		if ((strcmp(begin->pTop->element, "#") == 0) && TOKEN[i].kind == default) {
			cout << "#ƥ��ɹ�" << endl;
			return;
		}
	}
	return;
}
void tableprint() {//��LL(1)���������뵽�ļ���
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
bool val(char cmp[15]) {//�������
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
void initnote() {//��ʼ���ڵ�
	for (int i = 0; i < 1000; i++) {
		strcpy_s(note[i], "#");
	}
	return;
}
void yufa() {
	tokenaly();//�Ƚ��дʷ������õ�TOKEN����
	PSTACK alystack = new STACK;//����ջ
	int i = 0;
	ifstream outfile;
	outfile.open("wordsequence.txt", ios::in);//���ļ��е�TOKEN����д�뵽TOKEN������
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
	gram.open("grammar.txt", ios::in);//���﷨����������ķ��ļ�
	for (int i = 0;; i++) {//��ȡ����ʽ
		gram >> pro[i];
		if (strcmp(pro[i], "#") == 0) {
			break;
		}
	}
	initnote();//�ڵ��ʼ��
	gram.close();
	outfile.close();
	inittable();//�������ʼ��
	initproduct();//����ʽ��ʼ��
	tableend();//���������
	tableprint();//�����������뵽�ļ���
	inistack(alystack);//��ʼ������ջ
	cout << "�����﷨�����жϾ����Ƿ���ȷ" << endl;
	gra(alystack);//�﷨��������
	ofstream act;//����������д���ļ�
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



