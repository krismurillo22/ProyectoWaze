#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grafo.h"
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void dibujarGrafo();
    void agregarCiudad(const QString& nombre, int x, int y);
    void inicializarCiudades();

    void on_actionCARGAR_triggered();

private:
    Ui::MainWindow *ui;
    Grafo grafo;
    QWidget *mapaWidget;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
