#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

double getCPUUSage(){
    static long prevIdle=0, prevTotal=0;

    ifstream file("/proc/stat");
    string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    file>>cpu>>user>>nice>>system>>iowait>>irq>>softirq>>steal;

    long idleTime = idle + iowait;

    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    long totalDiff= totalTime-prevTotal;
    long idleDiff= idleTime - prevIdle;

    prevTotal=totalTime;
    prevIdle= idleTime;

   return (1.0-(double)idleDiff/totalDiff)*100.0;
   
   if(totalDiff==0) return 0.0;
}

double getMemoryUsage(){
    ifstream file("/proc/meminfo");
    string key;
    long total=0, free=0, buffers =0, cached=0;

    while(file>>key){
        if(key=="MemTotal:") file>>total;
        else if(key=="MemFree:") file>>free;
        else if(key=="Buffers:") file>>buffers;
        else if(key=="Cached:") file>>cached;
    }

    long used = total - free - buffers - cached;

    return (double)used / total*100.0;
}

void clearScreen(){
    system("cls");
}

int main(){
    while(1){
        clearScreen();
        double cpu= getCPUUSage();
        double mem = getMemoryUsage();
        cout << "=== Unix System Monitor ===\n\n";
        cout << "CPU Usage: " << cpu << " %\n";
        cout << "Memory Usage: " << mem << " %\n";

        this_thread::sleep_for(chrono::seconds(2));
    }
    return 0;
}