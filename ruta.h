#ifndef RUTA_H
#define RUTA_H

#include "ciudad.h"
#include <QPainterPath>
#include <QVector>
#include <QPoint>

class Ruta {
public:
    Ruta();
    Ruta(Ciudad* ciudadInicio, Ciudad* ciudadFin);

    // Métodos para agregar puntos intermedios
    void agregarPuntoIntermedio(const QPoint& punto);

    void dibujarRuta(QPainter* painter);

private:
    Ciudad* ciudadInicio;
    Ciudad* ciudadFin;
    QVector<QPoint> puntosIntermedios;  // Puntos intermedios para suavizar la curva
};

#endif // RUTA_H
