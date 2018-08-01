#pragma once

#include <string>
#include "Parser.h"
#include <fstream>
#include <array>

class CodeWriter
{
public:
	CodeWriter(std::string fileName);
	~CodeWriter();

	void setParser(const Parser* parser);
	void writeArithmetic(std::string command);
	void writePushPop(CommandType command, std::string segment, int index);
	void writeCommentln(const std::string& comment);
	void writeInit();
	void writeLabel(std::string label);
	void writeGoto(std::string label);
	void writeIf(std::string label);
	void writeCall(std::string functionName, int numArgs);
	void writeReturn();
	void writeFunction(std::string functionName, int numLocals);

private:
	inline std::string getCurrentFileName() { return mParser->getFileName(); }
	void logFatal(std::string message) const;

private:
	const Parser* mParser;
	std::ofstream mStream;
	std::string mCurrentFunctionName;
	int mReturnLabelCounter;
	int mArithmeticLabelCounter;

public:
	static const std::array<std::string, 9> const arithmeticCommands;
};
