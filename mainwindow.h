#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grafo.h"
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dibujarGrafo(const QVector<int>& rutaMasCorta = QVector<int>());
    double convertirAPesoReal(int peso);
    void moverCarro();

private slots:
    void on_pushButton_clicked();

    void agregarCiudad(const QString& nombre, int x, int y);
    void inicializarCiudades();

    void on_actionCARGAR_triggered();

    void on_pushButton_3_clicked();

    void on_restablecer_clicked();

    void on_recorrido_clicked();

private:
    Ui::MainWindow *ui;
    Grafo grafo;
    QWidget *mapaWidget;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *carroItem;
    QTimer *timer;
    QVector<QPoint> rutaCarro;
    int indiceRuta;
    int velocidad;

};
#endif // MAINWINDOW_H
