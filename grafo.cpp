#include "grafo.h"
#include <QMap>
#include <QSet>
#include <limits>

Grafo::Grafo() {
    cargarDesdeArchivo("C:/Users/avril/Desktop/Proyectos/ProyectoWaze/archivos/grafo.gra");
}

Grafo::~Grafo() {
    guardarEnArchivo("C:/Users/avril/Desktop/Proyectos/ProyectoWaze/archivos/grafo.gra");
}
void Grafo::agregarNodo(const QString& nombre, const QPoint& posicion) {
    int nuevoId = nodosAVL.obtenerTodos().size();
    nodosAVL.insertar({nuevoId, nombre, posicion});
}

void Grafo::agregarArista(int idNodoOrigen, int idNodoDestino, const QVector<QPoint>& puntosIntermedios) {
    Arista nuevaArista;
    nuevaArista.idNodoOrigen = idNodoOrigen;
    nuevaArista.idNodoDestino = idNodoDestino;
    nuevaArista.puntosIntermedios = puntosIntermedios;
    nuevaArista.peso = puntosIntermedios.size();
    aristas.push_back(nuevaArista);
}

QVector<Nodo> Grafo::obtenerNodos() const {
    return nodosAVL.obtenerTodos();
}

QVector<Arista> Grafo::obtenerAristas() const {
    return aristas;
}

QVector<int> Grafo::dijkstra(int idNodoInicio, int idNodoFin, double& distanciaTotal) const {
    QMap<int, double> distancias;
    QMap<int, int> previos;
    QSet<int> nodosNoVisitados;

    for (const Nodo& nodo : obtenerNodos()) {
        distancias[nodo.id] = std::numeric_limits<double>::infinity();
        previos[nodo.id] = -1;
        nodosNoVisitados.insert(nodo.id);
    }

    distancias[idNodoInicio] = 0;

    while (!nodosNoVisitados.isEmpty()) {
        int nodoActual = -1;
        double distanciaMinima = std::numeric_limits<double>::infinity();

        for (int nodo : nodosNoVisitados) {
            if (distancias[nodo] < distanciaMinima) {
                nodoActual = nodo;
                distanciaMinima = distancias[nodo];
            }
        }

        if (nodoActual == -1 || nodoActual == idNodoFin) {
            break;
        }

        nodosNoVisitados.remove(nodoActual);

        for (const Arista& arista : aristas) {
            if (arista.idNodoOrigen == nodoActual || arista.idNodoDestino == nodoActual) {
                int nodoVecino = (arista.idNodoOrigen == nodoActual) ? arista.idNodoDestino : arista.idNodoOrigen;
                double distanciaAlternativa = distancias[nodoActual] + arista.peso;

                if (distanciaAlternativa < distancias[nodoVecino]) {
                    distancias[nodoVecino] = distanciaAlternativa;
                    previos[nodoVecino] = nodoActual;
                }
            }
        }
    }

    distanciaTotal = distancias[idNodoFin];
    QVector<int> ruta;
    int nodoActual = idNodoFin;
    while (nodoActual != -1) {
        ruta.prepend(nodoActual);
        nodoActual = previos[nodoActual];
    }

    return ruta;
}

QVector<int> Grafo::obtenerRuta(int idNodoInicio, int idNodoFin, double& distanciaTotal) {
    // Primero, buscar en el historial
    QVector<int> rutaGuardada = historialRutas.buscarRuta(idNodoInicio, idNodoFin, distanciaTotal);
    if (!rutaGuardada.isEmpty()) {
        std::cout << "Ruta obtenida del historial con distancia: " << distanciaTotal << std::endl;
        return rutaGuardada;
    }

    // Si no está en el historial, ejecutar Dijkstra
    QVector<int> rutaCalculada = dijkstra(idNodoInicio, idNodoFin, distanciaTotal);

    // Guardar en el historial si se encontró una ruta válida
    if (!rutaCalculada.isEmpty()) {
        historialRutas.insertar(idNodoInicio, idNodoFin, rutaCalculada, distanciaTotal);
        std::cout << "Nueva ruta calculada y guardada en historial." << std::endl;
    }

    return rutaCalculada;
}

QString Grafo::obtenerNombreNodo(int id) const {
    QVector<Nodo> nodos = obtenerNodos();
    for (const Nodo& nodo : nodos) {
        if (nodo.id == id) {
            return nodo.nombre;
        }
    }
    return "Desconocido"; // En caso de que no lo encuentre
}

void Grafo::todasLasRutas(int idNodoInicio, int idNodoFin, QVector<QVector<int>>& rutas, QVector<int> rutaActual, QSet<int> nodosVisitados) const {
    rutaActual.append(idNodoInicio);
    nodosVisitados.insert(idNodoInicio);

    if (idNodoInicio == idNodoFin) {
        rutas.append(rutaActual);
    } else {
        for (const Arista& arista : aristas) {
            int vecino = (arista.idNodoOrigen == idNodoInicio) ? arista.idNodoDestino :
                         (arista.idNodoDestino == idNodoInicio) ? arista.idNodoOrigen : -1;

            if (vecino != -1 && !nodosVisitados.contains(vecino)) {
                todasLasRutas(vecino, idNodoFin, rutas, rutaActual, nodosVisitados);
            }
        }
    }
}

QVector<QVector<int>> Grafo::obtenerTodasLasRutas(int idNodoInicio, int idNodoFin) const {
    QVector<QVector<int>> rutas;
    todasLasRutas(idNodoInicio, idNodoFin, rutas, {}, {});
    return rutas;
}

QSet<int> Grafo::obtenerCiudadesEntre(int idNodoInicio, int idNodoFin) const {
    QVector<QVector<int>> rutas = obtenerTodasLasRutas(idNodoInicio, idNodoFin);
    QSet<int> ciudadesIntermedias;

    for (const QVector<int>& ruta : rutas) {
        for (int ciudad : ruta) {
            if (ciudad != idNodoInicio && ciudad != idNodoFin) {
                ciudadesIntermedias.insert(ciudad);
            }
        }
    }

    return ciudadesIntermedias;
}

QVector<int> Grafo::calcularRutaConParadas(const QVector<int>& paradas, double& distanciaTotal) const {
    if (paradas.isEmpty()) return {};

    QVector<int> rutaFinal;
    distanciaTotal = 0;

    int nodoActual = paradas.first();
    for (int i = 1; i < paradas.size(); ++i) {
        double distanciaParcial = 0;
        QVector<int> subRuta = dijkstra(nodoActual, paradas[i], distanciaParcial);

        if (subRuta.isEmpty()) {
            return {}; // Si alguna ruta entre paradas no es posible, devolvemos vacío.
        }

        if (!rutaFinal.isEmpty()) {
            subRuta.pop_front(); // Evita duplicar nodos en la unión de rutas.
        }

        rutaFinal.append(subRuta);
        distanciaTotal += distanciaParcial;
        nodoActual = paradas[i];
    }

    return rutaFinal;
}

void Grafo::guardarEnArchivo(const QString& nombreArchivo) const {
    QFile archivo(nombreArchivo);
    if (archivo.open(QIODevice::WriteOnly)) {
        QDataStream out(&archivo);

        // Guardar nodos
        QVector<Nodo> nodos = nodosAVL.obtenerTodos();
        out << nodos.size();
        for (const Nodo& nodo : nodos) {
            out << nodo.id << nodo.nombre << nodo.posicion;
        }

        // Guardar aristas
        out << aristas.size();
        for (const Arista& arista : aristas) {
            out << arista.idNodoOrigen << arista.idNodoDestino << arista.puntosIntermedios << arista.peso;
        }

        archivo.close();
    }
}

void Grafo::cargarDesdeArchivo(const QString& nombreArchivo) {
    QFile archivo(nombreArchivo);
    if (archivo.open(QIODevice::ReadOnly)) {
        QDataStream in(&archivo);

        // Limpiar estructuras actuales
        aristas.clear();

        // Cargar nodos
        int cantidadNodos;
        in >> cantidadNodos;
        for (int i = 0; i < cantidadNodos; ++i) {
            Nodo nodo;
            in >> nodo.id >> nodo.nombre >> nodo.posicion;
            nodosAVL.insertar(nodo);
        }

        // Cargar aristas
        int cantidadAristas;
        in >> cantidadAristas;
        for (int i = 0; i < cantidadAristas; ++i) {
            Arista arista;
            in >> arista.idNodoOrigen >> arista.idNodoDestino >> arista.puntosIntermedios >> arista.peso;
            aristas.append(arista);
        }

        archivo.close();
    }
}
