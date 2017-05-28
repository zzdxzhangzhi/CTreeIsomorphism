#pragma once

#ifndef __CROOTEDTREE_H_
#define __CROOTEDTREE_H_

#include <vector>
#include <list>

using std::vector;
using std::list;

class CRootedTree 
{
public:
	CRootedTree(): m_root(nullptr), m_depth(0), m_order(0) {}
	CRootedTree(const vector<int> &labels);
	CRootedTree(const CRootedTree &ex_tree);
	CRootedTree& operator = (const CRootedTree &ex_tree);
	
	bool isIsomorphism(const CRootedTree &ex_tree);
	
	bool isEmpty()
	{
		return (m_order == 0);
	}

	int depth()
	{
		return m_depth;
	}

	int order()
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
		list<CTreeNode *> m_children;

		CTreeNode(): m_label(-1) {}
		CTreeNode(int label): m_label(label) {}
		CTreeNode(const CTreeNode &ex_node);
	} *pCTreeNode;

	CTreeNode *m_root;
	int m_depth;
	int m_order;

	static const int ROOT_LABEL = -1;

	pCTreeNode copyTreeNode(const pCTreeNode pnode);
	pCTreeNode buildTree(int iSerialNo, const vector<int> &labels, int &tree_depth);
	bool isIsomorphism(const pCTreeNode tnode1, const pCTreeNode tnode2);

	//list<int> getLayout();
};

//const int CRootedTree::ROOT_LABEL = -1;

//bool isEachElemNumEqual(vector<int> v1, vector<int> v2);

#endif // !__CROOTEDTREE_H_
