#include "stdafx.h"
#include "StateManager.h"
#include "StructureFactories.h"

namespace ViewModelLayer
{
	StateManager::StateManager()
	{
		m_dbDsn = "\\\\LUKE-DESKTOP\\SQLEXPRESS\\ManIntDB";
		m_dbUsername = "sa";
		m_dbPassword = "30Chapman";
	}

	StateManager::~StateManager()
	{
		m_database.Disconnect();
	}

	bool StateManager::ReconnectToDatabase()
	{
		if (m_database.IsConnected())
		{
			m_database.Disconnect();
		}

		return m_database.Connect(m_dbDsn, m_dbUsername, m_dbPassword);
	}

	bool StateManager::ConnectToDatabase(std::string dsn, std::string username, std::string password)
	{
		m_dbDsn = dsn;
		m_dbUsername = username;
		m_dbPassword = password;

		return ReconnectToDatabase();
	}

	void StateManager::LoadFromDatabase(ViewState *view)
	{
		LoadEquipmentLevels();
		LoadHierarchyScopes();
		*view = m_loadedView;
	}

	void StateManager::LoadEquipmentLevels()
	{
		StructureLayer::EquipmentLevelFactory levelFac;
		char queryBuf[256] = { 0 };

		sprintf(queryBuf, "SELECT * FROM [dbo].[EquipmentLevel]");

		m_loadedView.equipmentLevels = levelFac.CreateEquipmentLevel(m_database.Query(queryBuf));
		m_loadedView.currentEquipLevel = &m_loadedView.equipmentLevels[0];
	}

	void StateManager::LoadHierarchyScopes()
	{
		StructureLayer::HierarchyScopeFactory scopeFac;
		char queryBuf[256] = { 0 };

		//return all hierarchy scopes because they need to be visible in the drop down box
		sprintf(queryBuf, "SELECT * FROM [dbo].[HierarchyScope]");// where Hierarchy like '%s'", equipScope.c_str());

		m_loadedView.hierarchyScopes = scopeFac.CreateHierarchyScope(m_database.Query(queryBuf));
		m_loadedView.currentHierarchyScope = &m_loadedView.hierarchyScopes[0];
	}

	bool StateManager::AddRowsToDatabase(ViewState view)
	{
		bool ret = false;

		//Add rows that did not exist when the view was loaded (i.e. for a new piece of equipment, hierarchy scope, etc.

		return ret;
	}



	//bool StateManager::UpdateRowsInDatabase(ViewState view)
	//{
	//	bool ret = false;
	//	
	//	//Add rows that have changed from the loaded view (i.e. when editing existing equipment)

	//	return ret;
	//}

	bool StateManager::GetViewForEquipment(std::string equipScope, std::string equipID, ViewState *view)
	{
		bool ret = false;

		if (view != nullptr && m_database.IsConnected())
		{
			char queryBuf[256] = { 0 };
			StructureLayer::EquipmentFactory equipFac;
			StructureLayer::EquipmentPropertyFactory propFac;
			bool found = false;

			//now see if the requested one exists... we can just use the returned rows
			for (int i = 0; i < view->hierarchyScopes.size(); i++)
			{
				if (view->hierarchyScopes[i].hierarchy == equipScope)
				{
					view->currentHierarchyScope = &view->hierarchyScopes[i];
					found = true;
					break;
				}
			}

			if (found)
			{
				//query to return a row from the Equipment table
				memset(queryBuf, 0, 255);
				sprintf(queryBuf, "SELECT * FROM [dbo].[Equipment] where HierarchyId = %d and Id like '%s'", view->currentHierarchyScope->ID, equipID.c_str());
				std::vector<StructureLayer::Equipment> eq = equipFac.CreateEquipment(m_database.Query(queryBuf));

				if (eq.size() == 1)	//bad
				{
					view->equipment = eq[0];

					found = false;

					for (int i = 0; i < view->equipmentLevels.size(); i++)
					{
						if (view->equipmentLevels[i].ID == view->equipment.levelID)
						{
							view->currentEquipLevel = &view->equipmentLevels[i];
							found = true;
							break;
						}
					}

					//finally, retrieve all the trees of equipment properties
					//this involves searching the db for every property ID that turns up and is not very efficient
					if (found)
					{
						memset(queryBuf, 0, 255);
						sprintf(queryBuf, "SELECT * FROM [dbo].[EquipmentProperty] where ParentId like '%s'", view->equipment.compositeID.c_str());

						std::vector<StructureLayer::EquipmentProperty> propvec = propFac.CreateEquipmentProperty(m_database.Query(queryBuf));
						
						//construct a tree of properties
						for (StructureLayer::EquipmentProperty prop : propvec)
						{
							view->equipmentProperties.push_back(EquipmentPropertyTree());
							//Add nodes to the tree
							//use this child node's ID and look for additional properties referencing it
							EquipmentPropertyNode *n = view->equipmentProperties.back().AddNode(nullptr, prop);
							
							GetEquipmentPropertiesByParentID(prop.ID, n);
						}

						ret = true;
					}
				}
			}
		}

		return ret;
	}

	void StateManager::GetEquipmentPropertiesByParentID(const std::string parentId, EquipmentPropertyNode *parentnode)
	{
		StructureLayer::EquipmentPropertyFactory pfac;

		//search for properties matching parentId
		std::string query = "SELECT * FROM [dbo].[EquipmentProperty] where ParentId like '" + parentId + "'";

		std::vector<StructureLayer::EquipmentProperty> pvec = pfac.CreateEquipmentProperty(m_database.Query(query));

		//add as children to parentnode
		for (StructureLayer::EquipmentProperty prop : pvec)
		{
			parentnode->children.push_back(new EquipmentPropertyNode);
			parentnode->children.back()->data = prop;
			parentnode->children.back()->parent = parentnode;
			GetEquipmentPropertiesByParentID(parentnode->children.back()->data.ID, parentnode->children.back());
		}
	}
}