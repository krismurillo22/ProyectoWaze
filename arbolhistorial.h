#ifndef ARBOLHISTORIAL_H
#define ARBOLHISTORIAL_H

#include <iostream>
#include <QString>
#include <QVector>

enum Color { ROJO, NEGRO };

struct NodoRN {
    int idInicio, idFin;
    QVector<int> ruta;
    double distancia;
    Color color;
    NodoRN* izquierda;
    NodoRN* derecha;
    NodoRN* padre;

    NodoRN(int inicio, int fin, const QVector<int>& r, double d)
        : idInicio(inicio), idFin(fin), ruta(r), distancia(d),
          color(ROJO), izquierda(nullptr), derecha(nullptr), padre(nullptr) {}
};

class ArbolRN {
private:
    NodoRN* raiz;
    NodoRN* nil; // Nodo nulo para facilitar balanceo

    void rotarIzquierda(NodoRN* x);
    void rotarDerecha(NodoRN* x);
    void balancearInsercion(NodoRN* z);
    void limpiar(NodoRN* nodo);

public:
    ArbolRN();
    ~ArbolRN();
    void insertar(int inicio, int fin, const QVector<int>& ruta, double distancia);
    QVector<int> buscarRuta(int inicio, int fin, double& distancia) const;
    QString obtenerHistorial() const;
    QString obtenerHistorialHelper(NodoRN* nodo) const;
};


#endif // ARBOLHISTORIAL_H
