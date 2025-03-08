#include "grafo.h"
#include <cmath>

void Grafo::agregarNodo(const QString& nombre, const QPoint& posicion) {
    Nodo nuevoNodo;
    nuevoNodo.nombre = nombre;
    nuevoNodo.posicion = posicion;
    nodos.push_back(nuevoNodo);
}

void Grafo::agregarArista(int idNodoOrigen, int idNodoDestino, const QVector<QPoint>& puntosIntermedios) {
    Arista nuevaArista;
    nuevaArista.idNodoOrigen = idNodoOrigen;
    nuevaArista.idNodoDestino = idNodoDestino;
    nuevaArista.puntosIntermedios = puntosIntermedios;

    QPoint origen = nodos[idNodoOrigen].posicion;
    QPoint destino = nodos[idNodoDestino].posicion;

    double distanciaTotal = std::sqrt(std::pow(destino.x() - origen.x(), 2) + std::pow(destino.y() - origen.y(), 2));

    // Sumar la distancia de todos los puntos intermedios
    double distanciaPromedio = distanciaTotal / (puntosIntermedios.size() + 1); // incluyendo el origen y el destino

    // Calcular el peso de la arista como la distancia promedio
    nuevaArista.peso = distanciaPromedio;
    aristas.push_back(nuevaArista);
}

QVector<Nodo> Grafo::obtenerNodos() const {
    return nodos;
}

QVector<Arista> Grafo::obtenerAristas() const {
    return aristas;
}
