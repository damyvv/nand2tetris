#pragma once

#include <cstring>
#include <iostream>

#include <map>

class Code
{
public:
	static char dest(const char* mnemonic);
	static char comp(const char* mnemonic);
	static char jump(const char* mnemonic);

private:
	static const std::map<std::string, char> compTable;
};
