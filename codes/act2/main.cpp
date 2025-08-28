#include <iostream>
#include <thread>
#include <vector>
#include <memory>  
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <random>
#include <algorithm>

std::mutex mtx;

class ThreadSum {
private:
    int id;
    int sum;

public:
    ThreadSum(int thread_id) : id(thread_id), sum(0) {}

    int getSum() const {
        return sum;
    }

    int getId() const {
        return id;
    }

    void generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1000);

        for (int i = 0; i < 100; i++) {
            sum += dis(gen);
        }

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "El Thread " << id + 1 << " obtuvo una suma = " << sum << std::endl;
    }
};

int main() {
   
    std::vector<std::unique_ptr<ThreadSum>> thread_sums;
    std::vector<std::thread> threads;

    // lanzar los 10 threads

    for (int i = 0; i < 10; ++i) {
        thread_sums.emplace_back(std::make_unique<ThreadSum>(i));
        threads.emplace_back(&ThreadSum::generate, thread_sums[i].get());
    }

    // esperar que terminen

    for (std::thread &t : threads) {
        t.join();
    }

    // encontrar el ganador

    /*int maxScore = 0;
    int bestThreadId = -1;

    for (const auto& thread_sum : thread_sums) {
        if (thread_sum->getSum() > maxScore) {
            maxScore = thread_sum->getSum();
            bestThreadId = thread_sum->getId();
        }
    }*/

    std::sort(thread_sums.begin(), thread_sums.end(),
            [](const std::unique_ptr<ThreadSum>& a, const std::unique_ptr<ThreadSum>& b) {
                return a->getSum() < b->getSum();
            });

    
    const auto& ganador = thread_sums.back();

    std::cout << "\nEl thread con la puntuacion mas alta fue el Thread "
            << ganador->getId() + 1 << " con una suma = " << ganador->getSum() << std::endl;

    return 0;
}