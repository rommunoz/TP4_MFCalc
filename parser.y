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
%precedence NEG
%right 	'^'

%code provides {
void yyerror(const char *);
extern int yylexerrs;
}
%union YYSTYPE {
    char*   id;
    double  nro;
}
%type   <id>  PR_VAR PR_SALIR ID 
%type   <nro> NRO 
%nterm  <nro> expresion

%%

programa: sesion                    { if (semerrs || yynerrs || yylexerrs) YYABORT;}
    ;
sesion 	: linea NL                  { printf("\n"); }
    | sesion linea NL               { printf("\n"); }
    ;
linea   :  expresion                { printf("Expresión, resultado: %f\n", $<nro>1); }
    | error
    | PR_VAR ID                     { if (fueDeclarado($<id>2)) { printf("Error semántico, ID ya declarado"); 
                                      } else {  declararId($<id>2); printf("Define ID como variable\n"); } 
                                    }
    | PR_VAR ID '=' expresion       { if (fueDeclarado($<id>2)) { printf("Error semántico, ID ya declarado");
                                      } else { declararId($<id>2); asignarA($<id>2, $<nro>4); 
                                        printf("Define ID '%s' como variable con valor inicial '%f'\n", $<id>2, $<nro>4 ); }
                                    }
    | PR_SALIR NL                   { printf("Palabra reservada salir\n\n"); return (yynerrs || yylexerrs);}
    ;
expresion : ID                      { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;}
                                        $$ = valorDe($<id>1);     printf("ID\n");}               
    | ID '=' expresion              { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;} 
                                        $$ = $<nro>3; asignarA($<id>1, $<nro>3); }
    | ID "+=" expresion             { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;} 
                                        $$ = valorDe($<id>1) + $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "-=" expresion             { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;} 
                                        $$ = valorDe($<id>1) - $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "*=" expresion             { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;} 
                                        $$ = valorDe($<id>1) * $<nro>3; asignarA($<id>1, $<nro>$); }
    | ID "/=" expresion             { if(!fueDeclarado($<id>1)) {yyerror("Error semántico, identificador no definido"); semerrs++; YYERROR;}
                                      if(!valorDe($<id>3))      {yyerror("Error semántico, división por cero"); semerrs++; YYERROR;}
                                      $$ = valorDe($<id>1) / $<nro>3; asignarA($<id>1, $<nro>$); }
    | NRO                           { $$ = $1;          printf("Número\n");}
    | expresion '+' expresion       { $$ = $1 + $3;     printf("Suma\n");}			
    | expresion '-' expresion       { $$ = $1 - $3;     printf("Resta\n");}	
    | expresion '*' expresion       { $$ = $1 * $3;     printf("Multiplicación\n");}
    | expresion '/' expresion       { if(!$<nro>3)      {yyerror("Error semántico, división por cero"); YYERROR;}
                                      $$ = $<nro>1 / $<nro>3;  printf("División\n");}
    | expresion '^' expresion       { $$ = pow($1, $3); printf("Potenciación\n");}
    | '-' expresion  %prec NEG      { $$ = -$<nro>2;    printf("Cambio de signo\n");}
    | '(' expresion ')'             { $$ = $<nro>2;     printf("Cierra paréntesis\n");}
    | ID '(' expresion ')'          { struct NodoId* nodo = buscarEnDict($<id>1);
                                      if(!esFuncion(nodo)) {yyerror("Error semántico, la funcion no está definida"); YYERROR;} 
                                      $<nro>$ = nodo->info.valor.fun ($<nro>3); printf("Llamado a función\n");}
    ;

%%

void yyerror(const char *s){
	printf("línea #%d: %s\n", yylineno, s);
	return;
}
