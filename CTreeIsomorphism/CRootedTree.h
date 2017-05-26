#pragma once

#include <vector>
#include <list>

using std::vector;
using std::list;

class CRootedTree 
{
public:
	CRootedTree(): m_root(nullptr) {}
	CRootedTree(vector<int> labels);
	bool isLayoutEqual(CRootedTree *ex_tree);
	bool isEmpty();
	virtual ~CRootedTree();
//protected:
private:
	typedef class CTreeNode
	{
	public:
		int label;
		list<CTreeNode *> children;
	} *pCTreeNode;

	pCTreeNode m_root;

	list<int> getLayout();

};

bool isEachElemNumEqual(vector<int> v1, vector<int> v2);
