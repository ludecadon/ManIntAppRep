#ifndef GENERIC_QUERY_STRUCTURES_H
#define GENERIC_QUERY_STRUCTURES_H

#include <vector>

namespace DataLayer
{
	struct GenericColumn
	{
		std::string name;
		int size;
		int numDecimalDigits;
		int allowNull;
		int dataType;
		std::string value;
	};

	struct GenericRow
	{
		std::vector<GenericColumn> columns;
	};

	struct GenericTable
	{
		std::vector<GenericRow> rows;
	};
}

#endif