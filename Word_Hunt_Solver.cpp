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

using namespace std;

// List of Words Class
class List_of_Words {
private:
	map<string, int> list_of_words;
public:
	// EFFECTS: Creates list of valid words
	// Time Complexity: O(n)
	void create_list(void) {
		ifstream file;
		file.open("Collins Scrabble Words (2019).txt");
		string str_temp;
		getline(file, str_temp);
		while (file >> str_temp) {
			list_of_words[str_temp] += 1;
		}
		file.close();
	}

	// EFFECTS: Given a string, returns true if string is in list_of_words.
	// Returns false if string if string is not in list_of_words.
	// Time Complexity: O(1)
	bool in_list(string str) { 
		if (list_of_words[str] == 1) return true;
		return false;
	}
};




int main() {
	List_of_Words list;
	list.create_list();
	
	
	cout << list.in_list("APPLE") << endl;
	
	return 0;
}