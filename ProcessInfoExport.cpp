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
    double completeTime;
    string blockColor;

    ProcessInfo(string n,int id,double at,double p,double bt,double ut,double ct,string c) 
        :name(n),id(id),priority(p),arrivalTime(at),burstTime(bt),usedCpuTime(ut),completeTime(ct),blockColor(c) {}
};

#endif