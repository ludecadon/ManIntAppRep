#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include "DataStructures.h"
#include "EquipmentPropertiesTree.h"

/* View State to hold the current items visible to the UI
 * Will take it's data from the View Manager class
 */
namespace ViewModelLayer
{
	class ViewState
	{
		public:
			std::vector<StructureLayer::HierarchyScope>		hierarchyScopes;
			StructureLayer::Equipment						equipment;
			std::vector<StructureLayer::EquipmentLevel>		equipmentLevels;
			std::vector<EquipmentPropertyTree>				equipmentProperties;
			StructureLayer::EquipmentLevel					*currentEquipLevel;
			StructureLayer::HierarchyScope					*currentHierarchyScope;
			EquipmentPropertyNode							*selectedEquipProperty;
	};
}

#endif