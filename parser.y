%code top{
#include <stdio.h>
#include <math.h>
#include <ctype.h>
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
%precedence NEG
%right 	'^'

%code provides {
void yyerror(const char *);
void semerror(const char *);
extern int yylexerrs;
}
%union YYSTYPE {
    struct NodoId* sym; //simbolo de la tabla
    double  nro;
}
%type   <sym> ID 
%type   <nro> NRO 
%nterm  <nro> expresion

%%

programa: sesion                { if (semerrs || yynerrs || yylexerrs) YYABORT;}
    ;
sesion 	: linea NL              
    | sesion linea NL           
    ;
linea   :  expresion            { printf(" %f\n> ", $<nro>1); }
    | error
    | PR_VAR ID                 { if ($2->info.declarado)   {semerror("Error, ID ya declarado\n> "); YYERROR;} 
                                    declararId($2->info.nom);
                                    printf("%s: '%f'\n> ", $2->info.nom, $2->info.valor.var);
                                }
    | PR_VAR ID '=' expresion   { if ($2->info.declarado)   {semerror("Error, ID ya declarado\n> "); YYERROR;}
                                    struct NodoId* nuevo = declararId($2->info.nom);
                                    asignarA(nuevo, $4);
                                    printf("%s: '%f'\n> ", nuevo->info.nom, nuevo->info.valor.var);
                                }
    | PR_SALIR                  { return (yynerrs || yylexerrs || semerrs);}
    ;
expresion : ID                  { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    $$ = valorDe($1);
                                }               
    | ID '=' expresion          { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    $$ = $3;                 
                                    asignarA($1, $3); 
                                }
    | ID "+=" expresion         { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    $$ = valorDe($1) + $3;   
                                    asignarA($1, $$); 
                                }
    | ID "-=" expresion         { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    $$ = valorDe($1) - $3;   
                                    asignarA($1, $$); 
                                }
    | ID "*=" expresion         { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    $$ = valorDe($1) * $3;   
                                    asignarA($1, $$); 
                                }
    | ID "/=" expresion         { if(! fueDeclarado($1))    {semerror("ID no definido"); YYERROR;}
                                    if(! $3)                {semerror("Error, división por cero"); YYERROR;}
                                    $$ = valorDe($1) / $3;   
                                    asignarA($1, $$); 
                                }
    | NRO                           
    | expresion '+' expresion   { $$ = $1 + $3; }			
    | expresion '-' expresion   { $$ = $1 - $3; }	
    | expresion '*' expresion   { $$ = $1 * $3; }
    | expresion '/' expresion   { if(!$3)                   {semerror("Error, división por cero"); YYERROR;}
                                    $$ = $1 / $3;   
                                }
    | expresion '^' expresion   { $$ = pow($1, $3); }
    | '-' expresion  %prec NEG  { $$ = -$2; }
    | '(' expresion ')'         { $$ = $2;  }
    | ID '(' expresion ')'      { if(! esFuncion($1))       {semerror("Error, la función no está definida"); YYERROR;}
                                  if(! isnormal($1->info.valor.fun ($<nro>3))) {semerror("Error matemático"); YYERROR;}
                                    $$ = $1->info.valor.fun ($<nro>3);
                                }  
    ;

%%

void yyerror(const char *s){
	printf("línea #%d: %s\n> ", yylineno, s);
	return;
}

void semerror (const char *s){
    yyerror(s);
    semerrs++;
    return;
}