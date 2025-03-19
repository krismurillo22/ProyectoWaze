#ifndef ARBOLHISTORIAL_H
#define ARBOLHISTORIAL_H

#include <iostream>
#include <QString>
#include <QVector>

struct NodoHistorial {
    QString ruta;
    NodoHistorial* padre;
    NodoHistorial* izquierda;
    NodoHistorial* derecha;
    bool esRojo;

    NodoHistorial(const QString& r) : ruta(r), padre(nullptr), izquierda(nullptr), derecha(nullptr), esRojo(true) {}
};

class ArbolHistorial {
private:
    mutable NodoHistorial* raiz;
    void rotarIzquierda(NodoHistorial* &nodo) const;
    void rotarDerecha(NodoHistorial* &nodo) const;
    void balancearInsercion(NodoHistorial* &nodo) const;

public:
    ArbolHistorial();
    ~ArbolHistorial();
    void insertar(const QString& ruta) const;
    NodoHistorial* buscar(const QString& ruta) const;
    QString obtenerHistorial() const;
};

#endif // ARBOLHISTORIAL_H
