#include "stdafx.h"
#include "EquipmentPropertiesTree.h"

namespace ViewModelLayer
{
	EquipmentPropertyTree::EquipmentPropertyTree()
	{
		m_root = nullptr;
		m_count = 0;
	}

	EquipmentPropertyTree::~EquipmentPropertyTree()
	{
		if(m_root != nullptr) DestroyTree();
	}

	EquipmentPropertyNode *EquipmentPropertyTree::AddNode(EquipmentPropertyNode *parent, const StructureLayer::EquipmentProperty equipProp)
	{
		EquipmentPropertyNode *ret = nullptr;

		if (parent == nullptr)
		{
			if (m_root == nullptr)
			{
				m_root = new EquipmentPropertyNode;
				m_root->data = equipProp;
				m_root->parent = nullptr;

				ret = m_root;
			}
		}
		else
		{
			parent->children.push_back(new EquipmentPropertyNode);
			parent->children.back()->data = equipProp;
			parent->children.back()->parent = parent;
			ret = parent->children.back();
		}

		m_count++;

		return ret;
	}
	
	EquipmentPropertyNode *EquipmentPropertyTree::FindNode(std::string ID, EquipmentPropertyNode *currentNode)
	{
		EquipmentPropertyNode *result = nullptr;

		if (currentNode == nullptr)
		{
			currentNode = m_root;
		}

		if (currentNode->data.ID == ID)
		{
			result = currentNode;
		}
		else
		{
			for (EquipmentPropertyNode *node : currentNode->children)
			{
				if (node->data.ID == ID)
				{
					result = node;
					return result;
				}
				else if (node->children.size())
				{
					result = FindNode(ID, node);
					return result;
				}
			}
		}

		return result;
	}

	void EquipmentPropertyTree::DestroyTree(EquipmentPropertyNode *currentNode)
	{
		for (int i = 0; i < m_root->children.size(); i++)
		{
			delete m_root->children[i];
			m_root->children[i] = nullptr;
		}
		delete m_root;
		m_root = nullptr;
	}
}