// Author: Timothy Brua
// Final Exam Extra Credit
// Course: COSC 2425
// Comments: None

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Important variables

fstream file;

// This is the line that the program is currently running on
int commandLine = 0;

class ACSLCompiler {
	
	// Stores any variable names
	vector<string> labels;
	// Keeps track of all variable values
	vector<int> variables;
	// Keeps track of all lines of the program (except for input)
	vector<string> lines;
	// Accumulator
	int ACC;
	// Counts the number of times print has been called
	int printCount;

	// A utility method for executing commands
	void command(string OPCODE, string LOC, string LABEL);

public:
	// Compiler constructor
	ACSLCompiler();
	// Parses lines
	void parse(string toParse);
	// Adds lines to vector 'lines'
	void addLine(string line, int index);
	// Returns number of lines in the program (except input)
	int getLines();
	// Returns the line at a specific line number
	string getLine(int index);

};

int main() {

	// Keeps track of the string we want to parse
	// Also acts as a temporary string holder
	string stringToParse = "";

	// Gathers input for the filename
	string filename = "";
	
	// Variable used to count lines
	int lineCount = 0;

	// Prompts filename from user
	cout << "Name of the file to open: " << endl;
	cin >> filename;

	// Creates a new compiler object, this is 
	// the main part of the program
	ACSLCompiler* compiler = new ACSLCompiler();

	file.open(filename);

	// Only continues if there are more lines in the file to grab
	while (getline(file, stringToParse)) {

		// Adds each individual line to vector lines, at
		// lineCount index
		compiler->addLine(stringToParse, lineCount);

		lineCount++;

		// If program detects the END opcode, ends the while loop
		if (stringToParse.find("END") != string::npos) {
			break;
		}
	}

	// Uses commandLine to run whatever line the program needs.
	// Not using a traditional for loop because the program can run lines multiple times,
	// Examples include: BE, BG, BL, and BU.
	for (commandLine = 0; commandLine < compiler->getLines(); commandLine++) {
		// Calls the compiler's parse command to run the line that commandLine is on
		compiler->parse(compiler->getLine(commandLine));
	}

	file.close();

	return 0;

}

// Compiler constructor

ACSLCompiler::ACSLCompiler() {
	// Initializes important compiler variables
	ACC = 0;
	printCount = 0;
	labels.resize(1);
	variables.resize(1);
}

// Takes a string and an int, inserts the line at the index
// in the vector 'lines'

void ACSLCompiler::addLine(string line, int index) {
	lines.push_back(line);
}

// Returns the amount of lines in the program

int ACSLCompiler::getLines() {
	return lines.size();
}

// Takes an int and returns the line at that index

string ACSLCompiler::getLine(int index) {
	return lines[index];
}

// Takes a string (a line of ACSL code) and parses that string into three
// parts, the LABEL, the OPCODE, and the LOC. After it parses it calls 
// command() to execute the line's operation

void ACSLCompiler::parse(string toParse) {

	// Important temporary variables
	string LABEL = "";
	string OPCODE = "";
	string LOC = "";

	// Keeps track of where the substring will begin
	int start = 0;
	
	// Important temporary variable, holds the individual
	// part, which it compares to specific criteria in order
	// to determine its role (LABEL, OPCODE, or LOC).
	string subStringToParse = "";
	
	// Runs through the toParse string, checks for a space (' '),
	// If found, creates a substring and compares values
	for (int i = 0; i < toParse.length(); i++) {
		if (toParse.at(i) == ' ') {

			// Creates a substring of the entire toParse string
			// The start location is the location of the last space (' ') + 1
			// i - start indicates the length of the substring
			subStringToParse = toParse.substr(start, i - start);

			// Checking if OPCODE has been set so we know LOC is next
			// If it is empty then we know this should be the LABEL
			if (OPCODE != "") {
				LOC = subStringToParse;
			}

			// Compares the substring to a predetermined OPCODE string,
			// if it matches it stores the OPCODE for future use
			if (subStringToParse == "LOAD") {
				OPCODE = "LOAD";
			}
			else if (subStringToParse == "STORE") {
				OPCODE = "STORE";
			}
			else if (subStringToParse == "ADD") {
				OPCODE = "ADD";
			}
			else if (subStringToParse == "SUB") {
				OPCODE = "SUB";
			}
			else if (subStringToParse == "MULT") {
				OPCODE = "MULT";
			}
			else if (subStringToParse == "DIV") {
				OPCODE = "DIV";
			}
			else if (subStringToParse == "BE") {
				OPCODE = "BE";
			}
			else if (subStringToParse == "BG") {
				OPCODE = "BG";
			}
			else if (subStringToParse == "BL") {
				OPCODE = "BL";
			}
			else if (subStringToParse == "BU") {
				OPCODE = "BU";
			}
			else if (subStringToParse == "END") {
				OPCODE = "END";
			}
			else if (subStringToParse == "READ") {
				OPCODE = "READ";
			}
			else if (subStringToParse == "PRINT") {
				OPCODE = "PRINT";
			}
			else if (subStringToParse == "DC") {
				OPCODE = "DC";
			}
			
			// If after the first pass OPCODE is empty, we know there is a LABEL
			// If OPCODE has been set, we know that this must be a LOC
			if (OPCODE == "") {
				LABEL = subStringToParse;
			}

			// Sets new start location at the index of the last space (' ') + 1
			start = i + 1;
		}
	}
	command(OPCODE, LOC, LABEL);
}

// Takes three strings, the OPCODE, LOC, and LABEL
// Compares the OPCODE to predefined OPCODEs, if it matches
// it carries out the operation of that OPCODE.

void ACSLCompiler::command(string OPCODE, string LOC, string LABEL) {

	// Creates a variable with the name LABEL and value LOC
	if (OPCODE == "DC") {
		// Loops through all labels, finds a new location, places the LABEL
		// in a specific index, resizes the vector, puts the value LOC
		// into that specific index of the 'variables' vector, and
		// also resizes the 'variables' vector
		for (int i = 0; i < labels.size(); i++) {
			if (labels[i] == "") {
				labels[i] = LABEL;
				labels.resize(labels.size()+1);
				variables[i] = stoi(LOC);
				variables.resize(variables.size()+1);
				break;
			}
		}
	}
	// Places the value of LOC into the ACC
	else if (OPCODE == "LOAD") {
		// If it detects an '=' it knows it will be a hard value and not a variable
		if (LOC.at(0) == '=') {
			// Converts the string in the LOC (after the '=') to an int and places it into the ACC
			ACC = stoi(LOC.substr(1, LOC.length() - 1));
		}
		// Otherwise loops through all labels, if it matches with one, it
		// takes the value from the same index of the 'variables' vector
		// and places it into the ACC
		else {
			for (int i = 0; i < labels.size(); i++) {
				if (LOC == labels[i]) {
					ACC = variables[i];
					break;
				}
			}
		}
	}
	// Takes a value from the ACC and puts it into a variable labeled LOC
	else if (OPCODE == "STORE") {
		// Loops through all labels, if it matches, places the contents
		// of the ACC into the variable
		for (int i = 0; i < labels.size(); i++) {
			if (labels[i] == LOC) {
				variables[i] = ACC;
				break;
			}
			// Otherwise, creates a new label and resizes both vectors
			else if (labels[i] == "") {
				labels[i] = LOC;
				labels.resize(labels.size()+1);
				variables[i] = ACC;
				variables.resize(variables.size()+1);
				break;
			}
		}
	}
	// Takes a value of LOC and adds it to the ACC
	else if (OPCODE == "ADD") {
		// If it detects an '=' it knows it will be a hard value and not a variable
		if (LOC.at(0) == '=') {
			// Converts the string in the LOC (after the '=') to an int and adds it to the ACC
			ACC += stoi(LOC.substr(1, LOC.length() - 1));
		}
		// Otherwise loops through all labels, if it matches with one, it
		// takes the value from the same index of the 'variables' vector
		// and adds it to the ACC
		else {
			for (int i = 0; i < labels.size(); i++) {
				if (labels[i] == LOC) {
					ACC += variables[i];
					break;
				}
			}
		}
		// Modulus all arithmetic operations so it doesn't go over 1 million (1,000,000)
		ACC %= 1000000;
	}
	// Takes a value of LOC and subtracts it from the ACC
	else if (OPCODE == "SUB") {
		// If it detects an '=' it knows it will be a hard value and not a variable
		if (LOC.at(0) == '=') {
			// Converts the string in the LOC (after the '=') to an int and subtracts it from the ACC
			ACC -= stoi(LOC.substr(1, LOC.length() - 1));
		}
		// Otherwise loops through all labels, if it matches with one, it
		// takes the value from the same index of the 'variables' vector
		// and subtracts it from the ACC
		else {
			for (int i = 0; i < labels.size(); i++) {
				if (labels[i] == LOC) {
					ACC -= variables[i];
					break;
				}
			}
		}
		// Modulus all arithmetic operations so it doesn't go over 1 million (1,000,000)
		ACC %= 1000000;
	}
	// Takes a value of LOC and multiplies it to the ACC
	else if (OPCODE == "MULT") {
		// If it detects an '=' it knows it will be a hard value and not a variable
		if (LOC.at(0) == '=') {
			// Converts the string in the LOC (after the '=') to an int and multiplies it to the ACC
			ACC *= stoi(LOC.substr(1, LOC.length() - 1));
		}
		// Otherwise loops through all labels, if it matches with one, it
		// takes the value from the same index of the 'variables' vector
		// and multiplies it to the ACC
		else {
			for (int i = 0; i < labels.size(); i++) {
				if (labels[i] == LOC) {
					ACC *= variables[i];
					break;
				}
			}
		}
		// Modulus all arithmetic operations so it doesn't go over 1 million (1,000,000)
		ACC %= 1000000;
	}
	// Takes a value of LOC and divides it from the ACC
	else if (OPCODE == "DIV") {
		// If it detects an '=' it knows it will be a hard value and not a variable
		if (LOC.at(0) == '=') {
			// If it is going to divide by zero, throws an error
			if (stoi(LOC.substr(1, LOC.length() - 1)) == 0) {
				throw invalid_argument("Cannot divide by zero");
			}
			// Converts the string in the LOC (after the '=') to an int and multiplies it to the ACC
			ACC /= stoi(LOC.substr(1, LOC.length() - 1));
		}
		// Otherwise loops through all labels, if it matches with one, it
		// takes the value from the same index of the 'variables' vector
		// and divides it from the ACC
		else {
			for (int i = 0; i < labels.size(); i++) {
				if (labels[i] == LOC) {
					// If it is going to divide by zero, throws an error
					if (variables[i] == 0) {
						throw invalid_argument("Cannot divide by zero");
					}
					ACC /= variables[i];
					break;
				}
			}
		}
		// Modulus all arithmetic operations so it doesn't go over 1 million (1,000,000)
		ACC %= 1000000;
	}
	// Takes a value of LOC and prints the matching LABEL's value
	// Also counts the number of times the program has printed
	else if (OPCODE == "PRINT") {
		printCount++;
		// Loops through all labels, if it matches, prints the contents
		// of the variable to the screen
		for (int i = 0; i < labels.size(); i++) {
			if (labels[i] == LOC) {
				cout << printCount << ": " << variables[i] << endl;
				break;
			}
		}
	}
	// Puts an integer input and puts it into LABEL
	else if (OPCODE == "READ") {

		// Tracks the user input
		int tempNum;

		// Gathers the input from the last line of the file
		file >> tempNum;

		// Modulus the number so it does not exceed 1000000
		tempNum %= 1000000;

		// Loops through all labels, if it matches, inputs the 
		// number into the label
		for (int i = 0; i < labels.size(); i++) {
			if (labels[i] == LOC) {
				variables[i] = tempNum;
			}
			// Otherwise, creates a new label and resizes both vectors
			else if (labels[i] == "") {
				labels[i] = LOC;
				labels.resize(labels.size()+1);
				variables[i] = tempNum;
				variables.resize(variables.size()+1);
				break;
			}
		}
	}
	// If the ACC == 0, jumps the program to the line where the label is LOC
	// Equal
	else if (OPCODE == "BE") {
		if (ACC == 0) {
			for (int i = 0; i < getLines(); i++) {
				// Only creates a substring with LOC length to find it faster
				if (lines[i].substr(0, LOC.length()) == LOC) {
					// Sets commandLine to i-1 because the loop in main will increment it again
					commandLine = i-1;
					break;
				}
			}
		}
	}
	// If the ACC > 0, jumps the program to the line where the label is LOC
	// Greater
	else if (OPCODE == "BG") {
		if (ACC > 0) {
			for (int i = 0; i < getLines(); i++) {
				// Only creates a substring with LOC length to find it faster
				if (lines[i].substr(0, LOC.length()+1) == LOC) {
					// Sets commandLine to i-1 because the loop in main will increment it again
				commandLine = i-1;
					break;
				}
			}
		}
	}
	// If the ACC < 0, jumps the program to the line where the label is LOC
	// Lower
	else if (OPCODE == "BL") {
		if (ACC < 0) {
			for (int i = 0; i < getLines(); i++) {
				// Only creates a substring with LOC length to find it faster
				if (lines[i].substr(0, LOC.length()+1) == LOC) {
					// Sets commandLine to i-1 because the loop in main will increment it again
					commandLine = i-1;
					break;
				}
			}
		}
	}
	// Jumps the program to the line where the label is LOC
	// Unconditional
	else if (OPCODE == "BU") {
		for (int i = 0; i < getLines(); i++) {
			// Only creates a substring with LOC length to find it faster
			if (lines[i].substr(0, LOC.length()) == LOC) {
				// Sets commandLine to i-1 because the loop in main will increment it again
				commandLine = i-1;
				break;
			}
		}
	}
}
