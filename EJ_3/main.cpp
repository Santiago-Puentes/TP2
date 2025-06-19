#include "events.hpp"


int main(){
    safe_print("Inicio del programa");  // Para garantizar la seguridad del cout
    TaskQueue q;        // Inicializamos la cola de tareas
    {
        Manager(q, 3, 3, 1); // Se inicializa el manager con 3 sensores y 3 robots. Cada sensor genera 1 tarea
    }
    // A partir de este punto los threads están destruidos por lo que cout es seguro de usar
    cout << "Los eventos de creacion y procesamiento de tareas han finalizado" 
            << "\nTOTAL GENERADAS: " << q.getGen() 
            << "\nTOTAL PROCESADAS: " << q.getPro() 
            << endl;
}