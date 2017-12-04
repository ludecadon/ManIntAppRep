#ifndef EQUIPMENT_PROPERTIES_TREE_H
#define EQUIPMENT_PROPERTIES_TREE_H

#include "DataStructures.h"
#include <vector>

/* Property Node is part of a tree to represent equipment properties on the UI
 * Hence it is put in the View Model Layer, as it isn't a direct mapping from SQL
 * Due to the way these are stored in the View State, nodes with a nullptr parent are properties of the equipment
 */
namespace ViewModelLayer
{
	struct EquipmentPropertyNode
	{
		StructureLayer::EquipmentProperty	data;

		EquipmentPropertyNode *parent;
		std::vector<EquipmentPropertyNode*> children;
	};

	class EquipmentPropertyTree
	{
		private:
			EquipmentPropertyNode	*m_root;
			int						m_count;

		public:
			EquipmentPropertyTree();
			~EquipmentPropertyTree();

			EquipmentPropertyNode *AddNode(EquipmentPropertyNode *parent, const StructureLayer::EquipmentProperty equipProp);
			EquipmentPropertyNode *FindNode(std::string ID, EquipmentPropertyNode *currentNode = nullptr);
			void DestroyTree(EquipmentPropertyNode *currentNode = nullptr);
			EquipmentPropertyNode *GetRoot(){ return m_root; }	//oh well
	};
}

#endif