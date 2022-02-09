#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

// helper debug function kindly given by the reviewer
#define printVariableNameAndValue(x) cout<<"The name of variable **"<<(#x)<<"** and the value of variable is => "<<x<<"\n"

#include <fstream>
#include <regex>
#include <string>
#include <iostream>
#include <unistd.h>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kProcStatFilename{"/proc/stat"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// helper names
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
const std::string filterMemTotalString("MemTotal:");
const std::string filterMemFreeString("MemFree:");
const std::string filterCpu("cpu");
const std::string filterUID("Uid:");
const std::string filterProcMem("VmRSS:"); 
const std::string filterMemAvailable("MemAvailable:");
const std::string filterBuffers("Buffers:");
const std::string filterCached("Cached:");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
float CpuUtilization(const int &pid);
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser


// helper functions to get a float or int value from a string line st. myKey myValue
bool getFloatValue(const std::string &line, const std::string &myKey, float &myValue);
bool getIntValue(const std::string &line, const std::string &myKey, int &myValue);
// helper function to read an specific key value
std::string readStringFromFile(const std::string myFile,const std::string myKey);
int readIntFromFile(const std::string myFile,const std::string myKey);
// helper to read a number from a file at location myInt
long readNumberAt(const std::string &myFile, const int &myInt);
// helper function to get a string  value from a string line st. myKey value
bool getStringValue(const std::string &line, const std::string &myKey, std::string& value);
// helper function to check string has numbers and is not empty and has no weird characters
long getLong(const std::string& myString);
#endif