#include "processor.h"
#include "linux_parser.h"
#include "system.h"
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::string key, user, nice, system, 
                idle, iowait, irq, softirq, 
                steal, guest, guest_nice, line;
    std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);

    float cpu_use = 0.0;

    if (filestream.is_open()) {
        //int mico = 0;
        Processor::GotoLine(filestream, this->getPos());
        filestream >> key >> user >> nice >> system >> idle >> 
            iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        
        float user_ = stof(user);
        float nice_ = stof(nice);
        float system_ = stof(system);
        float idle_ = stof(idle);
        float iowait_ = stof(iowait);
        float irq_ = stof(irq);
        float softirq_ = stof(softirq);
        float steal_ = stof(steal);
        // float guest_ = stof(guest);
        // float guest_nice_ = stof(guest_nice);

        float PrevIdle = this->getIdle() + this->getIowait();
        float Idle = idle_ + iowait_;


        float PrevNonIdle = this->getUser() + this->getNice() + this->getSystem() + this->getIrq() + this->getSoftirq() + this->getSteal();
        float NonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;

        float PrevTotal = PrevIdle + PrevNonIdle;
        float Total = Idle + NonIdle;

        float totald = Total - PrevTotal;
        float idled = Idle - PrevIdle;

        cpu_use = (totald - idled)/totald;
        
        this->setUser(user_);
        this->setSystem(system_);
        this->setIdle(idle_);
        this->setNice(nice_);
        this->setIowait(iowait_);
        this->setSoftirq(softirq_);
        this->setSteal(steal_);
        this->setIrq(irq_);

    }
    return cpu_use; 
}

std::string Processor::getKey() const { return key_; }
int Processor::getPos() const { return pos_; }
float Processor::getUser() { return user_; }
float Processor::getNice() { return nice_; }
float Processor::getSystem() { return system_; }
float Processor::getIdle() { return idle_; }
float Processor::getIowait() { return iowait_; }
float Processor::getIrq() { return irq_; }
float Processor::getSoftirq() { return softirq_; }
float Processor::getSteal() { return steal_; }


void Processor::setUser(float u) { user_ = u; }
void Processor::setNice(float u) { nice_ = u; }
void Processor::setSystem(float u) { system_ = u; }
void Processor::setIdle(float u) { idle_ = u; }
void Processor::setIowait(float u) { iowait_ = u; }
void Processor::setIrq(float u) { irq_ = u; }
void Processor::setSoftirq(float u) { softirq_ = u; }
void Processor::setSteal(float u) { steal_ = u; }

std::ifstream& Processor::GotoLine(std::ifstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}