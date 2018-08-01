#include <iostream>
#include <Windows.h>
#include <fstream>

#include "Parser.h"

#define USE_FIXED_FILE 0

using namespace std;

BOOL WINAPI winExitHandler(DWORD s) {
	std::cerr << "Exited with code " << s << std::endl;
	exit(1);
	return TRUE;
}

int main(int argc, char **argv) {
	if (!SetConsoleCtrlHandler(winExitHandler, TRUE)) {
		std::cerr << "Critical error. Failed to attach control handler\n";
		return 1;
	}

#if USE_FIXED_FILE == 0
	if (argc <= 1) {
		std::cout << "Please supply a filename to assemble and try again\n";
		return 1;
	}
	else if (argc > 2) {
		std::cout << "Too many arguments specified. You can only compile one file at a time.\n";
		return 1;
	}
	else 
#endif
	{
		const char* fileName =
#if USE_FIXED_FILE == 0
			argv[1];
#else
			"StaticsTest.asm";
#endif
		std::cout << "Assembling file: '" << fileName << "'\n";
		SymbolTable symbolTable;

		{
			Parser fileParser(fileName, symbolTable, true);

			while (true) {
				fileParser.advance();
				if (!fileParser.hasMoreCommands()) break;
			}
		}

		Parser fileParser(fileName, symbolTable);
		string binaryFileName = fileParser.getFileBaseName() + ".hack";
		std::cout << "Creating file: '" << binaryFileName << "'\n";
		ofstream binaryFile(binaryFileName);
		if (!binaryFile.is_open()) {
			std::cerr << "Fatal error. Could not create file '" << binaryFileName << "'\n";
		}

		while (true) {
			fileParser.advance();
			if (!fileParser.hasMoreCommands()) break;
				
			if (fileParser.commandType() == A_COMMAND) {
				binaryFile << fileParser.symbol() << std::endl;
			}
			else if (fileParser.commandType() == C_COMMAND) {
				binaryFile << "111";
				binaryFile << fileParser.comp();
				binaryFile << fileParser.dest();
				binaryFile << fileParser.jump();
				binaryFile << endl;
			}
		}
		std::cout << "Finished!\n";
	}
	return 0;
}