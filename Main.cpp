#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include "Scheduler.cpp"
#include "Updatable.cpp"

using namespace std;

std::vector<Updatable*> objectToUpdate;

void input(Scheduler& scheduler,std::vector<ProcessControlBlock>& processes) {
    freopen("input.in","r",stdin);
    freopen("output.out","w",stdout);

    std::string processName;
    int processPriority = 0,processArrivalTime = 0,processBurstTime = 0,processUsedCupTime = 0,processState;
    int dividedId = 1;
    
    while(std::cin >> processName) {
        std::cin >> processPriority >> processArrivalTime >> processBurstTime;
        processes.push_back(ProcessControlBlock(dividedId ++,processName,processPriority,processArrivalTime,processBurstTime));
    }
}

void initProcessQueue(Scheduler& scheduler,const std::vector<ProcessControlBlock>& processes) {
    scheduler.clearProcess();
    for(auto& process : processes) {
        scheduler.addWaitProcess(process);
    }
}

void startHighestPriorityFisrst(Scheduler& scheduler,const std::vector<ProcessControlBlock>& processes) {
    initProcessQueue(scheduler,processes);

    std::cout << "Highest Priority First:\n";
    scheduler.highestPriorityFirst();
}

void startFirstComeFirstServe(Scheduler& scheduler,const std::vector<ProcessControlBlock>& processes) {
    initProcessQueue(scheduler,processes);

    std::cout << "First Come First Serve:\n";
    scheduler.firstComeFirstServe();
}

void startShortestJobFirst(Scheduler& scheduler,const std::vector<ProcessControlBlock>& processes) {
    initProcessQueue(scheduler,processes);

    std::cout << "Shortest Job First:\n";
    scheduler.shortestJobFirst();
}

int main() {
    Scheduler scheduler;
    std::vector<ProcessControlBlock> processes;

    input(scheduler,processes);

    startShortestJobFirst(scheduler,processes);

    // scheduler.printWaitProcess();

    double preUpdateTime = 0.25,currentTime = 0;
    while(true) {
        for(auto& object : objectToUpdate) {
            object -> update(preUpdateTime);
        }

        Sleep(preUpdateTime * 1000);
    }

    return 0;
}