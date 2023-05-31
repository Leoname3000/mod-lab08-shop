//
// Created by Leonid on 20.05.2023.
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "../include/task.h"

Shop::Shop(int maxCheckouts, double spawnIntensity, int itemTime, int averageItems, int maxClients) {
    this->maxCheckouts = maxCheckouts;
    this->spawnIntensity = spawnIntensity;
    this->itemTime = itemTime;
    this->averageItems = averageItems;
    this->maxClients = maxClients;

    this->shopShouldWork = true;
    this->spawnPeriod = static_cast<int>(1 / spawnIntensity * 1000);
    this->syncPeriod = 50;
    this->totalClients = 0;
    this->servedClients = 0;
    this->lostClients = 0;

    this->totalWorkTime = 0;
    this->sumServingTime = 0;
    this->sumIdlingTime = 0;
    this->sumQueueLength = 0;
    this->sumWaitingTime = 0;
    this->avgServingTime = 0;
    this->avgIdlingTime = 0;
    this->avgQueueLength = 0;
    this->avgWaitingTime = 0;
}

void Shop::SpawnClients() {
    while (shopShouldWork) {
        std::this_thread::sleep_for(std::chrono::milliseconds(spawnPeriod));
        Client client;
        client.first = totalClients;
        totalClients++;
        int items = 1 + (std::rand() % static_cast<int>(averageItems * 2));
        client.second = items;
        if (clients.size() < maxClients) {
            clients.push(client);
            spawnTime.push_back(std::chrono::system_clock::now());
            shopMutex.lock();
            std::cout << "Spawned client " << client.first << " with " << client.second << " items" << std::endl;
            shopMutex.unlock();
        } else {
            shopMutex.lock();
            std::cout << "Client " << client.first << " left!" << std::endl;
            shopMutex.unlock();
        }
    }
}

void Shop::OpenCheckout() {
    while (shopShouldWork) {
        shopMutex.lock();
        if (!clients.empty()) {
            Client current = clients.front();
            int clientID = current.first;
            clients.pop();
            std::cout << "Serving client " << current.first << " for " << itemTime * current.second << "ms... Clients waiting: " << clients.size() << std::endl;
            sumServingTime += itemTime * current.second;
            servedClients++;
            shopMutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(itemTime * current.second));
            shopMutex.lock();
            std::chrono::duration waitingTime = std::chrono::system_clock::now() - spawnTime[clientID];
            sumWaitingTime += waitingTime.count();
            sumQueueLength += clients.size();
        } else {
            sumIdlingTime += syncPeriod;
        }
        shopMutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(syncPeriod));
    }
}

void Shop::Start() {
    for (int i = 0; i < maxCheckouts; i++) {
        checkouts.push_back(std::thread([&] { OpenCheckout(); }));
    }
}

void Shop::End(int workTime) {
    shopShouldWork = false;
    lostClients = totalClients - servedClients;
    totalWorkTime = workTime * 1000;
    avgQueueLength = static_cast<double>(sumQueueLength) / servedClients;
    avgWaitingTime = static_cast<double>(sumWaitingTime) / (servedClients * 1000000);
    avgServingTime = static_cast<double>(sumServingTime) / (maxCheckouts * 1000);
    avgIdlingTime = static_cast<double>(sumIdlingTime) / (maxCheckouts * 1000);
}

void Shop::PrintStatistics() {
    std::cout << std::endl << "---------- STATISTICS ----------" << std::endl;
    std::cout << "Shop worked for " << static_cast<double>(totalWorkTime) / 1000 << " sec" << std::endl;
    std::cout << "Served clients: " << servedClients << std::endl;
    std::cout << "Lost clients: " << lostClients << std::endl;
    std::cout << "Average queue length: " << avgQueueLength << std::endl;
    std::cout << "Average waiting time: " << avgWaitingTime << " sec/client" << std::endl;
    std::cout << "Average serving time: " << avgServingTime << " sec/checkout" << std::endl;
    std::cout << "Average idling time: " << avgIdlingTime << " sec/checkout" << std::endl;
}

void Shop::PrintActualValues() {
    std::cout << std::endl << "---------- ACTUAL VALUES ----------" << std::endl;
    double AL = ActualLambda();
    std::cout << "Actual Lambda: " << AL << std::endl;
    double AM = ActualMu();
    std::cout << "Actual Mu: " << AM << std::endl;
    std::cout << "Actual Rho: " << Rho(AL, AM) << std::endl;
    std::cout << "Actual Idling Probability: " << ActualIdlingProbability() << std::endl;
    double ARP = ActualRejectionProbability();
    std::cout << "Actual Rejection Probability: " << ARP << std::endl;
    double ART = RelativeThroughput(ARP);
    std::cout << "Actual Relative Throughput: " << ART << std::endl;
    std::cout << "Actual Absolute Throughput: " << AbsoluteThroughput(AL, ART) << std::endl;
}

void Shop::PrintTheoreticalValues() {
    std::cout << std::endl << "---------- THEORETICAL VALUES ----------" << std::endl;
    double TL = TheoreticalLambda();
    std::cout << "Theoretical Lambda: " << TL << std::endl;
    double TM = TheoreticalMu();
    std::cout << "Theoretical Mu: " << TM << std::endl;
    std::cout << "Theoretical Rho: " << Rho(TL, TM) << std::endl;
    std::cout << "Theoretical Idling Probability: " << TheoreticalIdlingProbability() << std::endl;
    double TRP = TheoreticalRejectionProbability();
    std::cout << "Theoretical Rejection Probability: " << TRP << std::endl;
    double TRT = RelativeThroughput(TRP);
    std::cout << "Theoretical Relative Throughput: " << TRT << std::endl;
    std::cout << "Theoretical Absolute Throughput: " << AbsoluteThroughput(TL, TRT) << std::endl;
}

double Shop::Rho(double lambda, double mu) {
    return lambda / mu;
}
double Shop::RelativeThroughput(double rejectionProbability) {
    return 1.0 - rejectionProbability;
}
double Shop::AbsoluteThroughput(double lambda, double relativeThroughput) {
    return lambda * relativeThroughput;
}
int Shop::Factorial(int f) {
    int result = 1;
    for (int i = 2; i <= f; i++) {
        result *= i;
    }
    return result;
}

double Shop::ActualLambda() {
    return static_cast<double>(totalClients * 1000) / totalWorkTime;
}
double Shop::ActualMu() {
    return static_cast<double>(servedClients * 1000) / totalWorkTime;
}
double Shop::ActualIdlingProbability() {
    return static_cast<double>(sumIdlingTime) / totalWorkTime;
}
double Shop::ActualRejectionProbability() {
    return static_cast<double>(lostClients) / totalClients;
}

double Shop::TheoreticalLambda() {
    return spawnIntensity;
}
double Shop::TheoreticalMu() {
    return static_cast<double>(maxCheckouts * 1000) / (averageItems * itemTime);
}
double Shop::TheoreticalIdlingProbability() {
    double result = 0;
    double rho = Rho(TheoreticalLambda(), TheoreticalMu());
    int n = maxCheckouts;
    int m = maxClients;
    for (int i = 0; i <= n; i++) {
        result += std::pow(rho, i) / Factorial(i);
    }
    for (int i = 1; i <= m; i++) {
        result += std::pow(rho, n + i) / (pow(n, i) * Factorial(n));
    }
    result = std::pow(result, -1);
    return result;
}
double Shop::TheoreticalRejectionProbability() {
    double rho = Rho(TheoreticalLambda(), TheoreticalMu());
    int n = maxCheckouts;
    int m = maxClients;
    return (std::pow(rho, n + m) * TheoreticalIdlingProbability()) / (std::pow(n, m) * Factorial(n));
}