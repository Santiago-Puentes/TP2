#pragma once

#include <iostream> // std::cout
#include <string>   //std::string
#include <thread>   //std::thread
#include <chrono>   //std::steady_clock
#include <queue>    //std::queue
#include <mutex>    //std::mutex, std::lock, std::unique_lock y std::lock_guard
#include <condition_variable>   //std::condition_variable
#include <atomic>   //std::atomic_int, std::atomic_bool y métodos de lectura/escritura como load() y store()
#include <list>     //std::list

using namespace std;

// Importante definir los variables extern o no compilará (están definidas en events.cpp)
extern const std::chrono::steady_clock::time_point START_T; // Guarda el momento de arranque del programa (usado para timestamp)
extern mutex COUT_MTX; // mutex global para el uso de cout 

/** 
 * Permite hacer uso de cout protegiendo el acceso al recurso con
 * un mutex global denominado COUT_MTX el cual se encuentra definido en events.cpp
 * Esto se implementa por ser el cout un recurso compartido y evitar race conditions (mensajes solapados)
 */
void safe_print(string message);

struct Tarea {
    Tarea() = default;  // Constructor para inicialización default 
    Tarea(int id_sensor, int id_task, string d) : //Inicialización común
            idSensor(id_sensor), idTarea(id_task), descripcionTarea(d) {}

    int idSensor, idTarea;
    string descripcionTarea;
    // Devuelve un string con toda la info de la tarea
    string toString() const {
        return ("Tarea #" + to_string(idTarea) + " creada por: SENSOR[" + to_string(idSensor) + "]\nDescripción:" + descripcionTarea);
    }
};

class Manager;

/**
 * Es un contenedor para la cola de tareas, cuenta con un mutex de acceso a la cola.
 * Implementa una condition_variable para permitir su uso con multithreading y 
 * contadores atomicos de tareas procesadas y generadas. Además tiene una flag 
 * de estado booleano para cuando los productores de la cola han dejado de trabajar se avise 
 * (mediante un manager del entorno) a los consumidores. 
 * Lo que nos permite esta implementación es encapsular el comportamiento de una cola 
 * multithreading para independzarlo de sus trabajadores. Entonces si se quiere producir
 * una tarea se invoca al método newTask con los parametros de construcción de la tarea y la 
 * estructura la produce e inserta en la queue. Lo mismo si se quiere consumir, se invoca
 * popTask y se recibe.
 * 
 */
class TaskQueue {
    queue<Tarea> tasks;         // Cola de tareas
    mutex handler;              // MTX de la cola
    condition_variable cv;      // Condicion variable para avisar a los consumidores de tareas
    atomic_bool done{false};    // Estado de producción de tareas terminada {false (activa), true (finalizada)}
    atomic_int t_generated{0};  // Contador total de tareas generadas
    atomic_int t_procesed{0};   // Contador total de tareas procesadas

    public:
        TaskQueue() = default;
        // Permite insert de una tarea dado el id y la descripción
        void newTask(int sensor, string descripcion);
        // Hace pop de una tarea si la cola se encuentra disponible, devuelve true si sucede y escribe el objeto sobre la referencia pasada
        bool popTask(Tarea& task);
        // Cambia el estado de done a true
        void setDone();
        // Devuelve el estado de done
        bool Done() const {return done.load();}
        // Devuelve la cantidad de tareas generadas 
        int getGen() const {return t_generated.load();}
        // Devuelve la cantidad de tareas procesadas 
        int getPro() const {return t_procesed.load();}
        ~TaskQueue() = default;
};

/**
 * Es un sensor que genera tareas de forma independiente en sinergía con el 
 * Manager y la TaskQueue. Cuenta con un thread que trabaja de forma paralela
 * y es lo que permite hacer una gran cantidad de sensores trabajando en simultáneo
 * para sensar y generar tareas en base a las mediciones.
 */
class Sensor {
    TaskQueue &queue;   // Cola de tareas para la cual produce tareas
    Manager& manager;   // Manager para el cual trabaja
    int id;             // id del Sensor
    thread worker;      // thread asignado al Sensor
    public:
        /**
         * Constructor de Sensor
         * 
         * params:
         *      q (es la cola de tareas que se le asigna)
         *      m (es el manager que se le asigna)
         *      i (el id del sensor)
         *      n_tasks (la cantidad de tareas que va a producir)
         * 
         * idea:
         *      Hay una particularidad en el constructor del sensor y es la forma
         *      forma de inicializar sus miembros. Luego de la lista de inicialización convencional
         *      que siempre utilizamos se le asigna al worker un objeto thread cuya función es una lambda.
         *      Primero, notar que worker es reasignado por el "=", esto no es un problema ya que la inicialización
         *      default del thread no lo bindea a una función.
         *      Segundo, la función lambda captura del scope el Sensor creado mediante "this" y el numero de tareas con el que 
         *      el sensor fue asignado. No recibe parametros y lo que hace en su cuerpo es llamar a la función produce(int)
         *      que pone a trabajar al worker instantaneamente. Con esto lo que garantizamos es que el worker sea inicializado
         *      correctamente con su función respectiva y que apenás se inicializa el Sensor se lo ponga a producir tareas.
         *      Otra forma de inicializarlo sería en la lista mediante: worker([this, n_tasks](){produce(n_tasks); })
         *      El problema que puede surgir es que la lista no garantiza una inicialización específica. Esto significa
         *      que podríamos inicializar el miembro worker sin que se haya inicializado antes la cola o el manager, lo cual
         *      sí implica un problema al tener que interactuar con ellos.
         */
        Sensor(TaskQueue& q, Manager& m, int i, int n_tasks) : 
            queue(q), manager(m), id(i) {
                worker = thread([this, n_tasks](){produce(n_tasks); });
            }

        // Función de producción de tareas. Recibe la cantidad de tareas que el Sensor tiene que generar
        void produce(int n_tasks);
        
        // Destructor especificado para multithreading
        ~Sensor() {
            worker.join();  // Hacemos join para evitar que el Sensor sea destruido cuando su worker siga en producción
        }
};

/**
 * Es un robot que procesa tareas de forma independiente. Cuenta con un hilo
 * que permite el uso de multiples robots trabajando en paralelo para procesar
 * tareas de la cola que tiene asignada.
 */
class Robot {
    TaskQueue &queue;   // Cola de tareas asignada
    int id;             // id del Robot
    thread worker;      // thread del Robot
    public:
        /**
         * Constructor de Robot
         * 
         * params:
         *      q (es la cola de tareas que se le asigna)
         *      i (el id del Robot)
         * 
         * idea: 
         *      Es análogo a Sensor solo que implemetado para el worker de
         *      Robot, poniendolo a consumir e inicializando sus miembros, cola e id
         */
        Robot(TaskQueue& q, int i) : 
            queue(q), id(i) {
                worker = thread([this] { consume(); });
            }
        
        // Función de consumo de taeras
        void consume();

        // Destructor especificado para multithreading
        ~Robot() {
            worker.join();  // Hacemos join para evitar que el Robot sea destruido cuando su worker siga en producción
        }
};

/**
 * Es un Manager de eventos. Basicamente es quien interactua con la cola, los sensores
 * y los robots. Es el encargado de asginar la cola a sensores y robots, cambiar el estado
 * de prodcucción de la cola y avisar a los sensores cuando la cola deja de estar en funcionamiento
 * Cuenta con contenedores de Sensor y Robot implementado con listas (para evitar move de la estructura por extension)
 * y así implementar o simular el funcionamiento en paralelo de los sensores y robots junto con la cola
 */
class Manager {
    TaskQueue& queue;                   // Cola de tareas con la cual trabaja
    list<Sensor> sensores;              // lista de sensores a cargo
    list<Robot> robots;                 // lista de robots a cargo
    atomic_int sensoresTerminados{0};   // contador de sensores terminados

    public:
        /**
         * Constructor de Managaer
         * 
         * params:
         *      q (cola de tareas a asignar)
         *      sensoresCount (cantidad de sensores a cargo)
         *      robotsCount (cantidad de robots a cargo)
         *      n_tasks (la cantidad de tareas que cada sensor debe producir)
         * 
         * idea:
         *      Se asgina la cola, luego en el cuerpo se inicializan las listas de sensores
         *      y de robots. Se hace uso de emplace ya que llama al constructor respectivo
         *      del contenido pasandole los parametros de interes. 
         *      Notar que cuando es asignado, cada robot/sensor arranca su tarea
         */
        Manager(TaskQueue& q, int sensoresCount, int robotsCount, int n_tasks) : queue(q) 
        {
            for (int i = 0; i < sensoresCount; ++i)
                sensores.emplace_back(queue, *this, i, n_tasks);    // Se le pasa this para que el sensor asgine al manager que lo construye
            for (int i = 0; i < robotsCount; ++i)
                robots.emplace_back(queue, i);
        }

        // Verifica si la cola finalizo su producción al terminar un sensor y avisa a los robots de ser así
        void notifySensorDone() {if (++sensoresTerminados == int(sensores.size())) queue.setDone();}    // Si los terminados son igual a los totales, seteamos la cola finalizada
        
        ~Manager() = default;
};