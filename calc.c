#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#define DIM_FUNC 8
#define DIM_CONST 2


struct Identificador {
    char* nom;
    int   tipo;
    int   declarado;
    union {
        double (*fun)(double);
        double var;
    } valor;
};

struct NodoId {
    struct Identificador info;
    struct NodoId* sig;
};

extern int semerrs;
enum TipoID {VAR, FUNC};
struct NodoId* diccionario;
struct NodoId temp;

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

struct Constante {
    char*   nom;
    double  valor;
};

struct Constante constantes[DIM_CONST] = {
    {"e" , 2.71828},
    {"pi", 3.14159}
};

/* --------------------------- */
/* ---- Fin declaraciones ---- */
/* --------------------------- */

struct NodoId* push (struct Identificador* info, struct NodoId** pila){
    struct NodoId* nuevo = malloc(sizeof(struct NodoId));
    nuevo->info = *info;
    struct NodoId* aux = *pila;
    *pila = nuevo;
    nuevo->sig = aux;
    return nuevo;
}

void pop(struct NodoId** pila){
    if((*pila) == NULL){
        printf("Error interno, pop en lista vacía");
        return;
    }
    struct NodoId* aux = *pila;
    (*pila) = (*pila)->sig;
    free(aux);
}

/* ---------------------------- */
/* -- Fin funciones internas -- */
/* ---------------------------- */

//despues se debe controlar el tipo del nodo encontrado
struct NodoId* buscarEnDict(char* idVariable){
    struct NodoId* paux = diccionario;
    while(paux != NULL && strcmp(paux->info.nom, idVariable) != 0){ //estaria bueno una lista ordenada para optimizar
        paux = paux->sig;
    }
    return paux != NULL && strcmp(paux->info.nom, idVariable) == 0 ? paux: NULL;
}

struct NodoId* declararId(char* id){
    struct Identificador info;
    info.nom = strdup(id);
    info.tipo = VAR;
    info.declarado = 1;
    info.valor.var = 0.0;
    return push(&info, &diccionario);
}

void asignarA(struct NodoId* nodvar, const double num){
    if(nodvar != NULL){
        nodvar->info.valor.var = num;
    }
}

void iniciarDiccionario(void){
    diccionario = NULL;
    struct Identificador info;
    for (int i=0; i < DIM_FUNC; i++){
        info.nom = tablaDeFunciones[i].nom;
        info.tipo = FUNC;
        info.declarado = 1;
        info.valor.fun = tablaDeFunciones[i].direccion;
        push(&info, &diccionario);
    }
    for (int i=0; i < DIM_CONST; i++){//esto por si se agregan mas constantes, aunque al ser dos tomaría 4 lineas de código
        info.nom = constantes[i].nom;
        info.tipo = VAR;
        info.declarado = 1;
        info.valor.var = constantes[i].valor;
        push(&info, &diccionario);
    }
    printf("> ");
}


//booleano
int fueDeclarado (struct NodoId* nodo){
    return nodo->info.declarado && nodo->info.tipo == VAR;
}

//booleano
int esFuncion (struct NodoId* nodo){
    return nodo->info.declarado && nodo->info.tipo == FUNC;
}

//accessor - surge por usar union en el parser y en el momento de asignar
double valorDe(struct NodoId* nodId){
    return nodId->info.valor.var;
}

void vaciarLista(struct NodoId** lista){
    while ((*lista) != NULL){
        pop((lista));
    }
}

/* ---------------------------- */
/* -- Fin funciones externas -- */
/* ---------------------------- */