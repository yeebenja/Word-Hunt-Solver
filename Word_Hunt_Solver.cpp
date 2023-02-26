// Word Hunt Solver (for GamePigeon)
// By: Benjamin Yee (Yu Anmin / Yeo Anmin)
// Email: yeebenja@umich.edu
// LinkedIn: https://www.linkedin.com/in/yeebenja/
// GitHub: https://github.com/yeebenja
// Project Identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cctype>
#include <getopt.h>

using namespace std;

class File_Not_Open{ /*...*/ };

enum class Cardinal_Output_Mode { kNone = 0, k_Cardinal_Mode_On, };
enum class Index_Output_Mode { kNone = 0, k_Index_Mode_On, };
enum class Point_Output_Mode { kNone = 0, k_Point_Mode_On, };
enum class Alpha_Output_Mode { kNone = 0, k_Alpha_Mode_On, };
enum class Search_Depth_Output_Mode { kNone = 0, k_Search_Depth_Mode_On, };
enum class Board_Mode { kNone = 0, k_Board_Mode_On, };


struct Options {
	Cardinal_Output_Mode cardinal_output_mode = Cardinal_Output_Mode::kNone;
	Index_Output_Mode index_output_mode = Index_Output_Mode::kNone;
	Point_Output_Mode point_output_mode = Point_Output_Mode::kNone;
	Alpha_Output_Mode alpha_output_mode = Alpha_Output_Mode::kNone;
	Search_Depth_Output_Mode search_depth_output_mode = Search_Depth_Output_Mode::kNone;
	Board_Mode board_mode = Board_Mode::kNone;
	int search_depth = 6;					// Default Search Depth value is 6
	string board_filename = "board.txt";	// Default Board filename is "board.txt"
};


// Word Hunt Solver Class
class Word_Hunt_Solver {
private:
	vector<vector<char>> board;
	vector<string> list_of_words;
	int width = 0;
	int height = 0;
	bool cardinal_on = false;
	bool index_on = false;
	bool point_on = false;
	bool alpha_on = false;
	bool search_depth_on = false;
	bool board_on = false;
	int search_depth = 6;				// Default Search Depth value is 6
	string board_filename = "board.txt";// Default Board filename is "board.txt"
	
	struct Found_Word {
		string found_word = "";
		vector<char> cardinal_directions;
		vector<int> index_directions;
		unsigned int length = 0;
	};

	
	// EFFECTS: Given character, returns capitolized 
	// version of the character
	char capitol(const char& c) {
		if (c >= 65 && c <= 90) return c;
		else if (c >= 97 && c <= 122) return c - 32;
		else {
			cout << "Invalid character\n";
			assert(false);
		}
		return 0;
	}
	
	// EFFECTS: Creates list of valid words
	// Time Complexity: O(n)
	void create_list(void) {
		ifstream file;
		// File can be found here: https://drive.google.com/file/d/1oGDf1wjWp5RF_X9C7HoedhIWMh5uJs8s/view
		file.open("Collins_Scrabble_Words_(2019).txt");
		// Throw exception if file is not open
		File_Not_Open error1;
		if (file.is_open() == false) throw error1;
		string str_temp;
		getline(file, str_temp);
		while (file >> str_temp) {
			list_of_words.push_back(str_temp);
		}
		file.close();
	}

public:
	// Defualt Constructor
	Word_Hunt_Solver(const Options& options) : search_depth(6) {
		if (options.cardinal_output_mode == Cardinal_Output_Mode::k_Cardinal_Mode_On) cardinal_on = true;
		if (options.index_output_mode == Index_Output_Mode::k_Index_Mode_On) index_on = true;
		if (options.point_output_mode == Point_Output_Mode::k_Point_Mode_On) point_on = true;
		if (options.alpha_output_mode == Alpha_Output_Mode::k_Alpha_Mode_On) alpha_on = true;
		if (options.search_depth_output_mode == Search_Depth_Output_Mode::k_Search_Depth_Mode_On) search_depth_on = true;
		if (options.board_mode == Board_Mode::k_Board_Mode_On) board_on = true;
		// Change search_depth value if specified
		if (search_depth_on == true) search_depth = options.search_depth;
		if (board_on == true) board_filename = options.board_filename;
		create_list();
	}

	// EFFECTS: Top level module to run Word_Hunt_Solver
	void run() {
		create_board();
		print_board();
		run_solver();
	}
	

	// EFFECTS: Runs solver for each potential word in word list
	void run_solver(void) {
		for (auto word : list_of_words) {
			find_word(word);
		}
	}

	// EFFECTS: Given word, finds all occurances of word inside board
	void find_word(const string& word) {
		// Case 1: word is longer than search_depth
		if (word.length() > search_depth) return;
		bool word_is_apple = word == "APPLE" ? true : false;
	
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				// Case 2: character in board != first letter in word
				char current_letter = board[row][col];
				if (current_letter != word[0]) continue;
				// Case 3: character matches
				look(word, row, col);
			}
		}
	}

	// EFFECTS: Looks in all directions to find word
	void look(const string& word, int row, int col) {
		int depth = 0;
		int size_of_word = (int) word.length();
		vector<char> directions;
		pair<int, int> original_gangster;	// original ganster = original coordinates of word (row,col)
		original_gangster.first = row;
		original_gangster.second = col;
		look_recursive(word, row, col, depth, size_of_word, 'x', directions, original_gangster); // x is starting at beginning
		
	}


	void look_recursive(const string word, int row, int col, int depth, 
		int size_of_word, char prev, vector<char>& directions, const pair<int, int>& original_gangster) {

		directions.push_back(prev);

		// Base Case:
		// Dimensions invalid
		if (dimensions_valid(row, col) == false) {
			directions.pop_back();
			return;
		}
		// depth > search_depth
		if (depth > search_depth) {
			directions.pop_back();
			return;
		}
		// Not Found Case:
		if (board[row][col] != word[depth]) {
			directions.pop_back();
			return;
		}
		// Found Case:
		if (board[row][col] == word[word.length() - 1] && size_of_word - 1 == depth) {
			cout << word << endl;
			cout << "found!" << endl;

			// We have original coordinates and directions 
			// The dirty work is over :)


			return;
		}


		// Recursive Case
		if (prev == 'x') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'n') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'a') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'e') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'b') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
		}
		else if (prev == 's') {
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'c') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'w') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// Southeast
			look_recursive(word, row + 1, col + 1, depth + 1, size_of_word, 'b', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else if (prev == 'd') {
			// North
			look_recursive(word, row - 1, col, depth + 1, size_of_word, 'n', directions, original_gangster);
			// Northeast
			look_recursive(word, row - 1, col + 1, depth + 1, size_of_word, 'a', directions, original_gangster);
			// East
			look_recursive(word, row, col + 1, depth + 1, size_of_word, 'e', directions, original_gangster);
			// South
			look_recursive(word, row + 1, col, depth + 1, size_of_word, 's', directions, original_gangster);
			// Southwest
			look_recursive(word, row + 1, col - 1, depth + 1, size_of_word, 'c', directions, original_gangster);
			// West
			look_recursive(word, row, col - 1, depth + 1, size_of_word, 'w', directions, original_gangster);
			// Northwest
			look_recursive(word, row - 1, col - 1, depth + 1, size_of_word, 'd', directions, original_gangster);
		}
		else assert(false);
	}


	// EFFECTS: Given row and col returns true if dimensions are valid
	bool dimensions_valid(int row, int col) {
		if (row >= 0 && row < height && col >= 0 && col < width) return true;
		return false;
	}


	// EFFECTS: Creates character board
	void create_board(void) {
		ifstream board_file;
		board_file.open(board_filename);
		// Throw exception if boardfile is not open
		File_Not_Open error2;
		if (board_file.is_open() == false) throw error2;
		int w, h;
		board_file >> w >> h;
		width = w;
		height = h;
		char char_temp;
		board.resize(height, vector<char>(width));
		for (size_t row = 0; row < height; ++row) {
			for (size_t col = 0; col < width; ++col) {
				board_file >> char_temp;
				board[row][col] = capitol(char_temp);
			}
		}
	}
	
	// EFFECTS: Prints board
	void print_board(void) {
		assert(false);
	}
};





void getMode(int argc, char* argv[], Options& options) {
	// These are used with getopt_long()
	opterr = false; // Let us handle all error output for command line options
	int choice;
	int index = 0;
	option long_options[] = {
		{ "cardinal", no_argument, nullptr, 'c' },
		{ "index", no_argument, nullptr, 'i' },
		{ "point", no_argument, nullptr, 'p' },
		{ "alpha", no_argument, nullptr, 'a' },
		{ "search-depth", required_argument, nullptr, 's' },
		{ "board", required_argument, nullptr, 'b' },

	};  // long_options[]

	// TODO: Fill in the double quotes, to match the mode and help options.
	// Note: s->no arguments, q->no arguments, h->no arguements, o->required arguments
	while ((choice = getopt_long(argc, argv, "cipas:b:", long_options, &index)) != -1) {
		switch (choice) {
		case 'c': {
			options.cardinal_output_mode = Cardinal_Output_Mode::k_Cardinal_Mode_On;
			break;
		}
		case 'i': {
			options.index_output_mode = Index_Output_Mode::k_Index_Mode_On;
			break;
		}
		case 'p': {
			options.point_output_mode = Point_Output_Mode::k_Point_Mode_On;
			break;
		}
		case 'a': {
			options.alpha_output_mode = Alpha_Output_Mode::k_Alpha_Mode_On;
			break;
		}
		case 's': {
			options.search_depth_output_mode = Search_Depth_Output_Mode::k_Search_Depth_Mode_On;
			string arg{ optarg };
			options.search_depth = stoi(optarg);
			break;
		}
		case 'b': {
			options.board_mode = Board_Mode::k_Board_Mode_On;
			string arg{ optarg };
			options.board_filename = optarg;
			break;
		}
		default: {
			cerr << "error: invalid option" << endl;
			exit(1);
		}
		}
	}
}

// Main function
int main(int argc, char** argv) {
	
	
	Options i_got_options; // I can pass that {  } like stockton
	getMode(argc, argv, i_got_options); // just joshin ima spend this holiday locked in

	Word_Hunt_Solver solver_1(i_got_options);
	solver_1.run();

	return 0;
}