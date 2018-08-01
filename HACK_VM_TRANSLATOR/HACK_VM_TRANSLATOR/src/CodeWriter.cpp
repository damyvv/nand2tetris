#include "CodeWriter.h"

#include <iostream>

#define HACK_ASM(x) mStream << x << "\n"
#define STATIC_SYMBOL_NAME(i) "@" << mParser->getFileName().substr(mParser->getFileName().find_last_of("\\") + 1) << "." << i

using namespace std;

CodeWriter::CodeWriter(std::string fileName)
{
	mParser = nullptr;
	mArithmeticLabelCounter = 0;
	mReturnLabelCounter = 0;
	mCurrentFunctionName = "";
	mStream.open(string(fileName) + ".asm", ofstream::out);
	if (!mStream.is_open()) {
		cerr << "Fatal error: Failed to create file '" << fileName << "'. Do you have the required permissions?\n";
		exit(1);
	}
	std::cout << "Creating file: '" << fileName << ".asm'\n";
}

CodeWriter::~CodeWriter()
{
	mStream.close();
}

void CodeWriter::setParser(const Parser* parser)
{
	mParser = parser;
}

void CodeWriter::writeArithmetic(string command)
{
	// TODO: Decrease the amount of instuctions needed by using the general purpose registers

	if (command == "neg" ||
		command == "not") { // Single operand
		HACK_ASM("@SP"); // Load the stack pointer register in the A register
		HACK_ASM("A=M-1"); // Load the stack pointer in the A register
	}
	else { // Two operands
		HACK_ASM("@SP"); // Load the stack pointer register in the A register
		HACK_ASM("MD=M-1"); // Decrement the stack pointer
		HACK_ASM("@R13"); // Load the address of R13 in the A register
		HACK_ASM("AM=D"); // Store a copy of the stack pointer in the R13 register and load the stack pointer in the A register
		HACK_ASM("D=M"); // Load the contents from the top of the stack in the D register
		HACK_ASM("@R13"); // Load the stack pointer register in the A register
		HACK_ASM("AM=M-1"); // Decrement the copied stack pointer and load the address in the A register
	}
	if (command == "add") {
		HACK_ASM("M=D+M"); // Add the D and M registers
	}
	else if (command == "sub") {
		HACK_ASM("M=M-D"); // Substract the M and D registers
	}
	else if (command == "neg") {
		HACK_ASM("M=-M");
	}
	else if (command == "eq" || command == "gt" || command == "lt") {
		HACK_ASM("D=M-D"); // Substract M and D
		HACK_ASM("@ARITH" << mArithmeticLabelCounter); // Load label + 0 address
		if (command == "eq") {
			HACK_ASM("D;JEQ"); // Jump if M and D were equal
			HACK_ASM("D=1"); // Set D to 1 to be later substracted to 0
			HACK_ASM("(ARITH" << mArithmeticLabelCounter++ << ")"); // Label 0 creation
			HACK_ASM("D=D-1"); // If equal value was 0 otherwise 1. Substract 1 to set equal to -1 and not equal to 0.
		}
		else {
			if (command == "gt") {
				HACK_ASM("D;JGT"); // Jump if greater
			}
			else if (command == "lt") {
				HACK_ASM("D;JLT"); // Jump if less
			}
			HACK_ASM("D=0"); // False
			HACK_ASM("@ARITH" << mArithmeticLabelCounter + 1);
			HACK_ASM("0;JMP"); // Jump to label + 1
			HACK_ASM("(ARITH" << mArithmeticLabelCounter++ << ")"); // label + 0
			HACK_ASM("D=-1"); // True
			HACK_ASM("(ARITH" << mArithmeticLabelCounter++ << ")"); // label + 1
		}

		HACK_ASM("@R13"); // Load copied stack pointer register
		HACK_ASM("A=M"); // Load the stack pointer in the A register
		HACK_ASM("M=D"); // Store the boolean result on the stack
	}
	else if (command == "and") {
		HACK_ASM("M=D&M");
	}
	else if (command == "or") {
		HACK_ASM("M=D|M");
	}
	else if (command == "not") {
		HACK_ASM("M=!M");
	}
	else {
		cerr << "Fatal internal error: Unknown arithmetic command\n";
		exit(1);
	}
}

void CodeWriter::writePushPop(CommandType command, string segment, int index)
{
	if (index < 0) {
		logFatal("Invalid index '" + to_string(index) + "'. Negative indices are not allowed.");
	}

	if (command == C_PUSH) {
		if (segment == "constant") {
			HACK_ASM("@" << index); // Load the constant into the A register
			HACK_ASM("D=A"); // Load the constant in the D register
		}
		else if (segment == "static") {
			HACK_ASM(STATIC_SYMBOL_NAME(index)); // Load the symbol address in the A register
			HACK_ASM("D=M"); // Load the value in the D register
		}
		else if (segment == "pointer") {
			if (index == 0) {
				HACK_ASM("@THIS");
			}
			else if (index == 1) {
				HACK_ASM("@THAT");
			}
			else {
				logFatal("Invalid pointer entry '" + to_string(index) + "'. Pointer segment does not hold more than two entries.");
				exit(1);
			}
			HACK_ASM("D=M");
		}
		else if (segment == "temp") {
			if (index > 7) {
				logFatal("Invalid pointer entry '" + to_string(index) + "'. Temp segment does not hold more than eight entries.");
			}
			HACK_ASM("@" << (index + 5));
			HACK_ASM("D=M");
		}
		else if (segment == "local" || segment == "this" || segment == "that" || segment == "argument") {
			if (segment == "local") {
				HACK_ASM("@LCL"); // Load the local segment pointer register
			}
			else if (segment == "this") {
				HACK_ASM("@THIS"); // Load the this pointer register
			}
			else if (segment == "argument") {
				HACK_ASM("@ARG"); // Load the argument base pointer register
			}
			else {
				HACK_ASM("@THAT"); // Load the that pointer register
			}
			HACK_ASM("D=M");
			HACK_ASM("@" << index);
			HACK_ASM("A=D+A");
			HACK_ASM("D=M");
		}
		else {
			logFatal("Invalid segment '" + segment + "' for push command");
			exit(1);
		}
		HACK_ASM("@SP"); // Load the stack pointer register in the A register
		HACK_ASM("M=M+1"); // Increment the stack pointer ahead, so we don't have to load it later
		HACK_ASM("A=M-1"); // Store the stack pointer back in the A register, but decrement one so it is the correct address
		HACK_ASM("M=D"); // Assign the constant to the top of the stack
	}
	else if (command == C_POP) {
		if (segment == "local" || segment == "this" || segment == "that" || segment == "argument") {
			HACK_ASM("@" << index); // Load the offset in the A register
			HACK_ASM("D=A"); // Store the offset in the D register
			if (segment == "local") {
				HACK_ASM("@LCL"); // Load the local segment pointer register
			}
			else if (segment == "this") {
				HACK_ASM("@THIS"); // Load the this pointer register
			}
			else if (segment == "argument") {
				HACK_ASM("@ARG"); // Load the argument base pointer register
			}
			else {
				HACK_ASM("@THAT"); // Load the that pointer register
			}

			HACK_ASM("D=D+M"); // Add the offset to the local segment pointer, store in the D register
			HACK_ASM("@R13"); // Load the R13 register
			HACK_ASM("M=D"); // Assign pointer offset to R13
			HACK_ASM("@SP"); // Load the stack pointer register in the A register
			HACK_ASM("AM=M-1"); // Decrement the stack pointer and load the address in the A register
			HACK_ASM("D=M"); // Load the top stack item in the D register
			HACK_ASM("@R13"); // Load R13 register address
			HACK_ASM("A=M"); // Load the pointer offset in the A register
		}
		else {
			HACK_ASM("@SP"); // Load the stack pointer register in the A register
			HACK_ASM("AM=M-1"); // Decrement the stack pointer and load the address in the A register
			HACK_ASM("D=M"); // Load the top stack item in the D register
			if (segment == "static") {
				HACK_ASM(STATIC_SYMBOL_NAME(index)); // Load the static variable address
			}
			else if (segment == "pointer") {
				if (index == 0) {
					HACK_ASM("@THIS");
				}
				else if (index == 1) {
					HACK_ASM("@THAT");
				}
				else {
					logFatal("Invalid pointer entry '" + to_string(index) + "'. Pointer segment does not hold more than two entries.");
					exit(1);
				}
			}
			else if (segment == "temp") {
				if (index > 7) {
					logFatal("Invalid pointer entry '" + to_string(index) + "'. Temp segment does not hold more than eight entries.");
				}
				HACK_ASM("@" << (index + 5));
			}
			else {
				logFatal("Invalid segment '" + segment + "' for pop command");
				exit(1);
			}
		}
		
		HACK_ASM("M=D"); // Store the top stack item in the address pointed to by the A register
	}
}

void CodeWriter::writeCommentln(const std::string& comment)
{
	mStream << "// " << comment << endl;
}

void CodeWriter::writeInit()
{
	HACK_ASM("@256");
	HACK_ASM("D=A");
	HACK_ASM("@SP");
	HACK_ASM("M=D");

	writeCall("Sys.init", 0);

//	HACK_ASM("@Sys.init");
//	HACK_ASM("0;JMP");
}

void CodeWriter::writeLabel(std::string label)
{
	HACK_ASM("(" << mCurrentFunctionName << "$" << label << ")");
}

void CodeWriter::writeGoto(std::string label)
{
	HACK_ASM("@" << mCurrentFunctionName << "$" << label);
	HACK_ASM("0;JMP");
}

void CodeWriter::writeIf(std::string label)
{
	HACK_ASM("@SP"); // Load the stack pointer register in the A register
	HACK_ASM("AM=M-1"); // Decrement the stack pointer and load the address in the A register
	HACK_ASM("D=M"); // Load the top stack item in the D register
	HACK_ASM("@" << mCurrentFunctionName << "$" << label); // Load the label address in the A register
	HACK_ASM("D;JNE"); // Jump if the top stack item is true
}

void CodeWriter::writeCall(std::string functionName, int numArgs)
{
	HACK_ASM("@return-" << mReturnLabelCounter); // Load the return address
	// Push the return address on the stack
	HACK_ASM("D=A"); // Store the return address in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("A=M"); // Load the stack pointer in the A register
	HACK_ASM("M=D"); // Store the return address on the stack

	// Push the local register on the stack
	HACK_ASM("@LCL"); // Load the local register
	HACK_ASM("D=M"); // Store the local pointer in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("AM=M+1"); // Increment and load the stack pointer in the A register
	HACK_ASM("M=D"); // Store the local pointer on the stack

	// Push the argument register on the stack
	HACK_ASM("@ARG"); // Load the argument register
	HACK_ASM("D=M"); // Store the argument pointer in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("AM=M+1"); // Increment and load the stack pointer in the A register
	HACK_ASM("M=D"); // Store the argument pointer on the stack

	// Push the this register on the stack
	HACK_ASM("@THIS"); // Load the this register
	HACK_ASM("D=M"); // Store the this pointer in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("AM=M+1"); // Increment and load the stack pointer in the A register
	HACK_ASM("M=D"); // Store the this pointer on the stack

	// Push the that register on the stack
	HACK_ASM("@THAT"); // Load the that register
	HACK_ASM("D=M"); // Store the that pointer in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("AM=M+1"); // Increment and load the stack pointer in the A register
	HACK_ASM("M=D"); // Store the that pointer on the stack

	// Reposition the argument pointer
	HACK_ASM("@" << (numArgs + 5)); // Load the argument offset
	HACK_ASM("D=A"); // Store the argument offset in the D register
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("M=M+1"); // Increment and load the stack pointer in the A register
	HACK_ASM("D=M-D"); // Calculate the argument pointer position
	HACK_ASM("@ARG"); // Load the argument register
	HACK_ASM("M=D"); // Set the new argument pointer

	// Reposition the local pointer
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("D=M"); // Load the stack pointer in the D register
	HACK_ASM("@LCL"); // Load the local register
	HACK_ASM("M=D"); // Set the new local pointer

	// Jump to the function
	HACK_ASM("@" << functionName);
	HACK_ASM("0;JMP");

	// Create a label for the return address
	HACK_ASM("(return-" << mReturnLabelCounter++ << ")"); 
}

void CodeWriter::writeReturn()
{
	HACK_ASM("@LCL"); // Load the local register
	HACK_ASM("D=M"); // Load the local register content in the D register
	HACK_ASM("@R13"); // Load the R13 register. FRAME variable
	HACK_ASM("MD=D"); // Store the local register content in the R13 register and D register
	HACK_ASM("@5"); // Load 5 in the A register
	HACK_ASM("A=D-A"); // Substract 5 from the FRAME
	HACK_ASM("D=M"); // Load the return address in the D register
	HACK_ASM("@R14"); // Load the R14 register. RET variable
	HACK_ASM("M=D"); // Store the return address in R14
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("A=M-1"); // Load the stack pointer address - 1 in the A register
	HACK_ASM("D=M"); // Store the top stack element in the D register
	HACK_ASM("@ARG"); // Load the argument register
	HACK_ASM("A=M"); // Load the argument pointer in the A register
	HACK_ASM("M=D"); // Load the return value in the ARG register, which is the top of the stack when returned from this function
	HACK_ASM("@ARG"); // Load the argument register
	HACK_ASM("D=M+1"); // Load the argument register content + 1, which will be the new stack pointer address
	HACK_ASM("@SP"); // Load the stack pointer register
	HACK_ASM("M=D"); // Store the new stack pointer address
	HACK_ASM("@13"); // Load the FRAME register
	HACK_ASM("AM=M-1"); // Load the saved THAT register content from the FRAME
	HACK_ASM("D=M"); // Store the content from the saved THAT register in the D register
	HACK_ASM("@THAT"); // Load the THAT register
	HACK_ASM("M=D"); // Restore the old THAT value in the THAT register
	HACK_ASM("@13"); // Load the FRAME register
	HACK_ASM("AM=M-1"); // Load the saved THIS register content from the FRAME
	HACK_ASM("D=M"); // Store the content from the saved THIS register in the D register
	HACK_ASM("@THIS"); // Load the THIS register
	HACK_ASM("M=D"); // Restore the old THIS value in the THIS register
	HACK_ASM("@13"); // Load the FRAME register
	HACK_ASM("AM=M-1"); // Load the saved argument register content from the FRAME
	HACK_ASM("D=M"); // Store the content from the saved argument register in the D register
	HACK_ASM("@ARG"); // Load the argument register
	HACK_ASM("M=D"); // Restore the old argument value in the argument register
	HACK_ASM("@13"); // Load the FRAME register
	HACK_ASM("AM=M-1"); // Load the saved local register content from the FRAME
	HACK_ASM("D=M"); // Store the content from the saved local register in the D register
	HACK_ASM("@LCL"); // Load the local register
	HACK_ASM("M=D"); // Restore the old local value in the local register
	HACK_ASM("@R14"); // Load the return address in the A register
	HACK_ASM("A=M;JMP"); // Jump to the return address
}

void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
	mCurrentFunctionName = functionName;
	HACK_ASM("(" << mCurrentFunctionName << ")");
	if (numLocals < 1) return;

	HACK_ASM("@SP"); // Load the stack pointer
	HACK_ASM("D=M"); // Store the pointer in the D register
	HACK_ASM("@R13"); // Load the R13 register
	HACK_ASM("M=D"); // Assign the stack pointer to the R13 register
	HACK_ASM("@" << numLocals); // Load the amount of locals as a number in the A register
	HACK_ASM("D=A"); // Load the amount of locals in the D register
	HACK_ASM("@SP"); // Load the stack pointer
	HACK_ASM("M=D+M"); // Increase the stack pointer by the amount of local variables
	HACK_ASM("@R13"); // Load the R13 register
	HACK_ASM("A=M"); // Load the old stack pointer in the A register
	int i = 1;
	while (1) {
		HACK_ASM("M=0"); // Initialize all locals to 0
		if (i >= numLocals) break;

		HACK_ASM("A=A+1"); // Increment the stack pointer
		i++;
	}
}

void CodeWriter::logFatal(std::string message) const
{
	cerr << "Error in file '" << mParser->getFileName() << ".vm'. Syntax error on line " << mParser->getCurrentLineNumber() << ": " << message << endl;
}

const std::array<std::string, 9> const CodeWriter::arithmeticCommands = {
	"add", "sub", "neg", "eq", "gt",
	"lt", "and", "or", "not",
};
