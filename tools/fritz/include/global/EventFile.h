#ifndef EVENT_FILE_H_
#define EVENT_FILE_H_

#include "FritzConfig.h"

//! \brief An event file with cross section information for the events in the file.
class EventFile {
public:

	inline double getCrossSection() {
       Global::print(name, " Returning cross section of "+ Global::doubleToStr(xsect)+ " fb");
	   return xsect;
	}
	inline double getCrossSectionErr() {
       Global::print(name, " Returning cross section error of "+ Global::doubleToStr(xsectErr)+ " fb");
	   return xsectErr;
	}
	
	//! the path of the event file
	std::string filepath;
	//! cross section of the events in the file
	double xsect;
	//! cross section error of the events in the file
	double xsectErr;
	//! name for printing in logiles
	std::string name;
	
};


EventFile setupEventFile(Properties props);

#endif // EVENT_FILE_H_
