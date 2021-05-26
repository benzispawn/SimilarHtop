#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    this->cpu_ = (float)LinuxParser::ActiveJiffies(pid_);
    return this->cpu_; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    std::string command = LinuxParser::Command(pid_);
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    std::string mem = LinuxParser::Ram(pid_);
    return mem; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    std::string user = LinuxParser::User(pid_);
    return user; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long time = LinuxParser::UpTime(pid_);
    return time / sysconf(_SC_CLK_TCK); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    int pidA = a.pid_;
    return pid_ < pidA ? false : true;
}