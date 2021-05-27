#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

#include <filesystem>

#include "linux_parser.h"
// #include "format.h"

// using std::stof;
// using std::string;
// using std::to_string;
// using std::vector;

namespace fs = std::filesystem;
// TRANSFORM TO A STRING USING STREAM
template <typename T> std::string tostr(const T& t) { 
   std::ostringstream os; 
   os<<t; 
   return os.str(); 
} 

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          linestream.str("");
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::Kernel() {
  std::string os, kernel, version;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  for (const auto & entry : fs::directory_iterator(kProcDirectory.c_str())) {
      if (entry.is_directory()) {
          //std::cout << entry.path().filename().string() << std::endl;
          std::string filename = entry.path().filename().string();
          // std::cout << filename << std::endl;
          if (isdigit(filename[0])) {
              int pid = stoi(filename);
              // std::cout << pid << std::endl;
              pids.emplace_back(pid);
          }
      }
  }

  return pids;


  // vector<int> pids;
  // DIR* directory = opendir(kProcDirectory.c_str());
  // struct dirent* file;
  // while ((file = readdir(directory)) != nullptr) {
  //   // Is this a directory?
  //   if (file->d_type == DT_DIR) {
  //     // Is every character of the name a digit?
  //     string filename(file->d_name);
  //     if (std::all_of(filename.begin(), filename.end(), isdigit)) {
  //       int pid = stoi(filename);
  //       pids.push_back(pid);
  //     }
  //   }
  // }
  // closedir(directory);
  // return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  std::string key, memValue, measure, line;
  float memTotal = 100.0;
  float memFree = 0.0;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
      //std::cout << "Passou aqui no if" << std::endl;
      while (std::getline(filestream, line)) {
          std::replace(line.begin(), line.end(), ':', ' ');
          // std::cout << line << std::endl;
          std::istringstream linestream(line);
          while (linestream >> key >> memValue >> measure) {
              //std::cout << key << std::endl;
              if (key == "MemTotal") {
                  memTotal = std::stof(memValue);
              }

              if (key == "MemFree") {
                  memFree = std::stof(memValue);
                  filestream.close();
                  linestream.str("");
                  break;
              }
          }
      }
  }
  //std::cout << ((memTotal - memAvail) / memTotal) * 100 << std::endl;
  //return ((memTotal - memFree) / memTotal) * 100;  
  return ((memTotal - memFree) / memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string value1, value2, line;
  long time1 = 0;
  //long time2 = 0;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> value1 >> value2) {
        time1 = std::stol(value1);
        filestream.close();
        linestream.str("");
      }
    }
  }
  // std::cout << time1 << std::endl;
  return time1;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();  
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
double LinuxParser::ActiveJiffies(int pid) { 
  std::string line, key;
  double cpu_usage = 0.00;
  double utime, stime, cutime, cstime, starttime;
  double uptime = LinuxParser::UpTime();
  std::vector<std::string> res = {};
  std::ifstream filestream(LinuxParser::kProcDirectory+std::to_string(pid)+LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
        res.emplace_back(line);
    }
  }

  filestream.close();

  utime = std::stod(res[14]);
  stime = std::stod(res[15]);
  cutime = std::stod(res[16]);
  cstime = std::stod(res[17]);
  starttime = std::stod(res[22]);

  double total_time = utime + stime + cutime + cstime;

  double seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));

  cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);

  return cpu_usage; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::string line, key, kUser_, kNice_, kSystem_, kIdle_, 
              kIOwait_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_, kGuestNice_;
  long active = 0;
  std::vector<LinuxParser::CPUStates> cpuStates {};

  std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      if (active != 0) {
        break;
      }
      std::istringstream linestream(line);
      while (linestream >> key >> kUser_ >> kNice_ >> kSystem_ >> kIdle_ >> 
                           kIOwait_ >> kIRQ_ >> kSoftIRQ_ >> kSteal_ >> kGuest_ >> kGuestNice_) 
      {
        if (key == "cpu") {
          active = std::stoll(kUser_) + std::stoll(kNice_) + std::stoll(kSystem_) + std::stoll(kIRQ_) + std::stoll(kSoftIRQ_) + std::stoll(kSteal_);
          break;
        }
      }
    }
  }

  return active; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::string line, key, kUser_, kNice_, kSystem_, kIdle_, 
              kIOwait_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_, kGuestNice_;
  long idles = 0;
  std::vector<LinuxParser::CPUStates> cpuStates {};
  std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      if (idles != 0) {
        break;
      }
      std::istringstream linestream(line);
      while (linestream >> key >> kUser_ >> kNice_ >> kSystem_ >> kIdle_ >> 
                           kIOwait_ >> kIRQ_ >> kSoftIRQ_ >> kSteal_ >> kGuest_ >> kGuestNice_) 
      {
        if (key == "cpu") {
          idles = std::stoll(kIdle_) + std::stoll(kIOwait_);
          break;
        }
      }
    }
  }

  return idles; 
}

// TODO: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string key, value, line;
  int processes_ = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      while(getline(filestream, line)) {
          std::istringstream linestream(line);
          while(linestream >> key >> value) {
              if (key == "processes") {
                  processes_ = stoi(value);
                  filestream.close();
                  linestream.str("");
                  break;
              }
          }
      }
  }
  // std::cout << processes_ << std::endl;
  return processes_;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string key, value, line;
  //std::vector<std::istringstream * > vec_linestream;
  int proc_running_ = 0;
  // int pos = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      while(getline(filestream, line)) {
          std::istringstream linestream(line);
          while(linestream >> key >> value) {
              if (key == "procs_running") {
                  proc_running_ = stoi(value);
                  filestream.close();
                  linestream.str("");
                  break;
              }
          }
      }
  }
  // std::cout << processes_ << std::endl;
  return proc_running_;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Command(int pid) { 
  std::string line;
  std::string command = "";
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      command = line;
      if (command.length() > 40) {
        command = command.substr(0, 40) + "...";
      }
      filestream.close();
      break;
    }
  }

  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Ram(int pid) { 
  std::string line, vmSize, key;
  std::string mem = "0";
  int memParam = 0;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      // EXIT loop
      // if (mem.length() > 0) {
      //   break;
      // }
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> vmSize) {
        if (key == "VmSize") {
          //memParam = stod(vmSize) / 1000;
          memParam = stoi(vmSize.substr(0,vmSize.length() - 3));
          //float rounded = pow(10.0f, 3.0);std::to_string(round(rounded * memParam) / rounded)
          mem = tostr(memParam);
          filestream.close();
          linestream.str("");
          break;
        }
      }
    }
  }
  // std::cout << userId << std::endl;
  return mem; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Uid(int pid) { 
  std::string line, uid, key;
  std::string userId = "";
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      // EXIT loop
      if (userId.length() > 0) {
        filestream.close();
        break;
      }
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid") {
          userId = uid;
          filestream.close();
          linestream.str("");
          break;
        }
      }
    }
  }
  // std::cout << userId << std::endl;
  return userId; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::User(int pid) { 
  std::string userId = LinuxParser::Uid(pid);
  std::string pwLine, pwName, pwUid, x_;
  std::string name = "";

  std::ifstream passwdstream(kPasswordPath);
  if (passwdstream.is_open()) {
    while(getline(passwdstream, pwLine)) {
      if (name.length() > 0) {
        break;
      }
      std::replace(pwLine.begin(), pwLine.end(), ':', ' ');
      std::istringstream pwlinestream(pwLine);
      while (pwlinestream >> pwName >> x_ >> pwUid) {
        if (pwUid == userId) {
          name = pwName;
          passwdstream.close();
          pwlinestream.str("");
          break;
        }
      }
    }
  }

  return name; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string time, line;
  long time1 = 0;
  int counter = 0;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
      while(getline(filestream, line, ' ')) {
          counter++;
          // std::cout << line << std::endl;
          if (counter == 22) {
              time1 = stol(line) / sysconf(_SC_CLK_TCK);
              filestream.close();
              break;
          }
          
      }
  }
  // std::cout << time1 << std::endl;
  return time1;
}
