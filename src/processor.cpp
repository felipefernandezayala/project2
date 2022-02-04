#include "processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  
  // formulas take from :
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long idlealltime =  LinuxParser::IdleJiffies(); //stol(output[3]) + stol(output[4]);  // idletime + ioWait;
  long totaltime = LinuxParser::Jiffies();  // usertime + nicetime + systemalltime
                                          // + idlealltime + steal + virtalltime;

  float cpu = (float)(totaltime - idlealltime) / (float)totaltime;
  return cpu;
}