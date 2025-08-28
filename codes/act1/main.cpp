#include <memory>
#include <string>
#include <iostream>
#include <thread>
//#include <print>
#include <mutex>
#include <condition_variable>

class Fork {
private:
    // bool occupied;
    std::mutex mutex;
    std::condition_variable taken;

public:
    bool occupied = false;

    void pickup(std::string_view philosopherName, std::string_view hand) {
        {
        std::unique_lock lock(mutex);
        taken.wait(lock, [&] {
            std::cout << philosopherName << " is waiting for " << hand << " hand fork" << std::endl;
            return !occupied;
        });
        std::cout << philosopherName << " is taking " << hand << " hand fork" << std::endl;

        occupied = true;
        }
    }

    void putDown(std::string_view philosopherName, std::string_view hand) {
        std::cout << philosopherName << " is releasing " << hand << " hand fork" << std::endl;
        occupied = false;
        taken.notify_all();
    }

    void printState(const std::string &name, const std::string &hand) const {
        std::cout << "{} is {} and {}", name, hand, occupied ? "occupied" : "available";

    }
};

class Philosopher {
private:
    std::string name;
    std::shared_ptr<Fork> leftFork;
    std::shared_ptr<Fork> rightFork;

public:
    std::shared_ptr <Fork> RightFork() const {
        return rightFork;
    }

    std::shared_ptr <Fork> &RightFork() {
        return rightFork;
    }

    std::shared_ptr <Fork> LeftFork() const {
        return rightFork;
    }

    std::shared_ptr <Fork> &LeftFork() {
        return rightFork;
    }


     void eat() {
        while(true) {
        leftFork->pickup(name, "left");
        if (rightFork->occupied) {
            leftFork->putDown(name, "left");
            continue;
        }
        rightFork->pickup(name, "right");

        // Simulating the Philosopher eating
        std::cout << name << " started eating" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << name << " is done eating" << std::endl;

        leftFork->putDown(name, "left");

        rightFork->putDown(name, "right");
        }
    }

    Philosopher(std::string name ) : name(name){}

};

int main() {
    std::shared_ptr<Fork> fork1 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork2 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork3 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork4 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork5 = std::make_shared<Fork>();

    Philosopher socrates("Socrates");
    socrates.LeftFork() = fork1;
    socrates.RightFork() = fork2;
    Philosopher descartes("Descartes");
    socrates.LeftFork() = fork2;
    socrates.RightFork() = fork1;
    // socrates.eat();
    Philosopher plato("Plato");
    plato.LeftFork() = fork3;
    plato.RightFork() = fork4;
    Philosopher aristotle("Aristotle");
    aristotle.LeftFork() = fork4;
    aristotle.RightFork() = fork5;
    Philosopher kant("Kant");
    kant.LeftFork() = fork5;
    kant.RightFork() = fork1;

    std::thread threadDescartes([&](){ descartes.eat(); });
    std::thread threadSocrates([&](){ socrates.eat(); });
    std::thread threadPlato([&](){ plato.eat(); });
    std::thread threadAristotle([&](){ aristotle.eat(); });
    std::thread threadKant([&](){ kant.eat(); });

    threadSocrates.join();
    threadDescartes.join();
    threadPlato.join();
    threadAristotle.join();
    threadKant.join();


    return 0;
}