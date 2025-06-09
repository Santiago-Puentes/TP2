#include <iostream>
#include <string>
#include <array>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

/*  Clase para cada dron*/
class Drone{
    int id;   //numero de dron
    array<int, 2> usa;  //numero de zonas que usa
    public:
        Drone() = default;      //constructor para inicializar dentro de containers default
        Drone(int drone_id, array<int, 2>&& zonas_asignadas) : id(drone_id), usa(zonas_asignadas) {} //Se usa rvalue

        /**
         * params: 
         *      print (mutex&) //maneja el recurso global del cout
         *      zonas (array<mutex, 5>) //es un arreglo de los mutex que controlan cada zona de despegue
         * 
         * Idea:
         *      Cada vez que se quiera hacer un print por cout, se llama al metodo log.
         *      Por otro lado la forma de evitar un deadlock por uso de zonas compartidas es mediante
         *      lock(zonas[usa[0]], zonas[usa[1]]) que no es mas que lockear simulteaneamente los mutex de las 
         *      2 zonas subyacentes al dron (las que usa). Se pausa el hilo por 5000 ms, justo cuando alcanza la altura 
         *      para liberar las zonas, entonces se avisa que alcanzó altura de 10m y se liberan los mutex.
         */
        void despegar(mutex& print, array<mutex, 5>& zonas, chrono::steady_clock::time_point start) {   
            log(" esperando para despegar...", start, print);
            lock(zonas[usa[0]], zonas[usa[1]]);
            log(" despegando...", start, print);
            this_thread::sleep_for(chrono::milliseconds(5000));
            log(" alcanzó altura de 10m", start, print);
            zonas[usa[0]].unlock(); zonas[usa[1]].unlock();
        }

        /**
         * params:
         *      msg (string&&)  //rvalue del mensaje a logear
         *      start (chrono::steady_clock::time_point)    //cronometro marcando el inicio del conteo inicializado en main
         *      print (mutex&) //referencia al mutex para el uso de cout
         * 
         * Idea:
         *      Cada vez que necesitemos hacer un log de estado (imprimir mensaje de espera, despegue o finalizacion)
         *      se llama la funcion para dejar un mensaje al cout. De esta forma se puede tambien, loggear en el tiempo en el que 
         *      sucede cada evento y tener una noción de la dinámica del sistema.
         */
        void log(string&& msg, chrono::steady_clock::time_point start, mutex& print) const {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();

            print.lock();
            cout << "[+" << elapsed << "ms] " << "Drone " << id << msg << endl;
            print.unlock();
        }

        ~Drone() = default;
};


int main() {
    auto start_time = std::chrono::steady_clock::now(); //Inicializamos el cronometro
    mutex print;    //mutex para cout
    array<mutex, 5> zonas;  //arreglo de zonas
    array<Drone, 5> drone_o;    //arreglo de objetos Drone
    for (int i = 0; i < 5; ++i) {
        drone_o[i] = Drone(i, {i, (i+1)%5});    //inicializamos cada drone
    }
    array<thread, 5> drones;    //arreglo de threads

    for (int i = 0; i < 5; ++i) {
        drones[i] = thread(&Drone::despegar, &drone_o[i], ref(print), ref(zonas), start_time);  //inicializamos cada thread con las respectivas referencias y parametros
    }

    for (auto& t : drones) t.join();    //Se espera a que todos los threads terminen
}