#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <vector>

#include "CodeWriter.h"
#include "Parser.h"

#define USE_FIXED_FILE 0
#define WITH_DEBUG_COMMENTS 0

using namespace std;

BOOL WINAPI winExitHandler(DWORD s) {
	std::cerr << "Exited with code " << s << std::endl;
	exit(1);
	return TRUE;
}

void parseFile(std::string baseFileName, CodeWriter& writer) {
	Parser parser(baseFileName);

	writer.setParser(&parser);

	while (true) {
		parser.advance();
		if (!parser.hasMoreCommands()) break;
#if WITH_DEBUG_COMMENTS == 1
		writer.writeCommentln(parser.getFullCodeLine());
#endif
		if (parser.commandType() == C_ARITHMETIC) {
			writer.writeArithmetic(parser.arg1());
		}
		else if (parser.commandType() == C_PUSH || parser.commandType() == C_POP) {
			writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
		}
		else if (parser.commandType() == C_LABEL) {
			writer.writeLabel(parser.arg1());
		}
		else if (parser.commandType() == C_GOTO) {
			writer.writeGoto(parser.arg1());
		}
		else if (parser.commandType() == C_IF) {
			writer.writeIf(parser.arg1());
		}
		else if (parser.commandType() == C_FUNCTION) {
			writer.writeFunction(parser.arg1(), parser.arg2());
		}
		else if (parser.commandType() == C_RETURN) {
			writer.writeReturn();
		}
		else if (parser.commandType() == C_CALL) {
			writer.writeCall(parser.arg1(), parser.arg2());
		}
		else {
			cerr << "Fatal internal error. Command on line " << parser.getCurrentLineNumber() << " is not yet supported\n";
			exit(1);
		}
	}
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
			"Damy";
#endif

		string fileNameString(fileName);
		auto slashPos = fileNameString.find_last_of('\\');
		size_t extensionPos = fileNameString.substr(slashPos == string::npos ? 0 : slashPos).find_last_of('.');
		bool directory = false;

		std::vector<std::string> vmFileList;

		string baseFileName = fileNameString.substr(0, extensionPos);
		if (extensionPos != string::npos) {
			string extension = fileNameString.substr(extensionPos);
			if (extension != ".vm") {
				std::cerr << "Invalid file extension. File should end with '.vm'\n";
				exit(1);
			}
			vmFileList.push_back(baseFileName);
		}
		else {
			directory = true;
			std::cout << "Assembling all files in the '" << fileName << "' directory\n";
			for (auto &p : std::experimental::filesystem::directory_iterator(fileNameString)) {
				string file = p.path().string();
				extensionPos = file.find_last_of('.');
				if (extensionPos != string::npos) {
					string extension = file.substr(extensionPos);
					if (extension == ".vm") {
						vmFileList.push_back(file.substr(0, extensionPos));
					}
				}
			}
		}

		if (vmFileList.size() < 1) {
			std::cerr << "No .vm files found\n";
			exit(1);
		}

		if (directory) {
			auto lastSlashPos = baseFileName.find_last_of('\\');
			if (lastSlashPos != string::npos) {
				string endDirName = baseFileName.substr(lastSlashPos + 1);
				baseFileName = baseFileName + '\\' + endDirName;
			}
			else {
				baseFileName = baseFileName + '\\' + baseFileName;
			}
		}
		CodeWriter writer(baseFileName);
		writer.writeInit();

		for (int i = 0; i < vmFileList.size(); i++) {
			std::string f = vmFileList[i];
			std::cout << "Assembling file: '" << f << ".vm'\n";
			parseFile(f, writer);
		}

		std::cout << "Finished!\n";
	}
	return 0;
}