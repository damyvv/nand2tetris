#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	mSymbolTable["SP"]		= 0x0000;
	mSymbolTable["LCL"]		= 0x0001;
	mSymbolTable["ARG"]		= 0x0002;
	mSymbolTable["THIS"]	= 0x0003;
	mSymbolTable["THAT"]	= 0x0004;
	mSymbolTable["R0"]		= 0x0000;
	mSymbolTable["R1"]		= 0x0001;
	mSymbolTable["R2"]		= 0x0002;
	mSymbolTable["R3"]		= 0x0003;
	mSymbolTable["R4"]		= 0x0004;
	mSymbolTable["R5"]		= 0x0005;
	mSymbolTable["R6"]		= 0x0006;
	mSymbolTable["R7"]		= 0x0007;
	mSymbolTable["R8"]		= 0x0008;
	mSymbolTable["R9"]		= 0x0009;
	mSymbolTable["R10"]		= 0x000A;
	mSymbolTable["R11"]		= 0x000B;
	mSymbolTable["R12"]		= 0x000C;
	mSymbolTable["R13"]		= 0x000D;
	mSymbolTable["R14"]		= 0x000E;
	mSymbolTable["R15"]		= 0x000F;
	mSymbolTable["SCREEN"]	= 0x4000;
	mSymbolTable["KBD"]		= 0x6000;
}

void SymbolTable::addEntry(std::string symbol, uint16_t address)
{
	mSymbolTable[symbol] = address;
}

bool SymbolTable::contains(std::string symbol) const
{
	auto it = mSymbolTable.find(symbol);
	if (it != mSymbolTable.end())
		return true;
	return false;
}

uint16_t SymbolTable::getAddress(std::string symbol) const
{
	auto it = mSymbolTable.find(symbol);
	if (it != mSymbolTable.end())
		return it->second;
	return -1;
}
