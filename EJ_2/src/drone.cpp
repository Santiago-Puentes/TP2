#include "drone.hpp"

void Drone::despegar(mutex& print, array<mutex, 5>& zonas, chrono::steady_clock::time_point start) {   
    log(" esperando para despegar...", start, print);
    lock(zonas[z1], zonas[z2]);
    log(" despegando...", start, print);
    this_thread::sleep_for(chrono::milliseconds(5000));
    log(" alcanzó altura de 10m", start, print);
    zonas[z1].unlock(); zonas[z2].unlock();
}

void Drone::log(string msg, chrono::steady_clock::time_point start, mutex& print) const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();

    print.lock();
    cout << "[T: " << elapsed << "ms] " << "Drone " << id << msg << endl;
    print.unlock();
}