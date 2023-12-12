#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include "Scheduler.cpp"
#include "Updatable.cpp"
#include "ProcessInfoExport.cpp"
#include "exports.h"

using namespace std;
using namespace emscripten;

std::vector<Updatable*> objectToUpdate;
Scheduler scheduler;
std::vector<ProcessControlBlock> processes;
int dividedId = 1;

void initByRandom() { 
    scheduler.reset();

    // 使用默认的随机数引擎和分布器
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(1, 10); // 生成范围在 1 到 10 之间的均匀分布的浮点数
    std::uniform_int_distribution<int> distributionCount(5,10);

    int processcount = distributionCount(generator);

    for(int i = 1;i <= processcount;i ++ ) {
        scheduler.addWaitProcess(ProcessControlBlock(dividedId,"P" + to_string(dividedId),distribution(generator),distribution(generator),distribution(generator)));
        dividedId ++;
    }
}

void selectMode(string mode) {
    if(mode == "FCFS") scheduler.firstComeFirstServe();
    else if(mode == "SJF") scheduler.shortestJobFirst();
    else if(mode == "HPF") scheduler.highestPriorityFirst();
    else printf("select error");
}


void runProcess(double time) {
    for(auto& object : objectToUpdate) {
        object -> update(time);
    }
}


val getWaitProcess() {
    vector<ProcessControlBlock> waitProcess = scheduler.getWaitProcess();

    // 使用 emscripten::val 将 C++ vector 转换为 JavaScript 数组
    val jsArray = val::array();
    for (size_t i = 0; i < waitProcess.size(); ++i) {
        auto& process = waitProcess[i];
        jsArray.set(i, ProcessInfo(process.name,process.id,process.arrivalTime,process.priority,process.burstTime,process.usedCpuTime));
    }

    return jsArray;
}

val getHandleProcess() {
    vector<ProcessControlBlock> handleProcess = scheduler.getHandleProcess();

    // 使用 emscripten::val 将 C++ vector 转换为 JavaScript 数组
    val jsArray = val::array();
    for (size_t i = 0; i < handleProcess.size(); ++i) {
        auto& process = handleProcess[i];
        jsArray.set(i, ProcessInfo(process.name,process.id,process.arrivalTime,process.priority,process.burstTime,process.usedCpuTime));
    }

    return jsArray;
}