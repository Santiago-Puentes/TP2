#include "events.hpp"


int main(){
    safe_print("Inicio del programa");
    {
        EventManager(50, 50, 1); // Se inicializa el manager con 3 sensores y 3 robots
    }
    cout << "Los eventos de creacion y procesamiento de tareas han finalizado" << endl;
}