
%{
/*********************************************
1 将所有的词法分析功能均放在 yylex 函数内实现，为 +、-、*、\、(、 ) 每个运算符及整数分别定义一个单词类别，在 yylex 内实现代码，能
识别这些单词，并将单词类别返回给词法分析程序。

2 实现功能更强的词法分析程序，可识别并忽略空格、制表符、回车等空白符，能识别多位十进制整数。
YACC file
**********************************************/


#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
%}

//TODO:给每个符号定义一个单词类别
%token ADD MINUS
%token MUL 
%token DIV
%token LPAREN
%token RPAREN
%token NUMBER


%left ADD MINUS 
%left MUL DIV
%right UMINUS         

%%


lines   :       lines expr ';' { printf("%f\n", $2); }
        |       lines ';'
        |
        ;
//TODO:完善表达式的规则
expr    :       expr ADD expr   { $$=$1+$3; }
        |       expr MINUS expr   { $$=$1-$3; }
        |       expr MUL expr   { $$=$1*$3; }
        |       expr DIV expr   { $$=$1/$3; }
        |       LPAREN expr RPAREN      { $$=$2;}
        |       MINUS expr %prec UMINUS   {$$=-$2;}
        |       NUMBER  {$$=$1;}
        ;

%%




// programs section

int yylex() {
    int t;
    int c;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t' || t == '\n') {
            // 忽略空格、制表符和回车
            continue;
        } else if (isdigit(t)) {
            // 解析多位整数
            int val = t - '0';
            while (1) {
                c = getchar();
                if (isdigit(c)) {
                    val = val * 10 + (c - '0');
                } else {
                    ungetc(c, stdin); // 将多读的字符放回输入流
                    yylval = val;     // 设置全局变量 yylval，用于传递数字值给语法分析器
                    return NUMBER;
                }
            }
        } else if (t == '+') {
            return ADD;
        } else if (t == '-') {
           return MINUS;
        } else if (t == '*') {
            return MUL;
        } else if (t == '/') {
            return DIV;
        } else if (t == '(') {
            return LPAREN;
        } else if (t == ')') {
            return RPAREN;
        } else if (t == EOF) {
            return 0; // 返回0表示已经到达文件末尾
        } else {
            return t; // 对于未知字符，直接返回其 ASCII 值
        }
    }
}

int main(void)
{
    yyin=stdin;
    do{
        yyparse();
    }while(!feof(yyin));
    return 0;
}
void yyerror(const char* s){
    fprintf(stderr,"Parse error: %s\n",s);
    exit(1);
}