#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    Process myProcess;
    processes_.clear();
    vector<int> myPIDs = LinuxParser::Pids();
    for (auto i=myPIDs.begin();i!=myPIDs.end();i++)
    {
        myProcess.setPid(*i);
        processes_.emplace_back(myProcess);
    }

    // sort with cpu usage
    sort(processes_.begin(),processes_.end(), greaterThan);
   

    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }
