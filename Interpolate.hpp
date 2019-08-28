#ifndef CS540_INTERPOLATE_HPP
#define CS540_INTERPOLATE_HPP

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <typeinfo>
#include <sstream>
using namespace std;

namespace cs540{

class WrongNumberOfArgs: public std::exception{
    const char* what() const throw() {
        return "Error: indices provided are out of range for this array.";
    }
};

//Interpolate takes arguments of different types
std::string ffr(std::ostream& (*funcPtr)(std::ostream&)){
    std::stringstream temp;
    std::string tempStr;
    temp << funcPtr;
    tempStr = temp.str();
    return tempStr;

}

template <typename T1>
bool isManipulator(T1& arg1){
    std::stringstream tempSS;
    tempSS << arg1;
    std::string tempStr = tempSS.str();

    if(typeid(T1) == typeid(std::ios_base&(*)(std::ios_base&)) || (tempStr.empty())){
        //if it is ios_base then it is a manipulator,
        //also if pushing it to the stream makes the string empty
        return true;
    } else{
        return false;
    }
}

std::string HelpInterpolate(const std::string& inputStr){
    std::string holderString;
    int argPos = 0;
    int nextSubstrIndex = 0;
    bool stillLooking = true;

    while(stillLooking){
        argPos = inputStr.find("%", nextSubstrIndex);
        nextSubstrIndex = argPos+1;
        //if no matches find returns -1
        if(argPos == -1){
            stillLooking = false;
        } else if (argPos == 0){
            throw WrongNumberOfArgs();
        }
        if(stillLooking && inputStr.at(argPos-1) != '\\') { throw WrongNumberOfArgs(); }
    }
    if(!stillLooking){
        holderString += inputStr;
        stillLooking = true;
        argPos = 0;
        nextSubstrIndex = 0;
        while(stillLooking){
            argPos = holderString.find("\\%", nextSubstrIndex);
            if(argPos != -1){
                //need to get rid of \\ and %
                holderString.replace(argPos, 2, "%");
                nextSubstrIndex = argPos;
            } else{
                stillLooking = false;
            }
        }
    }
    return holderString;
}

template <typename T, typename... Types>
std::string HelpInterpolate(const std::string& inputStr, T& arg1, Types&... args){
    std::stringstream tempSS;
    //std::string tempString;
    std::string holderString;
    int argPos = 0;
    int nextArgPos = 0;
    int nextSubstrIndex = 0;
    int nextSubstrIndex2 = 0;
    bool stillLooking = true;
    bool isMan = isManipulator(arg1);
    if(typeid(ffr) == typeid(T)){
        tempSS << arg1;
        //pase other args
        tempSS << HelperInterpolate(inputStr, args...);
        return tempSS.str();
    }

    while(stillLooking){
        argPos = inputStr.find("%", nextSubstrIndex);
        //if no matches find returns -1
        if(argPos == -1){
            if(isMan){
                //send manipulator to the stream
                tempSS << inputStr << arg1;
                //parse other args
                tempSS << HelperInterpolate(inputStr, args...);
                return tempSS.str();
            } else{
                throw WrongNumberOfArgs();
            }
        }
        nextSubstrIndex = argPos+1;
        if(inputStr.at(argPos-1) != '\\') stillLooking = false;
    }

    if(!stillLooking){
        holderString += inputStr.substr(0, argPos);
        stillLooking = true;
        while(stillLooking){
            //look for any we missed
            nextArgPos = holderString.find("\\%", nextSubstrIndex2);
            if(nextArgPos != -1){
                holderString.replace(nextArgPos, 2, "%");
                nextSubstrIndex2 = nextArgPos;
            } else{
                stillLooking = false;
            }
        }
        tempSS << holderString;
        if(isMan){
            tempSS << arg1;
            tempSS << HelperInterpolate(inputStr.substr(argPos), args...);
            return tempSS.str();
        } else{
            tempSS << arg1;
            tempSS << HelperInterpolate(inputStr.substr(nextSubstrIndex), args...);
        }
    }
}


template <typename... Types>
std::string Interpolate(const std::string& inputStr, const Types&... args){
    std::stringstream tempSS;
    // va_list v1;
    // va_start(v1, args);
    // auto arg1 = va_arg(v1, auto);
    tempSS << "" << HelpInterpolate(inputStr, args...);
    return tempSS.str();

}


}

#endif
