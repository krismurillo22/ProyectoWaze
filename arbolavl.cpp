#include "arbolavl.h"
#include "grafo.h"
#include <algorithm>

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

int ArbolAVL::altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int ArbolAVL::factorEquilibrio(NodoAVL* nodo) {
    return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
}

NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = std::max(altura(y->izq), altura(y->der)) + 1;
    x->altura = std::max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = std::max(altura(x->izq), altura(x->der)) + 1;
    y->altura = std::max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

NodoAVL* ArbolAVL::insertarRec(NodoAVL* nodo, const Nodo& nuevoNodo) {
    if (!nodo) return new NodoAVL(nuevoNodo);

    if (nuevoNodo.id < nodo->dato.id)
        nodo->izq = insertarRec(nodo->izq, nuevoNodo);
    else if (nuevoNodo.id > nodo->dato.id)
        nodo->der = insertarRec(nodo->der, nuevoNodo);
    else
        return nodo;

    nodo->altura = 1 + std::max(altura(nodo->izq), altura(nodo->der));

    int balance = factorEquilibrio(nodo);

    if (balance > 1 && nuevoNodo.id < nodo->izq->dato.id)
        return rotacionDerecha(nodo);

    if (balance < -1 && nuevoNodo.id > nodo->der->dato.id)
        return rotacionIzquierda(nodo);

    if (balance > 1 && nuevoNodo.id > nodo->izq->dato.id) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && nuevoNodo.id < nodo->der->dato.id) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void ArbolAVL::insertar(const Nodo& nuevoNodo) {
    raiz = insertarRec(raiz, nuevoNodo);
}

void ArbolAVL::obtenerNodosRec(NodoAVL* nodo, QVector<Nodo>& lista) const {
    if (!nodo) return;
    obtenerNodosRec(nodo->izq, lista);
    lista.append(nodo->dato);
    obtenerNodosRec(nodo->der, lista);
}

QVector<Nodo> ArbolAVL::obtenerNodos() const {
    QVector<Nodo> lista;
    obtenerNodosRec(raiz, lista);
    return lista;
}
