#include <vector>
#include <list>
#include <algorithm>
#include "CRootedTree.h"


using std::vector;
using std::list;


CRootedTree::CTreeNode::CTreeNode(const CTreeNode &ex_node)
	: m_label(ex_node.m_label)
{
	if (!m_children.empty())
	{
		list<CTreeNode *>::const_iterator iter = ex_node.m_children.cbegin();
		list<CTreeNode *>::const_iterator lEnd = ex_node.m_children.cend();
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
		m_root->m_children.pop_front();
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
	pCTreeNode pnode = new CTreeNode(labels.at(iSerialNo));
	for (size_t i = 0;	i < labels.size(); i++)
	{
		if (labels.at(i) == iSerialNo)
			pnode->m_children.push_back(buildTree(i, labels, m_depth));
	}

	return pnode;
}

CRootedTree::CRootedTree(const vector<int> &labels)
	:m_root(nullptr), m_depth(0), m_order(labels.size())
{
	if (!labels.empty())
	{
		vector<int>::const_iterator iter = 
			find(labels.cbegin(), labels.cend(), ROOT_LABEL);

		int rootSerialNo = iter - labels.cbegin();
		m_root = buildTree(rootSerialNo, labels, m_depth);
	}
}

bool CRootedTree::isIsomorphism(const pCTreeNode tnode1, const pCTreeNode tnode2)
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
		for (list<CTreeNode *>::const_iterator iter1 = tnode1->m_children.cbegin();
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

		return true;
	}

}

bool CRootedTree::isIsomorphism(const CRootedTree &ex_tree)
{
	if (m_root == nullptr && ex_tree.m_root == nullptr)
		return true;
	else if (m_root == nullptr || ex_tree.m_root == nullptr)
		return false;
	else
		return isIsomorphism(m_root, ex_tree.m_root);
}