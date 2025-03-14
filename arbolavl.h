#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include "grafo.h"
#include <QVector>

struct NodoAVL {
    Nodo dato;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(const Nodo& nuevoNodo) : dato(nuevoNodo), izq(nullptr), der(nullptr), altura(1) {}
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* nodo);
    int factorEquilibrio(NodoAVL* nodo);
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);
    NodoAVL* insertarRec(NodoAVL* nodo, const Nodo& nuevoNodo);
    void obtenerNodosRec(NodoAVL* nodo, QVector<Nodo>& lista) const;

public:
    ArbolAVL();
    void insertar(const Nodo& nuevoNodo);
    QVector<Nodo> obtenerNodos() const;
};

#endif // ARBOLAVL_H

