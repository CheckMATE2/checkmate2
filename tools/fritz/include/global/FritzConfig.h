#ifndef FRITZ_CONFIG_H_
#define FRITZ_CONFIG_H_

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Global.h"

//! Known section types
const std::string keyGlobalSection = "global";
const std::string keyBTagSection = "btag";
const std::string keyTauTagSection = "tautag";
const std::string keyElectronIsoSection = "electroniso";
const std::string keyMuonIsoSection = "muoniso";
const std::string keyPhotonIsoSection = "photoniso";
const std::string keyAnalysisSection = "analysis";
const std::string keyEventFileSection = "eventfile";
const std::string keyPythiaHandlerSection = "pythiahandler";
const std::string keyDelphesHandlerSection = "delpheshandler";
const std::string keyAnalysisHandlerSection = "analysishandler";

//! A map for each section from key to value
typedef std::map<std::string, std::string> Properties;

//! A map for each section type from label to section
typedef std::map<std::string, Properties> Sections;

//! A map for the whole configuration from section type to sections of that type
typedef std::map<std::string, Sections> Config;

//! \brief Test if a key is set.
//!
//! \param p Properties to search for the key
//! \param key the key to look for
//! \returns if key is in p
template <typename T>
bool hasKey(std::map<std::string,T> p, std::string key) {
	return p.find(key) != p.end();
}


//! \brief Print a warning if there are unknown keys.
//!
//! Print a warning if there are keys in m that are not in keys.
//! \param m the map to check for unknown keys
//! \param keys a vector of known keys
//! \param location string describing where the warning comes from
//! \param msg warning message to print in case of unknown keys. The name of the unknown key is appended to msg.
template <typename T>
void warnUnknownKeys(std::map<std::string, T> m, std::vector<std::string> keys, std::string location, std::string msg) {
	typename std::map<std::string,T>::iterator it;
	for (it=m.begin(); it!=m.end(); it++) {
		if (std::find(keys.begin(), keys.end(), it->first) == keys.end()) {
			Global::warn(
					location,
					msg+": '"+it->first+"'"
					);
		}
	}
}

//! \brief Test if a key is set and if so return its value.
//!
//! \param p Properties to search for the key
//! \param key the key to search
//! \param def optional default value
//! \returns a pair, where the first element says if the key was found and the second
//!          argument is the value of the key or the default value
std::pair<bool,std::string> maybeLookup(Properties p, std::string key, std::string def="");

//! \brief Test if a key is set and if so return its value as double.
//!
//! If the key exists its value is converted to double.
//! \param p Properties to search for the key
//! \param key the key to search
//! \param def optional default value
//! \returns a pair, where the first element says if the key was found and the second
//!          argument is the value of the key or the default value
std::pair<bool,double> maybeLookupDouble(Properties p, std::string key, double def=0.0);

//! Test \brief if a key is set and if so return its value as int .
//!
//! If the key exists its value is converted to int.
//! \param p Properties to search for the key
//! \param key the key to search
//! \param def optional default value
//! \returns a pair, where the first element says if the key was found and the second
//!          argument is the value of the key or the default value
std::pair<bool,int> maybeLookupInt(Properties p, std::string key, int def=0);

//! \brief Lookup a key with default string value.
//!
//! Return the value of the key if it exists or the default value otherwise
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param def the default value to return if the key was not found
//! \returns the value associated with key if it exists and def if the key does not exist
std::string lookupOrDefault(Properties p, std::string key, std::string def);

//! \brief Lookup a key with default double value.
//!
//! Return the value of the key if it exists or the default value otherwise.
//! If the key exists its value is converted to double.
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param def the default value to return if the key was not found
//! \returns the value associated with key if it exists and def if the key does not exist
double lookupOrDefault(Properties p, std::string key, double def);

//! \brief Lookup a key with default int value.
//!
//! Return the value of the key if it exists or the default value otherwise.
//! If the key exists its value is converted to int.
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param def the default value to return if the key was not found
//! \returns the value associated with key if it exists and def if the key does not exist
int lookupOrDefault(Properties p, std::string key, int def);

std::vector<std::string> lookupList(Properties p, std::string key);

//! \brief Lookup a required key.
//!
//! Aborts if the key was not found.
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param location where the error took place, for the message
//! \param errorMsg the error message to print
//! \returns the value associated with key in p
template<typename T>
T lookupRequired(std::map<std::string, T> map, std::string key, std::string location, std::string errorMsg) {
	typename std::map<std::string, T>::iterator it;
	it = map.find(key);
	if (it != map.end()) {
		return it->second;
	} else {
		Global::abort(location, errorMsg);
	}
}

//! \brief Lookup a required key with double value.
//!
//! The value is converted to double.
//! Aborts if the key was not found.
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param location where the error took place, for the message
//! \param errorMsg the error message to print
//! \returns the value associated with key in p
double lookupRequiredDouble(Properties p, std::string key, std::string location, std::string errorMsg);

//! \brief Lookup a required key with int value.
//!
//! The value is converted to int.
//! Aborts if the key was not found.
//! \param p Properties to search for the key
//! \param key the key to look for
//! \param location where the error took place, for the message
//! \param errorMsg the error message to print
//! \returns the value associated with key in p
int lookupRequiredInt(Properties p, std::string key, std::string location, std::string errorMsg);

#endif // FRITZ_CONFIG_H_
