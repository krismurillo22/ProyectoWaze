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

QVector<int> Grafo::reconstruirRuta(QMap<int, int>& padres, int destino) {
    QVector<int> ruta;
    for (int at = destino; at != -1; at = padres[at]) {
        ruta.prepend(at);
    }
    return ruta;
}

// Algoritmo de Dijkstra
QVector<int> Grafo::calcularRutaDijkstra(int origen, int destino) {
    QMap<int, int> distancia;  // Almacena la distancia mínima a cada nodo
    QMap<int, int> padres;
    const int INF = std::numeric_limits<int>::max();

    // Inicializar todas las distancias a infinito
    for (const Nodo& nodo : obtenerNodos()) {
        distancia[nodo.id] = INF;
        padres[nodo.id] = -1;
    }

    // Cola de prioridad (peso, nodo)
    std::priority_queue<QPair<int, int>, QVector<QPair<int, int>>, std::greater<QPair<int, int>>> cola;

    // Distancia al nodo de origen es 0
    distancia[origen] = 0;
    cola.push(qMakePair(0, origen));

    while (!cola.empty()) {
        int nodoActual = cola.top().second;
        int costoActual = cola.top().first;
        cola.pop();

        // Si llegamos al destino, terminamos
        if (nodoActual == destino) break;

        // Procesar vecinos
        for (const Arista& arista : obtenerAristas()) {
            if (arista.idNodoOrigen == nodoActual || arista.idNodoDestino == nodoActual) {
                int vecino = (arista.idNodoOrigen == nodoActual) ? arista.idNodoDestino : arista.idNodoOrigen;
                int nuevoCosto = costoActual + arista.peso;

                if (nuevoCosto < distancia[vecino]) {
                    distancia[vecino] = nuevoCosto;
                    padres[vecino] = nodoActual;
                    cola.push(qMakePair(nuevoCosto, vecino));
                }
            }
        }
    }

    // Reconstruir ruta más corta
    QVector<int> ruta;
    for (int at = destino; at != -1; at = padres[at]) {
        ruta.prepend(at);
    }

    // Mostrar la ruta encontrada
    qDebug() << "Ruta más corta de" << origen << "a" << destino << ":";
    for (int nodo : ruta) {
        qDebug() << nodo;
    }
}
