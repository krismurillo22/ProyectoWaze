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
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), carroItem(nullptr), timer(nullptr), indiceRuta(0), velocidad(1)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::moverCarro);
    colorDefecto = QColor(245, 3, 144);
    colorRuta = Qt::yellow;
    ui->textParadas->setVisible(false);
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
        QColor colorNodo = (rutaMasCorta.contains(nodo.id)) ? colorRuta : colorDefecto;
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

        QColor colorArista = (esAristaMasCorta) ? colorRuta : colorDefecto;
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
    scene->setSceneRect(0, 0, 1280, 720);
    QGraphicsView *view = new QGraphicsView(scene, mapaWidget);
    view->setGeometry(0, 0, 1280, 720);
    view->setStyleSheet("background: transparent;");
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
    ui->textParadas->clear();
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

    historial.append(qMakePair(nombreOrigen, nombreDestino));

    double distanciaTotal = 0;
    QVector<int> rutaMasCorta = grafo.dijkstra(idOrigen, idDestino, distanciaTotal);
    double distanciaReal = convertirAPesoReal(distanciaTotal);

    if (rutaMasCorta.isEmpty()) {
        QMessageBox::warning(this, "Sin Ruta", "No hay una ruta disponible entre estas ciudades.");
        return;
    }

    const double velocidadPromedio = 60.0; // km/h
    const double rendimientoCombustible = 12.0; // km/L

    double tiempoHoras = distanciaReal / velocidadPromedio;

    // Verificamos si es transporte público
    if (ui->comboBoxTipoTransporte->currentText() == "Transporte Público") {
        int paradas = rutaMasCorta.size() - 2; // Restamos origen y destino
        tiempoHoras += paradas;
        ui->paradas->setText(QString::number(paradas) + (paradas == 1 ? " Parada" : " Paradas"));
        ui->textParadas->setVisible(true);
    } else {
        ui->paradas->setText("0 Paradas");
        ui->textParadas->setVisible(false);
    }

    int horas = static_cast<int>(tiempoHoras);
    int minutos = static_cast<int>((tiempoHoras - horas) * 60);

    QString tiempoStr;
    if (horas == 0) {
        tiempoStr = QString::number(minutos) + " min";
    } else if (minutos == 0) {
        tiempoStr = QString::number(horas) + " h";
    } else {
        tiempoStr = QString::number(horas) + " h " + QString::number(minutos) + " min";
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

        // Verificar si el vehículo pasa por una ciudad intermedia
        QString tipoTransporte = ui->comboBoxTipoTransporte->currentText();
        if (tipoTransporte == "Transporte Público") {
            // Verificar si el vehículo pasa por una ciudad intermedia
            const QVector<Nodo>& nodos = grafo.obtenerNodos();
            for (const Nodo& nodo : nodos) {
                if (carroItem->pos() == nodo.posicion + QPoint(4, 4)) {
                    // Si el nodo no es el origen ni el destino, registrar la parada y hacer una pausa
                    if (nodo.id != ui->comboBoxOrigen->currentIndex() && nodo.id != ui->comboBoxDestino->currentIndex()) {
                        ui->textParadas->append("Se realizó una parada en " + nodo.nombre);

                        // Detener el movimiento por 1 segundo
                        timer->stop();
                        QTimer::singleShot(1000, this, [this]() {
                            timer->start(1000 / velocidad);
                        });
                    }
                }
            }
        }
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
    ui->textParadas->clear();
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

                if (arista.idNodoOrigen == nodoActual && arista.idNodoDestino == nodoSiguiente) {
                    for (const QPoint& punto : arista.puntosIntermedios) {
                        rutaCarro.append(punto + QPoint(4, 4));
                    }
                } else {
                    for (int j = arista.puntosIntermedios.size() - 1; j >= 0; --j) {
                        rutaCarro.append(arista.puntosIntermedios[j] + QPoint(4, 4));
                    }
                }
                break;
            }
        }
    }

    rutaCarro.append(nodos[rutaMasCorta.last()].posicion + QPoint(4, 4));

    // Si ya existía un vehículo, eliminarlo antes de crear uno nuevo
    if (carroItem) {
        scene->removeItem(carroItem);
        delete carroItem;
        carroItem = nullptr;
    }

    // Seleccionar el tipo de vehículo según el transporte elegido
    QString tipoTransporte = ui->comboBoxTipoTransporte->currentText();
    QPixmap vehiculoPixmap;

    if (tipoTransporte == "Transporte Público") {
        vehiculoPixmap.load(":/fondos/bus.png");
    } else {
        vehiculoPixmap.load(":/fondos/carro.png");
    }

    if (vehiculoPixmap.isNull()) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen del vehículo.");
        return;
    }

    vehiculoPixmap = vehiculoPixmap.scaled(40, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    carroItem = new QGraphicsPixmapItem(vehiculoPixmap);
    scene->addItem(carroItem);
    carroItem->setOffset(-vehiculoPixmap.width() / 2, -vehiculoPixmap.height() / 2);

    // Iniciar el recorrido
    indiceRuta = 0;
    carroItem->setPos(rutaCarro[indiceRuta]);
    velocidad = ui->comboBoxVelocidad->currentIndex() + 1;
    timer->start(1000 / velocidad);
}

void MainWindow::on_actionModo_Claro_triggered()
{
    QPixmap fondo(":/fondos/mapaClaro.png");
    if (ui->label_2->hasScaledContents()) {
        fondo = fondo.scaled(ui->label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    ui->label_2->setPixmap(fondo);
    QString estiloLabel = "color: black; font: 87 14pt 'Segoe UI';";
    ui->combustible->setStyleSheet(estiloLabel);
    ui->distancia->setStyleSheet(estiloLabel);
    ui->paradas->setStyleSheet(estiloLabel);
    ui->tiempo->setStyleSheet(estiloLabel);
}


void MainWindow::on_actionModo_Oscuro_2_triggered()
{
    QPixmap fondo(":/fondos/mapaOscuro.png");
    if (ui->label_2->hasScaledContents()) {
        fondo = fondo.scaled(ui->label_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    ui->label_2->setPixmap(fondo);
    QString estiloLabel = "color: white; font: 87 14pt 'Segoe UI';";
    ui->combustible->setStyleSheet(estiloLabel);
    ui->distancia->setStyleSheet(estiloLabel);
    ui->paradas->setStyleSheet(estiloLabel);
    ui->tiempo->setStyleSheet(estiloLabel);
}


void MainWindow::on_actionCAMBIAR_COLOR_A_LAS_RUTAS_triggered()
{
    QStringList opciones = {"Rutas en general", "Ruta calculada"};
    bool ok;

    QString seleccion = QInputDialog::getItem(this, "Seleccionar Color", "¿Qué color deseas modificar?", opciones, 0, false, &ok);

    if (!ok) return; // Si el usuario cancela, no hace nada

    QColor nuevoColor = QColorDialog::getColor(QColor(245, 3, 144), this, "Selecciona un color");

    if (nuevoColor.isValid()) {
        if (seleccion == "Rutas en general") {
            colorDefecto = nuevoColor;
        } else {
            colorRuta = nuevoColor; // Agrega una variable para este color
        }
        dibujarGrafo();
    }
}

void MainWindow::on_actionVer_Historial_triggered()
{
    if (historial.isEmpty()) {
        QMessageBox::information(this, "Historial", "No hay rutas guardadas.");
        return;
    }

    QString historialTexto;
    for (const QPair<QString, QString>& ruta : historial) {
        historialTexto += "Origen: " + ruta.first + " -> Destino: " + ruta.second + "\n";
    }

    QMessageBox::information(this, "Historial de Rutas", historialTexto);
}


void MainWindow::on_actionGUIA_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_regresar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_anterior_clicked()
{
    if (indiceImagen > 0) {
        indiceImagen--;  // Disminuir el índice para mostrar la imagen anterior
        ui->labelImagen->setPixmap(QPixmap(listaImagenes[indiceImagen]));  // Cambiar la imagen del QLabel
    }
}

void MainWindow::on_siguiente_clicked()
{
    if (indiceImagen < 3) {
        indiceImagen++;  // Aumentar el índice para mostrar la siguiente imagen
        ui->labelImagen->setPixmap(QPixmap(listaImagenes[indiceImagen]));  // Cambiar la imagen del QLabel
    }
}
