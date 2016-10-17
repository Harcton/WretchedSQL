#include "Database.h"
#include <iostream>


void DataType::setName(std::string _name)
{
	for (int i(0); i < _name.size(); i++)
	{
		if (_name[i] == ' ')
		{
			std::cout << "\nError: data type names cannot contain spaces!";
			std::getchar();
			return;
		}
	}
	name = _name;
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
		std::cout << "\n" << azColName[i] << " = " << argv[i] ? argv[i] : "NULL";
	return 0;
}
Database::Database(std::string path) : open(true)
{
	if (sqlite3_open(path.data(), &database))
	{
		open = false;
		std::cout << "\nCan't open database: " << sqlite3_errmsg(database);
	}
	else
	{
		std::cout << "\nOpened database successfully";
	}
}
Database::~Database()
{
	if (open)
		sqlite3_close(database);
}
bool Database::executeSQL(std::string msg)
{
	if (!open)
	{
		std::cout << "\nCannot execute SQL command! Database is not open!";
		return false;
	}
	if (sqlite3_exec(database, msg.data(), callback, 0, &error))
	{
		std::cout << "\nError: " << error;
		return false;
	}
	return true;
}