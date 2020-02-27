#include "EventFile.h"
#include "Global.h"

// Keys into the Properties of an EventFile section
static const std::string keyName = "name";
static const std::string keyFile = "file";
static const std::string keyXSect = "xsect";
static const std::string keyXSectErr = "xsecterr";
static const std::string keyXSectErrFactor = "xsecterrfactor";

// Warn the user if there are keys that don't belong into an EventFile section
// and are therefore ignored
static void unknownKeys(Properties& props) {
	std::vector<std::string> knownKeys;
	knownKeys.push_back(keyName);
	knownKeys.push_back(keyFile);
	knownKeys.push_back(keyXSect);
	knownKeys.push_back(keyXSectErr);
	knownKeys.push_back(keyXSectErrFactor);
	warnUnknownKeys(props, knownKeys, "EventFile", "Unknown key in EventFile section");
}

EventFile setupEventFile(Properties props) {
	unknownKeys(props);
	EventFile efile;
    efile.name = props["name"];
	efile.filepath = lookupRequired(
			props,
			keyFile,
			efile.name,
			"You need to give a path to the file."
			);
	efile.xsect = lookupRequiredDouble(
			props,
			keyXSect,
			efile.name,
			"You need to give the cross section of the events in the file"
			);
	std::pair<bool, double> pair;

	pair = maybeLookupDouble(props, keyXSectErr);
	bool haveXSectErr = pair.first;
	efile.xsectErr = pair.second;

	pair = maybeLookupDouble(props, keyXSectErrFactor);
	bool haveXSectErrFactor = pair.first;
	if (haveXSectErrFactor) {
		efile.xsectErr = efile.xsect*pair.second;
	}

	if (haveXSectErr && haveXSectErrFactor) {
		Global::abort(efile.name, "You need to give either a cross section error or a cross section error factor, not both.");
	}
	if (!haveXSectErr && !haveXSectErrFactor) {
		Global::abort(efile.name, "You need to give either a cross section error or a cross section error factor.");
	}
	return efile;
}
