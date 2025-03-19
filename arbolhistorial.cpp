#include "arbolhistorial.h"
#include <QString>
#include <QMessageBox>

ArbolRN::ArbolRN() {
    nil = new NodoRN(-1, -1, {}, 0);
    nil->color = NEGRO;
    raiz = nil;
}

ArbolRN::~ArbolRN() {
    limpiar(raiz);
    delete nil;
}

void ArbolRN::limpiar(NodoRN* nodo) {
    if (nodo != nil) {
        limpiar(nodo->izquierda);
        limpiar(nodo->derecha);
        delete nodo;
    }
}

void ArbolRN::rotarIzquierda(NodoRN* x) {
    NodoRN* y = x->derecha;
    x->derecha = y->izquierda;
    if (y->izquierda != nil) y->izquierda->padre = x;
    y->padre = x->padre;
    if (x->padre == nullptr) raiz = y;
    else if (x == x->padre->izquierda) x->padre->izquierda = y;
    else x->padre->derecha = y;
    y->izquierda = x;
    x->padre = y;
}

void ArbolRN::rotarDerecha(NodoRN* x) {
    NodoRN* y = x->izquierda;
    x->izquierda = y->derecha;
    if (y->derecha != nil) y->derecha->padre = x;
    y->padre = x->padre;
    if (x->padre == nullptr) raiz = y;
    else if (x == x->padre->derecha) x->padre->derecha = y;
    else x->padre->izquierda = y;
    y->derecha = x;
    x->padre = y;
}

void ArbolRN::balancearInsercion(NodoRN* z) {
    while (z->padre && z->padre->color == ROJO) {
        NodoRN* abuelo = z->padre->padre;
        if (z->padre == abuelo->izquierda) {
            NodoRN* tio = abuelo->derecha;
            if (tio->color == ROJO) {
                z->padre->color = NEGRO;
                tio->color = NEGRO;
                abuelo->color = ROJO;
                z = abuelo;
            } else {
                if (z == z->padre->derecha) {
                    z = z->padre;
                    rotarIzquierda(z);
                }
                z->padre->color = NEGRO;
                abuelo->color = ROJO;
                rotarDerecha(abuelo);
            }
        } else {
            NodoRN* tio = abuelo->izquierda;
            if (tio->color == ROJO) {
                z->padre->color = NEGRO;
                tio->color = NEGRO;
                abuelo->color = ROJO;
                z = abuelo;
            } else {
                if (z == z->padre->izquierda) {
                    z = z->padre;
                    rotarDerecha(z);
                }
                z->padre->color = NEGRO;
                abuelo->color = ROJO;
                rotarIzquierda(abuelo);
            }
        }
    }
    raiz->color = NEGRO;
}

void ArbolRN::insertar(int inicio, int fin, const QVector<int>& ruta, double distancia) {
    NodoRN* nuevo = new NodoRN(inicio, fin, ruta, distancia);
    NodoRN* padre = nullptr;
    NodoRN* actual = raiz;

    while (actual != nil) {
        padre = actual;
        if (inicio < actual->idInicio || (inicio == actual->idInicio && fin < actual->idFin))
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }

    nuevo->padre = padre;
    if (!padre)
        raiz = nuevo;
    else if (inicio < padre->idInicio || (inicio == padre->idInicio && fin < padre->idFin))
        padre->izquierda = nuevo;
    else
        padre->derecha = nuevo;

    nuevo->izquierda = nil;
    nuevo->derecha = nil;
    nuevo->color = ROJO;
    balancearInsercion(nuevo);
}

QVector<int> ArbolRN::buscarRuta(int inicio, int fin, double& distancia) const {
    NodoRN* actual = raiz;
    while (actual != nil) {
        if (inicio == actual->idInicio && fin == actual->idFin) {
            distancia = actual->distancia;
            return actual->ruta;
        }
        if (inicio < actual->idInicio || (inicio == actual->idInicio && fin < actual->idFin))
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }
    distancia = -1;
    return {};
}

QString ArbolRN::obtenerHistorial() const {
    return obtenerHistorialHelper(raiz);
}

QString ArbolRN::obtenerHistorialHelper(NodoRN* nodo) const {
    if (nodo == nil) return "";

    QString historial;
    historial += obtenerHistorialHelper(nodo->izquierda);
    historial += QString("Ruta: %1 -> %2 | Distancia: %3 km\n")
                     .arg(nodo->idInicio)
                     .arg(nodo->idFin)
                     .arg(nodo->distancia);
    historial += obtenerHistorialHelper(nodo->derecha);

    return historial;
}


