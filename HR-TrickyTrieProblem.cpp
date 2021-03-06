// HR-TrickyTrieProblem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <numeric>
using std::cin;	using std::cout;
using std::vector;	using std::string;
using std::numeric_limits;	using std::streamsize;
/*
* Complete the contacts function below.
*/
struct Trienode {
	char data;
	bool isAWord = false;
	Trienode *children[26]{ nullptr };
	Trienode(char c) : data(c) { }
	Trienode *getChild(char c) {
		if (c < 'a' || c > 'z') return nullptr;
		if (children[c - 'a'] == nullptr) children[c - 'a'] = new Trienode(c);
		return children[c - 'a'];
	}
	Trienode *findChild(char c) const {
		// similar to getChild but we don't add one if not found, just return nullptr
		if (c < 'a' || c > 'z') return nullptr;
		return children[c - 'a'];
	}
	int countOfCompleteWordsWithStem() const {
		int total = 0;
		for (auto kid : children) {
			if (kid != nullptr) total += kid->countOfCompleteWordsWithStem();
			if (isAWord) ++total;
		}
		return total;
	}
};

	vector<int> contacts(vector<vector<string>> queries) {
		/*
		* results will be pushed back into "results" vector for each find
		*/
		vector<int> results;
		if (queries.size() == 0 || queries[0].size() < 2) return results;
		Trienode *pTrieRoot = new Trienode('?');
		// each query is either a two element string vector "add <contact>" or "find <stem>"
		for (const auto q : queries) {
			Trienode *pCurrentNode = pTrieRoot;  // start at top for either add or find.
			if (q[0] == "add") {
				for (const auto ch : q[1]) pCurrentNode = pCurrentNode->getChild(ch);
				pCurrentNode->isAWord = true;
			}
			else {   //  "find" - nobody wants error checking at this level for this challenge
					 // we want to results.push_back( number of isAWord descendants of node 
					 // corresponding to last char in q[1] );
				auto it = cbegin(q[1]);
				while (it != cend(q[1])) {
					char ch = *it;
					pCurrentNode = pCurrentNode->getChild(ch);
					if (pCurrentNode == nullptr) {
						results.push_back(0);
						break;
					}
					++it;
				}
				if (it == cend(q[1]))  // we made it, now count child words from here
					results.push_back(pCurrentNode->countOfCompleteWordsWithStem());
			}
		}
		return results;
	}

	int main()
	{
		cin.sync_with_stdio(false);
		cout.sync_with_stdio(false);
		//ofstream fout(getenv("OUTPUT_PATH"));


		int queries_rows;
		cin >> queries_rows;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		vector<vector<string>> queries(queries_rows);
		for (int queries_row_itr = 0; queries_row_itr < queries_rows; queries_row_itr++) {
			queries[queries_row_itr].resize(2);

			for (int queries_column_itr = 0; queries_column_itr < 2; queries_column_itr++) {
				cin >> queries[queries_row_itr][queries_column_itr];
			}

			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		vector<int> result = contacts(queries);

		for (int result_itr = 0; result_itr < result.size(); result_itr++) {
			cout << result[result_itr];

			if (result_itr != result.size() - 1) {
				cout << "\n";
			}
		}

		cout << "\n";

		//fout.close();

		return 0;
	}

