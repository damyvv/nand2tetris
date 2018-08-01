#pragma once

#include "SymbolTable.h"
#include <fstream>
#include <string>

typedef enum CommandType {
	A_COMMAND,
	C_COMMAND,
	L_COMMAND
} CommandType;

class Parser
{
public:
	Parser(const char* file, SymbolTable& symbolTable, bool fillSymbolTable = false);
	~Parser();

	void advance();
	const std::string symbol();
	const std::string dest() const;
	const std::string comp() const;
	const std::string jump() const;

	inline bool hasMoreCommands() const { return moreCommands; }
	inline CommandType commandType() const { return currentCommandType; }
	inline int getCurrentLineNumber() const { return lineNr; }

	inline std::string getFileBaseName() const { return baseFileName; }

private:
	static inline bool is_digits(const std::string &str) {
		return str.find_first_not_of("0123456789") == std::string::npos;
	}

private:
	std::ifstream mStream;
	std::string currentCommand;
	std::string baseFileName;
	bool moreCommands;
	CommandType currentCommandType;
	int lineNr;
	int romAddress;
	SymbolTable& mSymbolTable;
	bool mFillSymbolTable;
	int mVarCounter;
};