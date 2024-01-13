#ifndef CALC_H
#define CALC_H

extern double result;
extern int semerrs;

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

extern struct NodoId temp;
extern struct NodoId *diccionario;

void iniciarDiccionario(void);
int fueDeclarado(struct NodoId* nodo);
struct NodoId* buscarEnDict(char* idVariable);
struct NodoId* declararId(char* idVariable);
void asignarA(struct NodoId* nodvar, const double num);
int esFuncion (struct NodoId* nodo);
double valorDe(struct NodoId* nodId);
void vaciarLista(struct NodoId** lista);

#endif //CALC_H