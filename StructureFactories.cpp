#include "stdafx.h"
#include "StructureFactories.h"

/* Structure Factories are intended to map from a generic SQL query result into something more useful
 * They require the struct in DataFactories.h to match the SQL design precisely
 * However, it keeps all initialisation code out of the ODBC class and in one file here
 */
namespace StructureLayer
{
	std::vector<HierarchyScope> HierarchyScopeFactory::CreateHierarchyScope(DataLayer::GenericTable table)
	{
		std::vector<HierarchyScope> ret;

		for (DataLayer::GenericRow row : table.rows)
		{
			if (row.columns.size() == 2)
			{
				HierarchyScope sc;
				sc.ID = atoi(row.columns[0].value.c_str());
				sc.hierarchy = row.columns[1].value;
				ret.push_back(sc);
			}
		}

		return ret;
	}

	std::vector<EquipmentLevel> EquipmentLevelFactory::CreateEquipmentLevel(DataLayer::GenericTable table)
	{
		std::vector<EquipmentLevel> ret;

		for (DataLayer::GenericRow row : table.rows)
		{
			if (row.columns.size() == 2)
			{
				EquipmentLevel el;
				el.ID = atoi(row.columns[0].value.c_str());
				el.description = row.columns[1].value;
				ret.push_back(el);
			}
		}

		return ret;
	}

	std::vector<Equipment> EquipmentFactory::CreateEquipment(DataLayer::GenericTable table)
	{
		std::vector<Equipment> ret;

		for (DataLayer::GenericRow row : table.rows)
		{
			if (row.columns.size() == 5)
			{
				Equipment e;
				e.hierarchy = atoi(row.columns[0].value.c_str());
				e.ID = row.columns[1].value;
				e.description = row.columns[2].value;
				e.levelID = atoi(row.columns[3].value.c_str());
				e.compositeID = row.columns[4].value;
				ret.push_back(e);
			}
		}

		return ret;
	}

	std::vector<EquipmentProperty> EquipmentPropertyFactory::CreateEquipmentProperty(DataLayer::GenericTable table)
	{
		std::vector<EquipmentProperty> ret;

		for (DataLayer::GenericRow row : table.rows)
		{
			if (row.columns.size() == 4)
			{
				EquipmentProperty ep;
				ep.ID = row.columns[0].value;
				ep.description = row.columns[1].value;
				ep.value = row.columns[2].value;
				ep.parentID = row.columns[3].value;
				ret.push_back(ep);
			}
		}

		return ret;
	}
}