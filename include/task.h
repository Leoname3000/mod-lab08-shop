//
// Created by Leonid on 20.05.2023.
//

#ifndef MOD_LAB08_SHOP_SRC_TASK_H_
#define MOD_LAB08_SHOP_SRC_TASK_H_

#include <utility>
#include <vector>
#include <mutex>
#include <queue>
#include <thread>

typedef std::pair<int, int> Client;

class Shop {
 private:
    int maxCheckouts;
    double spawnIntensity;
    int itemTime;
    int averageItems;
    int maxClients;

    int totalClients;
    int servedClients;
    int lostClients;
    int totalWorkTime;
    int sumServingTime;
    int sumIdlingTime;
    int sumQueueLength;
    int sumWaitingTime;
    double avgServingTime;
    double avgIdlingTime;
    double avgQueueLength;
    double avgWaitingTime;

    bool shopShouldWork;
    int spawnPeriod;
    int syncPeriod;
    std::thread counter;
    std::vector<std::thread> checkouts;
    std::queue<Client> clients;
    std::vector<std::chrono::system_clock::time_point> spawnTime;

 public:
    std::mutex shopMutex;

    Shop(int maxCheckouts, double spawnIntensity, int itemTime, int averageItems, int maxClients);
    void SpawnClients();
    void OpenCheckout();
    void Start();
    void End(int workTime);

    void PrintStatistics();
    void PrintActualValues();
    void PrintTheoreticalValues();

    double Rho(double lambda, double mu);
    double RelativeThroughput(double rejectionProbability);
    double AbsoluteThroughput(double lambda, double relativeThroughput);
    int Factorial(int f);

    double TheoreticalLambda();
    double TheoreticalMu();
    double TheoreticalIdlingProbability();
    double TheoreticalRejectionProbability();

 private:
    double ActualLambda();
    double ActualMu();
    double ActualIdlingProbability();
    double ActualRejectionProbability();
};

#endif //MOD_LAB08_SHOP_SRC_TASK_H_
