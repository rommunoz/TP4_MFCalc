#include <stdio.h>
#include <strings.h>
#include <math.h>
#include "parser.h"
#define DIM_FUNC 8

struct Identificador {
    char* nom;
    int   tipo;
    double (*direccion)(double);
    double valor;
};

struct NodoId {
    struct Identificador info;
    struct NodoId* sig;
};

extern int semerrs = 0;
enum TipoID {VAR, FUNC}; //si hubieran más palabras reservadas se podría hacer algo similar a lo hecho con las funciones
struct NodoId* diccionario = NULL;

struct Funcion {
    char*   nom;
    double (*direccion)(double);
};

struct Funcion tablaDeFunciones[DIM_FUNC] = {
    {"sin" , sin},
    {"cos" , cos},
    {"tan" , tan},
    {"atan", atan},
    {"asin", asin},
    {"acos", acos},
    {"sqrt", sqrt},
    {"log" , log}
};

/* --------------------------- */
/* ---- Fin declaraciones ---- */
/* --------------------------- */

void push (struct Identificador* info, struct NodoId** pila){
    struct NodoId* nuevo = malloc(sizeof(struct NodoId));
    nuevo->info = *info;
    struct NodoId* aux = *pila;
    *pila = nuevo;
    nuevo->sig = aux;
}

void pop(struct NodoId** pila){
    if((*pila) == NULL){
        printf("Error interno, pop en lista vacía");
        return -1;
    }
    struct NodoId* aux = *pila;
    (*pila) = (*pila)->sig;
    free(aux);
}

//despues se debe controlar el tipo del id encontrado
struct NodoId* buscarEnDict(char* idVariable){
    struct NodoId* paux = diccionario;
    while(paux != NULL && strcmp(paux->info.nom, idVariable) != 0){ //estaria bueno una lista ordenada para optimizar
        paux = paux->sig;
    }
    return paux != NULL && strcmp(paux->info.nom, idVariable) == 0 ? paux: NULL;
}

/* ---------------------------- */
/* -- Fin funciones internas -- */
/* ---------------------------- */

//booleano
int fueDeclarado(char* id){
    struct NodoId* variable = buscarEnDict(id);
    return variable != NULL;
}

void declararId(char* id){
    struct Identificador info;
    info.nom = strdup(id);
    info.tipo = VAR;
    info.valor = 0;
    push(&info, &diccionario);
}

void iniciarDiccionario(void){
    struct Identificador info;
    for (int i=0; i < DIM_FUNC; i++){
        info.nom = tablaDeFunciones[i].nom;
        info.tipo = FUNC;
        info.direccion = tablaDeFunciones[i].direccion;
        push(&info, &diccionario);
    }
}

void vaciarLista(struct NodoId** lista){
    while ((*lista) != NULL){
        pop((lista));
    }
}

void asignarA(char* id, double nro){
    struct NodoId* variable = buscarEnDict(id);
    if(variable != NULL){
        variable->info.valor = nro;
    }
}

double llamarAFuncion(char* nombreFuncion, double argumento){
    struct NodoId* nodo = buscarEnDict(nombreFuncion);
    if (nodo != NULL && nodo->info.tipo == FUNC){
        double (*funcion)(double) = nodo->info.direccion;
        return (*funcion)(argumento);
    } else {
        printf("Error semántico, la funcion no está definida");
        YYERROR;
        return NAN;
    }
}

//accessor - surge por usar union en el parser y en el momento de asignar
double valorDe(char* idVariable){
    struct NodoId* nodId = buscarEnDict(idVariable);
    if(nodId != NULL && nodId->info.tipo == VAR){
        return nodId->info.valor;
    } else {
        printf("La variable '%s' es indefinida", idVariable);
        semerrs++;
        YYERROR;
        return NAN;
    }
}

/* ---------------------------- */
/* -- Fin funciones externas -- */
/* ---------------------------- */


void mostrarDiccionario(){
    struct NodoId* paux = diccionario;
    while(paux != NULL){
        if(paux->info.tipo == FUNC)
            printf("Funcion: %s, Direccion: \t%p\n", paux->info.nom, paux->info.direccion);
        if(paux->info.tipo == FUNC)
            printf("Variable: %s, Valor: \t%lf\n", paux->info.nom, paux->info.valor);
        paux = paux->sig;
    }
}