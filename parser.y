%code top{
#include <stdio.h>
#include <math.h>
#include "scanner.h"
#include "calc.h"
int yylex(void);
}
%defines "parser.h"
%output "parser.c"
%define parse.error verbose

%token PR_VAR PR_SALIR NL ID NRO
%token OP_MAS_IG    "+="
%token OP_MENOS_IG  "-="
%token OP_POR_IG    "*="
%token OP_DIV_IG    "/="

%right 	'=' "+=" "-=" "*=" "/="
%left 	'-' '+'
%left 	'*' '/'
%right 	'^'
%precedence NEG

%code provides {
YYERROR;
void yyerror(const char *);
extern int yylexerrs;
}
%union YYSTYPE {
    char*   id;
    double  nro;
}
%type   <id>  PR_VAR PR_SALIR ID 
%type   <nro> NRO 
%nterm  <nro> expresion sentencia

%%

programa: sesion                    { if (yynerrs || yylexerrs) YYABORT;}
    ;
sesion 	: linea                     { printf("\n"); }
    | sesion linea                  { printf("\n"); }
    ;
linea   :  sentencia NL             { printf("Expresión, resultado: %f\n", $<nro>1); }
    | error NL
    | PR_VAR ID NL                  { if (fueDeclarado($<id>2)) { 
                                        printf("Error semántico, ID ya declarado"); 
                                      } else {
                                        declararId($<id>2); printf("Define ID como variable\n"); } 
                                    }
    | PR_VAR ID '=' expresion NL    { if (fueDeclarado($<id>2)) { 
                                        printf("Error semántico, ID ya declarado");
                                      } else {
                                        declararId($<id>2); asignarA($<id>2, $<nro>3);
                                        printf("Define ID como variable con valor inicial\n"); }
                                    }
    | PR_SALIR NL                   { printf("Palabra reservada salir\n\n"); return (yynerrs || yylexerrs);}
    ;
sentencia : expresion               { $$ = $<nro>1; printf("Expresión\n"); }
    | ID '=' sentencia              { $$ = $<nro>3; asignarA($<id>1, $<nro>3); }
    | ID "+=" sentencia             { $$ = valorDe($<id>1) + $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "-=" sentencia             { $$ = valorDe($<id>1) - $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "*=" sentencia             { $$ = valorDe($<id>1) * $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "/=" sentencia             { double tmp = valorDe($<id>1); 
                                      $<nro>$ = controlDivision(tmp, $<nro>3); asignarA($<id>1, $<nro>$); }
    ;
expresion : ID                      { $$ = $<nro>1;     printf("ID\n");}
    | NRO                           { $$ = $1;          printf("Número\n");}
    | expresion '+' expresion       { $$ = $1 + $3;     printf("Suma\n");}			
    | expresion '-' expresion       { $$ = $1 - $3;     printf("Resta\n");}	
    | expresion '*' expresion       { $$ = $1 * $3;     printf("Multiplicación\n");}
    | expresion '/' expresion       { $<nro>$ = controlDivision($<nro>1, $<nro>3);  printf("División\n");}
    | expresion '^' expresion       { $$ = pow($1, $3); printf("Potenciación\n");}
    | '-' expresion  %prec NEG      { $$ = -$<nro>2;    printf("Cambio de signo\n");}
    | '(' expresion ')'             { $$ = $<nro>2;     printf("Cierra paréntesis\n");}
    | ID '(' expresion ')'          { $<nro>$ = llamarFuncion($<id>1, $<nro>3);     printf("Llamado a función\n");}
    ;


%%

void yyerror(const char *s){
	printf("línea #%d: %s\n", yylineno, s);
	return;
}
