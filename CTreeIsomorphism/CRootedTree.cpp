#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include "CRootedTree.h"


using std::vector;
using std::list;
using std::find;
using std::find_first_of;


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
