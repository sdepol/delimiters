/////////////////////////////////////////////////////////////////////////
//ECE 150 - PROJECT 1
//
//File:			delimiters.cpp
//Author: 		Stephanie De Pol
//Date:			2019-10-09
//Program Objective: 	Read through a test file and identify where
//			delimiters are unmatched, missing, and/or not
//			nested properly.
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include "project_1.hpp"

// Function declarations
int main();
void print_program();
bool does_delimiter_match( char del );
bool do_delimeters_nest();

// Function definitions
int main() {
	print_program();

	std::cout << does_delimiter_match('(') << std::endl;
	std::cout << does_delimiter_match('[') << std::endl;
	std::cout << does_delimiter_match('{') << std::endl;
	std::cout << do_delimeters_nest() << std::endl;

	return 0;
}

/////////////////////////////////////////////////////////////////////////
//print_program
//
//File:			delimiters.cpp
//Author: 		Stephanie De Pol
//Date:			2019-10-09
//
//Description: 	Function reads test file one character at a time and
//				prints each character to the console
/////////////////////////////////////////////////////////////////////////
void print_program() {
	ece150::start_reading_file();

	while (ece150::at_eof() != true) {
		std::cout << ece150::get_next_character();
	}

}

/////////////////////////////////////////////////////////////////////////
//does_delimiter_match
//
//File:		delimiters.cpp
//Author: 	Stephanie De Pol
//Date:		2019-10-09
//
//Parameter del:	del can be any character value, however, no operations will
//					occur unless del is equal to '(','[', or '{'
//Return:			a boolean value depending on whether the delimiters match (true)
//					or do not match (false)
//
//Description:	Function keeps track of opening and closing delimiters by
//				using a counter (opening adds one to the counter while
//				closing subtracts one from the counter). Each time there
//				is a closing delimiter without an opening delimiter, the
//				counter will become negative and thus print out an
//				"unmatched" error message and reset the counter. Once the
//				whole file is read, a positive count indicated opening
//				delimiters without closing delimiters, the program then
//				prints a "missing" error message.
////////////////////////////////////////////////////////////////////////
bool does_delimiter_match(char del) {
	if (del != '(' && del != '[' && del != '{') {
		return false;
	}

	//assert operation guarantees that parameter del can only equal '(','[', or '{'
	assert(del == '(' || del == '[' || del == '{');

	//declaration of variables and function that reads through the test file
	int count = 0;
	char closing_del { };
	char nextchar { };
	ece150::start_reading_file();

	//if there are unmatched or missing delimiters, the delimiters do not match, therefore making the return value false
	//track unmatched and missing responses by setting this boolean variable false when error messages are printed
	bool delimiter_does_match = true;

	//declaring the parameter's corresponding closing delimiter
	if (del == '(') {
		closing_del = ')';
	} else if (del == '[') {
		closing_del = ']';
	} else {
		assert(del == '{');
		closing_del = '}';
	}

	//method used to search through each character in the test file for delimiters
	do {
		nextchar = ece150::get_next_character();

		//record each instance of a delimiter by adding or subtracting from the delimiter count
		if (nextchar == del) {
			++count;
		} else if (nextchar == closing_del) {
			--count;

			//if there are unmatched closing delimiters, the count will be negative
			if (count < 0) {
				std::cout << "Unmatched " << closing_del << std::endl;
				//reset value of count so unmatched delimiter does not negate a missing closing delimiter (AKA an extra opening delimiter)
				count = 0;

				//
				delimiter_does_match = false;
			}
		}
	//repeat process of recording delimiters until program reaches the end of the test file
	} while (ece150::at_eof() != true);

	//once all of the delimiters are accounted for, count will be positive if there are any remaining opening delimiters without closing delimiters
	if (count > 0) {
		std::cout << "Missing ";
		for (int x = 0; x < count; x++) {
			std::cout << closing_del;
		}
		std::cout << std::endl;

		delimiter_does_match = false;

	}

	// Returns proper return value based on whether the delimiters match or not
	if (delimiter_does_match == false) {
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
//do_delimeters_nest
//
//File:			delimiters.cpp
//Author: 		Stephanie De Pol
//Date:			2019-10-09
//
//Return:		a boolean value depending on whether the delimiters nest (true)
//				or do not (false)
//
//Description:	Function checks if brackets are properly nested by reading
//				through the test file and placing any opening delimiters in
// 				a table. When the program comes across a closing delimiter
//				that is nested properly, it will remove the corresponding
//				opening delimiter from the table. If the table is empty when
//				the function comes across a closing delimiter, it will print
// 				an unmatched error to the console. If there are remaining
// 				delimiters in the table once the function is finished reading
//				through the test file, it will print a missing error message
//				to the console.
////////////////////////////////////////////////////////////////////////
bool do_delimeters_nest() {

	//declaration of variables, delimiter table, and function that reads through the test file
	ece150::initialize_table();
	ece150::start_reading_file();
	char nextchar { };
	char rightmost_char_in_table { };

	//if there are unmatched or missing delimiters, the delimiters do not nest, therefore making the return value false
	//track unmatched and missing responses by setting this boolean variable false when error messages are printed
	bool delimiters_do_nest = true;

	do {
		nextchar = ece150::get_next_character();

		if (nextchar == '(' || nextchar == '[' || nextchar == '{') {
			//place any opening delimiters into table
			ece150::place_character(nextchar);
		} else if (nextchar == ')' || nextchar == ']' || nextchar == '}') {

			rightmost_char_in_table = ece150::get_right_most_character();

			//check for proper nesting in the table and remove any opening delimiters that are nested properly
			if (nextchar == ')' && rightmost_char_in_table == '(') {

				ece150::erase_right_most_character();
			} else if (nextchar == ']' && rightmost_char_in_table == '[') {

				ece150::erase_right_most_character();
			} else if (nextchar == '}' && rightmost_char_in_table == '{') {

				ece150::erase_right_most_character();
			} else {
				//error message for any closing delimiters without a corresponding opening delimiter
				std::cout << "Unmatched " << nextchar << std::endl;
				delimiters_do_nest = false;
			}

		}
	//repeat process of recording opening delimiters and checking for proper nesting until program reaches the end of the test file
	} while (ece150::at_eof() != true);

	//if there are still delimiters in the table, print a "missing" error message
	if (ece150::is_table_empty() != true) {
		std::cout << "Missing ";
		delimiters_do_nest = false;

		do {

			rightmost_char_in_table = ece150::get_right_most_character();

			//read table from right to left, printing the corresponding closing delimiter to each of the opening delimiters in the table
			//once closing delimiter is printed, delete the delimiter from the table
			if (rightmost_char_in_table == '(') {
				std::cout << ")";
				ece150::erase_right_most_character();
			} else if (rightmost_char_in_table == '{') {
				std::cout << "}";
				ece150::erase_right_most_character();
			} else if (rightmost_char_in_table == '[') {
				std::cout << "]";
				ece150::erase_right_most_character();
			}
		//continue process until table is empty
		} while (ece150::is_table_empty() != true);

		std::cout << std::endl;

	}

	return delimiters_do_nest;

}
