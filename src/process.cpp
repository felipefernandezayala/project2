#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return mypid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    return LinuxParser::CpuUtilization(mypid); 
    }

//Return the command that generated this process
string Process::Command() { return LinuxParser::Command(mypid ); }

//Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(mypid ) ; }

//Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(mypid); }

//Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(mypid ) ; }

// Overload the "less than" comparison operator for Process objects
// ordered by cpu usage
bool Process::operator<(Process const& a) const { 
    float otherCpu = LinuxParser::CpuUtilization(a.mypid);
    float cpu = LinuxParser::CpuUtilization(this->mypid);
    return cpu<otherCpu; 
    }

// operator for sorting
bool greaterThan(const Process &a, const Process &b)
{
    return b<a;
}