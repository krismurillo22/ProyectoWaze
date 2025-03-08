#include "ruta.h"
#include <QPainter>

Ruta::Ruta() : ciudadInicio(nullptr), ciudadFin(nullptr){}

Ruta::Ruta(Ciudad* ciudadInicio, Ciudad* ciudadFin)
    : ciudadInicio(ciudadInicio), ciudadFin(ciudadFin) {}

void Ruta::agregarPuntoIntermedio(const QPoint& punto) {
    puntosIntermedios.push_back(punto);  // Añadir un nuevo punto intermedio
}

void Ruta::dibujarRuta(QPainter* painter) {
    QPainterPath path;
    int x1 = ciudadInicio->getX();
    int y1 = ciudadInicio->getY();
    int x2 = ciudadFin->getX();
    int y2 = ciudadFin->getY();

    // Empezar el path en el primer punto
    path.moveTo(x1, y1);

    // Dibujar los puntos intermedios como líneas
    for (const QPoint& punto : puntosIntermedios) {
        path.lineTo(punto);
    }

    // Terminar con la ciudad de fin
    path.lineTo(x2, y2);

    // Dibuja la ruta (curva) con el path
    painter->setPen(QPen(Qt::blue, 7));  // Ajustar color y grosor de la línea
    painter->drawPath(path);
}



