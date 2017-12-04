#include "stdafx.h"
#include "ODBCConnection.h"

#include <cstdlib>


#pragma comment(lib, "odbc32.lib")

namespace DataLayer
{
	ODBCConnection::ODBCConnection()
	{
		m_connected = false;
		m_envHandle = nullptr;
		m_connHandle = nullptr;
	}

	ODBCConnection::~ODBCConnection()
	{
		if (m_connected)
		{
			Disconnect();
		}
	}

	/*
	 *	ODBC makes us jump through a few hoops first, so aggregate them into one function
	 *	Note that the DSN has to be set up in the ODBC administrator tool on Windows...
	 */
	bool ODBCConnection::Connect(std::string targetDSN, const std::string &username, const std::string &password)
	{
		bool result = false;
		
		if (!m_connected)
		{
			if (SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_envHandle)))
			{
				if (SQL_SUCCEEDED(SQLSetEnvAttr(m_envHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0)))
				{
					if (SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, m_envHandle, &m_connHandle)))
					{
						//handles allocated, now we can actually connect using the given details
						SQLCHAR *dsn = (SQLCHAR*)targetDSN.c_str();
						SQLCHAR *userid = (SQLCHAR*)username.c_str();
						SQLCHAR *passwd = (SQLCHAR*)password.c_str();

						SQLRETURN ret = SQLConnectA(m_connHandle, dsn, SQL_NTS, userid, SQL_NTS, passwd, SQL_NTS);

						if (SQL_SUCCEEDED(ret))
						{
							result = true;
							m_connected = true;
						}
					}
				}
			}
		}
		//else
		//under better circumstances one would have a pop up box inform the user of the type of failure
		//presumably pumping a message out of the data layer (here) and into some sort of maintenance layer/bus
		//from there the UI could retrieve the message and display it appropriately for the platform

		return result;
	}

	void ODBCConnection::Disconnect()
	{
		
	}

	GenericTable ODBCConnection::Query(std::string sqlQuery)
	{
		GenericTable ret;

		if (m_connected)
		{
			SQLHANDLE stmtHandle;
			if (SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, m_connHandle, &stmtHandle)))
			{
				SQLCHAR *query = (SQLCHAR*)sqlQuery.c_str();

				if (SQL_SUCCEEDED(SQLExecDirectA(stmtHandle, query, SQL_NTS)))
				{
					SQLSMALLINT numColumns = 0;
					SQLINTEGER numRows = 0;
					
					SQLRETURN sret = SQLFetch(stmtHandle);
					sret = SQLNumResultCols(stmtHandle, &numColumns);

					int r = 0;
					do
					{
						GenericRow row;
						row.columns.resize(numColumns);
						r++;

						for (int c = 1; c <= numColumns; c++)
						{
							SQLCHAR buf[255] = { 0 };
							GenericColumn *gcPtr = &row.columns[c-1];
							SQLINTEGER size = 0;
							SQLUINTEGER columnSize = 0;
							SQLSMALLINT dataType = 0, numDec = 0, allowNull = 0;

							SQLDescribeColA(stmtHandle, c, buf, 255, NULL, &dataType, &columnSize, &numDec, &allowNull);
							gcPtr->allowNull = allowNull;
							gcPtr->numDecimalDigits = numDec;
							gcPtr->dataType = dataType;
							gcPtr->name = (char*)buf;
							memset(buf, 0, 255);

							sret = SQLGetData(stmtHandle, c, SQL_C_CHAR, buf, 255, (SQLINTEGER*)&gcPtr->size);
							if (SQL_SUCCEEDED(sret))
								gcPtr->value = (char*)buf;
							else
								gcPtr->value = "";
						}

						//validate data before adding the row
						if (row.columns[0].value != "")	//really crap tbh
							ret.rows.push_back(row);
	
					} while (SQL_SUCCEEDED(SQLFetch(stmtHandle)));
				}

				SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
			}
		}

		return ret;
	}
}