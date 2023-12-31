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
    double completeTime;
    double handleWeight;
    ProcessState state;
    std::string blockColor;

    ProcessControlBlock(int id,const std::string& n, double p, double at, double bt)
        : id(id) ,name(n), priority(p), arrivalTime(at), burstTime(bt), blockColor(NULL),completeTime(0),usedCpuTime(0), state(READY) {}
    
    ProcessControlBlock(int id,const std::string& n, double p, double at, double bt,string c)
        : id(id) ,name(n), priority(p), arrivalTime(at), burstTime(bt), blockColor(c),completeTime(0),usedCpuTime(0), state(READY) {}
    
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
#endif