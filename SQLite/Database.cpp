#include "Database.h"
#include <iostream>

void DataColumn::setName(std::string name)
{
	for (int i(0); i < name.size(); i++)
	{
		if (name[i] == ' ')
		{
			std::cout << "\nError: data type names cannot contain spaces! Data type name: " + name;
			std::getchar();
			return;
		}
	}
	_name = name;
}
void DataColumn::setDataTypeSQL(std::string dataTypeSQL)
{
	_dataTypeSQL = dataTypeSQL;
}
static int staticCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
		std::cout << "\n" << azColName[i] << " = " << argv[i] ? argv[i] : "NULL";
	return 0;
}
int SQLCallbackHandler(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (NotUsed)
		return ((Database*)NotUsed)->callback(argc, argv, azColName);
	return 0;
}










Database::Database() : _open(true)
{

}
Database::Database(const std::string path) : Database()
{
	if (sqlite3_open(path.data(), &database))
	{
		_open = false;
		std::cout << "\nCan't open database: " << sqlite3_errmsg(database);
	}
	else
	{
		std::cout << "\nOpened database successfully";
	}
}
Database::~Database()
{
	if (_open)
		sqlite3_close(database);
}
bool Database::open(std::string path)
{
	if (sqlite3_open(path.data(), &database))
	{
		_open = false;
		std::cout << "\nCan't open database: " << sqlite3_errmsg(database);
		return false;
	}
	else
	{
		std::cout << "\nOpened database successfully";
		return true;
	}
}
bool Database::executeSQL(std::string msg, const bool _printVallback)
{
	callbackColumns.clear();//Clear all callback columns at this point
	if (!_open)
	{
		std::cout << "\nCannot execute SQL command! Database is not open!";
		return false;
	}
	if (sqlite3_exec(database, msg.data(), /*staticCallback*/SQLCallbackHandler, (void*)this, &error))
	{
		std::cout << "\nError: " << error;
		return false;
	}
	if (_printVallback)
		printCallback();
	return true;
}
int Database::callback(int _argc, char** _argv, char** _azColName)
{
	for (int i = 0; i < _argc; i++)
	{
		callbackColumns.push_back(std::pair<std::string, std::string>(_azColName[i], _argv[i] ? _argv[i] : "NULL"));
	}
	return 0;
}
void Database::printCallback()
{
	for (int i = 0; i < callbackColumns.size(); i++)
		std::cout << "\n" + callbackColumns[i].first + " = " + callbackColumns[i].second;
}
std::vector<std::string> Database::getColumnNames(std::string tableName)
{
	executeSQL("PRAGMA table_info(" + tableName + ");", false);
	std::vector<std::string> columns;
	for (unsigned i = 0; i < callbackColumns.size() / 6; i++)
		columns.push_back(callbackColumns[i * 6 + 1].second);
	return columns;
}
unsigned Database::getColumnCount(std::string tableName)
{
	executeSQL("PRAGMA table_info(" + tableName + ");", false);
	return callbackColumns.size() / 6;
}
bool Database::containsTable(const std::string tableName)
{
	if (executeSQL("PRAGMA table_info(" + tableName + ");", false))
	{
		if (error)
			return false;
		if (callbackColumns.size() > 0)
			return true;
		else
			return false;
	}
	return false;
}
int Database::getTableElementCount(const std::string tableName)
{
	return 0;
}