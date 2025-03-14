#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QCoreApplication>
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), carroItem(nullptr), timer(nullptr), indiceRuta(0), velocidad(1)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::moverCarro);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete carroItem;
    delete timer;
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::dibujarGrafo(const QVector<int>& rutaMasCorta) {
    const QVector<Nodo>& nodos = grafo.obtenerNodos();
    const QVector<Arista>& aristas = grafo.obtenerAristas();
    for (const Nodo& nodo : nodos) {
        QColor colorNodo = (rutaMasCorta.contains(nodo.id)) ? Qt::blue : QColor(245, 3, 144);
        scene->addEllipse(nodo.posicion.x(), nodo.posicion.y(), 8, 8,
                          QPen(Qt::NoPen), QBrush(colorNodo));
    }

    for (const Arista& arista : aristas) {
        QPainterPath path;
        QPoint origen = nodos[arista.idNodoOrigen].posicion + QPoint(4, 4);
        path.moveTo(origen);

        for (const QPoint& punto : arista.puntosIntermedios) {
            path.lineTo(punto + QPoint(4, 4));
        }

        QPoint destino = nodos[arista.idNodoDestino].posicion + QPoint(4, 4);
        path.lineTo(destino);

        bool esAristaMasCorta = false;
        for (int i = 0; i < rutaMasCorta.size() - 1; ++i) {
            if ((rutaMasCorta[i] == arista.idNodoOrigen && rutaMasCorta[i + 1] == arista.idNodoDestino) ||
                (rutaMasCorta[i] == arista.idNodoDestino && rutaMasCorta[i + 1] == arista.idNodoOrigen)) {
                esAristaMasCorta = true;
                break;
            }
        }

        QColor colorArista = (esAristaMasCorta) ? Qt::blue : QColor(245, 3, 144);
        scene->addPath(path, QPen(colorArista, 3));
    }
}

void MainWindow::agregarCiudad(const QString& nombre, int x, int y) {
    grafo.agregarNodo(nombre, QPoint(x, y));
    update();
}

void MainWindow::inicializarCiudades() {

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

    const QVector<Nodo>& nodos = grafo.obtenerNodos();
    ui->comboBoxOrigen->clear();
    for (int i = 0; i < nodos.size(); ++i) {
        ui->comboBoxOrigen->addItem(nodos[i].nombre);
    }

    ui->comboBoxDestino->clear();
    for (int i = 0; i < nodos.size(); ++i) {
        ui->comboBoxDestino->addItem(nodos[i].nombre);
    }
}



void MainWindow::on_pushButton_3_clicked()
{
    int idOrigen = ui->comboBoxOrigen->currentIndex();
    int idDestino = ui->comboBoxDestino->currentIndex();

    if (idOrigen == idDestino) {
        QMessageBox::warning(this, "Error", "No puedes seleccionar la misma ciudad como origen y destino.");
        return;
    }

    QString nombreOrigen = ui->comboBoxOrigen->currentText();
    QString nombreDestino = ui->comboBoxDestino->currentText();

    QStringList ciudadesSinRuta = {"Barra Patuca", "Puerto Lempira", "Valencia"};

    if (ciudadesSinRuta.contains(nombreOrigen) || ciudadesSinRuta.contains(nombreDestino)) {
        QMessageBox::warning(this, "Sin Ruta", "No existen rutas disponibles para " + nombreOrigen + " a " + nombreDestino + ".");
        return;
    }

    double distanciaTotal = 0;
    QVector<int> rutaMasCorta = grafo.dijkstra(idOrigen, idDestino, distanciaTotal);
    double distanciaReal = convertirAPesoReal(distanciaTotal);

    if (rutaMasCorta.isEmpty()) {
        QMessageBox::warning(this, "Sin Ruta", "No hay una ruta disponible entre estas ciudades.");
        return;
    }

    const double velocidadPromedio = 80.0; // km/h
    const double rendimientoCombustible = 12.0; // km/L

    double tiempoHoras = distanciaReal / velocidadPromedio;
    QString tiempoStr;
    if (tiempoHoras < 1.0) {
        int minutos = static_cast<int>(tiempoHoras * 60);
        tiempoStr = QString::number(minutos) + " minutos";
    } else {
        tiempoStr = QString::number(tiempoHoras, 'f', 2) + " horas";
    }

    double consumoLitros = distanciaReal / rendimientoCombustible;
    ui->distancia->setText(QString::number(distanciaReal) + " KM.");
    ui->tiempo->setText(tiempoStr);
    ui->combustible->setText(QString::number(consumoLitros, 'f', 2) + " L.");

    scene->clear();
    dibujarGrafo(rutaMasCorta);

}

void MainWindow::on_restablecer_clicked()
{
    dibujarGrafo();
    const QVector<Nodo>& nodos = grafo.obtenerNodos();
    ui->comboBoxOrigen->clear();
    for (int i = 0; i < nodos.size(); ++i) {
        ui->comboBoxOrigen->addItem(nodos[i].nombre);
    }

    ui->comboBoxDestino->clear();
    for (int i = 0; i < nodos.size(); ++i) {
        ui->comboBoxDestino->addItem(nodos[i].nombre);
    }
    ui->combustible->setText("0 L.");
    ui->distancia->setText("0 KM.");
    ui->paradas->setText("0 Paradas");
    ui->tiempo->setText("0 horas");
    ui->textAlternas->clear();

}

double MainWindow::convertirAPesoReal(int peso) {
    const double factorConversion = 7.2;
    return peso * factorConversion;
}

void MainWindow::moverCarro() {
    if (indiceRuta < rutaCarro.size() - 1) {
        indiceRuta++;
        carroItem->setPos(rutaCarro[indiceRuta]);
    } else {
        timer->stop();
        QMessageBox::information(this, "Recorrido Completado", "El carro ha llegado a su destino.");

        // Eliminar el carro al terminar la simulación
        if (carroItem) {
            scene->removeItem(carroItem);
            delete carroItem;
            carroItem = nullptr;
        }
    }
}

void MainWindow::on_recorrido_clicked() {
    double distanciaTotal = 0;
    QVector<int> rutaMasCorta = grafo.dijkstra(ui->comboBoxOrigen->currentIndex(), ui->comboBoxDestino->currentIndex(), distanciaTotal);

    if (rutaMasCorta.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay una ruta disponible.");
        return;
    }

    rutaCarro.clear();
    const QVector<Nodo>& nodos = grafo.obtenerNodos();
    const QVector<Arista>& aristas = grafo.obtenerAristas();

    for (int i = 0; i < rutaMasCorta.size() - 1; ++i) {
        int nodoActual = rutaMasCorta[i];
        int nodoSiguiente = rutaMasCorta[i + 1];

        rutaCarro.append(nodos[nodoActual].posicion + QPoint(4, 4));

        for (const Arista& arista : aristas) {
            if ((arista.idNodoOrigen == nodoActual && arista.idNodoDestino == nodoSiguiente) ||
                (arista.idNodoOrigen == nodoSiguiente && arista.idNodoDestino == nodoActual)) {

                for (const QPoint& punto : arista.puntosIntermedios) {
                    rutaCarro.append(punto + QPoint(4, 4));
                }
                break;
            }
        }
    }

    rutaCarro.append(nodos[rutaMasCorta.last()].posicion + QPoint(4, 4));

    // Si ya existía un carro, eliminarlo antes de crear uno nuevo
    if (carroItem) {
        scene->removeItem(carroItem);
        delete carroItem;
        carroItem = nullptr;
    }

    QPixmap carroPixmap(":/fondos/carro.png");
    if (carroPixmap.isNull()) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen del carro.");
        return;
    }

    carroPixmap = carroPixmap.scaled(30, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    carroItem = new QGraphicsPixmapItem(carroPixmap);
    scene->addItem(carroItem);
    carroItem->setOffset(-carroPixmap.width() / 2, -carroPixmap.height() / 2);

    // Iniciar el recorrido
    indiceRuta = 0;
    carroItem->setPos(rutaCarro[indiceRuta]);
    velocidad = ui->comboBoxVelocidad->currentIndex() + 1;
    timer->start(1000 / velocidad);
}


