#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::list;
using std::endl;
using std::flush;
using std::bind;
using std::find_if;
using std::find;
using std::find_first_of;
using std::fprintf;
using namespace std::placeholders;



class CRootedTree
{
public:
	CRootedTree() : m_root(nullptr), m_depth(0), m_order(0) {}
	CRootedTree(const vector<int> &labels);
	CRootedTree(const CRootedTree &ex_tree);
	CRootedTree& operator = (const CRootedTree &ex_tree);

	bool isIsomorphism(const CRootedTree &ex_tree);

	bool isEmpty() const
	{
		return (m_root == nullptr);
	}

	int depth() const
	{
		return m_depth;
	}

	int order() const
	{
		return m_order;
	}

	virtual ~CRootedTree();
	//protected:
private:
	typedef class CTreeNode
	{
	public:
		int m_label;
		vector<CTreeNode *> m_children;

		CTreeNode() : m_label(-1) {}
		CTreeNode(int label) : m_label(label) {}
		CTreeNode(const CTreeNode &ex_node);
	} *pCTreeNode;

	CTreeNode *m_root;
	int m_depth;
	int m_order;

	static const int ROOT_LABEL = -1;

	pCTreeNode copyTreeNode(const pCTreeNode pnode);
	pCTreeNode buildTree(int iSerialNo, const vector<int> &labels, int &tree_depth);

	friend bool isIsomorphism(const pCTreeNode tnode1, const pCTreeNode tnode2);
	friend bool isIsomorphism(const CRootedTree *tree1, const CRootedTree *tree2);
	//friend void printTree(const CRootedTree &rTree);
	//friend void printTreeNode(CTreeNode *pNode);

	//list<int> getLayout();
};

const int CRootedTree::ROOT_LABEL;

//bool isEachElemNumEqual(vector<int> v1, vector<int> v2);


CRootedTree::CTreeNode::CTreeNode(const CTreeNode &ex_node)
	: m_label(ex_node.m_label)
{
	if (!m_children.empty())
	{
		vector<CTreeNode *>::const_iterator iter = ex_node.m_children.cbegin();
		vector<CTreeNode *>::const_iterator lEnd = ex_node.m_children.cend();
		for (; iter != lEnd; iter++)
		{
			m_children.push_back(new CTreeNode(*(*iter)));
		}
	}
}

CRootedTree::pCTreeNode CRootedTree::copyTreeNode(const pCTreeNode pnode)
{
	pCTreeNode res_node = new CTreeNode(*pnode);
	return res_node;
}

CRootedTree::CRootedTree(const CRootedTree &ex_tree)
	: m_depth(ex_tree.m_depth), m_order(ex_tree.m_order)
{
	m_root = copyTreeNode(ex_tree.m_root);
}

CRootedTree::~CRootedTree()
{
	while (!m_root->m_children.empty())
	{
		pCTreeNode eachNode = m_root->m_children.front();
		delete eachNode;
		m_root->m_children.erase(m_root->m_children.begin());
	}

	m_root = nullptr;
	m_depth = 0;
	m_order = 0;
}

CRootedTree& CRootedTree::operator = (const CRootedTree &ex_tree)
{
	m_depth = ex_tree.m_depth;
	m_order = ex_tree.m_order;

	delete m_root;
	m_root = copyTreeNode(ex_tree.m_root);

	return (*this);
}

CRootedTree::pCTreeNode CRootedTree::buildTree(int iSerialNo,
	const vector<int> &labels, int &tree_depth)
{
	++tree_depth;
	pCTreeNode pnode = new CTreeNode(labels.at(iSerialNo));
	//std::cout << "serials no: " << iSerialNo << " ";
	for (size_t i = 0; i < labels.size(); i++)
	{
		if (labels.at(i) == iSerialNo)
			pnode->m_children.push_back(buildTree(i, labels, tree_depth));
	}

	return pnode;
}

CRootedTree::CRootedTree(const vector<int> &labels)
	:m_root(nullptr), m_depth(0), m_order(labels.front())
{
	if (!labels.empty())
	{
		vector<int>::const_iterator iter =
			find(labels.cbegin(), labels.cend(), ROOT_LABEL);

		int rootSerialNo = iter - (labels.cbegin() + 1);
		//std::cout << "root no: " << rootSerialNo << std::endl;
		m_root = buildTree(rootSerialNo,
			vector<int>(labels.cbegin() + 1, labels.cend()),
			m_depth);
		//std::cout << std::endl;
	}
}

bool isIsomorphism(const CRootedTree::pCTreeNode tnode1, const CRootedTree::pCTreeNode tnode2)
{
	if (tnode1 == nullptr && tnode2 == nullptr)
		return true;
	else if (tnode1 == nullptr || tnode2 == nullptr)
		return false;
	else if (tnode1->m_children.empty() && tnode2->m_children.empty())
		return true;
	else if (tnode1->m_children.empty() || tnode2->m_children.empty())
		return false;
	else if (tnode1->m_children.size() != tnode2->m_children.size())
		return false;
	else
	{
		bool(*pred)(const CRootedTree::pCTreeNode, const CRootedTree::pCTreeNode) = &isIsomorphism;
		int isomorphismCount = 0;
		vector<CRootedTree::CTreeNode *>::const_iterator iter1 = tnode1->m_children.cbegin();
		while ((iter1 = std::find_first_of(iter1, tnode1->m_children.cend(),
			tnode2->m_children.cbegin(), tnode2->m_children.cend(),
			pred)) != tnode1->m_children.cend())
		{
			isomorphismCount++;
			++iter1;
		}
		//std::cout << "isomorphismCount: " << isomorphismCount << " " << "children size: " << tnode1->m_children.size() << " ";
		return (isomorphismCount == tnode1->m_children.size());

		/*for (list<CTreeNode *>::const_iterator iter1 = tnode1->m_children.cbegin();
		iter1 != m_root->m_children.cend(); iter1++)
		{
		bool isChildIsomorphism = false;
		for (list<CTreeNode *>::const_iterator iter2 = tnode2->m_children.cbegin();
		iter1 != tnode2->m_children.cend(); iter2++)
		{
		if (isChildIsomorphism |= isIsomorphism(*iter1, *iter2))
		break;
		}

		if (!isChildIsomorphism)
		return false;
		}

		return true;*/
	}

}

bool isIsomorphism(const CRootedTree *tree1, const CRootedTree *tree2)
{
	if (tree1->m_root == nullptr && tree2->m_root == nullptr)
		return true;
	else if (tree1->m_root == nullptr || tree2->m_root == nullptr)
		return false;
	else
		return ::isIsomorphism(tree1->m_root, tree2->m_root);
}

bool CRootedTree::isIsomorphism(const CRootedTree &ex_tree)
{
	return ::isIsomorphism(this, &ex_tree);
}

//void printTreeNode(CRootedTree::CTreeNode *pNode)
//{
//	if (pNode != nullptr)
//	{
//		cout << pNode->m_label << " ";
//		if (!pNode->m_children.empty())
//		{
//			for each (CRootedTree::CTreeNode * pChildNode in pNode->m_children)
//			{
//				printTreeNode(pChildNode);
//			}
//		}
//		//std::cout << std::endl;
//	}
//}
//
//void printTree(const CRootedTree &rTree)
//{
//	if (!rTree.isEmpty())
//	{
//		printTreeNode(rTree.m_root);
//	}
//}


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
	}
	cout << endl;
}

void outputTreesData(vector<CRootedTree *> &rootedTrees, vector<int> &scenario)
{
	vector<int> treeClasses(rootedTrees.size(), 0);
	int treeClassesMax = treeClasses[0];

	scenario.push_back(treeClasses[0]);
	bool(*pred)(const CRootedTree *, const CRootedTree *) = &isIsomorphism;
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

		scenario.push_back(treeClasses[j]);
	}

	/*fprintf(stdout, "%d: ", scenario.at(0));
	for (size_t j = 1; j < scenario.size(); j++)
	{
		fprintf(stdout, "%d ", scenario.at(j));
	}
	fprintf(stdout, "\n");*/
}

void outputResult(const vector<vector<int>> &scenarios)
{
	for (size_t i = 0; i < scenarios.size(); i++)
	{
		fprintf(stdout, "%d: ", scenarios[i].at(0));
		for (size_t j = 1; j < scenarios[i].size(); j++)
		{
			fprintf(stdout, "%d ", scenarios[i].at(j));
		}
		fprintf(stdout, "\n");
	}
}

int main()
{
	string strInput;

	int scenarioNo = 0;
	vector<vector<int>> scenarios;
	while (getline(cin, strInput))
	{
		if (strInput == "")
			continue;

		string line = trim(strInput);
		if (line == "0")
			break;		

		int treesNum = atoi(line.c_str());

		vector<CRootedTree *> rootedTrees;
		for (int i = 0; i < treesNum; i++)
		{
			/*strInput.clear();
			line.clear();*/

			getline(cin, strInput);
			line = trim(strInput);
			
			vector<string> serialStrs;
			split(line, " \t\r\n\f\v", serialStrs);
		
			getTreesFromLabels(serialStrs, rootedTrees);
		}

		vector<int> scenario;
		scenario.push_back(scenarioNo);
		outputTreesData(rootedTrees, scenario);
		/*cout << scenarioNo << ": " << flush;
		outputTrees(rootedTrees);*/
		
		scenarios.push_back(scenario);
		scenarioNo++;
	}

	outputResult(scenarios);

	return 0;
}