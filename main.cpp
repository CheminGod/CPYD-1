#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <random>

int results[10]; 
std::mutex mtx;  


void generate(int id) {
    
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, 1000); 

    int sum = 0;
    for (int i = 0; i < 100; i++) {
        int num = dis(gen); 
        sum += num;
    }
    results[id] = sum;

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << id + 1 << " obtuvo suma = " << sum << std::endl;
    }
}

int main() {
    srand(time(0)); 
    std::thread threads[10];

    // lanzar los 10 threads
    for (int i = 0; i < 10; i++) {
        threads[i] = std::thread(generate, i);
    }

    // esperar que terminen
    for (int i = 0; i < 10; i++) {
        threads[i].join();
    }

    // encontrar el ganador
    int maxScore = results[0];
    int bestThread = 0;
    for (int i = 1; i < 10; i++) {
        if (results[i] > maxScore) {
            maxScore = results[i];
            bestThread = i + 1;
        }
    }

    std::cout << "\nEl thread con la puntuacion mas alta fue el Thread "
              << bestThread << " con suma = " << maxScore << std::endl;

    return 0;
}