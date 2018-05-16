/*
 * Global.h
 *
 *  Created on: Feb 26, 2015
 *      Author: daschm
 */
#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "TStopwatch.h"

namespace Global {
    #define RESET   "\033[0m"
    #define BLACK   "\033[30m"      /* Black */
    #define RED     "\033[31m"      /* Red */
    #define GREEN   "\033[32m"      /* Green */
    #define YELLOW  "\033[33m"      /* Yellow */
    #define BLUE    "\033[34m"      /* Blue */
    #define MAGENTA "\033[35m"      /* Magenta */
    #define CYAN    "\033[36m"      /* Cyan */
    #define WHITE   "\033[37m"      /* White */
    #define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
    #define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
    #define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
    #define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
    #define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
    #define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
    #define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
    #define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

    #define CLEAR "\033[2J"  // clear screen escape code

    // The following are helper functions which are available to all handlers in the program
    void redirect_cout(std::string filename);
    void unredirect_cout();

    void print(std::string source, std::string message); // If mode is <= priority, print 'content' send from 'source'
    void warn(std::string source, std::string message);
    void abort(std::string source, std::string message); // aborts the run with error message
    extern bool quiet;
    extern std::ofstream* redirect_stream;
    extern std::streambuf* cout_buf;
    extern std::streambuf* cerr_buf;

    void checkIfFileExistsAndRemoveAfterQuery(std::string filename); // if filename exists, prompt if the user really wants to overwrite it. If so, file is deleted


    int strToInt(std::string s);
    double strToDouble(std::string s);
    std::string intToStr(int x);
    std::string doubleToStr(double x);
    std::string vectorBoolToStr(std::vector<bool> v);

    void printStopWatches();
    void bookStopwatch(std::string name);
    extern std::map<std::string, TStopwatch*> stopwatches;
    
    extern int randomSeed;
};

#endif /* GLOBAL_H */
