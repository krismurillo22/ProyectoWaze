#include "ciudad.h"
Ciudad::Ciudad() : nombre(""), x(0), y(0) {}

Ciudad::Ciudad(QString nombre, int x, int y)
    : nombre(nombre), x(x), y(y) {}

QString Ciudad::getNombre() const {
    return nombre;
}

int Ciudad::getX() const {
    return x;
}

int Ciudad::getY() const {
    return y;
}
