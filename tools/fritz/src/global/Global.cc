/*
 * Global.cc
 *
 *  Created on: Feb 26, 2015
 *      Author: daschm
 */

#include "Global.h"


namespace Global {
std::map<std::string, TStopwatch*> stopwatches =
        std::map<std::string, TStopwatch*> ();
bool quiet = false;
int randomSeed = 0;
std::ofstream* redirect_stream = NULL;
std::streambuf* cout_buf = std::cout.rdbuf();
std::streambuf* cerr_buf = std::cerr.rdbuf();

void redirect_cout(std::string filename) {
    Global::redirect_stream = new std::ofstream(filename.c_str(), std::ios::out | std::ios::app);
    std::cout.rdbuf(Global::redirect_stream->rdbuf());
    std::cerr.rdbuf(Global::redirect_stream->rdbuf());
}

void unredirect_cout() {
    delete Global::redirect_stream;
    Global::redirect_stream = NULL;
    std::cout.rdbuf(Global::cout_buf);
    std::cerr.rdbuf(Global::cerr_buf);
}


void print(std::string source, std::string message) {
    if (!quiet) {
        if (Global::redirect_stream == NULL) // TO STDOUT = TO PYTHON, we need to put |~| to filter our output from internal (e.. MG5) output)
            std::cout << "|~| " << source << ": " << message << std::endl;
        else
            std::cout << source << ": " << message << std::endl;
    }
}

void warn(std::string source, std::string message) {
    std::cerr << YELLOW << source << ": " << message << RESET << std::endl;
}


void abort(std::string source, std::string message) {
    unredirect_cout();
    std::cerr << std::endl;
    std::cerr << RED << "ERROR IN FRITZ - MODULE: " << source << std::endl;
    std::cerr << RED << "                MESSAGE: " << message << std::endl;
    std::cerr << RED << "-----> IMMEDIATE ABORT!" << RESET << std::endl;
    exit(1);
}

void checkIfFileExistsAndRemoveAfterQuery(std::string filename) {
    std::ifstream infile(filename.c_str());
    if(infile.good()) {
        std::cout << "CAUTION: File " << filename << " exists. " << std::endl;
        std::cout << "  - enter 'o' to overwrite " << std::endl;
        std::cout << "  - enter any other key to quit" << std::endl;
        if (std::cin.get() != 'o')
            exit(1);
        remove(filename.c_str());
    }
}

double strToDouble(std::string s) {
    double x;
    std::istringstream buffer(s);
    buffer >> x;
    return x;
}

int strToInt(std::string s) {
    int x;
    std::istringstream buffer(s);
    buffer >> x;
    return x;
}

std::string intToStr(int x) {
    std::stringstream buffer;
    buffer << x;
    return buffer.str();
}

std::string doubleToStr(double x) {
    std::stringstream buffer;
    buffer << x;
    return buffer.str();
}

std::string vectorBoolToStr(std::vector<bool> v) {
    std::string out = "";
    for(int i = 0; i < v.size(); i++) {
        if(v[i])
            out = out+" T";
        else
            out = out+" F";
    }
    return out;
}


void bookStopwatch(std::string name) {
    TStopwatch* newwatch = new TStopwatch();
    Global::stopwatches[name] = newwatch;
    std::cout << "booked " << name << std::endl;
}

void printStopWatches() {
    typedef std::map<std::string, TStopwatch*>::iterator it_type;
    for(it_type iterator = Global::stopwatches.begin(); iterator != Global::stopwatches.end(); iterator++) {
        std::cout << "Watch " << iterator->first << " shows " << (iterator->second)->RealTime() << " = " << 100*(iterator->second)->RealTime()/(Global::stopwatches["total"]->RealTime()) << std::endl;
    }
    for(it_type iterator = Global::stopwatches.begin(); iterator != Global::stopwatches.end(); iterator++) {
        delete iterator->second;
    }
}
}
