#ifndef ProcessInfoExport
#define ProcessInfoExport

#include <iostream>

using namespace std;

struct ProcessInfo
{
    string name;
    int id;
    double priority;
    double arrivalTime;
    double burstTime;
    double usedCpuTime;

    ProcessInfo(string n,int id,double at,double p,double bt,double ut) 
        :name(n),id(id),priority(p),arrivalTime(at),burstTime(bt),usedCpuTime(ut) {}
};

#endif