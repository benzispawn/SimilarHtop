#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  std::string getKey() const;
  int getPos() const;
  std::ifstream& GotoLine(std::ifstream& file, unsigned int num);
  void setUser(float u);
  void setNice(float u);
  void setSystem(float u);
  void setIdle(float u);
  void setIowait(float u);
  void setIrq(float u);
  void setSoftirq(float u);
  void setSteal(float u);

  float getUser();
  float getNice();
  float getSystem();
  float getIdle();
  float getIowait();
  float getIrq();
  float getSoftirq();
  float getSteal();


  Processor(int pos, std::string key) : pos_(pos), key_(key) {} 
 

  // TODO: Declare any necessary private members
 private:
    int pos_;
    std::string key_;
    float user_ = {0.0};
    float nice_ = {0.0};
    float system_ = {0.0};
    float idle_ = {0.0};
    float iowait_ = {0.0};
    float irq_ = {0.0};
    float softirq_ = {0.0};
    float steal_ = {0.0};
};

#endif