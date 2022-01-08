#include"��ͷ.h"
enum dif{ I, C, K, P, Ch, St, default };//TOKEN������
char t[100][15];//��ʱ����
enum type{number,ch,id,undefine};//���������
char actid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#"};//����
int sem[1000];//����ջ
struct fourelem {//��Ԫʽ�ṹ
	int idact;//����
	char id1[15];//����һ
	char id2[15];//�����
	type type1;
	type type2;//����
	char id3[15];//������
};
struct lan {//TOKEN����
	dif kind;
	char value1[25];
	int value2;
};
lan token[1000];
fourelem act2[100];//��Ԫʽ����
struct spo {//��������
	char name[15];
	char value1[15];
	float value2;
};
spo sport[100];
void initact() {//��ʼ����Ԫʽ����
	for (int i = 0; i < 100; i++) {
		act2[i].idact = -1;
		strcpy_s(act2[i].id1, "#");
		strcpy_s(act2[i].id2, "#");
		act2[i].type1 = undefine;
		act2[i].type2 = undefine;
		strcpy_s(act2[i].id3, "#");
	}
	return;
}
int act(char acti[20][10],char *cmp) {//Ѱ�Ҷ������
	for (int i = 0;i<20;i++){
		if (strcmp(acti[i], cmp) == 0) return i+1;
	}
	return 0;
}
void actanalysis() {//�������� 
	int i = 0;
	int j = 0;
	while (1) {
		if (strcmp(sport[i].name, "#") == 0)break;
		if (act(actid, sport[i].name))//�ҵ���Ӧ����
			act2[j++].idact = act(actid, sport[i].name)-1;
		i++;
	}
	int acloca = j - 1;
	for (int i = 0;; i++) {
		if (token[i].kind == default) {
			j = i-1;
			break;
		}
	}
	int loca = 0;
	int tn = -1;
	int s = 0;
	for (int i = j; i >= 0; i--) {//�����Ԫʽ����ȡ�Ե����Ϸ����ķ�
		if ((act2[acloca].idact == 12) || (act2[acloca].idact == 15) || (act2[acloca].idact == 11)||(act2[acloca].idact==14)) {//����ifend��whileendֱ������������Ԫʽ��������
			acloca--;
			i++;
			continue;
		}
		if ((token[i].kind == I) || (token[i].kind == C) || (token[i].kind == Ch)) {//Ϊ��ʶ�� ���� �ַ������
			sem[loca++] = i;
			continue;
		}
		if ((token[i].kind == P) && (token[i].value2 <= 10)) {//��ͬ������ ��Ӧ��Ԫʽ����
			if ((token[i - 2].value2 == 17)||(token[i-2].value2==18)) {
				i--;
				continue;
			}
			if (token[i].value2 == 4) {
				loca--;
				if (sem[loca] < 0) {
					char add[15];
					add[0] = 't';
					add[1] = fabs(sem[loca]) + 48;
					add[2] = '\0';
					strcpy_s(act2[acloca].id1, add);
					act2[acloca].type1 = id;
				}
				else {
					strcpy_s(act2[acloca].id1, token[sem[loca]].value1);
					switch (token[sem[loca]].kind) {
					case I:act2[acloca].type1 = id; break;
					case C:act2[acloca].type1 = number; break;
					case Ch:act2[acloca].type1 = ch;
					}
				}
				sem[loca] = 0;
				strcpy_s(act2[acloca].id3, token[--i].value1);
			}
			else {
				loca--;
				strcpy_s(act2[acloca].id1, token[sem[loca]].value1);
				switch (token[sem[loca]].kind) {
				case I:act2[acloca].type1 = id; break;
				case C:act2[acloca].type1 = number; break;
				case Ch:act2[acloca].type1 = ch;
				}
				sem[loca] = 0;
				strcpy_s(act2[acloca].id2, token[--i].value1);
				switch (token[i].kind) {
				case I:act2[acloca].type2 = id; break;
				case C:act2[acloca].type2 = number; break;
				case Ch:act2[acloca].type2 = ch;
				}
				sem[loca] = tn--;
				char add[15];
				add[0] = 't';
				add[1] = fabs(sem[loca++]) + 48;
				add[2] = '\0';
				strcpy_s(act2[acloca].id3, add);
			}
			acloca--;
		}
		if (token[i].kind == K) {//�ؼ������
			if ((token[i].value2 == 5) || (token[i].value2 == 11) || (token[i].value2 == 16)||(token[i].value2==17)||(token[i].value2==18)) {
				char add[15];
				loca--;
				if (sem[loca] < 0) {
					add[0] = 't';
					add[1] = fabs(sem[loca]) + 48;
					add[2] = '\0';
				}
				else {
					strcpy_s(add, token[sem[loca]].value1);
				}
				sem[loca] = 0;
				strcpy_s(act2[acloca--].id3, add);
			}
		}
	}
	return;
}
void range(char c[30]) {//�Ľ������ʽ
	int i = strlen(c);
	while (i++ < 16)cout << " ";
	return;
}
void printact() {//��ӡ �Լ������Ԫʽ
	cout << "��Ԫʽ����:" << endl;
	for (int i = 0; i < 100; i++) {
		if (act2[i].idact == -1)break;
		cout << actid[act2[i].idact]; range(actid[act2[i].idact]);
		cout << act2[i].id1; range(act2[i].id1);
		cout << act2[i].id2; range(act2[i].id2);
		cout << act2[i].id3 << endl;
	}
	ofstream act3;
	act3.open("fourelem.txt", ios::out);//�������Ԫʽ
	int he = 0;
	for (int i = 0; i < 100; i++) {
		act3 << actid[act2[i].idact];
		he = strlen(actid[act2[i].idact]);
		while (he++ < 16)act3 << " ";
		act3 << act2[i].id1; 
		he = strlen(act2[i].id1);
		while (he++ < 16)act3 << " ";
		act3 << act2[i].type1 << " ";
		act3 << act2[i].id2; 
		he = strlen(act2[i].id2);
		while (he++ < 16)act3 << " ";
		act3 << act2[i].type2 << " ";
		act3 << act2[i].id3 << endl;
		if (act2[i].idact == -1)break;
	}
	act3.close();
	ofstream act4;
	act4.open("fourelement.txt", ios::out);//����Ԫʽ�ṹ���� ���ں���ʹ��
	for (int i = 0; i < 100; i++) {
		act4 << act2[i].idact << endl;
		act4 << act2[i].id1 << endl;
		act4 << act2[i].type1 << endl;
		act4 << act2[i].id2 << endl;
		act4 << act2[i].type2 << endl;
		act4 << act2[i].id3 << endl;
		if (act2[i].idact == -1)break;
	}
	act4.close();
	return;
}
void translate1() {
	ifstream act1,act2;
	act1.open("action.txt", ios::in);//��������
	act2.open("wordsequence.txt", ios::in);//��������
	for (int i = 0; i < 100; i++) {//��ʼ��
		sport[i].value2 = -100;
		strcpy_s(sport[i].value1, "#");
		strcpy_s(sport[i].name, "#");
	}
	for (int i = 0;; i++) {//��ȡ��������
		act1 >> sport[i].name;
		if (strcmp(sport[i].name, "#") == 0)break;
	}
	act1.close();
	int i = 0;
	while (!act2.eof()) {//���뵥������
		int sty;
		act2 >> sty;
		token[i].kind = (dif)sty;
		switch (token[i].kind) {
		case P:
		case K:act2 >> token[i++].value2; break;
		case I:
		case Ch:
		case St:
		case C:act2 >> token[i++].value1; break;
		}
	}
	token[i - 1].kind = default;
	act2.close();
	initact();//��ʼ��
	actanalysis();//��������
	printact();//��ӡ����
	
	return;
}