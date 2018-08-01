#pragma once

#include <fstream>
#include <string>

typedef enum CommandType {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
} CommandType;

class Parser
{
public:
	Parser(std::string file);
	~Parser();

	void advance();

	inline bool hasMoreCommands() const { return moreCommands; }
	inline CommandType commandType() const { return currentCommandType; }
	inline int getCurrentLineNumber() const { return lineNr; }
	inline const std::string& getFullCodeLine() const { return fullCodeLine; }
	inline std::string getFileName() const { return mFileName; }

	std::string arg1() const;
	int arg2() const;


private:
	static inline bool is_digits(const std::string &str) {
		return str.find_first_not_of("0123456789") == std::string::npos;
	}

private:
	std::ifstream mStream;
	std::string currentCommand[3];
	std::string fullCodeLine;
	const std::string mFileName;
	bool moreCommands;
	CommandType currentCommandType;
	int lineNr;
};