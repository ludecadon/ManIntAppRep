#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "ODBCConnection.h"
#include "DataStructures.h"
#include "ViewState.h"

/* State Manager takes care of database operations, storing data structures, retrieving relevant data to display, etc.
 * This is taken by the UI and displayed via the ViewState
 * If the app had a config file, this class would load and store it's details, such as database urn, user, pass, etc. or any saved equipment forms that were in progress but not committed to the database
 * For now it just initialises that stuff in the default constructor - not the best design but it's fine for a demo
 */
namespace ViewModelLayer
{
	class StateManager
	{
		private:
			ViewState					m_loadedView;	//store the view as loaded for comparison when committing to db later

			std::string					m_dbDsn;
			std::string					m_dbUsername;
			std::string					m_dbPassword;
			DataLayer::ODBCConnection	m_database;
			
			void GetHierarchyScopeByEquip(const StructureLayer::Equipment equip, StructureLayer::HierarchyScope *hScope);
			void GetEquipmentPropertiesByParentID(const std::string parentID, EquipmentPropertyNode *node);
			void GetEquipmentLevelByEquip(const StructureLayer::Equipment equip, StructureLayer::EquipmentLevel *equipLevel);
			void LoadEquipmentLevels();
			void LoadHierarchyScopes();

		public:
			StateManager();
			~StateManager();

			bool ReconnectToDatabase();
			bool ConnectToDatabase(std::string dsn, std::string username, std::string password);
			void LoadFromDatabase(ViewState *view);
			
			bool AddRowsToDatabase(const ViewState view);
			bool AddEquipmentLevelRowToDatabase(StructureLayer::EquipmentLevel);
			bool AddEquipmentToDatabase(StructureLayer::Equipment);
			bool AddEquipmentPropertyToDatabase(StructureLayer::EquipmentProperty);

			//bool UpdateRowsInDatabase(const ViewState view);

			bool GetViewForEquipment(std::string equipScope, std::string equipID, ViewState *view);
	};
}

#endif