#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include "FritzConfig.h"

#include <istream>
#include <map>
#include <string>
#include <vector>

//! Status codes for various parser functions
enum Status {
	Ok,
	Eof,
	ParseError
};

//! Parser for the Fritz configuration file
class Parser {
	public:

	//! \brief Construct a new parser reading from an input stream
	//!
	//! \input file the input stream to read from
	Parser(std::istream &file);

	//! \brief Parses a section
        //!
	//! \input config Config object that the parsed section is stored into
	//! \return Ok if a section was successfully parsed
	//! \return Eof if there are no more sections left
	//! \exception if the section is malformed
	Status parseSection(Config& config);

	private:
	//! \brief Advance the Parser by one line
	//!
	//! Blank lines or lines that just contain comments are skipped.
	//! If a line has been pushed back, no new line is read, and
	//! the last line stays active.
        //!
	//! \exception if a read error occured
	//! \return Ok if a new line was read
	//! \return Eof if no new line is available
	Status nextLine();

	//! \brief Mark the last line as unread
        //!
        //! The next call to nextLine() will keep the parser at the same
        //! line, starting again at the first non whitespace character.
	//! Beware that this is independent of any characters being
	//! consumed by the parser, i.e. it is possible to parse the
	//! same line twice.
        //!
	//! \exception if no lines have been read yet
	void pushback();

	//! \brief Skips over zero or more white space characters.
        //!
        //! Newlines won't be skipped.
	void skipWhitespace();

	//! \brief Skips over white space and comments to the end of the line.
        //!
	//! \exception if a non white space character was encountered outside
	//! of a comment
	void skipToEOL();

	//! \brief Parses an identifier.
	//!
	//! Parses the longest possible string that does not contain white
	//! space or any of the following characters: ':', '='
        //! The parsed string is normalized to lower case.
        //!
	//! \input ident string to store the parsed identifier
	//! \input header if true, ']' is not allowed
	//! \exception if the length of the identifier would have to be zero
	void parseIdentifier(std::string& ident, bool header=false, bool toLower=true);

	//! \brief Parses a value.
	//!
	//! Parses the longest possible string that does not contain
	//! linebreaks. Leading and trailing whitespace is removed.
	//! White space within the value is preserved.
        //!
	//! \input ident string to store the parsed identifier
	//! \input header if true, ']' is not allowed
	//! \exception if the length of the identifier would have to be zero
	void parseValue(std::string& ident, bool header=false);

	//! \brief Tests if the next char is equal to the input char.
        //!
        //! The parser state is left as is, i.e. the char is not consumed.
	//!
	//! \input c char to test against
	//! \return wether the next char in the line is equal to c
	bool nextIs(char c);

	//! \brief Checks if there is a non comment character left.
        //!
        //! This is the same as (!nextIs(';') && !nextIs('#')).
        //! \return if there is a non comment, non whitespace character left
	bool haveNext();

	//! \brief Parses a single char
	//!
	//! \input c the char to parse
	//! \exception if the next char is not equal to c
	void parseChar(char c);

	//! \brief Parses a single property.
	//!
	//! Parses a key value pair separated by an equals sign.
	//! \input key string to store the parsed key
	//! \input value string to store the parsed value
	//! \return Ok if a property was parsed successfully
	//! \return ParseError if the line does not contain a property
	//! \exception if the property is malformed
	Status parseProperty(std::string& key, std::string& value);

	//! Parse all properties until the next header
	//!
	//! \input ps map to insert the parsed properties
	//! \exception if the section contains duplicate keys
	void parseProperties(Properties& ps);

	//! Parses the header of a section
	//!
	//! \input type string to store the type of the section
	//! \input name string to store the name of the section
	//! \input haveName gets set to true if there is a name, false otherwise
	//! \return Ok if a header was parsed successfully
	//! \return ParseError if the line does not contain a header
	//! \exception if the header is malformed
	Status parseHeader(Properties& ps, std::string& type, std::string& name, bool& haveName);

	//! Writes the current location in the file into msg
	//!
	//! \input msg stream to write the message to
	void errorLocation(std::stringstream& msg);

        //! Line number in the file.
	int lineNumber;
        //! Column index in the current line. Zero based.
	size_t index;
        //! Length of the current line.
	int len;
        //! Has a line been pushed back?
	bool pushedBack;
        //! The current line.
	std::string line;
        //! The input stream that we're parsing.
	std::istream &file;
};


//! Parses a configuration file
/*!
 * \input path the location of the configuration file
 * \result the contents of the configuration file read into a map
 * \exception if parsing the file fails
 */
Config parseConfigFile(std::string path);

#endif // CONFIG_PARSER_H_
