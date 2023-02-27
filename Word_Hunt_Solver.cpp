// Word Hunt Solver (for GamePigeon iMessage Word Hunt Game)
// By: Benjamin Yee (Yu Anmin / Yeo Anmin)
// Email: yeebenja@umich.edu
// LinkedIn: https://www.linkedin.com/in/yeebenja/
// GitHub: https://github.com/yeebenja

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cctype>
#include <getopt.h>
#include <queue>

using namespace std;

// Exceptions classes
class File_Not_Open{ /*...*/ };

// Option enumerated classes
enum class Cardinal_Output_Mode { kNone = 0, k_Cardinal_Mode_On, };
enum class Index_Output_Mode { kNone = 0, k_Index_Mode_On, };
enum class Point_Output_Mode { kNone = 0, k_Point_Mode_On, };
enum class Alpha_Output_Mode { kNone = 0, k_Alpha_Mode_On, };
enum class Search_Depth_Output_Mode { kNone = 0, k_Search_Depth_Mode_On, };
enum class Board_Mode { kNone = 0, k_Board_Mode_On, };
enum class Linear_Search_Only_Mode { kNone = 0, k_Linear_Search_Mode_On, };

// Options struct
struct Options {
	Cardinal_Output_Mode cardinal_output_mode = Cardinal_Output_Mode::kNone;
	Index_Output_Mode index_output_mode = Index_Output_Mode::kNone;
	Point_Output_Mode point_output_mode = Point_Output_Mode::kNone;
	Alpha_Output_Mode alpha_output_mode = Alpha_Output_Mode::kNone;
	Search_Depth_Output_Mode search_depth_output_mode = Search_Depth_Output_Mode::kNone;
	Board_Mode board_mode = Board_Mode::kNone;
	Linear_Search_Only_Mode linear_mode = Linear_Search_Only_Mode::kNone;
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
	bool linear_on = false;
	int search_depth = 6;				// Default Search Depth value is 6
	string board_filename = "board.txt";// Default Board filename is "board.txt"

	// Found Word struct
	struct Found_Word {
		string word = "";
		vector<char> cardinal_directions;
		vector<int> index_directions;
		unsigned int length = 0;
		pair<int, int> coordinates;
	};

	vector<Found_Word> found_word_vect;

	// String_Compare Functor
	class String_Compare {
	public:
		// EFFECTS: Returns true if a is alphabetically more than b
		// Returns false otherwise
		// NOTES: Just for clarification: words that start with 'A' will be 
		// prioritized more than words that start with 'B', since 'A' comes first
		// in the alphabet
		bool operator()(const string& a, const string& b) {
			return a > b;
		}
	};

	// Point_Compare Functor
	class Point_Compare {
	public:
		// EFFECTS: Returns true if points(a) < points(b)
		// False otherwise
		bool operator()(const Found_Word& a, const Found_Word& b){
			// Note: Length = points
			if (b.length > a.length) return true;
			else if (b.length < a.length) return false;
			else {
				// If a's and b's lenght is equal, than just compare
				// which string comes first alphabettically
				String_Compare c;
				return c(a.word, b.word);
			}
		}
	};

	// Name_Compare Functor
	class Name_Compare {
	public:
		// EFFECTS: Returns true if word a < word b (alphabetically)
		bool operator()(const Found_Word& a, const Found_Word& b) {
			String_Compare c;
			return c(a.word, b.word);
		}
	};

	priority_queue<Found_Word, vector<Found_Word>, Point_Compare> point_pq;
	priority_queue<Found_Word, vector<Found_Word>, Name_Compare> alpha_pq;
	
	// EFFECTS: Given character, returns capitalized 
	// version of the character
	char capital(const char& c) {
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

	// EFFECTS: Returns true if all directions are equal
	// Note: Start at directions[1] since directions[0] is always 'x'
	bool all_duplicates(const vector<char>& directions){
		if (directions.size() == 1) return true;
		char x = directions[1];
		//for (auto i : directions) {
		//	if (x != i) return false;
		//}
		for (int i = 1; i < directions.size(); ++i) {
			if (x != directions[i]) return false;
		}
		return true;
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
		if (options.linear_mode == Linear_Search_Only_Mode::k_Linear_Search_Mode_On) linear_on = true;
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
		results();
	}

	// EFFECTS: Outputs cardinal directions for each found word
	void cardinal_output(const Found_Word& found_word) {

		if (linear_on) {
			// check to see if all directions match
			if (all_duplicates(found_word.cardinal_directions) == true) {
				cout << found_word.word << "\n";
				int row = found_word.coordinates.first;
				int col = found_word.coordinates.second;
				cout << "Start at row " << row << " col " << col << ", go ";
				for (auto character : found_word.cardinal_directions) {
					switch (character) {
					case 'n':
						cout << "N ";
						break;
					case 'e':
						cout << "E ";
						break;
					case 's':
						cout << "S ";
						break;
					case 'w':
						cout << "W ";
						break;
					case 'a':
						cout << "NE ";
						break;
					case 'b':
						cout << "SE ";
						break;
					case 'c':
						cout << "SW ";
						break;
					case 'd':
						cout << "NW ";
						break;
					}
				}
				cout << "\n";
				return;
			}
			else return;
		}

		int row = found_word.coordinates.first;
		int col = found_word.coordinates.second;
		cout << "Start at row " << row << " col " << col << ", go ";
		for (auto character : found_word.cardinal_directions) {
			switch (character) {
			case 'n':
				cout << "N ";
				break;
			case 'e':
				cout << "E ";
				break;
			case 's':
				cout << "S ";
				break;
			case 'w':
				cout << "W ";
				break;
			case 'a':
				cout << "NE ";
				break;
			case 'b':
				cout << "SE ";
				break;
			case 'c':
				cout << "SW ";
				break;
			case 'd':
				cout << "NW ";
				break;
			}
		}
		cout << "\n";
	}
	
	// EFFECTS: Outputs indexes in order of each found word
	void index_output(const Found_Word& found_word) {
		if (linear_on) {
			// check to see if all directions match
			if (all_duplicates(found_word.cardinal_directions) == true) {
				cout << found_word.word << "\n";
				int row = found_word.coordinates.first;
				int col = found_word.coordinates.second;
				cout << "Start at row " << row << " col " << col << ", go to ";
				for (auto character : found_word.cardinal_directions) {
					switch (character) {
					case 'n':
						cout << "row " << --row << " col " << col << ", ";
						break;
					case 'e':
						cout << "row " << row << " col " << ++col << ", ";
						break;
					case 's':
						cout << "row " << ++row << " col " << col << ", ";
						break;
					case 'w':
						cout << "row " << row << " col " << --col << ", ";
						break;
					case 'a':
						cout << "row " << --row << " col " << ++col << ", ";
						break;
					case 'b':
						cout << "row " << ++row << " col " << ++col << ", ";
						break;
					case 'c':
						cout << "row " << ++row << " col " << --col << ", ";
						break;
					case 'd':
						cout << "row " << --row << " col " << --col << ", ";
						break;
					}
				}
				cout << "\n";
				return;
			}
			else return;
		}

		
		int row = found_word.coordinates.first;
		int col = found_word.coordinates.second;
		cout << "Start at row " << row << " col " << col << ", go to ";
		for (auto character : found_word.cardinal_directions) {
			switch (character) {
			case 'n':
				cout << "row " << --row << " col " << col << ", ";
				break;
			case 'e':
				cout << "row " << row << " col " << ++col << ", ";
				break;
			case 's':
				cout << "row " << ++row << " col " << col << ", ";
				break;
			case 'w':
				cout << "row " << row << " col " << --col << ", ";
				break;
			case 'a':
				cout << "row " << --row << " col " << ++col << ", ";
				break;
			case 'b':
				cout << "row " << ++row << " col " << ++col << ", ";
				break;
			case 'c':
				cout << "row " << ++row << " col " << --col << ", ";
				break;
			case 'd':
				cout << "row " << --row << " col " << --col << ", ";
				break;
			}
		}
		cout << "\n";
	}

	// EFFECTS: Outputs results based on command line options
	void results(void) {
		cout << "---Summary---\n\n";
		if (point_on) {
			cout << "---Sorted by Word Length---\n";
			if (linear_on == true) cout << "\n--Outputting Linear Solutions Only--\n";
			int current_length = 0;
			if (!point_pq.empty()) {
				current_length = point_pq.top().length;
				cout << "\n--Length: " << current_length << "--\n\n";
			}
			while (!point_pq.empty()) {
				Found_Word found_word_temp = point_pq.top();
				point_pq.pop();
				// Separate lengths when printing
				if (current_length != found_word_temp.length) {
					cout << "\n--Length: " << found_word_temp.length << "--\n\n";
					current_length = found_word_temp.length;
				}
				if (linear_on == false) cout << found_word_temp.word << "\n";
				if (cardinal_on) cardinal_output(found_word_temp);
				if (index_on) index_output(found_word_temp);
			}
			cout << "\n";
		}

		if (alpha_on) {
			cout << "---Sorted Alphabetically---\n\n";
			if (linear_on == true) cout << "--Outputting Linear Solutions Only--\n";
			while (!alpha_pq.empty()) {
				Found_Word found_word_temp = alpha_pq.top();
				alpha_pq.pop();
				if (linear_on == false) cout << found_word_temp.word << "\n";
				if (cardinal_on) cardinal_output(found_word_temp);
				if (index_on) index_output(found_word_temp);
			}
			cout << "\n";
		}

		if (point_on == false && alpha_on == false) {
			if (linear_on == true) cout << "--Outputting Linear Solutions Only--\n";
			for (auto found_word : found_word_vect) {
				//cout << found_word.word << "\n";
				if (linear_on == false) cout << found_word.word << "\n";
				if (cardinal_on) cardinal_output(found_word);
				if (index_on) index_output(found_word);
			}
			cout << "\n";
		}
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
		//bool word_is_apple = word == "APPLE" ? true : false; // for debugging purposes
	
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
		if (depth >= search_depth) {
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
			//cout << word << endl;
			//cout << "found!" << endl;

			// We have original coordinates and directions 
			// The dirty work is over :)
			Found_Word found_word;
			found_word.word = word;
			found_word.length = size_of_word;
			found_word.coordinates = original_gangster;
			if (cardinal_on) found_word.cardinal_directions = directions;
			
			found_word_vect.push_back(found_word);
			if (point_on) point_pq.push(found_word);
			if (alpha_on) alpha_pq.push(found_word);

			directions.pop_back();
			return;
		}


		// Recursive Case
		// Note: a = NE, b = SE, c = SW, d = NW
		switch (prev) {
		case 'x': {
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
			break;
		}
		case 'n': {
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
			break;
		}
		case 'a':{
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
			break;
		}
		case 'e': {
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
			break;
		}
		case 'b': {
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
			break;
		}
		case 's':{
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
			break;
		}
		case 'c': {
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
			break;
		}
		case 'w':{
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
			break;
		}
		case 'd': {
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
			break;
		}
		default: {
			assert(false);
			break;
		}
		} // ...switch (prev)

		directions.pop_back();
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
				board[row][col] = capital(char_temp);
			}
		}
	}
	
	// EFFECTS: Prints board (taken from previous Word Search
	// Puzzle Solver Project)
	void print_board(void) {
		cout << "---Word Hunt Puzzle Board Inputted---\n";
		// Print column
		cout << " " << " " << " ";
		for (int col = 0; col < width; ++col) {
			if (col < 10) cout << col << "  ";
			else cout << col << " ";
		}
		cout << endl;
		for (int row = 0; row < height; ++row) {
			if (row < 10) cout << row << " " << " ";
			else cout << row << " ";
			for (int col = 0; col < width; ++col) {
				cout << capital(board[row][col]) << " " << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "--Search Depth: " << search_depth << "--\n\n";
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
		{ "linear", no_argument, nullptr, 'l'}						// lowercase 'L'

	};  // long_options[]

	// TODO: Fill in the double quotes, to match the mode and help options.
	// Note: s->no arguments, q->no arguments, h->no arguements, o->required arguments
	while ((choice = getopt_long(argc, argv, "cipas:b:l", long_options, &index)) != -1) {
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
		case 'l': {
			options.linear_mode = Linear_Search_Only_Mode::k_Linear_Search_Mode_On;
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