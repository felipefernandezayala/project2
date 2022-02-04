#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include<linux_parser.h>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(){}
  Process(const int &input){mypid=input;}
  void setPid(const int &input){mypid=input;}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // Declare any necessary private members
 private:
    int mypid;
};


// operator for sorting
bool greaterThan(const Process &a, const Process &b);

#endif