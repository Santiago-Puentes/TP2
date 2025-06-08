#include <iostream>
#include <string>
#include <array>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

class Drone{        //clase para cada dron
    int drone_id;   //numero de dron
    array<int, 2> usa;  //numero de zonas que usa
    public:
        Drone() = default;      //constructor para inicializar dentro de containers default
        Drone(int id, array<int, 2> zonas) : drone_id(id), usa(zonas) {}  

        /**
         * params: 
         *      print (mutex&) //maneja el recurso global del cout
         *      zonas (array<mutex, 5>&) //es un arreglo de los mutex que controlan cada zona de despegue
         * 
         * Idea:
         *      Cada vez que se quiera hacer un print por cout, se debe evitar race conditiones por los threads.
         *      Por esto es que se usa print.lock() y print.unlock() encerrando al uso de cout
         *      Por otro lado la forma de evitar un deadlock por uso de zonas compartidas es mediante
         *      lock(zonas[usa[0]], zonas[usa[1]]) que no es mas que lockear simulteaneamente los mutex de las 
         *      2 zonas subyacentes al dron (las que usa). Se pausa el hilo por 5000 ms, justo cuando alcanza la altura 
         *      para liberar las zonas, entonces se avisa que alcanzó altura de 10m y se liberan los mutex.
         */
        void despegar(mutex& print, array<mutex, 5>& zonas) {   
            print.lock();
            cout << "Drone " << drone_id << " esperando para despegar..." << endl;
            print.unlock();
            lock(zonas[usa[0]], zonas[usa[1]]);
            print.lock();
            cout << "Drone " << drone_id << " despegando..." << endl;
            print.unlock();
            this_thread::sleep_for(chrono::milliseconds(5000));
            print.lock();
            cout << "Drone " << drone_id << "  alcanzó altura de 10m" << endl;
            print.unlock();
            zonas[usa[0]].unlock(); zonas[usa[1]].unlock();
        }
        ~Drone() = default;
};


int main() {
    mutex print;    //mutex para cout
    array<mutex, 5> zonas;  //arreglo de zonas
    array<Drone, 5> drone_o;    //arreglo de objetos Drone
    for (int i = 0; i < 5; ++i) {
        drone_o[i] = Drone(i, {i, (i+1)%5});    //inicializamos cada drone
    }
    array<thread, 5> drones;    //arreglo de threads

    for (int i = 0; i < 5; ++i) {
        drones[i] = thread(&Drone::despegar, &drone_o[i], ref(print), ref(zonas));  //inicializamos cada thread con las respectivas referencias y parametros
    }

    for (auto& t : drones) t.join();    //Se espera a que todos los threads terminen
}