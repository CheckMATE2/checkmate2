#include "ConfigParser.h"

#include "Global.h"

#include <algorithm>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>

Parser::Parser(std::istream &file) : lineNumber(0), pushedBack(false) , file(file) {
}

Status Parser::nextLine() {
	if (file.eof() && !pushedBack) {
		return Eof;
	}
        while(true) {
		if (pushedBack) {
			// A line has been pushed back.
			// No need to read a line from the file.
			pushedBack = false;
		} else {
			std::getline(file, line);
			if (file.eof()) {
				// If there is no newline at the end of the
				// file, then eof is reported even if
				// something was read. In that case the length
				// of the line that was read is not 0.
				if (line.length() == 0) {
					return Eof;
				}
			}
			if (file.fail()) {
				throw "Error reading from file";
			}
			lineNumber++;
		}
		index=0;
		len = line.length();
		size_t start = line.find_first_not_of(" \t", index);
		if (start == std::string::npos) {
			// No non space character found
			continue;
		}
                if (line[start] == ';' || line[start] == '#') {
                        // First non space character is a comment
                        continue;
                }
                break;
	}
	return Ok;
}

void Parser::pushback() {
	if (lineNumber == 0) {
		throw "BUG: pushback called before reading anything";
	}
	index = 0;
	pushedBack = true;
}

void Parser::skipWhitespace() {
	size_t next = line.find_first_not_of(" \t", index);
	if (next!=std::string::npos) {
		index = next;
	} else {
		// No non white space characters left.
		index = len;
	}
}

void Parser::skipToEOL() {
	skipWhitespace();
	if (haveNext()) {
		// Non whitespace or comment character encountered.
		std::stringstream msg;
		errorLocation(msg);
		msg << "Unexpected character '"
			<< line[index]
			<< "', EOL or comment expected";
		throw msg.str();
	}
}

void Parser::parseIdentifier(std::string& ident, bool header, bool toLower) {
	std::string end_chars = " \t:=";
	if (header) {
		end_chars = " \t]:=";
	}
	size_t end = line.find_first_of(end_chars, index);
	if (end == std::string::npos) {
		// No non identifier character left
		// => the identifier goes to the end of the line
		end = len;
		if (index == end) {
			// We are already at the end of the line
			// => no identifier to read
			std::stringstream msg;
			errorLocation(msg);
			msg << "Unexpected end of line, Identifier expected.";
			throw msg.str();
		}
	}
	if (index == end) {
		// Next character is not allowed in identifier
		// => Identifier has lenght 0
		std::stringstream msg;
		errorLocation(msg);
		msg << "Identifiers of length 0 are not allowed. (Unexpected character '"
			<< line[index]
			<< "', Identifier expected)";
		throw msg.str();
	}
	ident = line.substr(index, end-index);
	if (toLower)
	    std::transform(ident.begin(), ident.end(), ident.begin(), tolower);
	index = end;
}

void Parser::parseValue(std::string& ident, bool header) {
	if (!haveNext()) {
		// We are already at the end of the line
		// => no value to read
		std::stringstream msg;
		errorLocation(msg);
		msg << "Unexpected end of line, Value expected.";
		throw msg.str();
	}
	// By definition, end >= index, because the character at `index` has to
	// be a non white space character (or the end of the line, but we
	// checked for that already).
	// +1 because end points one after the last character to take
	size_t end = line.find_last_not_of(" \t")+1;

	if (header){
		if (line[end-1] != ']') {
			// The last character has to be ']' if we're parsing a
			// header, i.e. [Header: Value]
			std::stringstream msg;
			errorLocation(msg);
			msg << "Missing ']' in section header.";
			throw msg.str();
		}
		// If we're parsing a header, the closing ']' does not belong
		// to the header name and we need to find the end of the
		// name
		// -2: end points after the ], end-1 on the ] and end-2
		// before it
		end = line.find_last_not_of(" \t", end-2)+1;
	}

	if (end == std::string::npos) {
		// No non value character left
		// => the value goes to the end of the line
		end = len;
		if (index == end) {
			// We are already at the end of the line
			// => no value to read
			std::stringstream msg;
			errorLocation(msg);
			msg << "Unexpected end of line, Value expected.";
			throw msg.str();
		}
	}
	if (index >= end) {
		// Next character is not allowed in value
		// => Length of value is 0
		std::stringstream msg;
		errorLocation(msg);
		msg << "Empty values are not allowed. (Unexpected character '"
			<< line[index]
			<< "', Value expected)";
		throw msg.str();
	}
	ident = line.substr(index, end-index);
	index = end;
}

void Parser::parseChar(char c) {
	if (index == len) {
		std::stringstream msg;
		errorLocation(msg);
		msg << "Unexpected end of line, '" << c << "' expected";
		throw msg.str();
	}
	if (line[index] != c) {
		std::stringstream msg;
		errorLocation(msg);
		msg << "Unexpected character '" << line[index]
			<< "', '" << c << "' expected";
		throw msg.str();
	}
	index++;
}

bool Parser::nextIs(char c) {
	if (index == len) {
		// If we are at the end of the line, the next character obviously
		// is not equal to c
		return false;
	}
	return line[index] == c;
}

bool Parser::haveNext() {
	if (index == len) {
		// If we are at the end of the line, we don't have another
		// character to read
		return false;
	}
	if (line[index] == ';' && line[index] == '#') {
		// Comments don't count
		return false;
	}
	return true;
}

Status Parser::parseProperty(std::string& key, std::string& value) {
	skipWhitespace();
	if (nextIs('[')) {
		// If a line starts with a '[' it is a header, not a property
		return ParseError;
	}
	parseIdentifier(key);
	skipWhitespace();
	parseChar('=');
	skipWhitespace();
	parseValue(value);
	skipToEOL();
	return Ok;
}

void Parser::parseProperties(Properties& ps) {
	std::string key;
	std::string value;
	while (nextLine() == Ok && parseProperty(key, value) == Ok) {
		if (!ps.insert(make_pair(key, value)).second) {
			std::stringstream msg;
			errorLocation(msg);
			msg << "Duplicate key '" << key << "'";
			throw msg.str();
		}
	}
	// The last line we looked at must have been a header.
	// We still need it for the next run, so keep it.
	pushback();
}

Status Parser::parseHeader(Properties& ps, std::string& type, std::string& name, bool& haveName) {
	skipWhitespace();
	if (!nextIs('[')) {
		return ParseError;
	}
	parseChar('[');
	skipWhitespace();
	parseIdentifier(type, true, false);
	skipWhitespace();
	if (nextIs(':')) {
		parseChar(':');
		skipWhitespace();
		parseValue(name, true);
		skipWhitespace();
		haveName = true;
	}
	parseChar(']');
	skipToEOL();
	// push type+name as property "name"; use unlowered string for this
	ps.insert(make_pair("name", type+" '"+name+"'"));
	// and lower it afterwards
	std::transform(type.begin(), type.end(), type.begin(), tolower);

	return Ok;
}

Status Parser::parseSection(Config& conf) {
	std::string type;
	std::string name;
	bool haveName;
	Properties properties;
	if (nextLine() == Eof) {
		return Eof;
	}
	if (parseHeader(properties, type, name, haveName) == ParseError) {
		std::stringstream msg;
		errorLocation(msg);
		msg << "Header expected.";
		throw msg.str();
	}
	if (!haveName) {
		// Because of the maps used to store everything in, everything
		// must have a name. "" is a name that no named section can have.
		name = "";
	}
	parseProperties(properties);
	Sections& sect = conf[type];
	if (!sect.insert(make_pair(name, properties)).second) {
		std::stringstream msg;
		if (name == "") {
			msg << "Can not have two unnamed sections"
				" of the same type. (type: '" << type << "')";
		} else {
			msg << "Can not have two sections with the same name."
				" (type: '" << type << "', name: '" << name << "')";
		}
		throw msg.str();
	}
	return Ok;
}

void Parser::errorLocation(std::stringstream& msg) {
	// index is zero based
	msg << "Error in line " << lineNumber << " and column " << index+1 << ": ";
}

Config parseConfigFile(std::string path) {
	Config conf;

	std::fstream file;
	file.open(path.c_str(), std::fstream::in);
	if (file.fail()) {
		std::stringstream msg;
		msg << "Failed to open file '" << path << "'";
		Global::abort("ConfigParser", msg.str());
	}

        try {
            Parser p(file);
            while (p.parseSection(conf) == Ok);
        } catch(std::string msg) {
            Global::abort("ConfigParser", msg);
        }
        return conf;
}
