#pragma once

#include <iostream> //std::cout
#include <string>   //std::string
#include <array>    //std::array
#include <mutex>    //std::mutex std::lock()
#include <thread>   //std::thread
#include <chrono>   //std::steady_clock

using namespace std;

/**
 * Clase para cada dron. Permite uso compartido de zonas gracias a los 
 * numeros de zona utilziados que almacena. 
 * Estos serán indices para acceder al array de zonas.
 * 
*/
class Drone{
    int id;      //numero de dron
    int z1, z2;  //numeros de zonas que usa
    public:
        Drone() = default;      //constructor para inicializar dentro de containers default

        // Constructor de drone, toma las zonas asginadas y el id del dron
        Drone(int drone_id, int zona1, int zona2) : id(drone_id), z1(zona1), z2(zona2) {}

        /**
         * params: 
         *      print (mutex&) //maneja el recurso global del cout
         *      zonas (array<mutex, 5>) //es un arreglo de los mutex que controlan cada zona de despegue
         * 
         * Idea:
         *      Cada vez que se quiera hacer un print por cout, se llama al metodo log.
         *      Por otro lado la forma de evitar un deadlock por uso de zonas compartidas es mediante
         *      lock(zonas[z1], zonas[z2]) que no es mas que lockear simulteaneamente los mutex de las 
         *      2 zonas subyacentes al dron (las que usa). Se pausa el hilo por 5000 ms, justo cuando alcanza la altura 
         *      para liberar las zonas, entonces se avisa que alcanzó altura de 10m y se liberan los mutex.
         */
        void despegar(mutex& print, array<mutex, 5>& zonas, chrono::steady_clock::time_point start);

        /**
         * params:
         *      msg (string)  //mensaje a logear
         *      start (chrono::steady_clock::time_point)    //cronometro marcando el inicio del conteo inicializado en main
         *      print (mutex&) //referencia al mutex para el uso de cout
         * 
         * Idea:
         *      Cada vez que necesitemos hacer un log de estado (imprimir mensaje de espera, despegue o finalizacion)
         *      se llama la funcion para dejar un mensaje al cout. De esta forma se puede tambien, loggear en el tiempo en el que 
         *      sucede cada evento y tener una noción de la dinámica del sistema.
         */
        void log(string msg, chrono::steady_clock::time_point start, mutex& print) const;
        ~Drone() = default;
};
