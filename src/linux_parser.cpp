#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// helper function to get a float value from a string line st. myKey myValue
bool getFloatValue(const string &line, const string &myKey, float& myValue) {
  std::istringstream linestream(line);
  string value;
  string key;
  bool foundValue = false;
  while (linestream >> key >> value) {
    if (key == myKey) {
      myValue = stof(value);
      foundValue = true;
      // std::cout<<"memTotal:" << myValue <<std::endl;
      return foundValue;
    }
  }
  return foundValue;
}

// helper function to get a string  value from a string line st. myKey value
bool getStringValue(const string &line, const string &myKey, string& value) {
  std::istringstream linestream(line);
  string key;
  bool foundValue = false;
  while (linestream >> key >> value) {
    if (key == myKey) {
      foundValue = true;
      //std::cout<< myKey<<"  " << value <<std::endl;
      return true;
    }
  }
  return foundValue;
}

// helper function to get a float value from a string line st. myKey myValue
bool getIntValue(const string &line, const string &myKey, int& myValue) {
  
  string value;
  bool foundValue = getStringValue(line, myKey, value);
  myValue = stoi(value);
  //std::cout<< myKey<<"  " << value<< "  "<< myValue <<std::endl;
  return foundValue;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = 0, memFree = 0, memAvailable = 0, buffers = 0, cached = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  bool bMemTotal = false, bMemFree = false, bMemAvailable = false,
       bBuffers = false, bCached = false;

  // read the important data and store it     
  if (filestream.is_open()) {
    // std::cout<<"memory file is opened"<<std::endl;
    while (std::getline(filestream, line)) {
      if (getFloatValue(line, "MemTotal:", memTotal)) {
        bMemTotal = true;
      }

      if (getFloatValue(line, "MemFree:", memFree)) {
        bMemFree = true;
      }

      if (getFloatValue(line, "MemAvailable:", memAvailable)) {
        bMemAvailable = true;
      }
      if (getFloatValue(line, "Buffers:", buffers)) {
        bBuffers = true;
      }
      if (getFloatValue(line, "Cached:", cached)) {
        bCached = true;
      }
    }
  }

  // if data was not read in the file we call an error
  if (!bMemTotal)
    std::cerr << "Cached was not read from file "
              << kProcDirectory + kMeminfoFilename << std::endl;
  if (!bMemFree)
    std::cerr << "MemFree was not read from file "
              << kProcDirectory + kMeminfoFilename << std::endl;
  if (!bMemAvailable)
    std::cerr << "MemAvailable was not read from file "
              << kProcDirectory + kMeminfoFilename << std::endl;
  if (!bBuffers)
    std::cerr << "Buffers was not read from file "
              << kProcDirectory + kMeminfoFilename << std::endl;
  if (!bCached)
    std::cerr << "Cached was not read from file "
              << kProcDirectory + kMeminfoFilename << std::endl;

  // my formula
  float usage = (memTotal - memFree - buffers - cached) / (memTotal - memFree);
  return usage;
}

long readNumberAt(const std::string &myFile, const int &myInt)
{
  std::ifstream filestream(myFile);
  string line;
  string upTimeS;
  // read the important data and store it     
  if (filestream.is_open()) {
    //std::cout<<"uptime file is opened"<<std::endl;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=1;i<=myInt;i++)
      {
      linestream >> upTimeS;
      }
  }
  return stol(upTimeS);
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  return readNumberAt(kProcDirectory + kUptimeFilename, 1);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  // output[0] user: normal processes executing in user mode
  // output[1] nice: niced processes executing in user mode
  // output[2] system: processes executing in kernel mode
  // output[3] idle: idle time
  // output[4] iowait: waiting for I/O to complete
  // output[5] irq: servicing interrupts
  // output[6] softirq: servicing softirqs
  // output[7]  steal
  // output[8] guest
  // output[9]  guest_nice
  std::vector<std::string> output = LinuxParser::CpuUtilization();
  // formulas take from :
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long usertime = LinuxParser::ActiveJiffies();  // usertime - guest
  long nicetime = stol(output[1]) - stol(output[9]);  // nicetime - guestnice
  long idlealltime =  LinuxParser::IdleJiffies(); //stol(output[3]) + stol(output[4]);  // idletime + ioWait;
  long systemalltime = stol(output[2]) + stol(output[5]) +
                                stol(output[6]);  // systemtime + irq + softIrq;
  long virtalltime =  stol(output[8]) + stol(output[9]);  // guest + guestnice;
  long totaltime = usertime + nicetime + systemalltime + idlealltime +  stol(output[7]) + virtalltime;  // usertime + nicetime + systemalltime
                                          // + idlealltime + steal + virtalltime;

  return totaltime;
   }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid ) {
  std::string myFolder = to_string(pid);

  /* taken from magical website: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  #14 utime - CPU time spent in user code, measured in clock ticks
  #15 stime - CPU time spent in kernel code, measured in clock ticks
  #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
  #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
  #22 starttime - Time when the process started, measured in clock ticks
  */
  long utime = readNumberAt(kProcDirectory + myFolder+ kStatFilename,14);
  long stime = readNumberAt(kProcDirectory + myFolder+ kStatFilename,15);
  long cutime = readNumberAt(kProcDirectory + myFolder+ kStatFilename,16);
  long cstime = readNumberAt(kProcDirectory + myFolder+ kStatFilename,17);
  
  long total_time = utime + stime + cutime + cstime;
  
   return total_time; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  // usertime - guest
  vector<string>output =  LinuxParser::CpuUtilization();
  return stol(output[0]) - stol(output[8]); 
  }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  //output[3] idle: idle time
  // output[4] iowait: waiting for I/O to complete
  vector<string>myCPUU =  LinuxParser::CpuUtilization();
  return stol(myCPUU[3]) + stol(myCPUU[4]); 
  }

// cpu strings needed to compute utilization
float LinuxParser::CpuUtilization(const int &pid)
{
  std::string myFolder = to_string(pid);

  long uptime = UpTime();
  /* taken from magical website: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  #22 starttime - Time when the process started, measured in clock ticks
  */
  long starttime = readNumberAt(kProcDirectory + myFolder+ kStatFilename,22);
  long hertz = sysconf(_SC_CLK_TCK);

  long total_time = LinuxParser::ActiveJiffies(pid );
  float seconds = (float)uptime - ((float)starttime / (float)hertz);
  float cpu_usage = (((float)total_time / (float)hertz) / (float)seconds);
  return cpu_usage;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  
  vector<string> output;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line;
  string cpus;
  // read the important data and store it     
  if (filestream.is_open()) {
    //std::cout<<"cpu file is opened"<<std::endl;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpus;
    for (int i=0;i<10;i++)
    {
    linestream >> cpus;
    output.push_back(cpus);
    }
  }
  //output[0] user: normal processes executing in user mode
  //output[1] nice: niced processes executing in user mode
  //output[2] system: processes executing in kernel mode
  //output[3] idle: idle time
  //output[4] iowait: waiting for I/O to complete
  //output[5] irq: servicing interrupts
  //output[6] softirq: servicing softirqs
  return output; 
  }

// helper function to read an specific key value
string readStringFromFile(const string myFile,const string myKey)
{
  string myValue;
  bool isRead = false;
  string line;
  std::ifstream filestream(myFile);

  // read the important data and store it     
  if (filestream.is_open()) {
    // std::cout<<"memory file is opened"<<std::endl;
    while (std::getline(filestream, line)) {
      if (getStringValue(line, myKey, myValue)) {
        isRead = true;
        return myValue;
      }
    }
  }

  // if data was not read in the file we call an error
  if (!isRead)
    std::cerr << myKey <<" was not read from file "
              << myFile << std::endl;
  return myValue; 
}

// helper function to read an specific key value
int readIntFromFile(const string myFile,const string myKey)
{
  int myValue;
  bool isRead = false;
  string line;
  std::ifstream filestream(myFile);

  // read the important data and store it     
  if (filestream.is_open()) {
    //std::cout<<"memory file is opened"<<std::endl;
    while (std::getline(filestream, line)) {
      if (getIntValue(line, myKey, myValue)) {
        //std::cout<<myKey<<" "<<myValue<<std::endl;
        isRead = true;
        return myValue;
      }
    }
  }

  // if data was not read in the file we call an error
  if (!isRead)
    std::cerr << myKey <<" was not read from file "
              << myFile << std::endl;
  return myValue; 
}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return readIntFromFile(kProcStatFilename, "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return readIntFromFile(kProcStatFilename, "procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid ) { 
  std::string myFolder = to_string(pid);
  std::ifstream filestream(kProcDirectory +myFolder + kCmdlineFilename);
  string line;
  
  // read the important data and store it     
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line; 
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid ) 
{ 
  std::string myFolder = to_string(pid);
  // its on kb (0.0009765625 Mb)
  return readStringFromFile(kProcDirectory + myFolder + kStatusFilename, "VmSize:"); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid ) { 
  std::string myFolder = to_string(pid);  
  return readStringFromFile(kProcDirectory + myFolder + kStatusFilename,"Uid:");
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string myUid = LinuxParser::Uid(pid );
  string line;
  string key;
  string value;
  string dummy;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> dummy >> value) {
        if (value == myUid) {
          return key;
        }
      }
    }
  }
  return key;
  }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid ) 
{ 
  std::string myFolder = to_string(pid);
  // already in seconds
  return readNumberAt(kProcDirectory + myFolder + kStatFilename, 22)/sysconf(_SC_CLK_TCK);
  }
