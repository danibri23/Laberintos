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
bool termino = false;


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

void generarLaberinto(Laberinto &laberinto, int fila, int columna, mt19937 &gen, int inicioFila, int inicioColumna) {

    laberinto[fila][columna].celdaVisitada = true;

    laberinto[inicioFila][inicioColumna].paredIzquierda = false;
    laberinto[1][0].paredInferior= false;

    // Comprobar si es el punto de inicio o salida y actualizar las paredes correspondientes

    vector<pair<int, int> > direcciones;
    
    direcciones.push_back(make_pair(1, 0));
    direcciones.push_back(make_pair(-1, 0));
    direcciones.push_back(make_pair(0, 1));
    direcciones.push_back(make_pair(0, -1));


    shuffle(direcciones.begin(), direcciones.end(), gen);

    for (int i = 0; i < direcciones.size(); ++i) {
        const pair<int, int> &dir = direcciones[i]; 
        
        int nuevaFila = fila + dir.first;
        int nuevaColumna = columna + dir.second;

        if ((nuevaFila >= 0 && nuevaFila < filas) && (nuevaColumna >= 0 && nuevaColumna < columnas) && (!laberinto[nuevaFila][nuevaColumna].celdaVisitada)) {
            if (dir.first == 1) {
                laberinto[fila][columna].paredInferior = false;
                laberinto[nuevaFila][nuevaColumna].paredSuperior = false;
            } else if (dir.first == -1) {
                laberinto[fila][columna].paredSuperior = false;
                laberinto[nuevaFila][nuevaColumna].paredInferior = false;
            } else if (dir.second == 1) {
                laberinto[fila][columna].paredDerecha = false;
                laberinto[nuevaFila][nuevaColumna].paredIzquierda = false;
            } else if (dir.second == -1) {
                laberinto[fila][columna].paredIzquierda = false;
                laberinto[nuevaFila][nuevaColumna].paredDerecha = false;
            }

            generarLaberinto(laberinto, nuevaFila, nuevaColumna, gen, inicioFila, inicioColumna);
        }
    }
}
 void imprimirLaberinto(Laberinto &laberinto) {

    laberinto[0][0].paredIzquierda = false;
    laberinto[filas - 1][columnas - 1].paredInferior = false;

    // Itera por la cantidad de filas.
    for (size_t i = 0; i < laberinto.size(); ++i) {
        // Imprime las paredes superiores de la fila actual.
        for (size_t j = 0; j < laberinto[i].size(); ++j) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // Itera por la cantidad de columnas, pero esta vez imprime las paredes izquierdas y derechas.
        for (size_t j = 0; j < laberinto[i].size(); ++j) {
            // Imprime la pared izquierda si está presente o un espacio en blanco si no.
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");
            cout << "   ";
            // Imprime la pared derecha si está presente en la celda actual o una pared vertical si es la última columna.
            if (j == laberinto[i].size() - 1) {
                cout << "|";
            }
        }
        cout << endl;
    }

    // Imprime las paredes inferiores para la última fila.
    for (size_t j = 0; j < laberinto[0].size(); ++j) {
        cout << "+";
        cout << (laberinto[laberinto.size() - 1][j].paredInferior ? "---" : "   ");
    }
    cout << "+" << endl;
}

// Función para resolver el laberinto utilizando DFS y marcar solo las celdas del camino final
bool resolverLaberinto(Laberinto &laberinto, int filaActual, int columnaActual, int filaFinal, int columnaFinal) {
    // Si alcanzamos la celda final, hemos encontrado el camino
    if (filaActual == filaFinal && columnaActual == columnaFinal) {
        termino = true;

        return termino;
    }

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
            !laberinto[nuevaFila][nuevaColumna].paredSuperior &&
            !laberinto[nuevaFila][nuevaColumna].paredInferior &&
            !laberinto[nuevaFila][nuevaColumna].paredIzquierda &&
            !laberinto[nuevaFila][nuevaColumna].paredDerecha &&
            !laberinto[nuevaFila][nuevaColumna].celdaVisitada && termino != true) {
            
            // Intentar resolver el laberinto desde la nueva celda
            if (resolverLaberinto(laberinto, nuevaFila, nuevaColumna, filaFinal, columnaFinal)) {
                // Marcar la celda como parte del camino
                laberinto[nuevaFila][nuevaColumna].celdaVisitada = true;
                return true;
            }
        }
    }

    // Si no se encontró el camino desde ninguna de las celdas adyacentes, retornar falso
    return false;
}

void imprimirLaberintoResuelto(Laberinto &laberinto) {
    laberinto[0][0].paredIzquierda = false;
    laberinto[filas - 1][columnas - 1].paredInferior = false;

    // Itera por la cantidad de filas.
    for (size_t i = 0; i < laberinto.size(); ++i) {
        // Imprime las paredes superiores de la fila actual.
        for (size_t j = 0; j < laberinto[i].size(); ++j) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // Itera por la cantidad de columnas, pero esta vez imprime las paredes izquierdas y derechas.
        for (size_t j = 0; j < laberinto[i].size(); ++j) {
            // Imprime la pared izquierda si está presente o un espacio en blanco si no.
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");
            // Imprime " X " si la celda ha sido marcada como parte del camino, de lo contrario, imprime tres espacios en blanco.
            cout << (laberinto[i][j].celdaVisitada ? " X " : "   ");
            // Imprime la pared derecha si está presente en la celda actual o una pared vertical si es la última columna.
            if (j == laberinto[i].size() - 1) {
                cout << "|";
            }
        }
        cout << endl;
    }

    // Imprime las paredes inferiores para la última fila.
    for (size_t j = 0; j < laberinto[0].size(); ++j) {
        cout << "+";
        cout << (laberinto[laberinto.size() - 1][j].paredInferior ? "---" : "   ");
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

    int filaFinal = filas - 1;  
    int columnaFinal = columnas - 1;  


    // lLamamos a la funcion de generar el Laberinto.
    generarLaberinto(laberinto, 0, 0, generadorNumero, 0, 0);


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
         resolverLaberinto(laberinto, 0, 0,  filaFinal, columnaFinal);
         imprimirLaberintoResuelto(laberinto);

    } else {
        cout << "¡Gracias por jugar!";
    }
};




