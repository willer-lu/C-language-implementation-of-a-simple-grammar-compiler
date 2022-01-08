#include "标头.h"
int main() {//主函数，用于读入词法分析完成得到的单词序列
	yufa();//进行语法分析
	cout << "结束运行，语法分析无错误,进入符号表构建";
	analysistable();//进行符号表的填写
	cout << "符号表填写完成，进入中间代码翻译";
	translate1();//进行中间代码的翻译
	cout << "中间代码翻译完成,进入目标代码生成";
	targetcode();//进行目标代码的生成
	cout << "目标代码完成生成.";
	getchar();
	return 0;
}