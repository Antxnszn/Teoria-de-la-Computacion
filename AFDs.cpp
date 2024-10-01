#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    ifstream file("imparceros.txt");
    
    if (!file) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }

    // Leer y mostrar el contenido del archivo
    cout << "Contenido del archivo:" << endl;
    //string fileContent(...) crear cadena de texto a partir de todo el contenido del archivo iterando sobre el archivo
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); //iterar sobre el archivo file carácter por carácter, 
    //desde el principio (istreambuf_iterator<char>(file)
    // hasta el final (istreambuf_iterator<char>())
    cout << fileContent << endl;
    file.clear();
    file.seekg(0, ios::beg); //seek get es usado para mover el puntero de lectura, desde 0 (inicio del archivo) y ios::beg indica que se mueva desde el inicio del archivo
    //usado para que el archivo pueda ser leído nuevamente desde el principio, solo para el bucle while que sigue

    int n_q; // n de q's
    const int alfabeto = 2; // tamaño del alfabeto (alfabeto {0, 1})
    
    file >> n_q;  // leer estados
    if (n_q <= 0) {
        cerr << "Error: Número de estados inválido." << endl;
        return 1;
    }
    
    vector<bool> finales(n_q, false); // Vector para indicar si un estado es final o no
    vector<vector<int>> matriz(n_q, vector<int>(alfabeto)); // Matriz de transiciones

    cout << "Número de estados: " << n_q << endl;

    // Leer los estados finales
    cout << "Estados finales: ";
    string line; // Variable para leer una línea del archivo
    file.ignore(); // Ignorar el salto de línea después del número de estados
    getline(file, line); // Leer la linea de estados finales
    istringstream ss(line); // Convertir la línea en un stream de texto para poder leer los números
    int num_final;
    bool lectura_efinal = false;
    while (ss >> num_final) { // Leer los números de la línea uno por uno hasta que no haya más
        cout << num_final << " ";
        if (num_final >= n_q || num_final < 0) {
            cerr << "Error: Estado final inválido." << endl; // ceer es para imprimir un mensaje de error evitando que el mensaje se mezcle con el flujo del archivo
            return 1;
        }
        finales[num_final] = true; // Marcar el estado como final
        lectura_efinal = true; // Indicar que se leyeron estados finales
    }
    if (!lectura_efinal) {
        cerr << "Error: No se leyeron estados finales." << endl;
        return 1;
    }
    cout << endl;

    // Leer la matriz de transiciones
    cout << "Matriz de transiciones:" << endl;
    for (int i = 0; i < n_q; i++) {
        for (int j = 0; j < alfabeto; j++) {
            if (!(file >> matriz[i][j])) {
                cerr << "Error: No se pudo leer la matriz de transiciones completa." << endl;
                return 1;
            }
            cout << matriz[i][j] << " ";
            if (matriz[i][j] >= n_q || matriz[i][j] < 0) {
                cerr << "Error: Transición inválida en estado " << i << " con símbolo " << j << endl;
                return 1;
            }
        }
        cout << endl;
    }
    file.close();

    // Verificar estados finales
    cout << "Estados finales: ";
    for (int i = 0; i < n_q; i++) {
        if (finales[i]) cout << i << " ";
    }
    cout << endl;

    // Procesar cadenas de entrada
    string cadena;
    while (true) {
        cout << "\nIntroduce la cadena (formada por 0s y 1s), o 'q' para salir: ";
        cin >> cadena;
        
        if (cadena == "q") break;

        int estado = 0; // Estado inicial
        cout << "Estado inicial: " << estado << endl;
        
        bool cadenaValida = true;
        for (char c : cadena) {
            if (c != '0' && c != '1') {
                cerr << "Error: Cadena inválida. Debe estar compuesta solo por 0s y 1s." << endl;
                cadenaValida = false;
                break;
            }
            int simbolo = c - '0'; 
            int estadoAnterior = estado; // Guardar estado anterior
            estado = matriz[estado][simbolo]; // Transicion
            cout << "Símbolo: " << c << ", Transición: " << estadoAnterior << " -> " << estado << endl;
        }

        if (cadenaValida) {
            cout << "Estado final: " << estado << endl;
            cout << "Es estado final: " << (finales[estado] ? "Sí" : "No") << endl;

            // Verificar estado final (aceptacion)
            if (finales[estado]) {
                cout << "Cadena aceptada." << endl;
            } else {
                cout << "Cadena rechazada." << endl;
            }
        }
    }

    return 0;
}