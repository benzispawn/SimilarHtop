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
double Processor::Utilization() {
    std::string key, user, nice, system,
                idle, iowait, irq, softirq,
                steal, guest, guest_nice, line;
    std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);

    double cpu_use = 0.0;

    if (filestream.is_open()) {
        //int mico = 0;
        Processor::GotoLine(filestream, this->getPos());
        filestream >> key >> user >> nice >> system >> idle >>
            iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        double user_ = stod(user);
        double nice_ = stod(nice);
        double system_ = stod(system);
        double idle_ = stod(idle);
        double iowait_ = stod(iowait);
        double irq_ = stod(irq);
        double softirq_ = stod(softirq);
        double steal_ = stod(steal);
        // double guest_ = stof(guest);
        // double guest_nice_ = stof(guest_nice);

        double PrevIdle = this->getIdle() + this->getIowait();
        double Idle = idle_ + iowait_;


        double PrevNonIdle = this->getUser() + this->getNice() + this->getSystem() + this->getIrq() + this->getSoftirq() + this->getSteal();
        double NonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;

        double PrevTotal = PrevIdle + PrevNonIdle;
        double Total = Idle + NonIdle;

        double totald = Total - PrevTotal;
        double idled = Idle - PrevIdle;

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
double Processor::getUser() { return user_; }
double Processor::getNice() { return nice_; }
double Processor::getSystem() { return system_; }
double Processor::getIdle() { return idle_; }
double Processor::getIowait() { return iowait_; }
double Processor::getIrq() { return irq_; }
double Processor::getSoftirq() { return softirq_; }
double Processor::getSteal() { return steal_; }


void Processor::setUser(double u) { user_ = u; }
void Processor::setNice(double u) { nice_ = u; }
void Processor::setSystem(double u) { system_ = u; }
void Processor::setIdle(double u) { idle_ = u; }
void Processor::setIowait(double u) { iowait_ = u; }
void Processor::setIrq(double u) { irq_ = u; }
void Processor::setSoftirq(double u) { softirq_ = u; }
void Processor::setSteal(double u) { steal_ = u; }

std::ifstream& Processor::GotoLine(std::ifstream& file, int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}
