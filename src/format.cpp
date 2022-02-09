#include <string>

#include "format.h"

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long hours = seconds / 3600;
    seconds = seconds % 3600;
    long minutes = seconds / 60;
    seconds = seconds % 60;

    std::stringstream myStringS{};

    myStringS << std::setw(2) << std::setfill('0') << hours     
     << std::setw(1) << ":"                                    
     << std::setw(2) << std::setfill('0') << minutes   
     << std::setw(1) << ":"                                    
     << std::setw(2) << std::setfill('0') << seconds;

    return myStringS.str();   
}