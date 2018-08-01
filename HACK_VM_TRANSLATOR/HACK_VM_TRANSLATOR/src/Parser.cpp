#include "Parser.h"
#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#include "CodeWriter.h"

using namespace std;

Parser::Parser(std::string file)
	: mFileName(file)
{
	lineNr = 0;

	mStream.open(file + ".vm", std::ifstream::in);

	if (!mStream.is_open()) {
		std::cerr << "File not found: " << file << std::endl;
		exit(1);
	}
	moreCommands = true;
}

Parser::~Parser()
{
	mStream.close();
}

void Parser::advance()
{
	bool lineIsCommand = false;
	string line;

	while (!lineIsCommand) {
		if (mStream.eof()) {
			moreCommands = false;
			return;
		}
		getline(mStream, line);
		lineNr++;
		size_t commentPos = line.find_first_of("//");
		if (commentPos != string::npos) {
			line = line.substr(0, commentPos);
		}
		if (line.length() > 0) {
			lineIsCommand = true;
		}
	}

	fullCodeLine = line;

	std::replace(line.begin(), line.end(), '\t', ' ');

	for (int i = 0; i < 3; i++) {
		currentCommand[i] = "";
	}

	stringstream ss(line);
	string word;
	int index = 0;
	while (getline(ss, word, ' ')) {
		if (word.empty()) {
			continue;
		}
		currentCommand[index++] = word;

		if (index > 2) break;
	}

	if (currentCommand[0] == "push") {
		currentCommandType = C_PUSH;
	}
	else if (currentCommand[0] == "pop") {
		currentCommandType = C_POP;
	}
	else if ([](std::string command) {
		for (int i = 0; i < CodeWriter::arithmeticCommands.size(); i++) {
			if (CodeWriter::arithmeticCommands[i] == command)
				return true;
		}
		return false;
	}(currentCommand[0])) {
		currentCommandType = C_ARITHMETIC;
	}
	else if (currentCommand[0] == "label") {
		currentCommandType = C_LABEL;
	}
	else if (currentCommand[0] == "goto") {
		currentCommandType = C_GOTO;
	}
	else if (currentCommand[0] == "if-goto") {
		currentCommandType = C_IF;
	}
	else if (currentCommand[0] == "function") {
		currentCommandType = C_FUNCTION;
	}
	else if (currentCommand[0] == "return") {
		currentCommandType = C_RETURN;
	}
	else if (currentCommand[0] == "call") {
		currentCommandType = C_CALL;
	}

	else {
		cerr << "Syntax error on line: " << lineNr << ". Unknown command.\n";
		exit(1);
	}
}

std::string Parser::arg1() const
{
	if (currentCommandType == C_ARITHMETIC) {
		return currentCommand[0];
	}
	else {
		return currentCommand[1];
	}

	return "";
}

int Parser::arg2() const
{
	if (!is_digits(currentCommand[2])) {
		cerr << "Syntax error on line: " << lineNr << ". Second argument is not a number\n";
		exit(1);
	}
	return atoi(currentCommand[2].c_str());
}
