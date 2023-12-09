#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "Scheduler.cpp"
#include "Updatable.cpp"
#include <emscripten/emscripten.h>

using namespace std;

std::vector<Updatable*> objectToUpdate;
Scheduler scheduler;
std::vector<ProcessControlBlock> processes;

void input() {
    freopen("input.in","r",stdin);
    // freopen("output.out","w",stdout);

    std::string processName;
    int processPriority = 0,processArrivalTime = 0,processBurstTime = 0,processUsedCupTime = 0,processState;
    int dividedId = 1;
    
    while(std::cin >> processName) {
        std::cin >> processPriority >> processArrivalTime >> processBurstTime;
        processes.push_back(ProcessControlBlock(dividedId ++,processName,processPriority,processArrivalTime,processBurstTime));
    }
}

void initProcessQueue(Scheduler& scheduler,const std::vector<ProcessControlBlock>& processes) {
    scheduler.reset();
    for(auto& process : processes) {
        scheduler.addWaitProcess(process);
    }
}

void startHighestPriorityFisrst() {
    initProcessQueue(scheduler,processes);

    std::cout << "Highest Priority First:\n";
    scheduler.highestPriorityFirst();
}

void startFirstComeFirstServe() {
    initProcessQueue(scheduler,processes);

    std::cout << "First Come First Serve:\n";
    scheduler.firstComeFirstServe();
}

void startShortestJobFirst() {
    initProcessQueue(scheduler,processes);

    std::cout << "Shortest Job First:\n";
    scheduler.shortestJobFirst();
}

extern "C" {

    EMSCRIPTEN_KEEPALIVE void init() {
        input();
        startHighestPriorityFisrst();
    }

    EMSCRIPTEN_KEEPALIVE void run(double time) {
        for(auto& object : objectToUpdate) {
            object -> update(time);
        }
    }

    // debug
    EMSCRIPTEN_KEEPALIVE void printHandleProcess() {
        scheduler.printHandleProcess();
    }

    EMSCRIPTEN_KEEPALIVE void printWaitProcess() {
        scheduler.printWaitProcess();
    }
}

// int main() {
//     init();

//     double preUpdateTime = 0.25,currentTime = 0;
//     while(true) {
//         run(preUpdateTime);

//         std::this_thread::sleep_for(std::chrono::microseconds(int(preUpdateTime * 1000)));
//     }

//     return 0;
// }