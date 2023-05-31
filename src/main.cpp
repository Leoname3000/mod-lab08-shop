//
// Created by Leonid on 20.05.2023.
//

#include "task.h"
#include <thread>

typedef std::chrono::seconds sec;
const int WORK_FOR_SECONDS = 10;

int main() {
    Shop shop = Shop(3, 3, 100, 10, 10);
    shop.Start();
    std::thread spawn([&]() { shop.SpawnClients(); });
    std::thread counter([&] {
        std::this_thread::sleep_for(sec(WORK_FOR_SECONDS));
        shop.End(WORK_FOR_SECONDS);
    });
    spawn.join();

    shop.PrintStatistics();
    shop.PrintActualValues();
    shop.PrintTheoreticalValues();
    exit(0);
}