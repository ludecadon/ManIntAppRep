#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string>

namespace StructureLayer
{
	struct HierarchyScope
	{
		int ID;
		std::string hierarchy;
	};

	struct EquipmentLevel
	{
		int ID;
		std::string description;
	};

	struct Equipment
	{
		int hierarchy;
		std::string ID;
		std::string description;
		int levelID;
		std::string compositeID;
	};

	struct EquipmentProperty
	{
		std::string ID;
		std::string description;
		std::string value;
		std::string parentID;
	};
}

#endif