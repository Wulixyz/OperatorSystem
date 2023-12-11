#include <cstring>
#include <iostream>
#include <variant>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include "VariantWrapper.cpp"

using namespace std;
using namespace emscripten;

enum ProcessState {
    READY,
    RUNNING,
    FINISHED
};

struct ProcessControlBlock {
    std::string name;
    int id;
    double priority;
    double arrivalTime;
    double burstTime;
    double usedCpuTime;
    double handleWeight;
    ProcessState state;
    
    ProcessControlBlock(int id,const std::string& n, double p, double at, double bt)
        : id(id) ,name(n), priority(p), arrivalTime(at), burstTime(bt), usedCpuTime(0), state(READY) {}
    
    variant<double,string,ProcessState> getValueByName(const std::string& varName) {
        if(varName == "name") return this -> name;
        else if(varName == "priority") return this -> priority;
        else if(varName == "arrivalTime") return this -> arrivalTime;
        else if(varName == "burstTime") return this -> burstTime;
        else if(varName == "handleWeight") return this -> handleWeight;
        else if(varName == "state") return this -> state;
        else return "error";
    }
};

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


// ostream& operator<< (ostream& output,const ProcessControlBlock& process) {
//     output << "name: " << process.name << endl;
//     output << "id: " << process.id << endl;
//     output << "priority: " << process.priority << endl;
//     output << "arrivalTime: " << process.arrivalTime << endl;
//     output << "burstTime: " << process.burstTime << endl;
//     output << "usedCpuTime: " << process.usedCpuTime << endl;
//     output << "handleWeight: " << process.handleWeight << endl;
//     output << "state:" << process.state << endl;

//     return output;
// }
