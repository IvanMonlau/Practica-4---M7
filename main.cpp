#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

// Estructura para representar un animal
struct Animal {
    string nombre;
    int velocidadBase;
    int posicion;
    thread* hilo;
};

// Funcion para mover un animal
void moverAnimal(Animal& animal, int meta, int tiempoPausa) {
    random_device rd;
    mt19937 gen(rd()); //Utilizo mt19937 porque es mas eficiente en entornos multihilo, ya que me permite generar multiples secuencias aleatorias

    // Distribuciones aleatorias personalizadas para cada animal
    uniform_int_distribution<> disTortuga(1, 2);  // La tortuga siempre avanza al menos 1
    uniform_int_distribution<> disLiebre(-1, 4);  // La liebre puede avanzar 1, 2, 3 o 4 y retroceder -1
    uniform_int_distribution<> disGato(0, 3);   // El gato tambien puede avanzar 0, 1 o 2

    while (animal.posicion < meta) {
        if (animal.nombre == "Tortuga") {
            animal.posicion += disTortuga(gen);
        } else if (animal.nombre == "Liebre") {
            animal.posicion += disLiebre(gen);
        } else {
            animal.posicion += disGato(gen);
        }

        this_thread::sleep_for(chrono::milliseconds(tiempoPausa));
    }
}

// Funcion para mostrar la posicion de todos los animales
void mostrarPosiciones(const vector<Animal>& animales, int meta) {
    system("cls"); // Limpiar la pantalla
    for (int i = 0; i < meta; ++i) {
        cout << "-";
    }
    cout << endl;

    for (const Animal& animal : animales) {
        for (int i = 0; i < animal.posicion; ++i) {
            cout << " ";
        }
        cout << animal.nombre[0] << endl;
    }
}

int main() {
    const int meta = 50;
    const int tiempoPausa = 500; // Tiempo de pausa en milisegundos

    vector<Animal> animales = { //Inicializo el vector con velocidad base y posicion inicial
        {"Tortuga", 1, 0},
        {"Liebre", 5, 0},
        {"Gato", 3, 0}
    };

    // Crear los threads
    for (Animal& animal : animales) {
        animal.hilo = new thread(moverAnimal, ref(animal), meta, tiempoPausa);
    }

    // Mostrar la carrera
    while (true) {
        bool todosHanTerminado = true;
        for (const Animal& animal : animales) {
            if (animal.posicion < meta) {
                todosHanTerminado = false;
                break;
            }
        }
        if (todosHanTerminado) {
            break;
        }
        mostrarPosiciones(animales, meta);
        this_thread::sleep_for(chrono::milliseconds(tiempoPausa));
    }

    // Encontrar al ganador
    int maxPosicion = 0;
    string ganador;
    for (const Animal& animal : animales) {
        if (animal.posicion > maxPosicion) {
            maxPosicion = animal.posicion;
            ganador = animal.nombre;
        }
    }

    cout << "El ganador es: " << ganador << endl;

    return 0;
}