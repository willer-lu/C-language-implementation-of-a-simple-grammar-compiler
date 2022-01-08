#include"��ͷ.h"
enum KIND { number, ch, id, undefine };
char acid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };//ö�ٶ���

struct opts {//׼���Ż�����Ԫʽ�ṹ
	int idact;//��Ԫʽ�����ı��
	char id1[20];//����һ
	char id2[20];//�����
	KIND type1;//����һ������
	KIND type2;//�����������
	char id3[20];//���
};
opts opt1[1000];

struct DAG_node {//DAGͼ�ڵ㼰���ڵ�
	char mes[50];//����ڵ���Ϣ
	KIND kind;//�ڵ�����
	struct DAG_node* synonymous;//�൱��ָ�򸽼ӱ���
};

struct DAG {//DAGͼ�ṹ
	int op;//�ڵ������
	int level;//�ڵ���
	struct DAG* left;//��Ҷ��
	struct DAG* right;//��Ҷ��
	struct DAG_node node;//�ڵ�
};


void DAGPRO(int low, int up) {//�������� ����DAG�ľֲ��Ż�
	DAG image[1000];//DAGͼ
	DAG* tem1[1000];//��ʱ����DAG
	DAG* tem2[1000];
	for (int i = 0; i < 1000; i++) {
		tem1[i] = new DAG;
		tem2[i] = new DAG;
	}
	for (int i = low; i <= up; i++) {//���������
		image[i].op = opt1[i].idact;//�ڵ��������Ӧ��Ԫʽ�Ķ�������
		image[i].level = 0;//��ʼ�Ĳ㼶
		switch (opt1[i].idact) {//������Ԫʽ�Ķ�������
		case 0:
		case 1:
		case 2:
		case 3: {//�Ӽ��˳�����½ڵ�Ĳ���
			image[i].left = tem1[i];
			image[i].right = tem2[i];
			tem1[i]->op = 9;
			tem2[i]->op = 9;
			image[i].node.kind = id;
			strcpy_s(image[i].node.mes, opt1[i].id3);
			strcpy_s(tem1[i]->node.mes, opt1[i].id1);
			strcpy_s(tem2[i]->node.mes, opt1[i].id2);
			switch (opt1[i].type1) {//�ڵ�����
			case number:tem1[i]->node.kind = number; break;
			case ch:tem1[i]->node.kind = ch; break;
			case id:tem1[i]->node.kind = id;
			}
			switch (opt1[i].type2) {
			case number:tem2[i]->node.kind = number; break;
			case ch:tem2[i]->node.kind = ch; break;
			case id:tem2[i]->node.kind = id;
			}
			tem1[i]->level = -1;
			tem2[i]->level = -1;
			tem1[i]->right = NULL;
			tem1[i]->left = NULL;
			tem2[i]->right = NULL;
			tem2[i]->left = NULL;
			tem1[i]->node.synonymous = NULL;
			tem2[i]->node.synonymous = NULL;
		}break;
		case 9: {//��Ϊ���ں�ʱ
			image[i].left = NULL;
			image[i].right = NULL;
			strcpy_s(image[i].node.mes, opt1[i].id3);
			switch (opt1[i].type1) {
			case number:image[i].node.kind = number; break;
			case ch:image[i].node.kind = ch; break;
			case id:image[i].node.kind = id;
			}
			image[i].node.synonymous = new DAG_node;
			strcpy_s(image[i].node.synonymous->mes, opt1[i].id1);
			image[i].node.synonymous->kind = image[i].node.kind;
			image[i].node.synonymous->synonymous = NULL;
		}break;
		}
	}
	//������
	for (int i = low; i <= up; i++) {//�����ϲ�
		switch (image[i].op) {
		case 0:
		case 1:
		case 2:
		case 3: {
			if ((tem1[i]->node.kind == number) && (tem2[i]->node.kind == number)) {//�������������Ϊ��������Ҫ���г����ϲ�
				int value1 = atoi(tem1[i]->node.mes);//���ַ���ת��Ϊ������
				int value2 = atoi(tem2[i]->node.mes);
				int value3 = 0;
				switch (image[i].op) {//�ֱ��Ӧ�Ӽ��˳�
				case 0:value3 = value1 + value2; break;
				case 1:value3 = value1 - value2; break;
				case 2:value3 = value1 * value2; break;
				case 3:value3 = value1 / value2; break;
				}
				tem1[i] = NULL;
				tem2[i] = NULL;
				image[i].op = 9;//ֱ�������µĳ����ڵ�
				image[i].left = NULL;
				image[i].right = NULL;
				image[i].node.synonymous = new DAG_node;
				_itoa_s(value3, image[i].node.synonymous->mes, 10);
				image[i].node.synonymous->kind = number;//�ϲ������Ϊ����
				image[i].node.synonymous->synonymous = NULL;
			}
		}
		}
	}
	for (int i = low; i <= up; i++) {//�����ӱ��ʽ��ʡ
		if ((image[i].op >= 0) && (image[i].op <= 3)) {//�Ӽ��˳�
			for (int j = low; j < i; j++) {
				if (image[i].op == image[j].op) {//���ʽ����һ��
					if (strcmp(tem1[i]->node.mes, tem1[j]->node.mes) == 0) {
						if (strcmp(tem2[i]->node.mes, tem2[j]->node.mes) == 0) {//���ʽ����ֵҲ��ͬ
							char cmp[15];
							char change[15];
							strcpy_s(cmp, image[i].node.mes);
							image[i].op = -2;
							strcpy_s(change, image[j].node.mes);
							for (int k = i + 1; k <= up; k++) {
								if (image[k].op == 9) {
									if (strcmp(image[k].node.synonymous->mes, cmp) == 0)strcpy_s(image[k].node.synonymous->mes, change);
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	for (int i = low; i <= up; i++) {//�����Ż����DAGͼ�����Ż������Ԫʽ
		if ((opt1[i].idact == 17) || (opt1[i].idact == 18))continue;
		opt1[i].idact = image[i].op;
		if (image[i].op == -1)break;
		strcpy_s(opt1[i].id3, image[i].node.mes);
		switch (image[i].op) {
		case -2:break;//��ʡ���Ľڵ�
		case 0:
		case 1:
		case 2:
		case 3: {
			strcpy_s(opt1[i].id1, tem1[i]->node.mes);
			strcpy_s(opt1[i].id2, tem2[i]->node.mes);
			opt1[i].type1 = tem1[i]->node.kind;
			opt1[i].type2 = tem2[i]->node.kind;
		}break;
		case 9:
			strcpy_s(opt1[i].id1, image[i].node.synonymous->mes);
			opt1[i].type1 = image[i].node.synonymous->kind;
		}
	}
}

void basicblockdiv() {//�����黮��
	int low = 0;
	int up = 0;//�Ͻ���½�
	for (;; up++) {
		switch (opt1[up].idact) {//������Ԫʽ�Ķ������������������Ӧ�Ĳ��������л���
		case 10://if
		case 13: {//while//������if����whileʱ
			DAGPRO(low, up - 2);//ȷ��������ķ�Χ ����DAGͼ
			if (opt1[up].idact == 13)up++;
			low = up + 1;
			up = low;
		}break;
		case 16://return
		case 11://else
		case 12://ie
		case 15: {//whileend��������ת������ת�����ת������䣩
			DAGPRO(low, up - 1);
			low = up + 1;
			up = low;
		}
		}
		if (opt1[up].idact == -1)break;
	}
	DAGPRO(low, up);
}
void optimize_s() {
	int i = 0;
	int c1 = 3;
	int c2 = 3;
	ifstream opt;
	opt.open("youhua.txt", ios::in);//�򿪴�����������ļ�
	while (!opt.eof()) {
		opt >> opt1[i].idact;
		opt >> opt1[i].id1;
		opt >> c1;
		opt1[i].type1 = (KIND)c1;
		opt >> opt1[i].id2;
		opt >> c2;
		opt1[i].type2 = (KIND)c2;
		opt >> opt1[i++].id3;
	}
	basicblockdiv();
	ofstream finish;
	finish.open("youhuaend.txt",ios::out);//����Ԫʽ����Ż��ļ�
	cout << "�Ż���ɵ���Ԫʽ����" << endl;
	for (int i = 0;; i++) {
		if (opt1[i].idact == -2)continue;
		finish << opt1[i].idact<<endl;
		finish << opt1[i].id1<<endl;
		finish << opt1[i].type1<<endl;
		finish << opt1[i].id2<<endl;
		finish << opt1[i].type2<<endl;
		finish << opt1[i].id3<<endl;
		cout << acid[opt1[i].idact]; range(acid[opt1[i].idact]);
		cout << opt1[i].id1; range(opt1[i].id1);
		cout << opt1[i].id2; range(opt1[i].id2);
		cout << opt1[i].id3<<endl;
		if (opt1[i].idact == -1)break;
	}
	finish.close();
	cout << "�м�����Ż�������ǰ�˼��Ż�������ɣ�����������Լ�������Ŀ�����...";
	opt.close();
	getchar();
}