#ifndef PROCESSDEFINE_H
#define PROCESSDEFINE_H

#include <cstring>
#include <iostream>
#include <variant>

using namespace std;

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

#endif