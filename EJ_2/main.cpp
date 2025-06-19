#include "drone.hpp"

int main() {
    auto start_time = chrono::steady_clock::now(); //Inicializamos el cronometro
    mutex print;    //mutex para cout
    array<mutex, 5> zonas;  //arreglo de zonas
    array<Drone, 5> drone_o;    //arreglo de objetos Drone
    for (int i = 0; i < 5; ++i) {
        drone_o[i] = Drone(i, i, (i+1)%5);    //inicializamos cada drone
    }
    array<thread, 5> drones;    //arreglo de threads

    for (int i = 0; i < 5; ++i) {
        drones[i] = thread(&Drone::despegar, &drone_o[i], ref(print), ref(zonas), start_time);  //inicializamos cada thread con las respectivas referencias y parametros
    }

    for (auto& t : drones) t.join();    //Se espera a que todos los threads terminen
}