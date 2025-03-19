#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grafo.h"
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "arbolhistorial.h"

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
    void actualizarLabels(double distanciaReal, const QVector<int>& ruta);
    QVector<int> seleccionarParadas(int idOrigen, int idDestino, const QVector<int>& rutaMasCorta);

private slots:
    void on_pushButton_clicked();

    void agregarCiudad(const QString& nombre, int x, int y);
    void inicializarCiudades();

    void on_actionCARGAR_triggered();

    void on_pushButton_3_clicked();

    void on_restablecer_clicked();

    void on_recorrido_clicked();

    void on_actionModo_Claro_triggered();

    void on_actionModo_Oscuro_2_triggered();

    void on_actionCAMBIAR_COLOR_A_LAS_RUTAS_triggered();

    void on_actionVer_Historial_triggered();

    void on_actionGUIA_triggered();

    void on_regresar_clicked();

    void on_anterior_clicked();

    void on_siguiente_clicked();

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
    QColor colorDefecto;
    QColor colorRuta;
    int indiceImagen = 0;
    QString listaImagenes[4] = {":/fondos/guia1.png", ":/fondos/guia2.png", ":/fondos/guia3.png", ":/fondos/guia4.png"};
    QVector<QVector<int>> rutasEncontradas;
    QVector<int> rutaElegida;
    ArbolHistorial arbolHistorial;
    QVector<QPair<QString, QString>> historial;
};

#endif // MAINWINDOW_H
