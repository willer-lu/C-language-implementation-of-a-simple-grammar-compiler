#include"标头.h"
enum type { number, ch, id, undefine };//常数 字符 变量 未定义
char ctid[20][10] = { "+","-","*","/",">","<",">=","<=","==","=","if","else","ie","while","do","we" ,"return","cout","cin","#" };/*枚举动作*/
enum typ1 { in, fl, chars, boo };//int float char bool
enum typ2 { var, fu };//变量或是函数
typ1 kindget(char cmp[15]);//得到四元式的种类
struct actrecord {//标识符活动记录,类型登记
	char id[15];//标识符的名字
	int low;//下界
	int up;//上界
	typ1 s1;//数据类型
	typ2 s2;//种类
};
actrecord st1[100];//活动记录序列
struct elem4 {//四元式结构
	int idact;//四元式动作的编号
	char id1[15];//对象一
	char id2[15];//对象二
	type type1;//对象一的种类
	type type2;//对象二的种类
	char id3[15];//结果
};
elem4 action[100];//四元式序列
void actionprep() {//四元式预处理(对次序进行改进)，使之更适应生成目标代码(包括优化)
	elem4 exchange;
	for (int i = 0;; i++) {//四元式预处理
		if (action[i].idact == -1)break;
		if ((action[i].idact == 10) || (action[i].idact == 13)) {//当为if或while时
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
		}//将action[i]的值与action[i+1]的值进行替换
		else if (action[i].idact <= 3) {//当为加减乘除时
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
		}//将action[i]的值与action[i-1]的值进行替换
	}
	ofstream opt;
	opt.open("youhua.txt", ios::out);//打开代码所储存的文件
	for (int i = 0; i < 100; i++) {//对预处理完的四元式进行输出
		if (action[i].idact == -1)break;
		opt << action[i].idact << endl;
		opt << action[i].id1 << endl;
		opt << action[i].type1 << endl;
		opt << action[i].id2 << endl;
		opt << action[i].type2 << endl;
		opt << action[i].id3 << endl;
	}
	opt << -1 << endl;
	cout << "下面进行中间代码优化" << endl;
	opt.close();
	optimize_s();//进行中间代码优化

	cout << "按下任意键对符号表进行预处理..." << endl;
	return;
}
void symprint() {//打印符号表
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;//当到达结尾时
		cout << st1[i].id << '\t' << "      " << st1[i].low << " " << '\t' << "   " << st1[i].up << '\t' << "   ";
		switch (st1[i].s1) {//就具体情况进行打印
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
void symbolprep() {//对符号表进行预处理
	ifstream act1,act2;
	act1.open("symboltable.txt", ios::in);//打开符号表文件
	act2.open("vall.txt", ios::in);//打开活动记录文件
	int i = 0;
	act1 >> st1[i].id;
	char cmp[15];//暂存读入
	act2 >> cmp;
	while (strcmp(st1[i].id, cmp) != 0) {//找到标志符所在活动记录的位置
		strcpy_s(cmp, "#");
		act2 >> cmp;
	}
	act2 >> st1[i].low;
	act2 >> st1[i].up;
	strcpy_s(cmp, "");
	act1 >> cmp;
	if (strcmp(cmp, "int") == 0) st1[i].s1 = in;//判断数据类型
	else if (strcmp(cmp, "char") == 0)st1[i].s1 = chars;
	else if (strcmp(cmp, "float") == 0)st1[i].s1 = fl;
	strcpy_s(cmp, "");
	act1 >> cmp;
	st1[i++].s2 = fu;//函数
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
		st1[i++].s2 = var;//变量
	}
	strcpy_s(st1[i - 1].id, "#");
	int low = 0;
	int gdot = 0;
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0) {//找到末尾
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
		case 3: {//为加减乘除时
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			st1[gdot].up = low + 4 - 1;//占四个字节
			st1[gdot].s1 = in;
			st1[gdot].s2 = var;
			low = st1[gdot++].up + 1;
		}break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: {//为逻辑运算时
			strcpy_s(st1[gdot].id, action[i].id3);
			st1[gdot].low = low;
			st1[gdot].up = low;//占一个字节
			st1[gdot].s1 = boo;
			st1[gdot++].s2 = var;
			low++;
		}break;
		default:;
		}
	}
	strcpy_s(st1[gdot].id, "#");//结束
	cout << "目标代码符号表如下..." << endl;
	cout << "符号名      存储下界   存储上界   类型        种类" << endl;
	symprint();//将符号表输出
	cout << "按下任意键以生成最后的目标代码..." << endl;
	getchar();
}
typ1 kindget(char cmp[15]) {//返回对应的种类
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, cmp) == 0) return st1[i].s1;
	}
}
void targetlanpro() {//生成目标代码
	ofstream act;
	act.open("target.txt", ios::out);//打开目标代码文件
	ifstream fin;
	fin.open("youhuaend.txt", ios::in);//打开四元式完成优化文件
	int w = 0;
	int w1 = 3;
	int w2 = 3;
	while (!fin.eof()) {//先读入四元式
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
	act << "DSEG    SEGMENT  " << endl;//段定义伪指令
	for (int i = 0;; i++) {
		if (strcmp(st1[i].id, "#") == 0)break;
		if (st1[i].s2 == fu)continue;
		int s = strlen(st1[i].id);
		act << st1[i].id;//输出变量名
		while (s < 8) {
			act << " ";
			s++;
		}
		switch (st1[i].s1) {
		case fl:
		case in:act << "DW 0" << endl; break;//占用两字节
		case chars:act << "DB 0" << endl; break;//占用一字节
		case boo:act << "DB 0" << endl; break;
		}
	}
	act << "string  DB 'PLEASE INPUT:','$'" << endl;//从键盘输入字符串
	act << "int     DW 1000 DUP(0)" << endl;//定义内存缓冲区
	act << "float   DD 1000 DUP(0)" << endl;
	act << "bool    DB 1000 DUP(0)" << endl;
	act << "char    DB 1000 DUP(0)" << endl;
	int rpg = 0;
	int bat = 0;
	act << "DSEG    ENDS" << endl;//结束
	act << "SSEG    SEGMENT STACK" << endl;//定义一个堆栈段
	act << "SSEG    ENDS" << endl;
	act << "CSEG    SEGMENT" << endl;//代码段定义
	act << "        ASSUME  CS:CSEG,DS:DSEG" << endl;//分配代码段赋值
	act << "        ASSUME  SS:SSEG" << endl;
	char function[4][20];
	ifstream fun;
	fun.open("wordsequence.txt", ios::in);//打开词法序列文件
	fun >> function[0];
	fun >> function[1];
	fun >> function[2];
	fun >> function[3];
	fun.close();
	act << function[3] << ':' << endl;
	act << "        MOV AX,DSEG" << endl;//把desg 的值赋给ax
	act << "        MOV DS,AX" << endl;//把ax的值放到段寄存器ds
	act << "        MOV AX,SSEG" << endl;//eax是32位寄存器，ax是eax的低16位 ah是ax的高8位，al是ax的低8位
	act << "        MOV SS,AX" << endl;
	int ifendid = 0;
	int whileendid = 0;
	int elseid = 0;
	int doid = 0;
	int actid = 0;
	int showid = 0;
	int ifid = 0;
	int hexid = 0;//十六进制
	for (int i = 0;; i++) {
		if (action[i].idact == -1)break;
		if (action[i].idact == 16)continue;
		typ1 kind1;
		kind1 = kindget(action[i].id3);
		switch (action[i].idact) {
		case 0:
		case 1: {//加和减
			act << "        MOV AX,";
			switch (action[i].type1) {
			case number: {
				int value = atoi(action[i].id1);
				char cmp2[15];
				_itoa_s(value, cmp2, 16);//整数转为字符串
				int c0 = 4 - strlen(cmp2);
				while (c0 > 0) {
					act << '0';
					c0--;
				}
				act << cmp2 << 'H' << endl;//十六进制数
			}break;
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
				getchar(); exit(1);
			case id:act << action[i].id1 << endl;
			}
			switch (action[i].idact) {
			case 0:act << "        ADD AX,"; break;
			case 1:act << "        SUB AX,";//加或减
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
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
				getchar(); exit(1);
			case id:act << action[i].id2 << endl;
			}
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 2: {//乘法
			switch (action[i].type1) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
				getchar(); exit(1);
			case id: {
				act << "        MOV SI,OFFSET " << action[i].id1 << endl;//SI是源变址寄存器 OFFSET是将数值回送变量或标号的偏移地址值
				act << "        MOV AL,BYTE PTR [SI]" << endl;//byte ptr 用来定义所存入的是字节单元
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
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
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
		case 3: {//除法
			act << "        MOV AX,";
			switch (action[i].type2) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
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
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
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
			act << "        AND AH,00H" << endl;//AND逻辑与指令  在字符串末尾加0
			act << "        MOV " << action[i].id3 << ",AX" << endl;
		}break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8: {//逻辑运算
			act << "compare" << actid << ":   " << endl;//比较
			act << "        MOV AX,";
			switch (action[i].type1) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
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
			act << "        MOV BX,";//BX表示一个偏移地址
			switch (action[i].type2) {
			case ch:cout << "数据类型不匹配，语句有问题..." << endl; 
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
			act << "        CMP AX,BX" << endl;//将两者进行比较
			act << "        ";
			switch (action[i].idact) {
			case 4:act << "JA "; break;//条件转移指令
			case 5:act << "JB "; break;
			case 6:act << "JAE "; break;
			case 7:act << "JBE "; break;
			case 8:act << "JNE "; break;
			}
		}break;
		case 9: {//赋值
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
				case chars:cout << "数据类型不匹配，语句有问题..." << endl; 
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
		case 10: {//if语句
			act << "else" << elseid << endl;
			act << "if" << ifid++ << ':' << endl;
		}break;
		case 11: {
			act << "        JMP ifend" << ifendid << endl;//跳转语句
			act << "else" << elseid++ << ":" << endl;
		}break;
		case 12: {//ifend
			actid++;
			act << "ifend" << ifendid++ << ':' << endl;
		}break;
		case 13: {//while语句
			act << "whileend" << whileendid << endl;
		}break;
		case 14: {
			act << "do" << doid++ << ":" << endl;
		}break;
		case 15: {//whileend
			act << "        JMP compare" << actid++ << endl;
			act << "whileend" << whileendid++ << ":" << endl;
		}break;
		case 16: {//return语句
			act << "        MOV AH,4CH" << endl;
			act << "        INT 21H" << endl;//DOS的中断调用
		}break;
		case 17: {//cout
			act << "        MOV CL,16" << endl;
			act << "show" << showid << ':' << endl;
			act << "        SUB CL,04" << endl;
			act << "        MOV AX," << action[i].id3 << endl;
			act << "        SHR AX,CL" << endl;//SHR为右移指令
			act << "        AND AX,000FH" << endl;//ASCI码转为数字
			act << "        MOV DL,AL" << endl;
			act << "        CMP DL,10" << endl;
			act << "        JB  hex" << hexid << endl;
			act << "        ADD DL,07H" << endl;
			act << "hex" << hexid++ << ':' << endl;
			act << "        ADD DL,30H" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        CMP CL,0" << endl;
			act << "        JNE show" << showid++ << endl;//条件转移指令
			act << "        MOV DL,'H'" << endl;
			act << "        MOV AH,02H" << endl;
			act << "        INT 21H" << endl;
			act << "        MOV DL,0AH" << endl;//0AH是换行符
			act << "        MOV AH,02H" << endl;//02H是单个字符
			act << "        INT 21H" << endl;
			act << "        MOV DL,0DH" << endl;//0DH是回车符
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
	act << "CSEG    ENDS" << endl;//代码段结束
	act << "END     " << function[3] << endl;
	act.close();
	cout << "生成的目标代码如下所示" << endl;
	ifstream acto;
	acto.open("target.txt", ios::in);
	char stri[10000];
	for (int i = 0; i < 10000; i++) {
		stri[i] = NULL;
	}
	acto.get(stri, 10000, EOF);//将代码内容复制到待分析字符串
	acto.close();
	cout << stri;//将文件中的内容输出
	return;
}

void targetcode() {
	ifstream act;
	act.open("fourelement.txt", ios::in);//打开四元式文件
	int c1 = 3;
	int c2 = 3;
	int i = 0;
	while (!act.eof()) {//读取到四元式序列中
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
	actionprep();//四元式预处理
	symbolprep();//符号表预处理
	targetlanpro();//目标代码生成
	return;
}
