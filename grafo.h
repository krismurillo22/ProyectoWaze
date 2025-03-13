#ifndef GRAFO_H
#define GRAFO_H

#include <QPoint>
#include <QVector>
#include <QString>

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

class Grafo {
public:
    void agregarNodo(const QString& nombre, const QPoint& posicion);
    void agregarArista(int idNodoOrigen, int idNodoDestino, const QVector<QPoint>& puntosIntermedios);
    QVector<Nodo> obtenerNodos() const;
    QVector<Arista> obtenerAristas() const;
    QVector<int> dijkstra(int idNodoInicio, int idNodoFin, double& distanciaTotal) const;

private:
    QVector<Nodo> nodos;
    QVector<Arista> aristas;
};

#endif // GRAFO_H
