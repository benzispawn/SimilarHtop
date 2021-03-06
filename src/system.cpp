#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

//namespace lp = LinuxParser;


using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
//Processor& System::Cpu() { return cpu_; }

std::vector<Processor>& System::Cpus() {
    std::string key, user, nice, system, 
                idle, iowait, irq, softirq, 
                steal, guest, guest_nice, line;
    std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
    if (filestream.is_open() && this->cpus_.empty()) {
        int pos = 1;
        while(getline(filestream, line)) {
            std::istringstream linestream(line);
            while(linestream >> key >> user >> nice >> system >> idle >> 
                  iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
                if (key.size() == 4 && key.substr(0, 3) == "cpu") {
                    Processor cpu(pos, key);
                    this->cpus_.emplace_back(cpu);
                    pos++;
                }
            }
        }
        //return this->cpus_;
    }

    return this->cpus_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    this->processes_.clear();
    std::vector<int> pids = LinuxParser::Pids();

    for (int pid : pids) {
        // std::cout << pid << std::endl;
        this->processes_.emplace_back(pid);
    }    

    std::sort(processes_.begin(), processes_.end());

    return this->processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    string kernel = LinuxParser::Kernel();
    return kernel; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    float mem = LinuxParser::MemoryUtilization();
    return mem;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    string OS = LinuxParser::OperatingSystem();
    return OS; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    int runningProcesses_ = LinuxParser::RunningProcesses();
    return runningProcesses_; 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    int totalProcesses_ = LinuxParser::TotalProcesses(); 
    return totalProcesses_; 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 

    long time = LinuxParser::UpTime();

    return time; 
}