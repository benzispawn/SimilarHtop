#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  double Utilization();  // TODO: See src/processor.cpp
  std::string getKey() const;
  int getPos() const;
  std::ifstream& GotoLine(std::ifstream& file, int num);
  void setUser(double u);
  void setNice(double u);
  void setSystem(double u);
  void setIdle(double u);
  void setIowait(double u);
  void setIrq(double u);
  void setSoftirq(double u);
  void setSteal(double u);

  double getUser();
  double getNice();
  double getSystem();
  double getIdle();
  double getIowait();
  double getIrq();
  double getSoftirq();
  double getSteal();


  Processor(int pos, std::string key) : pos_(pos), key_(key) {}


  // TODO: Declare any necessary private members
 private:
    int pos_;
    std::string key_;
    double user_ = {0.0};
    double nice_ = {0.0};
    double system_ = {0.0};
    double idle_ = {0.0};
    double iowait_ = {0.0};
    double irq_ = {0.0};
    double softirq_ = {0.0};
    double steal_ = {0.0};
};

#endif
