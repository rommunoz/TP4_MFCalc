#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "calc.h"

int yylexerrs = 0;
int main(void){
    iniciarDiccionario();
	mostrarDiccionario();
	int parseRes = yyparse();
	printf("Fin del parser: %d", parseRes);
	vaciarLista(&diccionario);
	return 0;
}