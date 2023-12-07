#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "ProcessDefine.cpp"
#include "Updatable.cpp"

using namespace std;

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

public:
    Scheduler() {
        objectToUpdate.push_back(this);
    }

    void addWaitProcess(const ProcessControlBlock& process) {
        waitProcesses.push_back(process);
    }

    void addHandleProcess(const ProcessControlBlock& process) {
        handleProcesses.push_back(process);
        sortHandleProcesses();
    }

    void clearProcess() {
        waitProcesses.clear();
        handleProcesses.clear();
    }

    void deleteProcessById(vector<ProcessControlBlock>& processes,int id) {
        auto index = processes.begin();
        while(index != processes.end() && index -> id != id) {
            index ++;
        } 

        if(index != processes.end()) {
            processes.erase(index);
        }
    }

    void highestPriorityFirst() {
        settleHandleWeight("priority",1);
        sortWaitProcesses();
    }

    void firstComeFirstServe() {
        settleHandleWeight("arrivalTime",1);
        sortWaitProcesses();
    }

    void shortestJobFirst() {
        settleHandleWeight("burstTime",-1);
        sortWaitProcesses();
    }

    void sortWaitProcesses() {
        sort(waitProcesses.begin(),waitProcesses.end(),[&](ProcessControlBlock& a,ProcessControlBlock& b) {
            return a.handleWeight > b.handleWeight;
        });
    }

    void sortHandleProcesses() {
        sort(handleProcesses.begin(),handleProcesses.end(),[&](ProcessControlBlock& a,ProcessControlBlock& b) {
            return a.handleWeight > b.handleWeight;
        });
    }

    void settleHandleWeight(string varName,int type) {
        for(auto& process : waitProcesses) {
            auto value = process.getValueByName(varName);

            // 使用std::visit提取值
            double extractedValue = std::visit([](auto&& arg) -> double {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_same_v<T, double>) {
                    return arg;
                } else {
                    throw bad_variant_access();
                }
            }, value);

            // 将提取的值与类型相乘
            process.handleWeight = extractedValue * type;
        }
    }

    void updateHandleProcesses() {
        vector<int> deleteProcessId;
        for(auto& process : waitProcesses) {
            if(currentTime >= process.arrivalTime) {
                addHandleProcess(process);
                deleteProcessById(waitProcesses,process.id);
            }
        }
    }

    void update(double time) override {
        currentTime += time;
        
        updateHandleProcesses();

        // cout << currentTime << endl;
    } 

    // debug
    void printWaitProcess() {
        for(auto process : waitProcesses) cout << process << endl;
    }

    void printHandleProcess() {
        for(auto process : handleProcesses) cout << process << endl;
    }
}; 
