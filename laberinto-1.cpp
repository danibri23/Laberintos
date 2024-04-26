#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <utility>

using namespace std;

random_device rd; // se crea un generador de números aleatorios.
mt19937 generadorNumero(rd()); // se inicializa un generador de números aleatorios que viene del rd.

// declaramos las filas, colummnas y el valor ingresado.
int filas, columnas, valorIngresado;

// estructuramos la Celda.
struct Celda
{
    bool celdaVisitada;
    bool paredSuperior;
    bool paredInferior;
    bool paredIzquierda;
    bool paredDerecha;

    // esto es el constructor de la estructura Celda. Inicializamos por default cada uno de las variables.
    Celda() : celdaVisitada(false), paredSuperior(true), paredInferior(true), paredIzquierda(true), paredDerecha(true) {}
};

// definimos un nuevo tipo llamado Laberinto, el tipo Laberinto es un vector de vectores donde va cada celda.
typedef vector<vector<Celda> > Laberinto;

// definimos la funcion pasando como parametro la cantidad de filas de colummnas y los referenciamos respectivamente.
void generarLaberinto (Laberinto &laberinto, int fil, int col, mt19937 &generadorNumero){

    // se marca la celda en la posición [fil][col] como visitada.
    laberinto[fil][col].celdaVisitada = true;

    // se declara un vector de pares de enteros. Cada par representa una dirección.
    vector<pair<int, int> > direcciones;
    
    // se agrega al vector 'direcciones' pares de coordenadas que representan direcciones (x, y) derecha, izquierda, arriba, abajo.
    direcciones.push_back(make_pair(1, 0));
    direcciones.push_back(make_pair(-1, 0));
    direcciones.push_back(make_pair(0, 1));
    direcciones.push_back(make_pair(0, -1));

    // mezclamos las direcciones usando un generador de números aleatorios con la funcion shuffle.
    shuffle(direcciones.begin(), direcciones.end(), generadorNumero);

    // el bucle recorre cada elemento en el vector 'direcciones'. 
    for (size_t i = 0; i < direcciones.size(); ++i) {

        // obtenemos la dirección actual y referenciamos "dir".
        const pair<int, int> &dir = direcciones[i]; 
        
        // Calculamos la nueva posición.
        int nuevaFila = fil + dir.first;
        int nuevaColumna = col + dir.second;

        // verificamos si la celda está dentro del limite de Laberinto y si no ha sido visitada, por defecto nos devuelve falso, y con el operador logico le decimos true
        if ((nuevaFila >= 0 && nuevaFila < filas) && (nuevaColumna >= 0 && nuevaColumna < columnas) && (!laberinto[nuevaFila][nuevaColumna].celdaVisitada)) {

            // eliminamos las paredes entre las celdas actuales
            if (dir.first == 1) {
                laberinto[fil][col].paredInferior = false;
                laberinto[nuevaFila][nuevaColumna].paredSuperior = false;
            } else if (dir.first == -1) {
                laberinto[fil][col].paredSuperior = false;
                laberinto[nuevaFila][nuevaColumna].paredInferior = false;
            } else if (dir.second == 1) {
                laberinto[fil][col].paredDerecha = false;
                laberinto[nuevaFila][nuevaColumna].paredIzquierda = false;
            } else if (dir.second == -1) {
                laberinto[fil][col].paredIzquierda = false;
                laberinto[nuevaFila][nuevaColumna].paredDerecha = false;
            }

            // llamamos de vuelta a nuestra funcion recursivamente 
            generarLaberinto(laberinto, nuevaFila, nuevaColumna, generadorNumero);
        }
    }
};

// Función para resolver el laberinto utilizando DFS
void resolverLaberinto(Laberinto &laberinto, int filaActual, int columnaActual) {
    // Marcar la celda como visitada
    laberinto[filaActual][columnaActual].celdaVisitada = true;

    // Vector de direcciones posibles: arriba, abajo, izquierda, derecha
    vector<pair<int, int> > direcciones;

    direcciones.push_back(make_pair(1, 0));
    direcciones.push_back(make_pair(-1, 0));
    direcciones.push_back(make_pair(0, 1));
    direcciones.push_back(make_pair(0, -1));

    // Recorrer las direcciones
    for (size_t i = 0; i < direcciones.size(); ++i) {
        const pair<int, int> &dir = direcciones[i];
        int nuevaFila = filaActual + dir.first;
        int nuevaColumna = columnaActual + dir.second;

        // Verificar si la nueva celda es válida y no ha sido visitada
        if (nuevaFila >= 0 && nuevaFila < laberinto.size() &&
            nuevaColumna >= 0 && nuevaColumna < laberinto[0].size() &&
            !laberinto[nuevaFila][nuevaColumna].celdaVisitada) {
            // Verificar si no hay pared entre las celdas
            if (dir.first == -1 && !laberinto[filaActual][columnaActual].paredSuperior) {
                resolverLaberinto(laberinto, nuevaFila, nuevaColumna);
            } else if (dir.first == 1 && !laberinto[filaActual][columnaActual].paredInferior) {
                resolverLaberinto(laberinto, nuevaFila, nuevaColumna);
            } else if (dir.second == -1 && !laberinto[filaActual][columnaActual].paredIzquierda) {
                resolverLaberinto(laberinto, nuevaFila, nuevaColumna);
            } else if (dir.second == 1 && !laberinto[filaActual][columnaActual].paredDerecha) {
                resolverLaberinto(laberinto, nuevaFila, nuevaColumna);
            }
        }
    }
}

// definimos la funcion pasando como parametro el tipo laberinto y lo referenciamos.
void imprimirLaberinto(Laberinto &laberinto) {

    // itera por la cantiad de filas.
    for (int i = 0; i < filas; ++i) {

        // itera por la cantidad de columnas y imprime las paredes superiores.
        for (int j = 0; j < columnas; ++j) {
            cout << "+";

            // si la paredSuperior es true imprime "---" y si no espacio vacio
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // itera por la cantidad de columnas, pero esta vez imprime las paredes izquierdas.
        for (int j = 0; j < columnas; ++j) {

            // si la paredIzquierda es true imprime "|" y si no espacio vacio
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");
            cout << "   ";
        }
        cout << "|" << endl;
    }

    // itera por la cantidad de columnas, pero se imprime las paredes inferiores.
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

// Función para imprimir el laberinto, marcando el camino encontrado durante la resolución
void imprimirLaberintoResuelto(Laberinto &laberinto) {
    // Itera por las filas del laberinto
    for (int i = 0; i < filas; ++i) {
        // Imprime las paredes superiores
        for (int j = 0; j < columnas; ++j) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // Imprime las paredes izquierdas y el contenido de las celdas
        for (int j = 0; j < columnas; ++j) {
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");

            // Si la celda ha sido visitada, imprime un símbolo para marcar el camino
            if (laberinto[i][j].celdaVisitada) {
                cout << " X ";
            } else {
                cout << "   ";
            }
        }
        // Imprime la pared derecha de la última columna
        cout << "|" << endl;
    }

    // Imprime las paredes inferiores
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}


int main () {

    // imprimimos las instrucciones y pedimos los datos al usuario.
    cout << "\n¡Bienvenido al generador de laberintos Don Juanito!" <<endl;
    cout << "------------------------------------------------\n\n";

    cout << "Ingresa el número de filas: ";
    cin >> filas;
    cout << "Ingresa el número de colummnas: " ;
    cin >> columnas;

    cout << "\n------------------------------------------------\n" << endl;

    // llamamos a nuestro tipo Laberinto y le nombramos como laberinto, cargamos las filas del primer vector, luego dentro de ese vector cargamos las columnas.
    Laberinto laberinto(filas, vector<Celda>(columnas));

    // lLamamos a la funcion de generar el Laberinto.
    generarLaberinto(laberinto, 0, 0, generadorNumero);

    // aca llamamos a nuestra funcion donde imprimimos el laberinto
    imprimirLaberinto(laberinto);


    // mas intrucciones
    cout << "\n------------------------------------------------" << endl;
    cout << "¡Laberinto generado con exito! \n" <<endl;

    // aqui solicitamos al usuario el siguiente paso, si quiere ver el Laberinto resuelto o dejar de jugar.
    cout << "¿Le gustaria ver el laberinto resuelto, o lo resuelves usted mismo?"<< endl;

    cout << "Marque 1 (Sí) o marque cualquier otro número (No)" <<endl;
    cin >> valorIngresado;

    // si el usuario ingresa 1, le imprime el Laberinto resuelto.
    if ( valorIngresado == 1 ) {
         resolverLaberinto(laberinto, 0, 0);
         imprimirLaberintoResuelto(laberinto);

    } else {
        cout << "¡Gracias por jugar!";
    }
};




