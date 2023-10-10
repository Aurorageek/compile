%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define YYSTYPE char*
char idarr[50];
char numarr[50];
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char*s);
%}
%token NUMBER
%token ID
%token ADD SUB MUL DIV LPAREN RPAREN
%left LPAREN
%left ADD SUB
%left MUL DIV
%right UMINUS
%right RPAREN
%%

lines : lines expr '\n'{printf("%s\n",$2);}
      | lines '\n'
      |
      ;

expr : expr ADD expr {$$=(char*)malloc(50*sizeof(char)); strcpy($$,$1);
                      strcat($$,$3);strcat($$,"+"); }
     | expr SUB expr {$$=(char*)malloc(50*sizeof(char)); strcpy($$,$1);
                      strcat($$,$3);strcat($$,"-"); }
     | expr MUL expr {$$=(char*)malloc(50*sizeof(char)); strcpy($$,$1);
                      strcat($$,$3);strcat($$,"*"); }
     | expr DIV expr {$$=(char*)malloc(50*sizeof(char)); strcpy($$,$1);
                      strcat($$,$3);strcat($$,"/"); }
     | LPAREN expr RPAREN {$$=(char*)malloc(50*sizeof(char));
                           strcpy($$,$2);}
     | NUMBER   {$$ = (char*)malloc(50*sizeof(char)); strcpy($$,$1);
                 strcat($$," ");}
     | ID   {$$=(char*)malloc(50*sizeof(char));
             strcpy($$,$1);strcat($$," ");}


%%


int yylex()
{
      int t;
      while(1){
          t=getchar();     
          if(t==' '||t=='\t');
          else if((t>='0'&& t<='9')){
                   int ti=0;
                  while((t>='0'&& t<='9')){
                  numarr[ti]=t;
                  t=getchar();
                  ti++;
                }
           numarr[ti]='\0';
           yylval=numarr;
           ungetc(t,stdin);
           return NUMBER;
         }
      
       else 
          {
            switch(t)
           {
              case '+':
                   return ADD;
              case '-':
                   return SUB;
              case '*':
                   return MUL;
              case '/':
                   return DIV;
              case'(':
                   return LPAREN;
              case')':
                   return RPAREN;
              default: 
                   return t;
            }
          }
      return t;
   }
}

int main(void)
{
   yyin = stdin;
  do{
      yyparse();
    }while(!feof(yyin));
  return 0;
}
void yyerror(const char* s)
{
   fprintf(stderr,"Parse error:%s\n",s);
   exit(1);
}
