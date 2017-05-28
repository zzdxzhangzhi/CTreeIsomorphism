#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "CRootedTree.h"

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::flush;
using std::bind;
using std::find_if;
using std::find;
using std::find_first_of;
using namespace std::placeholders;


string& trim(string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" \t\r\n\v\f"));
	s.erase(s.find_last_not_of("  \t\r\n\v\f") + 1);
	return s;
}

void split(const string &origStr, const string &delim, vector<string> &resStr)
{
	size_t last = 0;
	size_t index = origStr.find_first_of(delim, last);
	while (index != string::npos)
	{
		resStr.push_back(origStr.substr(last, index - last));
		last = index + 1;
		index = origStr.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		resStr.push_back(origStr.substr(last, index - last));
	}
}

void getTreesFromLabels(const vector<string> &serialStrs, vector<CRootedTree *> &rootedTrees)
{
	vector<int> labelSerials;
	vector<string>::const_iterator iter = serialStrs.cbegin();
	for (; iter != serialStrs.cend(); iter++)
	{
		//cout << *iter << " ";
		labelSerials.push_back(atoi(iter->c_str()));
	}
	//cout << endl;

	rootedTrees.push_back(new CRootedTree(labelSerials));

	//printTree(*rootedTrees.back());
}

void outputTrees(vector<CRootedTree *> &rootedTrees)
{
	vector<int> treeClasses(rootedTrees.size(), 0);
	int treeClassesMax = treeClasses[0];

	cout << treeClasses[0] << " ";
	bool (*pred)(const CRootedTree *, const CRootedTree *) = &isIsomorphism;
	for (size_t j = 1; j < rootedTrees.size(); j++)
	{
		vector<CRootedTree *>::iterator iter = rootedTrees.begin();
		iter = find_first_of(iter, iter + j, iter + j, iter + (j + 1), pred);
		size_t i = iter - rootedTrees.begin();
		//cout << "(" << i << ", " << j << ")";
		if (i < j)
			treeClasses[j] = treeClasses[i];
		else
			treeClasses[j] = ++treeClassesMax;

		cout << treeClasses[j] << " ";

		/*for (int i = 0; i < j; i++)
		{

		}*/
	}
	cout << endl;
}

int main()
{
	string strInput;

	int scenarioNo = 0;
	while (getline(cin, strInput))
	{
		string line = trim(strInput);
		if (line == "0")
			break;		

		int treesNum = atoi(line.c_str());

		vector<CRootedTree *> rootedTrees;
		for (int i = 0; i < treesNum; i++)
		{
			strInput.clear();
			line.clear();

			getline(cin, strInput);
			line = trim(strInput);
			
			vector<string> serialStrs;
			split(line, " \t\r\n\f\v", serialStrs);
		
			getTreesFromLabels(serialStrs, rootedTrees);
		}
		
		cout << scenarioNo << ": " << flush;
		outputTrees(rootedTrees);
		
		scenarioNo++;
	}

	return 0;
}