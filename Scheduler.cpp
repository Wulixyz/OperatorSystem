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
    std::vector<ProcessControlBlock> waitProcesses,handleProcesses,completeProcesses;
    double currentTime = 0;
    double dealingTime = 0;

    void runProcesses() {
        if(handleProcesses.size() && dealingTime >= handleProcesses[0].burstTime) {
            outputCompletedProcess(handleProcesses[0]);
            deleteProcessById(handleProcesses,handleProcesses[0].id);
            dealingTime = 0;
        }
    }

    void sortWaitProcesses() {
        sort(waitProcesses.begin(),waitProcesses.end(),[&](ProcessControlBlock& a,ProcessControlBlock& b) {
            if(a.arrivalTime != b.arrivalTime) return a.arrivalTime < b.arrivalTime;
            else return a.id < b.id;
        });
    }

    void sortHandleProcesses() {
        sort(handleProcesses.begin(),handleProcesses.end(),[&](ProcessControlBlock& a,ProcessControlBlock& b) {
            if(a.handleWeight != b.handleWeight) return a.handleWeight > b.handleWeight;
            else return a.id < b.id;
        });
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
        for(auto& process : waitProcesses) {
            if(currentTime >= process.arrivalTime) {
                addHandleProcess(process);
                deleteProcessById(waitProcesses,process.id);
            }
        }
    }

    void outputCompletedProcess(ProcessControlBlock& process) {
        printf("%s has completed in %.2lf\n",process.name.c_str(),currentTime);
        printf("circulationTime is %.2lf\n",currentTime - process.arrivalTime);
        process.completeTime = currentTime;
        completeProcesses.push_back(process);
    }

public:
    Scheduler() {
        objectToUpdate.push_back(this);
    }

    void addWaitProcess(const ProcessControlBlock& process) {
        waitProcesses.push_back(process);
    }

    void addHandleProcess(const ProcessControlBlock& process) {
        if(handleProcesses.size() && process.handleWeight > handleProcesses[0].handleWeight) {
            dealingTime = 0;
        }
        handleProcesses.push_back(process);
        sortHandleProcesses();
    }

    void clearProcess() {
        waitProcesses.clear();
        handleProcesses.clear();
    }

    void reset() {
        clearProcess();
        currentTime = dealingTime = 0;
    }

    double getCurrentTime() {
        return currentTime;
    }

    double getHandleTime() {
        return dealingTime;
    }

    vector<ProcessControlBlock> getWaitProcess() {
        return waitProcesses;
    }

    vector<ProcessControlBlock> getHandleProcess() {
        return handleProcesses;
    }

    vector<ProcessControlBlock> getCompleteProcess() {
        return completeProcesses;
    }

    void highestPriorityFirst() {
        printf("start highest priority first\n");
        settleHandleWeight("priority",1);
        sortWaitProcesses();
    }

    void firstComeFirstServe() {
        printf("start first come first serve\n");
        settleHandleWeight("arrivalTime",-1);
        sortWaitProcesses();
    }

    void shortestJobFirst() {
        printf("shortest first\n");
        settleHandleWeight("burstTime",-1);
        sortWaitProcesses();
    }

    void update(double time) override {
        updateHandleProcesses();
        runProcesses();

        currentTime += time;
        if(handleProcesses.size()) dealingTime += time;
    } 
}; 
