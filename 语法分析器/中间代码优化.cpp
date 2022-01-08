#include"标头.h"
enum KIND { number, ch, id, undefine };
char acid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };//枚举动作

struct opts {//准备优化的四元式结构
	int idact;//四元式动作的编号
	char id1[20];//对象一
	char id2[20];//对象二
	KIND type1;//对象一的种类
	KIND type2;//对象二的种类
	char id3[20];//结果
};
opts opt1[1000];

struct DAG_node {//DAG图节点及链节点
	char mes[50];//储存节点信息
	KIND kind;//节点类型
	struct DAG_node* synonymous;//相当于指向附加变量
};

struct DAG {//DAG图结构
	int op;//节点运算符
	int level;//节点编号
	struct DAG* left;//左叶子
	struct DAG* right;//右叶子
	struct DAG_node node;//节点
};


void DAGPRO(int low, int up) {//基本块上 基于DAG的局部优化
	DAG image[1000];//DAG图
	DAG* tem1[1000];//临时变量DAG
	DAG* tem2[1000];
	for (int i = 0; i < 1000; i++) {
		tem1[i] = new DAG;
		tem2[i] = new DAG;
	}
	for (int i = low; i <= up; i++) {//产生树结点
		image[i].op = opt1[i].idact;//节点运算符对应四元式的动作序列
		image[i].level = 0;//初始的层级
		switch (opt1[i].idact) {//根据四元式的动作序列
		case 0:
		case 1:
		case 2:
		case 3: {//加减乘除情况下节点的产生
			image[i].left = tem1[i];
			image[i].right = tem2[i];
			tem1[i]->op = 9;
			tem2[i]->op = 9;
			image[i].node.kind = id;
			strcpy_s(image[i].node.mes, opt1[i].id3);
			strcpy_s(tem1[i]->node.mes, opt1[i].id1);
			strcpy_s(tem2[i]->node.mes, opt1[i].id2);
			switch (opt1[i].type1) {//节点种类
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
		case 9: {//当为等于号时
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
	//构造树
	for (int i = low; i <= up; i++) {//常数合并
		switch (image[i].op) {
		case 0:
		case 1:
		case 2:
		case 3: {
			if ((tem1[i]->node.kind == number) && (tem2[i]->node.kind == number)) {//如果运算符两侧均为常数则需要进行常数合并
				int value1 = atoi(tem1[i]->node.mes);//将字符串转化为整型数
				int value2 = atoi(tem2[i]->node.mes);
				int value3 = 0;
				switch (image[i].op) {//分别对应加减乘除
				case 0:value3 = value1 + value2; break;
				case 1:value3 = value1 - value2; break;
				case 2:value3 = value1 * value2; break;
				case 3:value3 = value1 / value2; break;
				}
				tem1[i] = NULL;
				tem2[i] = NULL;
				image[i].op = 9;//直接生成新的常数节点
				image[i].left = NULL;
				image[i].right = NULL;
				image[i].node.synonymous = new DAG_node;
				_itoa_s(value3, image[i].node.synonymous->mes, 10);
				image[i].node.synonymous->kind = number;//合并结果仍为常数
				image[i].node.synonymous->synonymous = NULL;
			}
		}
		}
	}
	for (int i = low; i <= up; i++) {//公共子表达式节省
		if ((image[i].op >= 0) && (image[i].op <= 3)) {//加减乘除
			for (int j = low; j < i; j++) {
				if (image[i].op == image[j].op) {//表达式符号一致
					if (strcmp(tem1[i]->node.mes, tem1[j]->node.mes) == 0) {
						if (strcmp(tem2[i]->node.mes, tem2[j]->node.mes) == 0) {//表达式左右值也相同
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
	for (int i = low; i <= up; i++) {//根据优化后的DAG图生成优化后的四元式
		if ((opt1[i].idact == 17) || (opt1[i].idact == 18))continue;
		opt1[i].idact = image[i].op;
		if (image[i].op == -1)break;
		strcpy_s(opt1[i].id3, image[i].node.mes);
		switch (image[i].op) {
		case -2:break;//节省掉的节点
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

void basicblockdiv() {//基本块划分
	int low = 0;
	int up = 0;//上界和下界
	for (;; up++) {
		switch (opt1[up].idact) {//根据四元式的动作（即运算或者语句对应的操作）进行划分
		case 10://if
		case 13: {//while//当遇到if或者while时
			DAGPRO(low, up - 2);//确定基本块的范围 构造DAG图
			if (opt1[up].idact == 13)up++;
			low = up + 1;
			up = low;
		}break;
		case 16://return
		case 11://else
		case 12://ie
		case 15: {//whileend（其他的转向语句或转向语句转到的语句）
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
	opt.open("youhua.txt", ios::in);//打开代码所储存的文件
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
	finish.open("youhuaend.txt",ios::out);//打开四元式完成优化文件
	cout << "优化完成的四元式如下" << endl;
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
	cout << "中间代码优化结束，前端加优化基本完成，按下任意键以继续生成目标代码...";
	opt.close();
	getchar();
}