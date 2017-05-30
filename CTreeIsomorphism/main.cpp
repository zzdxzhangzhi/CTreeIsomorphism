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
using std::swap;
using std::sort;
using std::less;
using namespace std::placeholders;



class CRootedTree
{
public:
	CRootedTree() : m_root(nullptr), m_levels(0), m_order(0) {}
	CRootedTree(const vector<int> &labels);
	CRootedTree(const CRootedTree &ex_tree);
	CRootedTree& operator = (const CRootedTree &ex_tree);

	bool isIsomorphism(const CRootedTree &ex_tree);
	//bool isIsomorphism2(CRootedTree &ex_tree);

	bool isEmpty() const
	{
		return (m_root == nullptr);
	}

	int depth() const
	{
		return m_levels;
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
		int m_serialNo;
		bool m_isLeaf;
		string m_rep;
		vector<CTreeNode *> m_children;

		CTreeNode() : m_label(-1), m_serialNo(0), m_isLeaf(true), m_rep("10") {}
		CTreeNode(int label, int serialNo) 
			: m_label(label), m_serialNo(serialNo), m_isLeaf(true), m_rep("10") {}
		CTreeNode(const CTreeNode &ex_node);
	} *pCTreeNode;

	CTreeNode *m_root;
	vector<vector<CTreeNode*>> m_nodes;
	int m_levels;
	int m_order;

	static const int ROOT_LABEL = -1;

	pCTreeNode copyTreeNode(const pCTreeNode pnode);
	pCTreeNode buildTree(int iSerialNo, const vector<int> &labels);
	void generateNodeList();
	void clearNodes();
	//int getlevels(const vector<int> &labels);

	friend bool isIsomorphism(const pCTreeNode tnode1, const pCTreeNode tnode2);
	friend bool isIsomorphism(const CRootedTree *tree1, const CRootedTree *tree2);
	friend bool isIsomorphism2(CRootedTree *tree1, CRootedTree *tree2);
	//friend void printTree(const CRootedTree &rTree);
	//friend void printTreeNode(CTreeNode *pNode);

	//list<int> getLayout();
};

const int CRootedTree::ROOT_LABEL;

//bool isEachElemNumEqual(vector<int> v1, vector<int> v2);


CRootedTree::CTreeNode::CTreeNode(const CTreeNode &ex_node)
	: m_label(ex_node.m_label), m_serialNo(ex_node.m_serialNo), 
	m_isLeaf(ex_node.m_isLeaf), m_rep(ex_node.m_rep)
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

void CRootedTree::clearNodes()
{
	for (vector<vector<CTreeNode *>>::iterator iter = m_nodes.begin();
		iter != m_nodes.end(); iter++)
	{
		iter->clear();
	}
	m_nodes.clear();
}

void CRootedTree::generateNodeList()
{
	clearNodes();

	if (m_root != nullptr)
	{
		//m_nodes.resize(m_levels);
		vector<CTreeNode *> currLevelNodes(1, m_root);
		vector<CTreeNode *> nextLevelNodes;
		
		//pCTreeNode pNode = m_root;
		vector<CTreeNode *> *pNodes1 = &currLevelNodes;
		vector<CTreeNode *> *pNodes2 = &nextLevelNodes;
		while (!pNodes1->empty())
		{
			m_levels++;
			m_nodes.push_back(*pNodes1);
			for (vector<CTreeNode *>::const_iterator citer = pNodes1->cbegin();
				citer != pNodes1->cend(); citer++)
			{				
				pNodes2->clear();
				if (!((*citer)->m_isLeaf))
				{
					if (pNodes2->empty())
						pNodes2->assign((*citer)->m_children.cbegin(), (*citer)->m_children.cend());
					else
						pNodes2->insert(pNodes2->cend() - 1, (*citer)->m_children.cbegin(), (*citer)->m_children.cend());
				}
			}
			swap(pNodes1, pNodes2);
		}
		//cout << "m level: " << m_levels << endl;
	}
}

CRootedTree::CRootedTree(const CRootedTree &ex_tree)
	: m_levels(ex_tree.m_levels), m_order(ex_tree.m_order)
{
	m_root = copyTreeNode(ex_tree.m_root);
	generateNodeList();
}

CRootedTree::~CRootedTree()
{
	while (!m_root->m_children.empty())
	{
		pCTreeNode eachNode = m_root->m_children.front();
		delete eachNode;
		m_root->m_children.erase(m_root->m_children.begin());
	}

	clearNodes();
	m_root = nullptr;
	m_levels = 0;
	m_order = 0;
}

CRootedTree& CRootedTree::operator = (const CRootedTree &ex_tree)
{
	m_levels = ex_tree.m_levels;
	m_order = ex_tree.m_order;

	delete m_root;
	m_root = copyTreeNode(ex_tree.m_root);
	generateNodeList();

	return (*this);
}

CRootedTree::pCTreeNode CRootedTree::buildTree(int iSerialNo, const vector<int> &labels)
{
	pCTreeNode pnode = new CTreeNode(labels.at(iSerialNo), iSerialNo);
	//std::cout << "serials no: " << iSerialNo << " ";
	
	//tree_depth++;
	for (size_t i = 0; i < labels.size(); i++)
	{
		if (labels.at(i) == iSerialNo)
		{
			pnode->m_isLeaf = false;
			pCTreeNode pChild = buildTree(i, labels);
			//pnode->m_rep.insert(pnode->m_rep.length() - 2, pChild->m_rep);
			pnode->m_children.push_back(pChild);
		}
	}
	//std::cout << "serials no: " << iSerialNo << " " << endl;
	return pnode;
}

//int CRootedTree::getlevels(const vector<int> &labels)
//{
//	remove
//}

CRootedTree::CRootedTree(const vector<int> &labels)
	:m_root(nullptr), m_levels(0), m_order(labels.front())
{
	if (!labels.empty())
	{
		vector<int>::const_iterator iter =
			find(labels.cbegin(), labels.cend(), ROOT_LABEL);

		int rootSerialNo = iter - (labels.cbegin() + 1);
		//std::cout << "root no: " << rootSerialNo << std::endl;
		m_root = buildTree(rootSerialNo,
			vector<int>(labels.cbegin() + 1, labels.cend()));
		//std::cout << std::endl;
		generateNodeList();
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

bool isIsomorphism2(CRootedTree *tree1, CRootedTree *tree2)
{
	if (tree1->m_root == nullptr && tree2->m_root == nullptr)
		return true;
	else if (tree1->m_root == nullptr || tree2->m_root == nullptr)
		return false;
	else if (tree1->m_levels != tree2->m_levels)
		return false;
	else
	{
		int h = tree1->m_levels;
		for (int i = h - 1; i >= 0; i--)
		{
			if (tree1->m_nodes[i].size() != tree2->m_nodes[i].size())
				return false;

			vector<string> t1LevelStrs, t2LevelStrs;
			vector<CRootedTree::CTreeNode *>::iterator nodeIt1 = tree1->m_nodes[i].begin();
			vector<CRootedTree::CTreeNode *>::iterator nodeIt2 = tree2->m_nodes[i].begin();
			for (; nodeIt1 != tree1->m_nodes[i].end() && nodeIt2 != tree2->m_nodes[i].end(); 
				nodeIt1++, nodeIt2++)
			{
				if (!((*nodeIt1)->m_isLeaf))
				{
					vector<CRootedTree::CTreeNode *>::const_reverse_iterator criter 
						= (*nodeIt1)->m_children.crbegin();
					for (; criter != (*nodeIt1)->m_children.crend(); criter++)
					{
						(*nodeIt1)->m_rep.insert(1, (*criter)->m_rep);
					}
				}
				t1LevelStrs.push_back((*nodeIt1)->m_rep);
				t2LevelStrs.push_back((*nodeIt2)->m_rep);
			}

			sort(t1LevelStrs.begin(), t1LevelStrs.end());
			sort(t2LevelStrs.begin(), t2LevelStrs.end());
			if (!equal(t1LevelStrs.cbegin(), t1LevelStrs.cend(),
				t2LevelStrs.cbegin(), t2LevelStrs.cend()))
				return false;

			for (size_t j = 0; j < t1LevelStrs.size(); j++)
			{
				tree1->m_nodes[i][j]->m_rep.assign(t1LevelStrs[j]);
				tree2->m_nodes[i][j]->m_rep.assign(t2LevelStrs[j]);
			}
		}

		return true;
	}
}

bool CRootedTree::isIsomorphism(const CRootedTree &ex_tree)
{
	return ::isIsomorphism(this, &ex_tree);
}

//bool CRootedTree::isIsomorphism2(CRootedTree &ex_tree)
//{
//	return ::isIsomorphism2(this, &ex_tree);
//}

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
	//bool (*pred)(const CRootedTree *, const CRootedTree *) = &isIsomorphism;
	bool(*pred)(CRootedTree *, CRootedTree *) = &isIsomorphism2;
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
	//bool(*pred)(const CRootedTree *, const CRootedTree *) = &isIsomorphism;
	bool(*pred)(CRootedTree *, CRootedTree *) = &isIsomorphism2;
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