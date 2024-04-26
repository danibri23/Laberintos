#include <iostream>
#include <vector>
#include <random>

using namespace std;

typedef vector<vector<int> > Laberinto;

enum class Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

// Función para verificar si una celda está dentro de los límites del laberinto
bool estaDentroLimites(int x, int y, int filas, int columnas) {
    return x >= 0 && x < filas && y >= 0 && y < columnas;
    
}


// Función para verificar si una celda está disponible para ser visitada
bool estaDisponible(int x, int y, const Laberinto& laberinto) {
    return estaDentroLimites(x, y, laberinto.size(), laberinto[0].size()) && laberinto[x][y] == 1;
}

// Función para verificar si una celda tiene vecinos no visitados
bool tieneVecinosNoVisitados(int x, int y, Laberinto &laberinto) {

    static vector<pair<int, int> > direcciones;

    if (direcciones.empty()) {
        direcciones.push_back(make_pair(1, 0));
        direcciones.push_back(make_pair(-1, 0));
        direcciones.push_back(make_pair(0, 1));
        direcciones.push_back(make_pair(0, -1));
    }

    for (int i = 0; i < direcciones.size(); ++i) {
        const auto &dir = direcciones[i];
        int nx = x + dir.first;
        int ny = y + dir.second;
        if (estaDisponible(nx, ny, laberinto)) {
            return true;
        }
    }

    return false;
}

void generarLaberinto(int x, int y, Laberinto& laberinto) {
    static vector<pair<int, int> > direcciones;

    if (direcciones.empty()) {
        direcciones.push_back(make_pair(1, 0));
        direcciones.push_back(make_pair(-1, 0));
        direcciones.push_back(make_pair(0, 1));
        direcciones.push_back(make_pair(0, -1));
    }

    random_device rd;
    mt19937 gen(rd());
    shuffle(direcciones.begin(), direcciones.end(), gen);

    for (int i = 0; i < direcciones.size(); ++i) {
        const auto& dir = direcciones[i];
        int nx = x + 2 * dir.first;
        int ny = y + 2 * dir.second;

        if (estaDentroLimites(nx, ny, laberinto.size(), laberinto[0].size()) && laberinto[nx][ny] == 1) {
            laberinto[x + dir.first][y + dir.second] = 0; // Eliminar la pared entre las celdas
            laberinto[nx][ny] = 0; // Marcar la celda como visitada
            generarLaberinto(nx, ny, laberinto); // Llamada recursiva
        }
    }
}

void imprimirLaberinto(const Laberinto& laberinto) {
    for (int i = 0; i < laberinto.size(); ++i) {
        for (int j = 0; j < laberinto[i].size(); ++j) {
            cout << (laberinto[i][j] == 1 ? "█" : " "); // Representación gráfica de las paredes
        }
        cout << endl;
    }
}

bool encontrarCamino(int x, int y, int destinoX, int destinoY, Laberinto& laberinto) {
    // Verificar si hemos alcanzado el destino
    if (x == destinoX && y == destinoY) {
        return true;
    }

    // Marcamos la celda actual como visitada
    laberinto[x][y] = 2; // Marcamos la celda como parte del camino

    // Definimos las posibles direcciones
    static vector<pair<int, int> > direcciones;

    if (direcciones.empty()) {
        direcciones.push_back(make_pair(1, 0));
        direcciones.push_back(make_pair(-1, 0));
        direcciones.push_back(make_pair(0, 1));
        direcciones.push_back(make_pair(0, -1));
    }

    // Recorremos las direcciones
    for (int i = 0; i < direcciones.size(); ++i) {
        const auto& dir = direcciones[i];
        int nx = x + dir.first;
        int ny = y + dir.second;

        // Verificamos si la siguiente celda es válida y no ha sido visitada
        if (estaDentroLimites(nx, ny, laberinto.size(), laberinto[0].size()) &&
            laberinto[nx][ny] == 1) {
            // Realizamos una llamada recursiva para explorar la siguiente celda
            if (encontrarCamino(nx, ny, destinoX, destinoY, laberinto)) {
                return true; // Si encontramos el destino, retornamos verdadero
            }
        }
    }

    // Si no encontramos un camino válido desde esta celda, la marcamos como no parte del camino
    laberinto[x][y] = 0;
    return false;
}

void imprimirCamino(const Laberinto& laberinto) {
    for (int i = 0; i < laberinto.size(); ++i) {
        for (int j = 0; j < laberinto[i].size(); ++j) {
            if (laberinto[i][j] == 2) {
                cout << "*"; // Representación gráfica del camino
            } else {
                cout << (laberinto[i][j] == 1 ? "█" : " "); 
            }
        }
        cout << endl;
    }
}


int main() {
    // Definir el tamaño del laberinto
    int filas = 13; // Debe ser impar
    int columnas = 13; // Debe ser impar

    // Inicializar el laberinto con todas las paredes
    Laberinto laberinto(filas, vector<int>(columnas, 1));

    // Generar el laberinto
    int inicioX = 1;
    int inicioY = 1;
    laberinto[inicioX][inicioY] = 0; // Marcar la celda de inicio como visitada
    generarLaberinto(inicioX, inicioY, laberinto);

    // Imprimir el laberinto
    imprimirLaberinto(laberinto);

    // Definimos las coordenadas de inicio y destino
    int destinoX = filas - 2;
    int destinoY = columnas - 2;

    // Encontramos el camino utilizando 
    encontrarCamino(inicioX, inicioY, destinoX, destinoY, laberinto);

    // Imprimimos el laberinto con el camino
    imprimirCamino(laberinto);

    return 0;
}