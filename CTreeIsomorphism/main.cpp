#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
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
using std::pair;
using std::make_pair;
//using std::less;
using namespace std::placeholders;



class CRootedTree
{
public:
	CRootedTree() : m_root(-1), m_levels(0), m_order(0) {}
	CRootedTree(const vector<int> &labels);
	CRootedTree(const CRootedTree &ex_tree);
	CRootedTree& operator = (const CRootedTree &ex_tree);

	//bool isIsomorphism(const CRootedTree &ex_tree);
	bool isIsomorphism2(CRootedTree &ex_tree);

	bool isEmpty() const
	{
		return (m_root == -1);
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
		//vector<CTreeNode *> m_children;
		vector<int> m_childrenSerials;

		CTreeNode() : m_label(-1), m_serialNo(0), m_isLeaf(true), m_rep("10") {}
		CTreeNode(int label, int serialNo) 
			: m_label(label), m_serialNo(serialNo), m_isLeaf(true), m_rep("10") {}
		CTreeNode(const CTreeNode &ex_node);
	} *pCTreeNode;

	//CTreeNode *m_root;
	int m_root;
	//vector<vector<CTreeNode*> > m_levelNodes;
	vector<vector<int> > m_levelNodeSerials;
	int m_levels;
	int m_order;
	vector<CTreeNode*> m_nodeList;

	static const int ROOT_LABEL = -1;

	pCTreeNode copyTreeNode(const pCTreeNode pnode);
	void copyTree(const CRootedTree &ex_tree);
	void buildTree(int iSerialNo, vector<pair<int, int> > &labels);
	void clearNodes();
	//void generateNodeList();
	//int getlevels(const vector<int> &labels);

	//friend bool isIsomorphism(const pCTreeNode tnode1, const pCTreeNode tnode2);
	//friend bool isIsomorphism(const CRootedTree *tree1, const CRootedTree *tree2);
	friend void assignNodeStr(CRootedTree::pCTreeNode tNode, CRootedTree *pTree);
	friend bool isIsomorphism2(CRootedTree *tree1, CRootedTree *tree2);
	//friend void printTree(const CRootedTree &rTree);
	//friend void printTreeNode(CTreeNode *pNode);

	//list<int> getLayout();
};

const int CRootedTree::ROOT_LABEL;

//bool isEachElemNumEqual(vector<int> v1, vector<int> v2);


CRootedTree::CTreeNode::CTreeNode(const CTreeNode &ex_node)
	: m_label(ex_node.m_label), m_serialNo(ex_node.m_serialNo), 
	m_isLeaf(ex_node.m_isLeaf), m_rep(ex_node.m_rep), 
	m_childrenSerials(ex_node.m_childrenSerials)
{
	/*if (!m_children.empty())
	{
		vector<CTreeNode *>::const_iterator iter = ex_node.m_children.cbegin();
		vector<CTreeNode *>::const_iterator lEnd = ex_node.m_children.cend();
		for (; iter != lEnd; iter++)
		{
			m_children.push_back(new CTreeNode(*(*iter)));
		}
	}*/
}

CRootedTree::pCTreeNode CRootedTree::copyTreeNode(const pCTreeNode pnode)
{
	pCTreeNode res_node = new CTreeNode(*pnode);
	return res_node;
}

void CRootedTree::copyTree(const CRootedTree &ex_tree)
{
	vector<CRootedTree::CTreeNode*>::const_iterator citer = ex_tree.m_nodeList.cbegin();
	for (; citer != ex_tree.m_nodeList.cend(); citer++)
	{
		m_nodeList.push_back(new CRootedTree::CTreeNode(*(*citer)));
	}
}

CRootedTree::CRootedTree(const CRootedTree &ex_tree)
	: m_root(ex_tree.m_root), m_levels(ex_tree.m_levels), m_order(ex_tree.m_order),
	m_levelNodeSerials(ex_tree.m_levelNodeSerials)
{
	//m_root = copyTreeNode(ex_tree.m_root);
	//generateNodeList();
	copyTree(ex_tree);
}

CRootedTree::~CRootedTree()
{
	/*while (!m_root->m_children.empty())
	{
		pCTreeNode eachNode = m_root->m_children.front();
		delete eachNode;
		m_root->m_children.erase(m_root->m_children.begin());
	}*/

	clearNodes();
	m_root = 0;
	m_levels = 0;
	m_order = 0;
}

CRootedTree& CRootedTree::operator = (const CRootedTree &ex_tree)
{
	clearNodes();

	m_root = ex_tree.m_root;
	m_levels = ex_tree.m_levels;
	m_order = ex_tree.m_order;

	copyTree(ex_tree);

	//delete m_root;
	//m_root = copyTreeNode(ex_tree.m_root);
	//generateNodeList();

	return (*this);
}

void CRootedTree::buildTree(int iSerialNo, vector<pair<int, int> > &labels)
{
	//std::cout << "serials no: " << iSerialNo << " " << endl;
	vector<pair<int, int> > currLevelLabels(1, labels.at(iSerialNo));
	vector<pair<int, int> > nextLevelLabels;
	vector<pair<int, int> > *pCurr = &currLevelLabels;
	vector<pair<int, int> > *pNext = &nextLevelLabels;
		
	//tree_depth++;
	while (!pCurr->empty())
	{
		m_levels++;

		vector<int> currLevelSerials;
		for (size_t i = 0; i < pCurr->size(); i++)
		{
			int currSerialNo = pCurr->at(i).second;
			//std::cout << "current serials no: " << currSerialNo << " " << endl;
			pCTreeNode pnode = new CTreeNode(pCurr->at(i).first, currSerialNo);
			m_nodeList[currSerialNo] = pnode;
			currLevelSerials.push_back(currSerialNo);

			vector<pair<int, int> >::const_iterator citer;
			int childCount = 0;
			while ((citer = find_if(labels.cbegin(), labels.cend(), [currSerialNo](pair<int, int> label) { return label.first == currSerialNo; })) != labels.cend())
			{
				childCount++;
				int childSerial = citer->second;
				pnode->m_childrenSerials.push_back(childSerial);
				pNext->push_back(*citer);

				labels.erase(citer);
			}

			//std::cout << "childs: " << childCount << " " << endl;
			if (childCount > 0)
				pnode->m_isLeaf &= false;
		}

		m_levelNodeSerials.push_back(currLevelSerials);

		swap(pCurr, pNext);
		pNext->clear();
	}
	//for (size_t i = 0; i < labels.size(); i++)
	//{
	//	if (labels.at(i) == iSerialNo)
	//	{
	//		pnode->m_isLeaf = false;
	//		//pnode->m_rep.insert(pnode->m_rep.length() - 2, pChild->m_rep);
	//		pnode->m_children.push_back(buildTree(i, labels));
	//	}
	//}
	//std::cout << "serials no: " << iSerialNo << " " << endl;
	//return pnode;
}

void CRootedTree::clearNodes()
{
	/*vector<vector<CRootedTree::CTreeNode*> >::iterator iter = m_levelNodes.begin();
	for (; iter != m_levelNodes.end(); iter++)
	{
		iter->clear();
	}
	m_levelNodes.clear();*/

	vector<CRootedTree::CTreeNode*>::iterator iter = m_nodeList.begin();
	for (; iter != m_nodeList.end(); iter++)
	{
		delete (*iter);
	}
	m_nodeList.clear();

	m_levelNodeSerials.clear();
}

//void CRootedTree::generateNodeList()
//{
//	clearNodes();
//
//	if (m_root != nullptr)
//	{
//		//m_levelNodes.resize(m_levels);
//		vector<CRootedTree::CTreeNode*> currLevelNodes(1, m_root);
//		vector<CRootedTree::CTreeNode*> nextLevelNodes;
//
//		//pCTreeNode pNode = m_root;
//		vector<CRootedTree::CTreeNode*> *pNodes1 = &currLevelNodes;
//		vector<CRootedTree::CTreeNode*> *pNodes2 = &nextLevelNodes;
//		while (!pNodes1->empty())
//		{
//			m_levels++;
//			m_levelNodes.push_back(*pNodes1);
//
//			vector<CRootedTree::CTreeNode*>::iterator iter = pNodes1->begin();
//			for (; iter != pNodes1->end(); iter++)
//			{
//				if (!((*iter)->m_isLeaf))
//				{
//					if (pNodes2->empty())
//						pNodes2->assign((*iter)->m_children.cbegin(), 
//										(*iter)->m_children.cend());
//					else
//						pNodes2->insert(pNodes2->end() - 1, 
//										(*iter)->m_children.cbegin(), 
//										(*iter)->m_children.cend());
//				}
//			}
//			swap(pNodes1, pNodes2);
//			pNodes2->clear();
//		}
//		//cout << "m level: " << m_levels << endl;
//	}
//}

//int CRootedTree::getlevels(const vector<int> &labels)
//{
//	remove
//}

//CRootedTree::CRootedTree(const vector<int> &labels)
//	:m_root(nullptr), m_levels(0), m_order(labels.front())
//{
//	if (!labels.empty())
//	{
//		vector<int>::const_iterator iter =
//			find(labels.cbegin(), labels.cend(), ROOT_LABEL);
//
//		int rootSerialNo = iter - (labels.cbegin() + 1);
//		//std::cout << "root no: " << rootSerialNo << std::endl;
//		m_root = buildTree(rootSerialNo,
//			vector<int>(labels.cbegin() + 1, labels.cend()));
//		//std::cout << std::endl;
//		generateNodeList();
//	}
//}

CRootedTree::CRootedTree(const vector<int> &labels)
	:m_root(-1), m_levels(0), m_order(labels.front()), m_nodeList(labels.size() - 1)
{
	if (!labels.empty())
	{
		vector<int>::const_iterator iter =
			find(labels.cbegin(), labels.cend(), ROOT_LABEL);

		m_root = iter - (labels.cbegin() + 1);
		//std::cout << "root no: " << m_root << std::endl;
		vector<pair<int, int> > treeLabels;
		for (size_t i = 1; i < labels.size(); i++)
		{
			treeLabels.push_back(make_pair(labels[i], i - 1));
		}

		buildTree(m_root, treeLabels);
		//std::cout << std::endl;
		//generateNodeList();
	}
}

//bool isIsomorphism(const CRootedTree::pCTreeNode tnode1, const CRootedTree::pCTreeNode tnode2)
//{
//	if (tnode1 == nullptr && tnode2 == nullptr)
//		return true;
//	else if (tnode1 == nullptr || tnode2 == nullptr)
//		return false;
//	else if (tnode1->m_children.empty() && tnode2->m_children.empty())
//		return true;
//	else if (tnode1->m_children.empty() || tnode2->m_children.empty())
//		return false;
//	else if (tnode1->m_children.size() != tnode2->m_children.size())
//		return false;
//	else
//	{
//		bool(*pred)(const CRootedTree::pCTreeNode, const CRootedTree::pCTreeNode) = &isIsomorphism;
//		int isomorphismCount = 0;
//		vector<CRootedTree::CTreeNode *>::const_iterator iter1 = tnode1->m_children.cbegin();
//		while ((iter1 = std::find_first_of(iter1, tnode1->m_children.cend(),
//			tnode2->m_children.cbegin(), tnode2->m_children.cend(),
//			pred)) != tnode1->m_children.cend())
//		{
//			isomorphismCount++;
//			++iter1;
//		}
//		//std::cout << "isomorphismCount: " << isomorphismCount << " " << "children size: " << tnode1->m_children.size() << " ";
//		return (isomorphismCount == tnode1->m_children.size());
//
//		/*for (list<CTreeNode *>::const_iterator iter1 = tnode1->m_children.cbegin();
//		iter1 != m_root->m_children.cend(); iter1++)
//		{
//		bool isChildIsomorphism = false;
//		for (list<CTreeNode *>::const_iterator iter2 = tnode2->m_children.cbegin();
//		iter1 != tnode2->m_children.cend(); iter2++)
//		{
//		if (isChildIsomorphism |= isIsomorphism(*iter1, *iter2))
//		break;
//		}
//
//		if (!isChildIsomorphism)
//		return false;
//		}
//
//		return true;*/
//	}
//
//}

//bool isIsomorphism(const CRootedTree *tree1, const CRootedTree *tree2)
//{
//	if (tree1->m_root == nullptr && tree2->m_root == nullptr)
//		return true;
//	else if (tree1->m_root == nullptr || tree2->m_root == nullptr)
//		return false;
//	else
//		return ::isIsomorphism(tree1->m_root, tree2->m_root);
//}

void assignNodeStr(CRootedTree::pCTreeNode tNode, CRootedTree *pTree)
{
	if (!(tNode->m_isLeaf))
	{
		vector<int>::const_reverse_iterator criter = tNode->m_childrenSerials.crbegin();
		for (; criter != tNode->m_childrenSerials.crend(); criter++)
		{
			tNode->m_rep.insert(1, pTree->m_nodeList[*criter]->m_rep);
		}
	}
}

bool equalStrs(const vector<string> &strs1, const vector<string> &strs2)
{
	vector<string>::const_iterator citer1 = strs1.cbegin();
	vector<string>::const_iterator citer2 = strs2.cbegin();
	for (; citer1 != strs1.cend() && citer2 != strs2.cend();
		citer1++, citer2++)
	{
		if (*citer1 != *citer2)
			return false;
	}

	return true;
}

bool isIsomorphism2(CRootedTree *tree1, CRootedTree *tree2)
{
	if (tree1->m_root == -1 && tree2->m_root == -1)
		return true;
	else if (tree1->m_root == -1 || tree2->m_root == -1)
		return false;
	else if (tree1->m_levels != tree2->m_levels)
		return false;
	else
	{
		int h = tree1->m_levels;
		for (int i = h - 1; i >= 0; i--)
		{
			if (tree1->m_levelNodeSerials[i].size() != tree2->m_levelNodeSerials[i].size())
				return false;

			vector<string> t1LevelStrs, t2LevelStrs;
			vector<int>::iterator nodeIt1 = tree1->m_levelNodeSerials[i].begin();
			vector<int>::iterator nodeIt2 = tree2->m_levelNodeSerials[i].begin();
			for (; nodeIt1 != tree1->m_levelNodeSerials[i].end() 
				&& nodeIt2 != tree2->m_levelNodeSerials[i].end();
				nodeIt1++, nodeIt2++)
			{
				CRootedTree::pCTreeNode t1Node = tree1->m_nodeList[*nodeIt1];
				CRootedTree::pCTreeNode t2Node = tree2->m_nodeList[*nodeIt2];
				
				assignNodeStr(t1Node, tree1);
				assignNodeStr(t2Node, tree2);

				/*if (!(t1Node->m_isLeaf))
				{
					vector<int>::const_reverse_iterator criter = t1Node->m_childrenSerials.crbegin();
					for (; criter != t1Node->m_childrenSerials.crend(); criter++)
					{
						t1Node->m_rep.insert(1, tree1->m_nodeList[*criter]->m_rep);
					}
				}*/

				/*if (!(t2Node->m_isLeaf))
				{
					vector<int>::const_reverse_iterator criter = t2Node->m_childrenSerials.crbegin();
					for (; criter != (*nodeIt2)->m_children.crend(); criter++)
					{
						(*nodeIt2)->m_rep.insert(1, (*criter)->m_rep);
					}
				}*/

				t1LevelStrs.push_back(t1Node->m_rep);
				t2LevelStrs.push_back(t2Node->m_rep);
			}

			if (t1LevelStrs.size() != t2LevelStrs.size())
				return false;

			sort(t1LevelStrs.begin(), t1LevelStrs.end());
			sort(t2LevelStrs.begin(), t2LevelStrs.end());
			/*if (!equal(t1LevelStrs.cbegin(), t1LevelStrs.cend(),
					t2LevelStrs.cbegin(), t2LevelStrs.cend()))
				return false;*/
			
			if (!equalStrs(t1LevelStrs, t2LevelStrs))
				return false;

			// re-assign representation strings for the nodes in this level
			for (size_t j = 0; j < t1LevelStrs.size(); j++)
			{
				int t1NodeNo = (tree1->m_levelNodeSerials[i])[j];
				int t2NodeNo = (tree2->m_levelNodeSerials[i])[j];
				(tree1->m_nodeList[t1NodeNo])->m_rep.assign(t1LevelStrs[j]);
				(tree2->m_nodeList[t2NodeNo])->m_rep.assign(t2LevelStrs[j]);
			}
		}

		return true;
	}
}

//bool CRootedTree::isIsomorphism(const CRootedTree &ex_tree)
//{
//	return ::isIsomorphism(this, &ex_tree);
//}

bool CRootedTree::isIsomorphism2(CRootedTree &ex_tree)
{
	return ::isIsomorphism2(this, &ex_tree);
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
	//vector<vector<int>> scenarios;
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

		/*vector<int> scenario;
		scenario.push_back(scenarioNo);
		outputTreesData(rootedTrees, scenario);*/
		cout << scenarioNo << ": " << flush;
		outputTrees(rootedTrees);
		
		//scenarios.push_back(scenario);
		scenarioNo++;
	}

	//outputResult(scenarios);

	return 0;
}