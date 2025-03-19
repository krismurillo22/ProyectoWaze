#include "arbolhistorial.h"
#include <QString>
#include <QMessageBox>

ArbolHistorial::ArbolHistorial() : raiz(nullptr) {}

ArbolHistorial::~ArbolHistorial() {
    // No es necesario limpiar, ya que no hay operaciones adicionales que realizar
}

void ArbolHistorial::insertar(const QString& ruta) const{
    NodoHistorial* nuevo = new NodoHistorial(ruta);
    if (!raiz) {
        raiz = nuevo;
        raiz->esRojo = false;
        return;
    }

    NodoHistorial* actual = raiz;
    NodoHistorial* padre = nullptr;
    while (actual) {
        padre = actual;
        if (ruta < actual->ruta)
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }

    nuevo->padre = padre;
    if (ruta < padre->ruta)
        padre->izquierda = nuevo;
    else
        padre->derecha = nuevo;

    balancearInsercion(nuevo);
}

NodoHistorial* ArbolHistorial::buscar(const QString& ruta) const {
    NodoHistorial* actual = raiz;
    while (actual) {
        if (ruta == actual->ruta)
            return actual;
        else if (ruta < actual->ruta)
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }
    return nullptr;  // Si no se encuentra la ruta
}

void ArbolHistorial::rotarIzquierda(NodoHistorial* &nodo) const{
    NodoHistorial* derecho = nodo->derecha;
    nodo->derecha = derecho->izquierda;
    if (derecho->izquierda)
        derecho->izquierda->padre = nodo;
    derecho->padre = nodo->padre;
    if (!nodo->padre)
        raiz = derecho;
    else if (nodo == nodo->padre->izquierda)
        nodo->padre->izquierda = derecho;
    else
        nodo->padre->derecha = derecho;
    derecho->izquierda = nodo;
    nodo->padre = derecho;
}

void ArbolHistorial::rotarDerecha(NodoHistorial* &nodo) const{
    NodoHistorial* izquierdo = nodo->izquierda;
    nodo->izquierda = izquierdo->derecha;
    if (izquierdo->derecha)
        izquierdo->derecha->padre = nodo;
    izquierdo->padre = nodo->padre;
    if (!nodo->padre)
        raiz = izquierdo;
    else if (nodo == nodo->padre->derecha)
        nodo->padre->derecha = izquierdo;
    else
        nodo->padre->izquierda = izquierdo;
    izquierdo->derecha = nodo;
    nodo->padre = izquierdo;
}

void ArbolHistorial::balancearInsercion(NodoHistorial* &nodo) const {
    while (nodo->padre && nodo->padre->esRojo) {
        NodoHistorial* abuelo = nodo->padre->padre;
        if (nodo->padre == abuelo->izquierda) {
            NodoHistorial* tio = abuelo->derecha;
            if (tio && tio->esRojo) {
                nodo->padre->esRojo = false;
                tio->esRojo = false;
                abuelo->esRojo = true;
                nodo = abuelo;
            } else {
                if (nodo == nodo->padre->derecha) {
                    nodo = nodo->padre;
                    rotarIzquierda(nodo);
                }
                nodo->padre->esRojo = false;
                abuelo->esRojo = true;
                rotarDerecha(abuelo);
            }
        } else {
            NodoHistorial* tio = abuelo->izquierda;
            if (tio && tio->esRojo) {
                nodo->padre->esRojo = false;
                tio->esRojo = false;
                abuelo->esRojo = true;
                nodo = abuelo;
            } else {
                if (nodo == nodo->padre->izquierda) {
                    nodo = nodo->padre;
                    rotarDerecha(nodo);
                }
                nodo->padre->esRojo = false;
                abuelo->esRojo = true;
                rotarIzquierda(abuelo);
            }
        }
    }
    raiz->esRojo = false;
}

QString ArbolHistorial::obtenerHistorial() const {
    QVector<NodoHistorial*> pila;
    NodoHistorial* actual = raiz;
    QString historial;
    while (actual || !pila.isEmpty()) {
        while (actual) {
            pila.push_back(actual);
            actual = actual->izquierda;
        }
        actual = pila.back();
        pila.pop_back();
        historial += actual->ruta + "\n";
        actual = actual->derecha;
    }
    return historial;
}
