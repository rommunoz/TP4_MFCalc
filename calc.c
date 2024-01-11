#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#define DIM_FUNC 8


struct Identificador {
    char* nom;
    int   tipo;
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
enum TipoID {VAR, FUNC}; //si hubieran más palabras reservadas se podría hacer algo similar a lo hecho con las funciones
struct NodoId* diccionario;

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
        return;
    }
    struct NodoId* aux = *pila;
    (*pila) = (*pila)->sig;
    free(aux);
}

//despues se debe controlar el tipo del nodo encontrado
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
    struct NodoId* nodoid = buscarEnDict(id);
    return nodoid != NULL;
}

void declararId(char* id){
    struct Identificador info;
    info.nom = strdup(id);
    info.tipo = VAR;
    info.valor.var = 0.0;
    push(&info, &diccionario);
}

void asignarA(char* id, double nro){
    struct NodoId* variable = buscarEnDict(id);
    if(variable != NULL){
        variable->info.valor.var = nro;
    }
}

void iniciarDiccionario(void){
    diccionario = NULL;
    struct Identificador info;
    for (int i=0; i < DIM_FUNC; i++){
        info.nom = tablaDeFunciones[i].nom;
        info.tipo = FUNC;
        info.valor.var = 0.0; //lo inicializo aunque sea funcion
        info.valor.fun = tablaDeFunciones[i].direccion;
        push(&info, &diccionario);
    }
    declararId("pi");
    asignarA("pi", 3.14159);
    declararId("e");
    asignarA("e", 2.71828);
}

void vaciarLista(struct NodoId** lista){
    while ((*lista) != NULL){
        pop((lista));
    }
}


//booleano
int esFuncion (struct NodoId* nodo){
    return nodo != NULL && nodo->info.tipo == FUNC;
}

//accessor - surge por usar union en el parser y en el momento de asignar
double valorDe(char* idVariable){
    struct NodoId* nodId = buscarEnDict(idVariable);
        return nodId->info.valor.var;
}

/* ---------------------------- */
/* -- Fin funciones externas -- */
/* ---------------------------- */


void mostrarDiccionario(){
    struct NodoId* paux = diccionario;
    while(paux != NULL){
        if(paux->info.tipo == FUNC)
            printf("Funcion: %s, Direccion: \t%p\n", paux->info.nom, paux->info.valor.fun);
        if(paux->info.tipo == VAR)
            printf("Variable: %s, Valor: \t%lf\n", paux->info.nom, paux->info.valor.var);
        paux = paux->sig;
    }
}