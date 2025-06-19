#include "events.hpp"   

const chrono::steady_clock::time_point START_T = chrono::steady_clock::now(); //Inicializamos el cronometro global del inicio del programa
mutex COUT_MTX; //Inicializamos el mutex

void safe_print(string message) {
    auto now = chrono::steady_clock::now(); // Toma el tiempo exacto en el que se invoca el print
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - START_T).count();  // Calcula el tiempo transcurrido
    COUT_MTX.lock();    // Lockea el mutex, si no puede se queda en espera
    cout << "[T: " << elapsed << "ms] " << message << endl; // Una vez que tiene el mutex usa cout (se imprime el tiempo y el mensaje)
    COUT_MTX.unlock();  // Desbloque el mutex
}

void Sensor::produce(int n_tasks) {
    for (int i = 0; i < n_tasks; ++i) { //Arranca el bucle para la cantidad de tareas que tiene que producir
        this_thread::sleep_for(chrono::milliseconds(175));  // Frena 175 ms simulando el tiempo de producción
        queue.newTask(id, "Esta es una tarea de prueba");   // Inserta la tarea generada en la cola
        safe_print("Sensor #" + to_string(id) + " generó una tarea");   // Avisa que genero una tarea (uso depurativo)
    }
    manager.notifySensorDone(); // IMPORTANTE: cuando termina de producir avisa al manager que el sensor salió de producción
}

void Robot::consume() {
    Tarea task; // Es la tarea que se va a ir popeando cuando consuma de la cola
    while (queue.popTask(task)) {   // Mientras se popee de la cola, el resultado será true, task tendrá una nueva tarea para procesar
        safe_print("Robot #" + to_string(id) + " procesando..."); // Avisa que está procesando
        this_thread::sleep_for(chrono::milliseconds(250));  // Espera 250 ms simulando el tiempo de procesamiento
        safe_print("Robot #" + to_string(id) + " procesó:\n"+ task.toString()); // Avisa lo que procesó
    }
    // Notemos que saldrá del loop unicamente cuando (por diseño de la cola) la cola haya salido de producción

    safe_print("Robot #" + to_string(id) + " no procesa más"); // Avisa cuando sale de procesamiento
}


void TaskQueue::newTask(int sensor, string descripcion) {
    {
    lock_guard<mutex> lock(handler); // Hacemos lock_guard para evitar unlock, cuando salga de scope se desbloquea automaticamente
    tasks.emplace(sensor, t_generated++, descripcion);  // Inserta una tarea nueva en la cola con los parametros correspondientes
    }
    cv.notify_one();    // IMPORATANTE: cuando se genera una tarea se avisa a un consumidor que hay una tarea disponible mediante la condition_vairable
    // Con esto garantizamos que si un consumidor estaba en espera, se despiere su worker y verifique si puede consumir y efectivamente se ponga a consumir
}

bool TaskQueue::popTask(Tarea& task) {
    unique_lock<mutex> lock(handler);   // unique_lock para poder hacer uso de cv.wait

    // Se pone en espera de la condition_variable. Entonces se trnasfiere el lock y se pasa la lambda
    // que verificará al despertarse el thread y antes de ponerse en espera.
    cv.wait(lock, [this]() { return !tasks.empty() || done.load(); });
    // Es importante que cuando salga de wait, se verifique de nuevo que haya tareas por eso no es else if
    if (!tasks.empty()) {   // Veriffica si hay tareas disponibles
        task = tasks.front();   // Guarda la tarea próxima en la referencia del parametro
        tasks.pop();    // Popea la tarea próxima
        t_procesed ++;  // Aumenta el contador de tareas procesadas
        return true;    // Devuelve el aviso
    }
    // false se devuelve únicamente cuando efectivamente la cola se encuentra vacia y fuera de produccion (finalizada)
    else return false;
}

void TaskQueue::setDone() {
    done.store(true);   // Guarda true en el flag de estado done de la cola
    cv.notify_all();    // Avisa a todos los consumidores en espera del cambio de estado
}