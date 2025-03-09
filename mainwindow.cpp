#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::dibujarGrafo() {
    const QVector<Nodo>& nodos = grafo.obtenerNodos();
    for (const Nodo& nodo : nodos) {
        scene->addEllipse(nodo.posicion.x(), nodo.posicion.y(), 8, 8,
                          QPen(Qt::NoPen), QBrush(QColor(245, 3, 144)));
    }

    // Dibujar aristas asegurando que la línea pase por el centro de cada nodo
    for (const Arista& arista : grafo.obtenerAristas()) {
        QPainterPath path;

        // Obtener el centro del nodo origen
        QPoint origen = nodos[arista.idNodoOrigen].posicion + QPoint(4, 4);
        path.moveTo(origen);

        // Agregar los puntos intermedios
        for (const QPoint& punto : arista.puntosIntermedios) {
            path.lineTo(punto + QPoint(4, 4));
        }

        // Conectar el último punto intermedio con el centro del nodo destino
        QPoint destino = nodos[arista.idNodoDestino].posicion + QPoint(4, 4);
        path.lineTo(destino);

        // Dibujar la línea ajustada
        scene->addPath(path, QPen(QColor(245, 3, 144), 3));

        qDebug() << "origen:" << arista.idNodoOrigen << "destino" << arista.idNodoDestino;
        qDebug() << "peso:" << arista.peso;
    }
}


void MainWindow::agregarCiudad(const QString& nombre, int x, int y) {
    grafo.agregarNodo(nombre, QPoint(x, y));
    update();
}

void MainWindow::inicializarCiudades() {
    agregarCiudad("Puerto Cortes", 408, 197);
    agregarCiudad("Omoa", 393, 212);
    agregarCiudad("Choloma", 402, 227);
    agregarCiudad("Choluteca", 506, 552);
    agregarCiudad("San Pedro Sula", 394, 242);
    agregarCiudad("Santa Barbara", 365, 325);
    agregarCiudad("La Lima", 411, 254);
    agregarCiudad("Tela", 478, 209);
    agregarCiudad("La Ceiba", 563, 211);
    agregarCiudad("El Progreso", 425, 263);
    agregarCiudad("Morazan", 456, 273);
    agregarCiudad("Santa Rita", 415, 285);
    agregarCiudad("Yoro", 515, 298);
    agregarCiudad("Olanchito", 593, 248);
    agregarCiudad("Trujillo", 679, 191);
    agregarCiudad("Sonaguera", 631, 233);
    agregarCiudad("Tocoa", 664, 236);
    agregarCiudad("Barra Patuca", 902, 205);
    agregarCiudad("Puerto Lempira", 964, 281);
    agregarCiudad("Catacamas", 712, 326);
    agregarCiudad("Juticalpa", 683, 343);
    agregarCiudad("Valencia", 772, 357);
    agregarCiudad("La Entrada", 292, 312);
    agregarCiudad("Copan", 241, 338);
    agregarCiudad("Santa Rosa de Copan", 290, 348);
    agregarCiudad("Gracias", 320, 376);
    agregarCiudad("Nuevo Ocotepeque", 236, 393);
    agregarCiudad("La Esperanza", 372, 415);
    agregarCiudad("La Paz", 440, 414);
    agregarCiudad("Marcala", 393, 436);
    agregarCiudad("Siguatepeque", 435, 382);
    agregarCiudad("Comayagua", 448, 395);
    agregarCiudad("Gualmaca", 553, 380);
    agregarCiudad("Talanga", 523, 402);
    agregarCiudad("Tegucigalpa", 503, 444);
    agregarCiudad("Danli", 590, 452);
    agregarCiudad("Yuscaran", 554, 467);
    agregarCiudad("Nacaome", 466, 522);
    agregarCiudad("San Lorenzo", 472, 537);

    // Agregar aristas con puntos intermedios
    QVector<QPoint> puntosIntermedios;
    puntosIntermedios.push_back(QPoint(406, 206));
    puntosIntermedios.push_back(QPoint(400, 210));
    grafo.agregarArista(0, 1, puntosIntermedios); // Puerto Cortes3 -> Omoa

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(392, 250));
    puntosIntermedios.push_back(QPoint(384, 256));
    puntosIntermedios.push_back(QPoint(378, 272));
    puntosIntermedios.push_back(QPoint(380, 285));
    puntosIntermedios.push_back(QPoint(380, 297));
    puntosIntermedios.push_back(QPoint(366, 303));
    puntosIntermedios.push_back(QPoint(354, 303));
    puntosIntermedios.push_back(QPoint(350, 315));
    puntosIntermedios.push_back(QPoint(352, 327));
    grafo.agregarArista(4, 5, puntosIntermedios); // sps -> santa barbara

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(420, 202));
    puntosIntermedios.push_back(QPoint(427, 206));
    puntosIntermedios.push_back(QPoint(428, 212));
    puntosIntermedios.push_back(QPoint(428,218));
    puntosIntermedios.push_back(QPoint(422, 224));
    puntosIntermedios.push_back(QPoint(416, 223));
    puntosIntermedios.push_back(QPoint(410, 225));
    grafo.agregarArista(0, 2, puntosIntermedios); // Puerto Cortes -> Choloma

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(402, 235));
    puntosIntermedios.push_back(QPoint(400, 240));
    grafo.agregarArista(2, 4, puntosIntermedios); // Choloma -> SPS

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(401, 249));
    puntosIntermedios.push_back(QPoint(406, 251));
    grafo.agregarArista(4, 6, puntosIntermedios); // SPS -> lima

    puntosIntermedios.clear();
}

void MainWindow::on_actionCARGAR_triggered()
{
    mapaWidget = ui->stackedWidget->widget(1);

    // En el constructor
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1280, 720); // Ajustar la escena al tamaño del mapa
    QGraphicsView *view = new QGraphicsView(scene, mapaWidget);
    view->setGeometry(0, 0, 1280, 720);
    view->setStyleSheet("background: transparent;"); // Fondo transparente
    view->setRenderHint(QPainter::Antialiasing);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Permitir que los eventos pasen a los botones y combos debajo
    view->setAttribute(Qt::WA_TransparentForMouseEvents);
    view->show();

    inicializarCiudades();
    dibujarGrafo();
}


