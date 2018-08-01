#pragma once

#include <map>
#include <string>

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable() {}

	void addEntry(std::string symbol, uint16_t address);
	bool contains(std::string symbol) const;
	uint16_t getAddress(std::string symbol) const;
private:
	std::map<std::string, uint16_t> mSymbolTable;
};
