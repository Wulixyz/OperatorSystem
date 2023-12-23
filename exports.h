#ifndef EXPORTS_H
#define EXPORTS_H

#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "ProcessDefine.cpp"
#include "ProcessInfoExport.cpp"

using namespace std;
using namespace emscripten;

void initByRandom();
void selectMode(string);
void runProcess(double);
val getWaitProcess();
val getHandleProcess();

EMSCRIPTEN_BINDINGS(process_state_enum) {
    enum_<ProcessState>("ProcessState")
        .value("READY", READY)
        .value("RUNNING", RUNNING)
        .value("FINISHED", FINISHED);
}

// 注册ProcessControlBlock结构体
EMSCRIPTEN_BINDINGS(process_control_block_struct) {
    class_<ProcessControlBlock>("ProcessControlBlock")
        .constructor<int, const std::string&, double, double, double>()
        .property("name", &ProcessControlBlock::name)
        .property("id", &ProcessControlBlock::id)
        .property("priority", &ProcessControlBlock::priority)
        .property("arrivalTime", &ProcessControlBlock::arrivalTime)
        .property("burstTime", &ProcessControlBlock::burstTime)
        .property("usedCpuTime", &ProcessControlBlock::usedCpuTime)
        .property("handleWeight", &ProcessControlBlock::handleWeight)
        .property("state", &ProcessControlBlock::state)
        .function("getValueByName", &ProcessControlBlock::getValueByName);
}

EMSCRIPTEN_BINDINGS(process_info_struct) {
    class_<ProcessInfo>("ProcessInfo")
        .constructor<string,int,double,double,double,double>()
        .property("name",&ProcessInfo::name)
        .property("id",&ProcessInfo::id)
        .property("priority",&ProcessInfo::priority)
        .property("arrivalTime",&ProcessInfo::arrivalTime)
        .property("burstTime",&ProcessInfo::burstTime)
        .property("usedCpuTime",&ProcessInfo::usedCpuTime);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("initByRandom",&initByRandom);
    emscripten::function("selectMode",&selectMode);
    emscripten::function("runProcess",&runProcess);
    emscripten::function("getWaitProcess",&getWaitProcess);
    emscripten::function("getHandleProcess",&getHandleProcess);
}

#endif // UPDATABLE_H