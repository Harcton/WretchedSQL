#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "sqlite3.h"

struct DataColumn
{
	friend class Database;
public:
	DataColumn(std::string name, std::string dataTypeSQL, const bool primaryKey, const bool notNull) : _primaryKey(primaryKey), _notNull(notNull)
	{
		setName(name);
		setDataTypeSQL(dataTypeSQL);
	}
	std::string toStringSQL() { return _name + " " + _dataTypeSQL + (_notNull ? " NOT NULL" : "") + (_primaryKey ? " PRIMARY KEY" : ""); }
	void setName(std::string name);
	void setDataTypeSQL(std::string dataTypeSQL);
protected:
	std::string _name;
	std::string _dataTypeSQL;
	const bool _primaryKey;
	const bool _notNull;
};
template<typename T>
std::string toStringSQL(T* dt)
{
	return ((DataColumn*)(dt))->toStringSQL();
}
template<typename T, typename ... Args>
std::string toStringSQL(T* dt, Args* ... args)
{
	return toStringSQL(dt) + ", " + toStringSQL(args...);
}
int SQLCallbackHandler(void *NotUsed, int argc, char **argv, char **azColName);

template<typename T>
std::string toCommaSeparatedString(T val)
{
	return Database::valueToString(val);
}
template<typename T, typename ...Args>
std::string toCommaSeparatedString(T val, Args ... args)
{
	return toCommaSeparatedString(val) + ',' + toCommaSeparatedString(args...);
}








class Database
{
public:
	static std::string valueToString(int val) { return std::to_string(val); }
	static std::string valueToString(float val) { return std::to_string(val); }
	static std::string valueToString(const char* val) { std::string value("\""); value += val; value.push_back('\"'); return value; }
	static std::string valueToString(std::string& val) { return "\"" + val + "\""; }
	enum class Operator
	{
		equal, notEqual, larger, smaller, equalOrLarger, equalOrSmaller
	};
	std::string operatorToString(Operator op)
	{
		switch (op)
		{
		case Operator::equal: return "=";
		case Operator::notEqual: return "<>";
		case Operator::larger: return ">";
		case Operator::smaller: return "<";
		case Operator::equalOrLarger: return ">=";
		case Operator::equalOrSmaller: return "<=";
		}
	}
public:
	Database();
	Database(const std::string path);
	~Database();
	bool open(std::string path);
	bool isOpen() { return _open; }
	bool containsTable(const std::string tableName);
	bool dropTable(std::string name) { return executeSQL("DROP TABLE " + name); }
	bool executeSQL(std::string msg, const bool _printVallback = true);//Executes given message in its native SQL format
	int getTableElementCount(const std::string tableName);
	int callback(int argc, char **argv, char **azColName);
	std::vector<std::string> getColumnNames(std::string tableName);
	unsigned getColumnCount(std::string tableName);

	template<typename T, typename ...Args>
	bool createTable(const std::string name, T* dt, Args* ... args)
	{
		std::string message("CREATE TABLE " + name + " ( " + toStringSQL(dt, args...) + ");");
		if (executeSQL(message) && !error)
		{
			std::cout << "\nTable created: " + name;
			return true;
		}
		std::cout << "\nFailed to create table: " + name + "\nError:" + error;
		return false;
	}

	//Insert into table
	template<typename T, typename ... Args>
	bool insertInto(std::string tableName, T t, Args ... args)
	{
		if (!_open)
			return false;
		
		std::string arguments(toCommaSeparatedString(t, args...));
		std::vector<std::string> columns(getColumnNames(tableName));
		if (columns.size() == 0)
			return false;

		std::string message = "INSERT INTO " + tableName + " (";
		for (unsigned i = 0; i < columns.size(); i++)
		{
			if (i != 0)
				message += ", ";
			message += columns[i];
		}
		message += ") VALUES (" + arguments + "); ";
		if (executeSQL(message, false))
		{
			std::cout << "\nValues inserted into " + tableName;
		}
	}
	template<typename T>
	std::vector<std::string> getRecords(const std::string tableName, const std::string columnName, T value, Operator comparisonOperator = Operator::equal)
	{
		std::vector<std::string> records;
		if (executeSQL("SELECT * FROM " + tableName + " WHERE " + columnName + " " + operatorToString(comparisonOperator) + " " + Database::valueToString(value), false) && callbackColumns.size() > 0)
			records = callbackColumns;
		return callbackColumns;
	}
	template<typename T>
	bool containsRecord(const std::string tableName, const std::string columnName, T value)
	{
		if (executeSQL("SELECT * FROM " + tableName + " WHERE " + columnName + " = " + Database::valueToString(value), false) && callbackColumns.size() > 0)
			return true;
		return false;
	}

private:
	bool _open;
	sqlite3* database;
	void printCallback();

	//Callback
	std::vector<std::pair<std::string, std::string>> callbackColumns;
	char* error;
};