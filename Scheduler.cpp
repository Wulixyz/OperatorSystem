#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "ProcessDefine.cpp"
#include "Updatable.cpp"

using std::vector;

extern std::vector<Updatable*> objectToUpdate;

class Scheduler : public Updatable {
private:
    std::vector<ProcessControlBlock> waitProcesses,handleProcesses;
    double currentTime = 0;

    void runProcesses() {
        for (auto& process : handleProcesses) {
            if (process.state == READY && process.arrivalTime <= currentTime) {
                std::cout << "Running process " << process.name << " (Priority: " << process.priority << ")\n";
                process.state = RUNNING;

                while (process.usedCpuTime < process.burstTime) {
                    process.usedCpuTime++;
                    currentTime++;
                }

                std::cout << "Process " << process.name << " completed\n";
                process.state = FINISHED;
            } else {
                std::cout << "process: " << process.name << " is discarded" << endl;
            }
        }
    }

    void addProcess(vector<ProcessControlBlock>& processes,const ProcessControlBlock& process) {
        processes.push_back(process);
    }

public:
    Scheduler() {
        objectToUpdate.push_back(this);
    }

    void addWaitProcess(const ProcessControlBlock& process) {
        waitProcesses.push_back(process);
    }

    void clearProcess() {
        waitProcesses.clear();
        handleProcesses.clear();
    }

    void highestPriorityFirst() {
        std::sort(handleProcesses.begin(), handleProcesses.end(), [](const ProcessControlBlock& a, const ProcessControlBlock& b) {
            return a.priority > b.priority;
        });

        runProcesses();
    }

    void firstComeFirstServe() {
        std::sort(handleProcesses.begin(), handleProcesses.end(), [](const ProcessControlBlock& a, const ProcessControlBlock& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        runProcesses();
    }

    void shortestJobFirst() {
        std::sort(handleProcesses.begin(), handleProcesses.end(), [](const ProcessControlBlock& a, const ProcessControlBlock& b) {
            return a.burstTime < b.burstTime;
        });

        runProcesses();
    }

    void update(double time) override {
        currentTime += time;
        for(auto& process : waitProcesses) {
            if(process.arrivalTime >= currentTime) {
                addProcess(handleProcesses,process);
            }
        }
        cout << currentTime << endl;
    } 
};
