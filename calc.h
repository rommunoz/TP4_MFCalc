#ifndef CALC_H
#define CALC_H

extern double result;
extern int semerrs;

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

extern struct NodoId *diccionario;

void iniciarDiccionario(void);
int fueDeclarado(char* idVariable);
struct NodoId* buscarEnDict(char* idVariable);
void declararId(char* idVariable);
void asignarA(char* idVariable, double exp);
int esFuncion (struct NodoId* nodo);
double valorDe(char* idVariable);
void vaciarLista(struct NodoId** lista);
void mostrarDiccionario(void);

#endif //CALC_H