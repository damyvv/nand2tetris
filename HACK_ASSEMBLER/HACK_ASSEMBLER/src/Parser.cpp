#include "Parser.h"
#include "Code.h"
#include <iostream>
#include <algorithm>
#include <bitset>

using namespace std;

Parser::Parser(const char* file, SymbolTable& symbolTable, bool fillSymbolTable)
	: mSymbolTable(symbolTable), mFillSymbolTable(fillSymbolTable)
{
	lineNr = 0;
	romAddress = 0;
	mVarCounter = 16;

	string fileNameString(file);
	size_t extensionPos = fileNameString.find_last_of('.');

	baseFileName = fileNameString.substr(0, extensionPos);
	string extension = fileNameString.substr(extensionPos);
	if (extension != ".asm") {
		std::cerr << "Invalid file extension. File should end with '.asm'\n";
		exit(1);
	}

	mStream.open(file, std::ifstream::in);

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
		getline(mStream, line);
		lineNr++;
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		size_t commentPos = line.find_first_of("//");
		if (commentPos != string::npos) {
			line = line.substr(0, commentPos);
		}
		if (line.length() > 0) {
			lineIsCommand = true;
		}
		if (mStream.eof()) {
			moreCommands = false;
			return;
		}
	}

	currentCommand = line;

	if (currentCommand.at(0) == '@') {
		currentCommandType = A_COMMAND;
		romAddress++;
	}
	else if (currentCommand.at(0) == '(') {
		currentCommandType = L_COMMAND;
		if (mFillSymbolTable) {
			if (currentCommand.at(currentCommand.length() - 1) != ')') {
				cerr << "Syntax error on line: " << lineNr << ". Invalid symbol definition. Expected ')'";
				exit(1);
			}
			string symbolStr = currentCommand.substr(1, currentCommand.length() - 2);
			if (symbolStr.length() < 1) {
				cerr << "Syntax error on line: " << lineNr << ". Invalid symbol definition. Expected symbol name after '('";
				exit(1);
			}
			if (mSymbolTable.contains(symbolStr)) {
				cerr << "Syntax error on line: " << lineNr << ". Symbol '" << symbolStr << "' already exists.";
				exit(1);
			}
			mSymbolTable.addEntry(symbolStr, romAddress);
		}
	}
	else {
		currentCommandType = C_COMMAND;
		romAddress++;
	}
}

const string Parser::symbol()
{
	if (currentCommandType != L_COMMAND && currentCommandType != A_COMMAND) {
		cerr << "Fatal internal error on line: " << lineNr << endl;
		exit(1);
	}

	string sym = currentCommand.substr(1);
	if (isdigit(sym[0])) {
		if (!is_digits(sym)) {
			cerr << "Syntax error on line: " << lineNr << ". Symbols may not start with numbers.\n";
			exit(1);
		}
		uint16_t number = atoi(sym.c_str());
		return "0" + std::bitset<15>(number).to_string();
	}

	if (mSymbolTable.contains(sym)) {
		return "0" + std::bitset<15>(mSymbolTable.getAddress(sym)).to_string();
	}

	if (currentCommandType == A_COMMAND) {
		mSymbolTable.addEntry(sym, mVarCounter);
		return "0" + std::bitset<15>(mVarCounter++).to_string();
	}
	cerr << "Syntax error on line: " << lineNr << ". Symbol '" << sym << "' not found.\n";
	exit(1);

	return "";
}

const string Parser::dest() const
{
	size_t destEnd = currentCommand.find_first_of('=');

	string destString = "";
	if (destEnd != string::npos)
		destString = currentCommand.substr(0, destEnd);

	char destBin = Code::dest(destString.c_str());
	if (destBin == -1) {
		cerr << "Syntax error on line: " << lineNr << ". Invalid destination for the computation.";
		exit(1);
	}
	return std::bitset<3>(destBin).to_string();
}

const string Parser::comp() const
{
	size_t destEnd = currentCommand.find_first_of('=');
	size_t jumpStart = currentCommand.find_first_of(';');
	if (destEnd == string::npos) destEnd = 0;
	else destEnd++;
	string compString = currentCommand.substr(destEnd, jumpStart - destEnd);

	char compBin = Code::comp(compString.c_str());
	if (compBin == -1) {
		cerr << "Syntax error on line: " << lineNr << ". Invalid computation command.";
		exit(1);
	}
	return std::bitset<7>(compBin).to_string();
}

const string Parser::jump() const
{
	size_t jumpStart = currentCommand.find_first_of(';');

	string jumpString = "";
	if (jumpStart != string::npos)
		jumpString = currentCommand.substr(jumpStart + 1);

	char jumpBin = Code::jump(jumpString.c_str());
	if (jumpBin == -1) {
		cerr << "Syntax error on line: " << lineNr << ". Invalid jump command.";
		exit(1);
	}
	return std::bitset<3>(jumpBin).to_string();
}
