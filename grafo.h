#ifndef GRAFO_H
#define GRAFO_H

#include <QPoint>
#include <QVector>
#include <QString>
#include <QFile>
#include <QDataStream>
#include "ArbolHistorial.h"

struct Nodo {
    int id;
    QString nombre;
    QPoint posicion;
};

struct Arista {
    int idNodoOrigen;
    int idNodoDestino;
    QVector<QPoint> puntosIntermedios;
    double peso;
};

class AVLTree {
private:
    struct NodoAVL {
        Nodo nodo;
        NodoAVL* izquierdo;
        NodoAVL* derecho;
        int altura;

        NodoAVL(const Nodo& n) : nodo(n), izquierdo(nullptr), derecho(nullptr), altura(1) {}
    };

    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int obtenerBalance(NodoAVL* nodo) {
        return nodo ? obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho) : 0;
    }

    NodoAVL* rotarDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierdo;
        NodoAVL* T2 = x->derecho;
        x->derecho = y;
        y->izquierdo = T2;
        y->altura = 1 + qMax(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho));
        x->altura = 1 + qMax(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho));
        return x;
    }

    NodoAVL* rotarIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecho;
        NodoAVL* T2 = y->izquierdo;
        y->izquierdo = x;
        x->derecho = T2;
        x->altura = 1 + qMax(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho));
        y->altura = 1 + qMax(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho));
        return y;
    }

    NodoAVL* insertarNodo(NodoAVL* nodo, const Nodo& nuevoNodo) {
        if (!nodo) return new NodoAVL(nuevoNodo);

        if (nuevoNodo.id < nodo->nodo.id)
            nodo->izquierdo = insertarNodo(nodo->izquierdo, nuevoNodo);
        else if (nuevoNodo.id > nodo->nodo.id)
            nodo->derecho = insertarNodo(nodo->derecho, nuevoNodo);
        else
            return nodo;

        nodo->altura = 1 + qMax(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

        int balance = obtenerBalance(nodo);

        if (balance > 1 && nuevoNodo.id < nodo->izquierdo->nodo.id)
            return rotarDerecha(nodo);
        if (balance < -1 && nuevoNodo.id > nodo->derecho->nodo.id)
            return rotarIzquierda(nodo);
        if (balance > 1 && nuevoNodo.id > nodo->izquierdo->nodo.id) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }
        if (balance < -1 && nuevoNodo.id < nodo->derecho->nodo.id) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    void obtenerNodosEnOrden(NodoAVL* nodo, QVector<Nodo>& nodos) const {
        if (!nodo) return;
        obtenerNodosEnOrden(nodo->izquierdo, nodos);
        nodos.append(nodo->nodo);
        obtenerNodosEnOrden(nodo->derecho, nodos);
    }

    Nodo* buscarNodo(NodoAVL* nodo, int id) const {
        if (!nodo) return nullptr;
        if (id == nodo->nodo.id) return &nodo->nodo;
        if (id < nodo->nodo.id) return buscarNodo(nodo->izquierdo, id);
        return buscarNodo(nodo->derecho, id);
    }

public:
    AVLTree() : raiz(nullptr) {}

    void insertar(const Nodo& nodo) {
        raiz = insertarNodo(raiz, nodo);
    }

    QVector<Nodo> obtenerTodos() const {
        QVector<Nodo> nodos;
        obtenerNodosEnOrden(raiz, nodos);
        return nodos;
    }

    Nodo* buscar(int id) const {
        return buscarNodo(raiz, id);
    }
};

class Grafo {
public:
    Grafo();
    ~Grafo();
    void agregarNodo(const QString& nombre, const QPoint& posicion);
    void agregarArista(int idNodoOrigen, int idNodoDestino, const QVector<QPoint>& puntosIntermedios);
    QVector<Nodo> obtenerNodos() const;
    QVector<Arista> obtenerAristas() const;
    void actualizarHistorial(const QString& ruta) const;
    QVector<int> dijkstra(int idNodoInicio, int idNodoFin, double& distanciaTotal) const;
    void guardarEnArchivo(const QString& nombreArchivo) const ;
    void cargarDesdeArchivo(const QString& nombreArchivo);
    QVector<QVector<int>> obtenerTodasLasRutas(int idNodoInicio, int idNodoFin) const;
    QSet<int> obtenerCiudadesEntre(int idNodoInicio, int idNodoFin) const;
    QVector<int> calcularRutaConParadas(const QVector<int>& paradas, double& distanciaTotal) const;
    QString obtenerNombreNodo(int id) const;
    ArbolHistorial historialRutas;

private:
    AVLTree nodosAVL;
    QVector<Arista> aristas;
    void todasLasRutas(int idNodoInicio, int idNodoFin, QVector<QVector<int>>& rutas, QVector<int> rutaActual, QSet<int> nodosVisitados) const;
};

#endif // GRAFO_H
