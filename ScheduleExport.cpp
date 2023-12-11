#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "Scheduler.cpp"
#include "Updatable.cpp"
#include "ProcessInfoExport.cpp"

using namespace std;
using namespace emscripten;

std::vector<Updatable*> objectToUpdate;
Scheduler scheduler;
std::vector<ProcessControlBlock> processes;
int dividedId = 1;

void startHighestPriorityFisrst() {
    printf("Highest Priority First:\n");
    scheduler.highestPriorityFirst();
}

void startFirstComeFirstServe() {
    printf("First Come First Serve:\n");
    scheduler.firstComeFirstServe();
}

void startShortestJobFirst() {
    printf("Shortest Job First:\n");
    scheduler.shortestJobFirst();
}

void initByRandom() { 
    scheduler.reset();

    // 使用默认的随机数引擎和分布器
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(1, 10); // 生成范围在 1 到 10 之间的均匀分布的浮点数

    for(int i = 1;i <= 5;i ++ ) {
        scheduler.addWaitProcess(ProcessControlBlock(dividedId,"P" + to_string(dividedId),distribution(generator),distribution(generator),distribution(generator)));
        dividedId ++;
    }
}

void selectMode(const char* str) {
    string mode(str);

    if(mode == "FCFS") startFirstComeFirstServe();
    else if(mode == "SJF") startShortestJobFirst();
    else if(mode == "HPF") startHighestPriorityFisrst();
    else printf("select error");
}

void run(double time) {
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

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("initByRandom",&initByRandom);
    emscripten::function("selectMode",&selectMode,allow_raw_pointer<const char*>());
    emscripten::function("getWaitProcess",&getWaitProcess);
    emscripten::function("getHandleProcess",&getHandleProcess);
}