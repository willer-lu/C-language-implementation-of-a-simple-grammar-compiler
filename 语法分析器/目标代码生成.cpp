#include"��ͷ.h"
enum type { number, ch, id, undefine };//���� �ַ� ���� δ����
char ctid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };/*ö�ٶ���*/
enum typ1 { in, fl, chars, boo };//int float char bool
enum typ2 { var, fu };//�������Ǻ���
typ1 kindget(char cmp[15]);//�õ���Ԫʽ������
struct actrecord {//��ʶ�����¼,���͵Ǽ�
	char id[15];//��ʶ��������
	int low;//�½�
	int up;//�Ͻ�
	typ1 s1;//��������
	typ2 s2;//����
};
actrecord st1[100];//���¼����
struct elem4 {//��Ԫʽ�ṹ
	int idact;//��Ԫʽ�����ı��
	char id1[15];//����һ
	char id2[15];//�����
	type type1;//����һ������
	type type2;//�����������
	char id3[15];//���
};
elem4 action[100];//��Ԫʽ����
void actionprep() {//��ԪʽԤ����(�Դ�����иĽ�)��ʹ֮����Ӧ����Ŀ�����(�����Ż�)
	elem4 exchange;
	for (int i = 0;; i++) {//��ԪʽԤ����
		if (action[i].idact == -1)break;
		if ((action[i].idact == 10) || (action[i].idact == 13)) {//��Ϊif��whileʱ
			exchange.idact = action[i].idact;
			strcpy_s(exchange.id1, action[i].id1);
			exchange.type1 = action[i].type1;
			strcpy_s(exchange.id2, action[i].id2);
			exchange.type2 = action[i].type2;
			strcpy_s(exchange.id3, action[i].id3);
			action[i].idact = action[i + 1].idact;
			strcpy_s(action[i].id1, action[i + 1].id1);
			action[i].type1 = action[i + 1].type1;
			strcpy_s(action[i].id2, action[i + 1].id2);
			action[i].type2 = action[i + 1].type2;
			strcpy_s(action[i].id3, action[i + 1].id3);
			action[i + 1].idact = exchange.idact;
			strcpy_s(action[i + 1].id1, exchange.id1);
			action[i + 1].type1 = exchange.type1;
			strcpy_s(action[i + 1].id2, exchange.id2);
			action[i + 1].type2 = exchange.type2;
			strcpy_s(action[i + 1].id3, exchange.id3);
			i++;
		}//��action[i]��ֵ��action[i+1]��ֵ�����滻
		else if (action[i].idact <= 3) {//��Ϊ�Ӽ��˳�ʱ
			exchange.idact = action[i].idact;
			strcpy_s(exchange.id1, action[i].id1);
			exchange.type1 = action[i].type1;
			strcpy_s(exchange.id2, action[i].id2);
			exchange.type2 = action[i].type2;
			strcpy_s(exchange.id3, action[i].id3);
			action[i].idact = action[i - 1].idact;
			strcpy_s(action[i].id1, action[i - 1].id1);
			action[i].type1 = action[i - 1].type1;
			strcpy_s(action[i].id2, action[i - 1].id2);
			action[i].type2 = action[i - 1].type2;
			strcpy_s(action[i].id3, action[i - 1].id3);
			action[i - 1].idact = exchange.idact;
			strcpy_s(action[i - 1].id1, exchange.id1);
			action[i - 1].type1 = exchange.type1;
			strcpy_s(action[i - 1].id2, exchange.id2);
			action[i - 1].type2 = exchange.type2;
			strcpy_s(action[i - 1].id3, exchange.id3);
		}//��action[i]��ֵ��action[i-1]��ֵ�����滻
	}
	ofstream opt;
	opt.open("youhua.txt", ios::out);//�򿪴�����������ļ�
	for (int i = 0; i < 100; i++) {//��Ԥ���������Ԫʽ�������
		if (action[i].idact == -1)break;
		opt << action[i].idact << endl;
		opt << action[i].id1 << endl;
		opt << action[i].type1 << endl;
		opt << action[i].id2 << endl;
		opt << action[i].type2 << endl;
		opt << action[i].id3 << endl;
	}
	opt << -1 << endl;
	cout << "��������м�����Ż�" << endl;
	opt.close();
	optimize_s();//�����м�����Ż�

	cout << "����������Է��ű����Ԥ����..." << endl;
	return;
}
void symprint() {//��ӡ���ű�
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;//�������βʱ
		cout << st1[i].id << '\t' << "      " << st1[i].low << " " << '\t' << "   " << st1[i].up << '\t' << "   ";
		switch (st1[i].s1) {//�;���������д�ӡ
		case in:cout << "int" << '\t'; break;
		case fl:cout << "float" << '\t'; break;
		case chars:cout << "char" << '\t'; break;
		case boo:cout << "bool" << '\t'; break;
		}
		cout << "      ";
		switch (st1[i].s2) {
		case var:cout << "var" << '\t'; break;
		case fu:cout << "function"; break;
		}
		cout << endl;
	}
}
void symbolprep() {//�Է��ű����Ԥ����
	ifstream act1,act2;
	act1.open("symboltable.txt", ios::in);//�򿪷��ű��ļ�
	act2.open("vall.txt", ios::in);//�򿪻��¼�ļ�
	int i = 0;
	act1 >> st1[i].id;
	char cmp[15];//�ݴ����
	act2 >> cmp;
	while (strcmp(st1[i].id, cmp) != 0) {//�ҵ���־�����ڻ��¼��λ��
		strcpy_s(cmp, "#");
		act2 >> cmp;
	}
	act2 >> st1[i].low;
	act2 >> st1[i].up;
	strcpy_s(cmp, "");
	act1 >> cmp;
	if (strcmp(cmp, "int") == 0) st1[i].s1 = in;//�ж���������
	else if (strcmp(cmp, "char") == 0)st1[i].s1 = chars;
	else if (strcmp(cmp, "float") == 0)st1[i].s1 = fl;
	strcpy_s(cmp, "");
	act1 >> cmp;
	st1[i++].s2 = fu;//����
	while (!act1.eof()) {
		act1 >> st1[i].id;
		act2 >> cmp;
		act2 >> st1[i].low;
		act2 >> st1[i].up;
		strcpy_s(cmp, "");
		act1 >> cmp;
		if (strcmp(cmp, "int") == 0) st1[i].s1 = in;
		else if (strcmp(cmp, "char") == 0)st1[i].s1 = chars;
		else if (strcmp(cmp, "float") == 0)st1[i].s1 = fl;
		act1 >> cmp;
		st1[i++].s2 = var;//����
	}
	strcpy_s(st1[i - 1].id, "#");
	int low = 0;
	int gdot = 0;
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0) {//�ҵ�ĩβ
			gdot = i;
			low = st1[i - 1].up + 1;
			break;
		}
	}
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		switch (action[i].idact) {
		case 0:
		case 1:
		case 2:
		case 3: {//Ϊ�Ӽ��˳�ʱ
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			st1[gdot].up = low + 4 - 1;//ռ�ĸ��ֽ�
			st1[gdot].s1 = in;
			st1[gdot].s2 = var;
			low = st1[gdot++].up + 1;
		}break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: {//Ϊ�߼�����ʱ
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			st1[gdot].up = low;//ռһ���ֽ�
			st1[gdot].s1 = boo;
			st1[gdot++].s2 = var;
			low++;
		}break;
		default:;
		}
	}
	strcpy_s(st1[gdot].id, "#");//����
	cout << "Ŀ�������ű�����..." << endl;
	cout << "������      �洢�½�   �洢�Ͻ�   ����        ����" << endl;
	symprint();//�����ű����
	cout << "�������������������Ŀ�����..." << endl;
	getchar();
}
typ1 kindget(char cmp[15]) {//���ض�Ӧ������
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, cmp) == 0) return st1[i].s1;
	}
}
void targetlanpro() {//����Ŀ�����
	ofstream act;
	act.open("target.txt", ios::out);//��Ŀ������ļ�
	ifstream fin;
	fin.open("youhuaend.txt", ios::in);//����Ԫʽ����Ż��ļ�
	int w = 0;
	int w1 = 3;
	int w2 = 3;
	while (!fin.eof()) {//�ȶ�����Ԫʽ
		fin >> action[w].idact;
		fin >> action[w].id1;
		fin >> w1;
		action[w].type1 = (type)w1;
		fin >> action[w].id2;
		fin >> w2;
		action[w].type2 = (type)w2;
		fin >> action[w++].id3;
	}
	fin.close();
	act << "DSEG    SEGMENT  " << endl;//�ζ���αָ��
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;
		if (st1[i].s2 == fu)continue;
		int s = strlen(st1[i].id);
		act << st1[i].id;//���������
		while (s < 8) {
			act << " ";
			s++;
		}
		switch (st1[i].s1) {
		case fl:
		case in:act << "DW 0" << endl; break;//ռ�����ֽ�
		case chars:act << "DB 0" << endl; break;//ռ��һ�ֽ�
		case boo:act << "DB 0" << endl; break;
		}
	}
	act << "string  DB 'PLEASE INPUT:','$'" << endl;//�Ӽ��������ַ���
	act << "int     DW 1000 DUP(0)" << endl;//�����ڴ滺����
	act << "float   DD 1000 DUP(0)" << endl;
	act << "bool    DB 1000 DUP(0)" << endl;
	act << "char    DB 1000 DUP(0)" << endl;
	int rpg = 0;
	int bat = 0;
	act << "DSEG    ENDS" << endl;//����
	act << "SSEG    SEGMENT STACK" << endl;//����һ����ջ��
	act << "SSEG    ENDS" << endl;
	act << "CSEG    SEGMENT" << endl;//����ζ���
	act << "        ASSUME  CS:CSEG,DS:DSEG" << endl;//�������θ�ֵ
	act << "        ASSUME  SS:SSEG" << endl;
	char function[4][20];
	ifstream fun;
	fun.open("wordsequence.txt", ios::in);//�򿪴ʷ������ļ�
	fun >> function[0];
	fun >> function[1];
	fun >> function[2];
	fun >> function[3];
	fun.close();
	act << function[3] << ':' << endl;
	act << "        MOV AX,DSEG" << endl;//��desg ��ֵ����ax
	act << "        MOV DS,AX" << endl;//��ax��ֵ�ŵ��μĴ���ds
	act << "        MOV AX,SSEG" << endl;//eax��32λ�Ĵ�����ax��eax�ĵ�16λ ah��ax�ĸ�8λ��al��ax�ĵ�8λ
	act << "        MOV SS,AX" << endl;
	int ifendid = 0;
	int whileendid = 0;
	int elseid = 0;
	int doid = 0;
	int actid = 0;
	int showid = 0;
	int ifid = 0;
	int hexid = 0;//ʮ������
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		if (action[i].idact == 16)continue;
		typ1 kind1;
		kind1 = kindget(action[i].id3);
		switch (action[i].idact) {
		case 0:
		case 1: {//�Ӻͼ�
			act << "        MOV AX,";
			switch (action[i].type1) {
			case number: {
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);//����תΪ�ַ���
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;//ʮ��������
			}break;
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id:act << action[i].id1 << endl;
			}
			switch (action[i].idact) {
			case 0:act << "        ADD AX,"; break;
			case 1:act << "        SUB AX,";//�ӻ��
			}
			switch (action[i].type2) {
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}break;
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id:act << action[i].id2 << endl;
			}
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 2: {//�˷�
			switch (action[i].type1) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id1 << endl;//SI��Դ��ַ�Ĵ��� OFFSET�ǽ���ֵ���ͱ������ŵ�ƫ�Ƶ�ֵַ
				act << "        MOV AL,BYTE PTR [SI]" << endl;//byte ptr ������������������ֽڵ�Ԫ
			}break;
			case number: {
				act << "        MOV AL,";
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			switch (action[i].type2) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id2 << endl;
				act << "        MUL BYTE PTR [SI]" << endl;
			}break;
			case number: {
				act << "        MUL ";
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 3: {//����
			act << "        MOV AX,";
			switch (action[i].type2) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id:act << action[i].id2 << endl; break;
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			switch (action[i].type1) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id1 << endl;
				act << "        DIV BYTE PTR [SI]" << endl;
			}break;
			case number: {
				act << "        DIV ";
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 2 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        AND AH,00H" << endl;//AND�߼���ָ��  ���ַ���ĩβ��0
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: {//�߼�����
			act << "compare" << actid << ":   " << endl;//�Ƚ�
			act << "        MOV AX,";
			switch (action[i].type1) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id: {
				act << action[i].id1 << endl;
			}break;
			case number: {
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        MOV BX,";//BX��ʾһ��ƫ�Ƶ�ַ
			switch (action[i].type2) {
			case ch:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
				getchar(); exit(1);
			case id: {
				act << action[i].id2 << endl;
			}break;
			case number: {
				int value = atoi(action[i].id2);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;
			}
			}
			act << "        CMP AX,BX" << endl;//�����߽��бȽ�
			act << "        ";
			switch (action[i].idact) {
			case 4:act << "JA "; break;//����ת��ָ��
			case 5:act << "JB "; break;
			case 6:act << "JAE "; break;
			case 7:act << "JBE "; break;
			case 8:act << "JNE "; break;
			}
		}break;
		case 9: {//��ֵ
			switch (action[i].type1) {
			case number: {
				switch (kind1) {
				case fl:
				case in: {
					int value = atoi(action[i].id1);
					char cmp2[15];
					_itoa_s(value, cmp2, 16);
					act << "        MOV " << action[i].id3 << ",";
					int c0 = 4 - strlen(cmp2);
					while (c0 > 0) {
						act << '0';
						c0--;
					}
					act << cmp2 << 'H' << endl;
				}break;
				case chars:cout << "�������Ͳ�ƥ�䣬���������..." << endl; 
					getchar(); exit(1);
				}
			}break;
			case ch: {
				act << "        MOV " << action[i].id3 << ",'" << action[i].id1 << "'" << endl;
			}break;
			case id:
				typ1 kind2 = kindget(action[i].id1);
				switch (kind2) {
				case in:
				case fl: {
					act << "        MOV AX," << action[i].id1 << endl;
					act << "        MOV " << action[i].id3 << ",AX" << endl;
				}break;
				case chars:
					act << "        MOV AL," << action[i].id1 << endl;
					act << "        MOV " << action[i].id3 << ",AL" << endl;
				}
			}
		}break;
		case 10: {//if���
			act << "else" << elseid << endl;
			act << "if" << ifid++ << ':' << endl;
		}break;
		case 11: {
			act << "        JMP ifend" << ifendid << endl;//��ת���
			act << "else" << elseid++ << ":" << endl;
		}break;
		case 12: {//ifend
			actid++;
			act << "ifend" << ifendid++ << ':' << endl;
		}break;
		case 13: {//while���
			act << "whileend" << whileendid << endl;
		}break;
		case 14: {
			act << "do" << doid++ << ":" << endl;
		}break;
		case 15: {//whileend
			act << "        JMP compare" << actid++ << endl;
			act << "whileend" << whileendid++ << ":" << endl;
		}break;
		case 16: {//return���
			act << "        MOV AH,4CH" << endl;
			act << "        INT 21H" << endl;//DOS���жϵ���
		}break;
		case 17: {//cout
			act << "        MOV CL,16" << endl;
			act << "show" << showid << ':' << endl;
			act << "        SUB CL,04" << endl;
			act << "        MOV AX," << action[i].id3 << endl;
			act << "        SHR AX,CL" << endl;//SHRΪ����ָ��
			act << "        AND AX,000FH" << endl;//ASCI��תΪ����
			act << "        MOV DL,AL" << endl;
			act << "        CMP DL,10" << endl;
			act << "        JB  hex" << hexid << endl;
			act << "        ADD DL,07H" << endl;
			act << "hex" << hexid++ << ':' << endl;
			act << "        ADD DL,30H" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        CMP CL,0" << endl;
			act << "        JNE show" << showid++ << endl;//����ת��ָ��
			act << "        MOV DL,'H'" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0AH" << endl;//0AH�ǻ��з�
			act << "        MOV AH,02H" << endl;//02H�ǵ����ַ�
			act << "        INT 21H" << endl;
			act << "        MOV DL,0DH" << endl;//0DH�ǻس���
			act << "        INT 21H" << endl;
		}break;
		case 18: {//cin
			act << "        MOV DX,OFFSET string" << endl;
			act << "        MOV AH,09H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV AH,01" << endl;
			act << "        INT 21H" << endl;
			act << "        SUB AL,30H" << endl;
			act << "        AND AX,00FFH" << endl;
			act << "        MOV " << action[i].id3 << ",AX" << endl;
			act << "        MOV DL,0AH" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0DH" << endl;
			act << "        INT 21H" << endl;
		}
		default:;
		}
	}
	act << "CSEG    ENDS" << endl;//����ν���
	act << "END     " << function[3] << endl;
	act.close();
	cout << "���ɵ�Ŀ�����������ʾ" << endl;
	ifstream acto;
	acto.open("target.txt", ios::in);
	char stri[10000];
	for (int i = 0; i < 10000; i++) {
		stri[i] = NULL;
	}
	acto.get(stri, 10000, EOF);//���������ݸ��Ƶ��������ַ���
	acto.close();
	cout << stri;//���ļ��е��������
	return;
}

void targetcode() {
	ifstream act;
	act.open("fourelement.txt", ios::in);//����Ԫʽ�ļ�
	int c1 = 3;
	int c2 = 3;
	int i = 0;
	while (!act.eof()) {//��ȡ����Ԫʽ������
		act >> action[i].idact;
		act >> action[i].id1;
		act >> c1;
		action[i].type1 = (type)c1;
		act >> action[i].id2;
		act >> c2;
		action[i].type2 = (type)c2;
		act >> action[i++].id3;
	}
	act.close();
	actionprep();//��ԪʽԤ����
	symbolprep();//���ű�Ԥ����
	targetlanpro();//Ŀ���������
	return;
}
