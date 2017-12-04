#ifndef ODBCCONNECTION_H
#define ODBCCONNECTION_H

#include <string>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include "GenericQueryStructures.h"

namespace DataLayer
{
	class ODBCConnection
	{
		private:
			SQLHANDLE	m_envHandle;
			SQLHANDLE	m_connHandle;
			std::string	m_targetDSN;
			bool		m_connected;

		public:
			ODBCConnection();
			~ODBCConnection();

			bool Connect(std::string targetDSN, const std::string &username, const std::string &password);
			void Disconnect();
			GenericTable Query(std::string sqlQuery);

			inline bool IsConnected() const { return m_connected; }
	};
}

#endif