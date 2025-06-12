#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <list>
#include <random>

using namespace std;

void safe_print(string message);

string random_string_gen(size_t length);

struct Tarea {
    Tarea() = default;
    Tarea(int id_sensor, int id_task, string d) : 
            idSensor(id_sensor), idTarea(id_task), descripcionTarea(d) {}

//   Tarea(const Tarea& t) : idSensor(t.idSensor), idTarea(t.idTarea), descripcionTarea(t.descripcionTarea) {}
    int idSensor, idTarea;
    string descripcionTarea;
    string toString() const {
        return ("Tarea #" + to_string(idTarea) + " creada por: SENSOR[" + to_string(idSensor) + "]\nDescripción:" + descripcionTarea);
    }
};

class EventManager;

class TaskQueue {
    queue<Tarea> tasks;
    mutex handler;
    condition_variable cv;
    atomic_bool done{false};

    public:
        TaskQueue() = default;
        bool zeroTask();
        void newTask(int Sensor, string d);
        bool popTask(Tarea& task);
        void setDone();
        ~TaskQueue() = default;

};

class Sensor {
    TaskQueue &queue;
    EventManager& manager;
    int id;
    jthread worker;
    public:
        Sensor(TaskQueue& q, EventManager& m, int i, int n_tasks) : 
                queue(q), manager(m), id(i), worker([this, n_tasks](){produce(n_tasks); }) {}

        void produce(int n_tasks);
};

class Robot {
    TaskQueue &queue;
    int id;
    jthread worker;
    public:
        Robot(TaskQueue& q, int i) : 
                queue(q), id(i), worker([this] { consume(); }) {}
        
        void consume();
};

class EventManager {
    TaskQueue queue;
    list<Sensor> sensores;
    list<Robot> robots;
    atomic_int sensoresTerminados = 0;
    int totalSensores;

    public:
        EventManager(int sensoresCount, int robotsCount, int n_tasks) : 
                    queue(), totalSensores(sensoresCount) {
            for (int i = 0; i < sensoresCount; ++i)
                sensores.emplace_back(queue, *this, i, n_tasks);
            for (int i = 0; i < robotsCount; ++i)
                robots.emplace_back(queue, i);
        }

        void notifySensorDone();
};