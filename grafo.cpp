#include "grafo.h"
#include <QVector>
#include <QMap>
#include <QSet>
#include <queue>
#include <QPair>
#include <QtMath>
#include <QDebug>

void Grafo::agregarNodo(const QString& nombre, const QPoint& posicion) {
    int nuevoId = nodos.size();
    nodos.append({nuevoId, nombre, posicion});
}

void Grafo::agregarArista(int idNodoOrigen, int idNodoDestino, const QVector<QPoint>& puntosIntermedios) {
    Arista nuevaArista;
    nuevaArista.idNodoOrigen = idNodoOrigen;
    nuevaArista.idNodoDestino = idNodoDestino;
    nuevaArista.puntosIntermedios = puntosIntermedios;
    nuevaArista.peso = puntosIntermedios.size();
    aristas.push_back(nuevaArista);
}

QVector<Nodo> Grafo::obtenerNodos() const {
    return nodos;
}

QVector<Arista> Grafo::obtenerAristas() const {
    return aristas;
}

QVector<int> Grafo::dijkstra(int idNodoInicio, int idNodoFin, double& distanciaTotal) const {
    QMap<int, double> distancias;
    QMap<int, int> previos;
    QSet<int> nodosNoVisitados;

    for (const Nodo& nodo : nodos) {
        distancias[nodo.id] = std::numeric_limits<double>::infinity();
        previos[nodo.id] = -1;
        nodosNoVisitados.insert(nodo.id);
    }

    distancias[idNodoInicio] = 0;

    while (!nodosNoVisitados.isEmpty()) {
        int nodoActual = -1;
        double distanciaMinima = std::numeric_limits<double>::infinity();

        for (int nodo : nodosNoVisitados) {
            if (distancias[nodo] < distanciaMinima) {
                nodoActual = nodo;
                distanciaMinima = distancias[nodo];
            }
        }

        if (nodoActual == -1 || nodoActual == idNodoFin) {
            break;
        }

        nodosNoVisitados.remove(nodoActual);

        for (const Arista& arista : aristas) {
            if (arista.idNodoOrigen == nodoActual || arista.idNodoDestino == nodoActual) {
                int nodoVecino = (arista.idNodoOrigen == nodoActual) ? arista.idNodoDestino : arista.idNodoOrigen;
                double distanciaAlternativa = distancias[nodoActual] + arista.peso;

                if (distanciaAlternativa < distancias[nodoVecino]) {
                    distancias[nodoVecino] = distanciaAlternativa;
                    previos[nodoVecino] = nodoActual;
                }
            }
        }
    }

    distanciaTotal = distancias[idNodoFin];
    QVector<int> ruta;
    int nodoActual = idNodoFin;

    while (nodoActual != -1) {
        ruta.prepend(nodoActual);
        nodoActual = previos[nodoActual];
    }

    return ruta;
}
