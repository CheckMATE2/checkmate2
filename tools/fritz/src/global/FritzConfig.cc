#include "FritzConfig.h"

#include "Global.h"

#include <sstream>

std::pair<bool,std::string> maybeLookup(Properties p, std::string key, std::string def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return std::make_pair(true, it->second);
	} else {
		return std::make_pair(false, def);
	}
}

std::pair<bool,double> maybeLookupDouble(Properties p, std::string key, double def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return std::make_pair(true, Global::strToDouble(it->second));
	} else {
		return std::make_pair(false, def);
	}
}

std::pair<bool,int> maybeLookupInt(Properties p, std::string key, int def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return std::make_pair(true, Global::strToInt(it->second));
	} else {
		return std::make_pair(false, def);
	}
}

std::string lookupOrDefault(Properties p, std::string key, std::string def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return it->second;
	} else {
		return def;
	}
}

double lookupOrDefault(Properties p, std::string key, double def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return Global::strToDouble(it->second);
	} else {
		return def;
	}
}

int lookupOrDefault(Properties p, std::string key, int def) {
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it != p.end()) {
		return Global::strToInt(it->second);
	} else {
		return def;
	}
}

std::vector<std::string> lookupList(Properties p, std::string key) {
	std::vector<std::string> list;
	std::map<std::string, std::string>::iterator it;
	it = p.find(key);
	if (it == p.end()) {
		return list;
	}
	std::stringstream rawList(it->second);
	std::string tmp;
	while(rawList >> tmp) {
		list.push_back(tmp);
	}
	return list;
}

double lookupRequiredDouble(
		Properties p,
		std::string key,
		std::string location,
		std::string errorMsg
		) {
	std::string value = lookupRequired<std::string> (p, key, location, errorMsg);
	return Global::strToDouble(value);
}

int lookupRequiredInt(
		Properties p,
		std::string key,
		std::string location,
		std::string errorMsg
		) {
	std::string value = lookupRequired<std::string>(p, key, location, errorMsg);
	return Global::strToInt(value);
}
