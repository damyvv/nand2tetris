#include "Code.h"
#include <stdexcept>

char Code::dest(const char* mnemonic) {
	if (!mnemonic[0])
		return 0x0;
	if (std::strcmp(mnemonic, "M") == 0)
		return 0x1;
	if (std::strcmp(mnemonic, "D") == 0)
		return 0x2;
	if (std::strcmp(mnemonic, "MD") == 0)
		return 0x3;
	if (std::strcmp(mnemonic, "A") == 0)
		return 0x4;
	if (std::strcmp(mnemonic, "AM") == 0)
		return 0x5;
	if (std::strcmp(mnemonic, "AD") == 0)
		return 0x6;
	if (std::strcmp(mnemonic, "AMD") == 0)
		return 0x7;

	std::cerr << "Syntax error. Invalid dest mnemonic: " << mnemonic << std::endl;
	return -1;
}

char Code::comp(const char* mnemonic) {
	std::string tmp(mnemonic);
	try {
		return compTable.at(tmp);
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Syntax error. Invalid comp mnemonic: " << mnemonic << std::endl;
		return -1;
	}
}

char Code::jump(const char* mnemonic) {
	if (!mnemonic[0])
		return 0x0;
	if (std::strcmp(mnemonic, "JGT") == 0)
		return 0x1;
	if (std::strcmp(mnemonic, "JEQ") == 0)
		return 0x2;
	if (std::strcmp(mnemonic, "JGE") == 0)
		return 0x3;
	if (std::strcmp(mnemonic, "JLT") == 0)
		return 0x4;
	if (std::strcmp(mnemonic, "JNE") == 0)
		return 0x5;
	if (std::strcmp(mnemonic, "JLE") == 0)
		return 0x6;
	if (std::strcmp(mnemonic, "JMP") == 0)
		return 0x7;

	std::cerr << "Syntax error. Invalid jump mnemonic: " << mnemonic << std::endl;
	return -1;
}

const std::map<std::string, char> Code::compTable = {
	{ "0",		0x2A },
	{ "1",		0x3F },
	{ "-1",		0x3A },
	{ "D",		0x0C },
	{ "A",		0x30 },
	{ "!D",		0x0D },
	{ "!A",		0x31 },
	{ "-D",		0x0F },
	{ "-A",		0x33 },
	{ "D+1",	0x1F },
	{ "A+1",	0x37 },
	{ "D-1",	0x0E },
	{ "A-1",	0x32 },
	{ "D+A",	0x02 },
	{ "D-A",	0x13 },
	{ "A-D",	0x07 },
	{ "D&A",	0x00 },
	{ "D|A",	0x15 },
	{ "M",		0x70 },
	{ "!M",		0x71 },
	{ "-M",		0x73 },
	{ "M+1",	0x77 },
	{ "M-1",	0x72 },
	{ "D+M",	0x42 },
	{ "D-M",	0x53 },
	{ "M-D",	0x47 },
	{ "D&M",	0x40 },
	{ "D|M",	0x55 },
};