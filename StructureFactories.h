#ifndef STRUCTURE_FACTORIES_H
#define STRUCTURE_FACTORIES_H

#include "GenericQueryStructures.h"
#include "DataStructures.h"

//it's probably better to make one from a row, rather than a vector from a whole table
namespace StructureLayer
{
	class HierarchyScopeFactory
	{	
		public:
			std::vector<HierarchyScope> CreateHierarchyScope(DataLayer::GenericTable table);
	};

	class EquipmentLevelFactory
	{
		public:
			std::vector<EquipmentLevel> CreateEquipmentLevel(DataLayer::GenericTable table);
	};

	class EquipmentFactory
	{
		public:
			std::vector<Equipment> CreateEquipment(DataLayer::GenericTable table);
	};

	class EquipmentPropertyFactory
	{
		public:
			std::vector<EquipmentProperty> CreateEquipmentProperty(DataLayer::GenericTable table);
	};
}

#endif