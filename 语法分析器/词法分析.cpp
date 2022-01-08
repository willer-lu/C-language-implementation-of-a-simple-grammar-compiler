#include"��ͷ.h"
static int state = 1;
int location1 = 0;//ɨ�赽��λ��
char str[10000];//����Դ��
char ch;//����chɨ��
char token[25];//�ݴ浥��ֵ
enum style{I,C,K,P,Ch,St,def};//��ʶ�� ���� �ؼ��� ��� �ַ� �ַ��� ��ʼ����
static char *KT[18] = { "int","float","char","void","if","else","switch","case","for","do","while","continue","break","default","sizeof","return","cout","cin" };//�ؼ��ֱ�
static char *PT[18] = { ">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")" ,"[","]"};//�����
char variate[16][10] = {};//������
int serial = 0;
int count1 = 0, count2 = 0, count3 = 0, count4 = 0,flag=0;
char  CT[10][20], ST[10][20];//�ֱ��ݴ��ʶ�� ���� �ַ���
char cT[20];//�����ַ�

static struct twoele {//Token���е����ݽṹ
	style kind;//����
	char value1[25];//������
	int value2;//���
};

static twoele TOKEN[1000];//Token����

static void init_twoele(twoele*tok) {//��ʼ��Token���кͱ�����
	for (int i = 0; i < 1000; i++) {
		tok[i].kind = def;
		strcpy_s(tok[i].value1, "#");
		tok[i].value2 = 0;
	}
	for (int i = 0; i < 16; i++) strcpy_s(variate[i], "#");
}


int tra(char*cmp) {//�жϻ�õ����Ƿ�Ϊ�ؼ��֣����򷵻عؼ������
	for (int i = 0; i < 18; i++) {
		if (strcmp(KT[i], cmp) == 0)return i + 1;
	}
	return 0;
}

void printf(twoele*tok) {//���Token����
	float ck = 10;
	for (int i = 0; i < 1000; i++) {
		if (tok[i].kind == def) break;//��������Ϊ����ֹͣ���
		float number=0;
		switch (tok[i].kind) {//���ݵ��ʵ�����
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
int addvar(char* cmp) {//���������
	int i = 0;
	for (; i < 16; i++) {
		if (strcmp(variate[i], "#") == 0)break;//�ѵ���β
		if (strcmp(variate[i], cmp) == 0)return i;//������������
	}
	strcpy_s(variate[i], cmp);
	return i;
}
void prep(char ch) {//����Դ�����е�ע��
	int state = 1;//state��ʾ�������
	int cycle = 0;//cycle��ָ����ע�� 
	int back;
	if (ch == '/') {//����'/'��׼�����й���ע��
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
			if (ch == '*')//����'*'����Ϊ�ѽ���ע���ֶ�
				state = 3;
			else {
				state = 1;
				cycle = 1;
			}
			break;
		case 3:if (ch == '*')//��һ�γ���'*'��׼������ע���ֶ�
			state = 4; break;
		case 4:if (ch == '/')//��һ�γ���'/'������ע���ֶΣ������������ע�͹���
			state = 5;
			  else
			state = 4; break;
		case 5:cycle = 1;
		}
		if (cycle == 1)
			break;//�������
		ch = str[location1++];
	}
	if (state == 1) {
		location1 = back;
	}
	return;
}
static void scanner() {//ɨ����
	int i = 0;
	ch = str[location1++];
	for (int i = 0; i < 25; i++)
	{
		token[i] = NULL;//�����ʱȽϴ������
	}
	
	prep(ch);//���˵������е�ע�Ͷ�
	ch = str[location1++];

	while (ch == ' ') {//�����հ�
		ch = str[location1++];
	}

	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {//�ж�Ϊ�ؼ��ֻ��ʶ��
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
		if (tra(token)) {//�ѻ�ȡ����Ϊ�ؼ���
			TOKEN[serial].kind = K;
			TOKEN[serial].value2 = tra(token);
		}
		else {//����Ϊ��ʶ��
			strcpy_s(TOKEN[serial].value1, token);
			TOKEN[serial].value2 = addvar(token);
		}
		serial++;//�����ж���һ������ 
	}
	else if (ch >= '0'&&ch <= '9') {//��������
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
			num = num * 10 + ch - '0';//ѭ����10���������λ
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
		switch (ch) {//����ж�
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
		case'\'': {//�ַ���ȡ
			TOKEN[serial].kind = Ch;
			token[i++] = str[location1++];
			token[i] = '\0';
			strcpy_s(TOKEN[serial++].value1, token);
			if (str[location1++] != '\'') {
				cout << "���������⣬�ַ�ӦΪ��������..." << endl;
				getchar();
				exit(1);
			}
			for (int i = 0; i < 10; i++)
			if (strcmp(CT[i], TOKEN[serial-1].value1) == 0) { flag = 3; TOKEN[serial-1].value2 = i + 1; break; }
			if (flag != 3) { strcpy_s(CT[count3], TOKEN[serial-1].value1); count3++; TOKEN[serial-1].value2 = count3; }
		}break;
		case'"': {//�ַ�����ȡ
			TOKEN[serial].kind = St;
			for (; i < 25; ) {
				if (str[location1] != '"') {
					token[i++] = str[location1++];
				}
				else break;
			}
			token[i] = '\0';
			if (str[location1++] != '"') {
				cout << "���������⣬�ַ���Ӧ�ԡ�����" << endl;
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
	outfile.get(str, 10000, EOF);//���������ݸ��ƽ�ȥ�������ַ���
	outfile.close();
	init_twoele(TOKEN);//��ʼ��TOKEN����
	cout << "����������£�" << endl << str << endl << "�����ʷ��������:" << endl;
	while (str[location1] != NULL) {//δ����ʱ����ɨ��
		scanner();
	}
	printf(TOKEN);//��ӡTOKEN��
	ofstream fout;
	fout.open("wordsequence.txt", ios::out);
	for (int i = 0; i < serial; i++) {//�������ֵ�͵������е��ļ���
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
	cout << "�����ϣ���������﷨����";
	fout.close();
}
