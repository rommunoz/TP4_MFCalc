#include <stdio.h>
#include "scanner.h"
#include "calc.h"
#include "parser.h"

int yylexerrs = 0;
int semerrs = 0;
int main(void){
    iniciarDiccionario();
	yyparse();
	vaciarLista(&diccionario);
	return 0;
}