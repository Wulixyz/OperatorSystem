#include <cstring>
#include <iostream>

using std::cin,std::endl,std::cout,std::ostream;

enum ProcessState {
    READY,
    RUNNING,
    FINISHED
};

struct ProcessControlBlock {
    std::string name;
    int priority;
    int arrivalTime;
    int burstTime;
    int usedCpuTime;
    ProcessState state;

    ProcessControlBlock(const std::string& n, int p, int at, int bt)
        : name(n), priority(p), arrivalTime(at), burstTime(bt), usedCpuTime(0), state(READY) {}
};

ostream& operator<< (ostream& output,const ProcessControlBlock& process) {
    output << "name: " << process.name << endl;
    output << "priority: " << process.name << endl;
    output << "arrivalTime: " << process.arrivalTime << endl;
    output << "burstTime: " << process.burstTime << endl;
    output << "usedCpuTime: " << process.usedCpuTime << endl;
    output << "state:" << process.state << endl;

    return output;
}