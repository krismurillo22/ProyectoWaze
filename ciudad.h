#ifndef CIUDAD_H
#define CIUDAD_H

#include <QString>


class Ciudad {
public:
    Ciudad();
    Ciudad(QString nombre, int x, int y);

    QString getNombre() const;
    int getX() const;
    int getY() const;

private:
    QString nombre;
    int x, y;
};

#endif // CIUDAD_H
