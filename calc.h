#ifndef CALC_H
#define CALC_H

extern double result;
extern int semerrs;

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

struct NodoId* diccionario;

void iniciarDiccionario(void);
int fueDeclarado(char* idVariable);
void declararId(char* idVariable);
void asignarA(char* idVariable, double exp);
double controlDivision(double arg1, double arg2);
double llamarFuncion(char* idFunc, double arg);
double valorDe(char* idVariable);
void vaciarLista(struct NodoId** lista);
void mostrarDiccionario(void);

#endif //CALC_H