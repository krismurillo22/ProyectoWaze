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
    puntosIntermedios.push_back(QPoint(416, 260));
    puntosIntermedios.push_back(QPoint(423, 263));
    grafo.agregarArista(6, 9, puntosIntermedios); // Lima -> Progreso

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(424, 270));
    puntosIntermedios.push_back(QPoint(422, 276));
    puntosIntermedios.push_back(QPoint(419, 283));
    grafo.agregarArista(9, 11, puntosIntermedios); // El Progreso -> Santa Rita

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(430, 252));
    puntosIntermedios.push_back(QPoint(440, 250));
    puntosIntermedios.push_back(QPoint(451, 246));
    puntosIntermedios.push_back(QPoint(452, 237));
    puntosIntermedios.push_back(QPoint(453, 228));
    puntosIntermedios.push_back(QPoint(458, 223));
    puntosIntermedios.push_back(QPoint(470, 221));
    puntosIntermedios.push_back(QPoint(473, 215));
    grafo.agregarArista(9, 7, puntosIntermedios); // El Progreso -> Tela

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(488, 215));
    puntosIntermedios.push_back(QPoint(497, 219));
    puntosIntermedios.push_back(QPoint(502, 222));
    puntosIntermedios.push_back(QPoint(511, 224));
    puntosIntermedios.push_back(QPoint(524, 222));
    puntosIntermedios.push_back(QPoint(538, 221));
    puntosIntermedios.push_back(QPoint(552, 219));
    puntosIntermedios.push_back(QPoint(566, 221));
    grafo.agregarArista(7, 8, puntosIntermedios); // Tela -> Ceiba

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(422, 293));
    puntosIntermedios.push_back(QPoint(430, 295));
    puntosIntermedios.push_back(QPoint(436, 301));
    puntosIntermedios.push_back(QPoint(439, 293));
    puntosIntermedios.push_back(QPoint(445, 296));
    puntosIntermedios.push_back(QPoint(453, 298));
    puntosIntermedios.push_back(QPoint(463, 292));
    puntosIntermedios.push_back(QPoint(472, 291));
    puntosIntermedios.push_back(QPoint(469, 284));
    puntosIntermedios.push_back(QPoint(461, 279));
    grafo.agregarArista(11, 10, puntosIntermedios); // Santa RIta -> Morazan

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(463, 270));
    puntosIntermedios.push_back(QPoint(475, 268));
    puntosIntermedios.push_back(QPoint(487, 266));
    puntosIntermedios.push_back(QPoint(498, 265));
    puntosIntermedios.push_back(QPoint(502, 270));
    puntosIntermedios.push_back(QPoint(500, 280));
    puntosIntermedios.push_back(QPoint(501, 289));
    puntosIntermedios.push_back(QPoint(507, 297));
    grafo.agregarArista(10, 12, puntosIntermedios); // Morazan -> Yoro

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(520, 294));
    puntosIntermedios.push_back(QPoint(531, 292));
    puntosIntermedios.push_back(QPoint(536, 291));
    puntosIntermedios.push_back(QPoint(545, 291));
    puntosIntermedios.push_back(QPoint(549, 282));
    puntosIntermedios.push_back(QPoint(540, 275));
    puntosIntermedios.push_back(QPoint(533, 273));
    puntosIntermedios.push_back(QPoint(531, 266));
    puntosIntermedios.push_back(QPoint(543, 261));
    puntosIntermedios.push_back(QPoint(551, 255));
    puntosIntermedios.push_back(QPoint(560, 251));
    puntosIntermedios.push_back(QPoint(573, 249));
    puntosIntermedios.push_back(QPoint(582, 247));
    grafo.agregarArista(12, 13, puntosIntermedios); // Yoro -> Olanchito

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(366, 336));
    puntosIntermedios.push_back(QPoint(366, 339));
    puntosIntermedios.push_back(QPoint(365, 348));
    puntosIntermedios.push_back(QPoint(355, 357));
    puntosIntermedios.push_back(QPoint(349, 366));
    puntosIntermedios.push_back(QPoint(341, 376));
    puntosIntermedios.push_back(QPoint(328, 377));
    grafo.agregarArista(5, 25, puntosIntermedios); // Santa Barbara -> Gracias

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(391, 250));
    puntosIntermedios.push_back(QPoint(382, 256));
    puntosIntermedios.push_back(QPoint(376, 261));
    puntosIntermedios.push_back(QPoint(364, 263));
    puntosIntermedios.push_back(QPoint(354, 265));
    puntosIntermedios.push_back(QPoint(345, 268));
    puntosIntermedios.push_back(QPoint(335, 269));
    puntosIntermedios.push_back(QPoint(323, 276));
    puntosIntermedios.push_back(QPoint(316, 284));
    puntosIntermedios.push_back(QPoint(311, 295));
    puntosIntermedios.push_back(QPoint(307, 307));
    puntosIntermedios.push_back(QPoint(303, 312));
    grafo.agregarArista(4, 22, puntosIntermedios); // SPS -> La Entrada

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(289, 311));
    puntosIntermedios.push_back(QPoint(278, 309));
    puntosIntermedios.push_back(QPoint(270, 309));
    puntosIntermedios.push_back(QPoint(262, 308));
    puntosIntermedios.push_back(QPoint(251, 313));
    puntosIntermedios.push_back(QPoint(247, 322));
    puntosIntermedios.push_back(QPoint(241, 324));
    puntosIntermedios.push_back(QPoint(236, 332));
    puntosIntermedios.push_back(QPoint(237, 342));
    grafo.agregarArista(22, 23, puntosIntermedios); // La Entrada -> Copan

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(292, 321));
    puntosIntermedios.push_back(QPoint(289, 327));
    puntosIntermedios.push_back(QPoint(274, 327));
    puntosIntermedios.push_back(QPoint(273, 338));
    puntosIntermedios.push_back(QPoint(273, 345));
    puntosIntermedios.push_back(QPoint(283, 348));
    grafo.agregarArista(22, 24, puntosIntermedios); // La Entrada -> Santa Rosa de Copan

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(283, 348));
    puntosIntermedios.push_back(QPoint(282, 354));
    puntosIntermedios.push_back(QPoint(283, 365));
    puntosIntermedios.push_back(QPoint(273, 364));
    puntosIntermedios.push_back(QPoint(273, 376));
    puntosIntermedios.push_back(QPoint(265, 371));
    puntosIntermedios.push_back(QPoint(257, 374));
    puntosIntermedios.push_back(QPoint(252, 379));
    puntosIntermedios.push_back(QPoint(249, 386));
    puntosIntermedios.push_back(QPoint(237, 385));
    grafo.agregarArista(24, 26, puntosIntermedios); // Santa Rosa de Copan -> Ocotepeque

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(297, 354));
    puntosIntermedios.push_back(QPoint(305, 360));
    puntosIntermedios.push_back(QPoint(305, 367));
    puntosIntermedios.push_back(QPoint(306, 373));
    puntosIntermedios.push_back(QPoint(312, 376));
    grafo.agregarArista(24, 25, puntosIntermedios); // Santa Rosa de Copan -> Gracias

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(327, 385));
    puntosIntermedios.push_back(QPoint(331, 391));
    puntosIntermedios.push_back(QPoint(332, 399));
    puntosIntermedios.push_back(QPoint(331, 408));
    puntosIntermedios.push_back(QPoint(338, 412));
    puntosIntermedios.push_back(QPoint(344, 422));
    puntosIntermedios.push_back(QPoint(345, 432));
    puntosIntermedios.push_back(QPoint(354, 431));
    puntosIntermedios.push_back(QPoint(357, 424));
    puntosIntermedios.push_back(QPoint(363, 414));
    grafo.agregarArista(25, 27, puntosIntermedios); // Gracias -> La Esperanza

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(378, 425));
    puntosIntermedios.push_back(QPoint(382, 431));
    puntosIntermedios.push_back(QPoint(385, 437));
    grafo.agregarArista(27, 29, puntosIntermedios); // La Esperanza -> Marcala

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(401, 435));
    puntosIntermedios.push_back(QPoint(410, 435));
    puntosIntermedios.push_back(QPoint(416, 440));
    puntosIntermedios.push_back(QPoint(421, 435));
    puntosIntermedios.push_back(QPoint(425, 429));
    puntosIntermedios.push_back(QPoint(431, 425));
    puntosIntermedios.push_back(QPoint(439, 422));
    grafo.agregarArista(29, 28, puntosIntermedios); // Marcala -> La Paz

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(414, 297));
    puntosIntermedios.push_back(QPoint(415, 304));
    puntosIntermedios.push_back(QPoint(417, 312));
    puntosIntermedios.push_back(QPoint(415, 321));
    puntosIntermedios.push_back(QPoint(411, 331));
    puntosIntermedios.push_back(QPoint(409, 341));
    puntosIntermedios.push_back(QPoint(403, 347));
    puntosIntermedios.push_back(QPoint(397, 353));
    puntosIntermedios.push_back(QPoint(392, 357));
    puntosIntermedios.push_back(QPoint(390, 367));
    puntosIntermedios.push_back(QPoint(396, 371));
    puntosIntermedios.push_back(QPoint(405, 376));
    puntosIntermedios.push_back(QPoint(408, 384));
    puntosIntermedios.push_back(QPoint(418, 381));
    puntosIntermedios.push_back(QPoint(422, 385));
    puntosIntermedios.push_back(QPoint(428, 382));
    grafo.agregarArista(11, 30, puntosIntermedios); // Santa Rita -> Siguatepeque

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(428, 382));
    puntosIntermedios.push_back(QPoint(422, 385));
    puntosIntermedios.push_back(QPoint(418, 392));
    puntosIntermedios.push_back(QPoint(416, 400));
    puntosIntermedios.push_back(QPoint(410, 405));
    puntosIntermedios.push_back(QPoint(407, 412));
    puntosIntermedios.push_back(QPoint(397, 416));
    puntosIntermedios.push_back(QPoint(388, 417));
    puntosIntermedios.push_back(QPoint(380, 416));
    grafo.agregarArista(30, 27, puntosIntermedios); // Siguatepeque -> La Esperanza

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(440, 388));
    puntosIntermedios.push_back(QPoint(445, 393));
    grafo.agregarArista(30, 31, puntosIntermedios); // Siguatepeque -> Comayagua

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(447, 403));
    puntosIntermedios.push_back(QPoint(445, 409));
    grafo.agregarArista(31, 28, puntosIntermedios); // Comayagua -> La Paz

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(457, 405));
    puntosIntermedios.push_back(QPoint(460, 412));
    puntosIntermedios.push_back(QPoint(468, 420));
    puntosIntermedios.push_back(QPoint(467, 428));
    puntosIntermedios.push_back(QPoint(476, 440));
    puntosIntermedios.push_back(QPoint(475, 449));
    puntosIntermedios.push_back(QPoint(480, 456));
    puntosIntermedios.push_back(QPoint(479, 465));
    puntosIntermedios.push_back(QPoint(490, 464));
    puntosIntermedios.push_back(QPoint(499, 464));
    puntosIntermedios.push_back(QPoint(501, 457));
    puntosIntermedios.push_back(QPoint(491, 453));
    puntosIntermedios.push_back(QPoint(495, 445));
    grafo.agregarArista(31, 34, puntosIntermedios); // Comayagua ->Tegucigalpa

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(457, 405));
    puntosIntermedios.push_back(QPoint(460, 412));
    puntosIntermedios.push_back(QPoint(468, 420));
    puntosIntermedios.push_back(QPoint(467, 428));
    puntosIntermedios.push_back(QPoint(476, 440));
    puntosIntermedios.push_back(QPoint(475, 449));
    puntosIntermedios.push_back(QPoint(480, 456));
    puntosIntermedios.push_back(QPoint(479, 465));
    puntosIntermedios.push_back(QPoint(468, 469));
    puntosIntermedios.push_back(QPoint(465, 477));
    puntosIntermedios.push_back(QPoint(464, 486));
    puntosIntermedios.push_back(QPoint(463, 498));
    puntosIntermedios.push_back(QPoint(472, 507));
    puntosIntermedios.push_back(QPoint(481, 517));
    puntosIntermedios.push_back(QPoint(475, 523));
    grafo.agregarArista(31, 37, puntosIntermedios); // Comayagua -> Nacaome

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(470, 530));
    puntosIntermedios.push_back(QPoint(471, 534));
    grafo.agregarArista(37, 38, puntosIntermedios); // Nacaome -> San Lorenzo

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(479, 546));
    puntosIntermedios.push_back(QPoint(488, 546));
    puntosIntermedios.push_back(QPoint(491, 555));
    puntosIntermedios.push_back(QPoint(502, 551));
    grafo.agregarArista(38, 3, puntosIntermedios); // San Lorenzo -> Choluteca

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(512, 442));
    puntosIntermedios.push_back(QPoint(526, 448));
    puntosIntermedios.push_back(QPoint(538, 444));
    puntosIntermedios.push_back(QPoint(542, 449));
    puntosIntermedios.push_back(QPoint(547, 450));
    puntosIntermedios.push_back(QPoint(544, 461));
    grafo.agregarArista(34, 36, puntosIntermedios); // Tegucigalpa -> Yuscaran

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(558, 460));
    puntosIntermedios.push_back(QPoint(564, 455));
    puntosIntermedios.push_back(QPoint(571, 460));
    puntosIntermedios.push_back(QPoint(580, 451));
    grafo.agregarArista(36, 35, puntosIntermedios); // Yuscaran -> Danli

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(599, 454));
    puntosIntermedios.push_back(QPoint(610, 457));
    puntosIntermedios.push_back(QPoint(616, 452));
    puntosIntermedios.push_back(QPoint(624, 445));
    puntosIntermedios.push_back(QPoint(615, 436));
    puntosIntermedios.push_back(QPoint(626, 433));
    puntosIntermedios.push_back(QPoint(644, 439));
    puntosIntermedios.push_back(QPoint(637, 423));
    puntosIntermedios.push_back(QPoint(647, 409));
    puntosIntermedios.push_back(QPoint(653, 399));
    puntosIntermedios.push_back(QPoint(664, 392));
    puntosIntermedios.push_back(QPoint(685, 388));
    puntosIntermedios.push_back(QPoint(676, 377));
    puntosIntermedios.push_back(QPoint(682, 372));
    puntosIntermedios.push_back(QPoint(683, 365));
    puntosIntermedios.push_back(QPoint(690, 357));
    grafo.agregarArista(35, 20, puntosIntermedios); // Danli -> Juticalpa

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(692, 340));
    puntosIntermedios.push_back(QPoint(697, 333));
    puntosIntermedios.push_back(QPoint(709, 336));
    grafo.agregarArista(20, 19, puntosIntermedios); // Juticalpa -> Catacamas

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(678, 341));
    puntosIntermedios.push_back(QPoint(668, 343));
    puntosIntermedios.push_back(QPoint(664, 350));
    puntosIntermedios.push_back(QPoint(663, 361));
    puntosIntermedios.push_back(QPoint(666, 373));
    puntosIntermedios.push_back(QPoint(649, 371));
    puntosIntermedios.push_back(QPoint(637, 373));
    puntosIntermedios.push_back(QPoint(624, 362));
    puntosIntermedios.push_back(QPoint(622, 380));
    puntosIntermedios.push_back(QPoint(613, 378));
    puntosIntermedios.push_back(QPoint(608, 386));
    puntosIntermedios.push_back(QPoint(601, 378));
    puntosIntermedios.push_back(QPoint(594, 383));
    puntosIntermedios.push_back(QPoint(584, 381));
    puntosIntermedios.push_back(QPoint(572, 383));
    puntosIntermedios.push_back(QPoint(562, 384));
    grafo.agregarArista(20, 32, puntosIntermedios); // Juticalpa -> Gualmaca

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(552, 389));
    puntosIntermedios.push_back(QPoint(545, 389));
    puntosIntermedios.push_back(QPoint(537, 392));
    puntosIntermedios.push_back(QPoint(529, 391));
    puntosIntermedios.push_back(QPoint(523, 396));
    grafo.agregarArista(32, 33, puntosIntermedios); // Gualmaca -> Talanga

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(526, 410));
    puntosIntermedios.push_back(QPoint(538, 411));
    puntosIntermedios.push_back(QPoint(554, 412));
    puntosIntermedios.push_back(QPoint(555, 423));
    puntosIntermedios.push_back(QPoint(559, 433));
    puntosIntermedios.push_back(QPoint(542, 449));
    puntosIntermedios.push_back(QPoint(538, 444));
    puntosIntermedios.push_back(QPoint(526, 448));
    puntosIntermedios.push_back(QPoint(512, 442));
    grafo.agregarArista(33, 34, puntosIntermedios); // Talanga -> Tegucigalpa

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(517, 307));
    puntosIntermedios.push_back(QPoint(523, 316));
    puntosIntermedios.push_back(QPoint(522, 327));
    puntosIntermedios.push_back(QPoint(528, 332));
    puntosIntermedios.push_back(QPoint(538, 341));
    puntosIntermedios.push_back(QPoint(531, 343));
    puntosIntermedios.push_back(QPoint(534, 351));
    puntosIntermedios.push_back(QPoint(545, 355));
    puntosIntermedios.push_back(QPoint(552, 362));
    puntosIntermedios.push_back(QPoint(553, 368));
    puntosIntermedios.push_back(QPoint(564, 381));
    grafo.agregarArista(12, 32, puntosIntermedios); // Yoro -> Gualmaca

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(604, 248));
    puntosIntermedios.push_back(QPoint(613, 244));
    puntosIntermedios.push_back(QPoint(624, 238));
    grafo.agregarArista(13, 15, puntosIntermedios); // Olanchito -> Sonaguera

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(573, 213));
    puntosIntermedios.push_back(QPoint(579, 217));
    puntosIntermedios.push_back(QPoint(587, 212));
    puntosIntermedios.push_back(QPoint(596, 217));
    puntosIntermedios.push_back(QPoint(605, 209));
    puntosIntermedios.push_back(QPoint(616, 222));
    puntosIntermedios.push_back(QPoint(623, 230));
    grafo.agregarArista(8, 15, puntosIntermedios); // La Ceiba -> Sonaguera

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(633, 241));
    puntosIntermedios.push_back(QPoint(639, 242));
    puntosIntermedios.push_back(QPoint(641, 250));
    puntosIntermedios.push_back(QPoint(651, 248));
    puntosIntermedios.push_back(QPoint(657, 240));
    grafo.agregarArista(15, 16, puntosIntermedios); // Sonaguera -> Tocoa

    puntosIntermedios.clear();
    puntosIntermedios.push_back(QPoint(672, 231));
    puntosIntermedios.push_back(QPoint(681, 228));
    puntosIntermedios.push_back(QPoint(693, 227));
    puntosIntermedios.push_back(QPoint(701, 219));
    puntosIntermedios.push_back(QPoint(698, 211));
    puntosIntermedios.push_back(QPoint(688, 209));
    puntosIntermedios.push_back(QPoint(676, 206));
    puntosIntermedios.push_back(QPoint(667, 201));
    puntosIntermedios.push_back(QPoint(674, 197));
    grafo.agregarArista(16, 14, puntosIntermedios); // Tocoa -> Trujillo
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


