#include "events.hpp"   

using namespace std;

const chrono::steady_clock::time_point START_T = chrono::steady_clock::now(); //Inicializamos el cronometro
mutex COUT_MTX;

void safe_print(string message) {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - START_T).count();
    COUT_MTX.lock();
    cout << "[T: " << elapsed << "ms] " << message << endl;
    COUT_MTX.unlock();
}

string random_string_gen(size_t length = 8) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static thread_local mt19937 generator(random_device{}());
    static thread_local uniform_int_distribution<size_t> dist(0, sizeof(charset) - 2);

    string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
        result += charset[dist(generator)];
    return result;
}

void Sensor::produce(int n_tasks) {
    for (int i = 0; i < n_tasks; ++i) {
        queue.newTask(id, random_string_gen(10));
        this_thread::sleep_for(chrono::milliseconds(175));
        safe_print("Sensor #" + to_string(id) + " generó una tarea");
    }
    manager.notifySensorDone();
}

void Robot::consume() {
    Tarea task;
    while (queue.popTask(task)) {
        this_thread::sleep_for(chrono::milliseconds(250));
        safe_print("Robot #" + to_string(id) + " procesando...\n" + task.toString());
    }
    safe_print("Robot #" + to_string(id) + " terminó");       
}


void TaskQueue::newTask(int Sensor, string d) {
    {
    lock_guard<mutex> lock(handler);
    tasks.emplace(Sensor, tasks.size(), d);
    }
    cv.notify_one();
}

bool TaskQueue::popTask(Tarea& task) {
    unique_lock<mutex> lock(handler);
    cv.wait(lock, [this]() { return !zeroTask() || done.load(); });
    if (!zeroTask()) {
        task = tasks.front();
        tasks.pop();
        lock.unlock();
        return true;
    }
    else {
        return false;
    }
}

bool TaskQueue::zeroTask() {
    lock_guard<mutex> lock(handler);
    return tasks.empty();
}

void TaskQueue::setDone() {
    done.store(true);
    cv.notify_all();
}

void EventManager::notifySensorDone() {
    if (++sensoresTerminados == totalSensores) queue.setDone();
}