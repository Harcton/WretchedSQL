#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include "sqlite3.h"

struct DataType
{
	friend class Database;
public:
	DataType(std::string name, std::string dataTypeSQL, const bool primaryKey, const bool notNull) : _primaryKey(primaryKey), _notNull(notNull)
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
	DataType* ptr(dt);
	return ptr->toStringSQL();
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
//	/*
//	if (std::is_integral<T>::value)
//		return std::to_string((int)val);
//	if (std::is_floating_point<T>::value)
//		return std::to_string((float)val);
//	*/
//	//if (std::is_integral<T>::value)
//	//	return std::to_string((int)val);
//	//if (std::is_pointer<T>::value)
//	//	return (const char*)val;
//	//if (std::is_floating_point<T>::value)
//	//	return std::to_string((float)val);
	return "0";
}
template<typename T, typename ...Args>
std::string toCommaSeparatedString(T val, Args ... args)
{
	return toCommaSeparatedString(val) + ',' + toCommaSeparatedString(args...);
}








class Database
{
public:
	Database();
	Database(std::string path);
	~Database();

	template<typename T, typename ...Args>
	bool createTable(std::string name, T* dt, Args* ... args)
	{
		std::string str = toStringSQL(dt, args...);
		std::string message("CREATE TABLE " + name + "( " + toStringSQL(dt, args...) + ");");
		//message = "CREATE TABLE COMPANY(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE INT NOT NULL, ADDRESS CHAR(50), SALARY REAL);";
		//message = "CREATE TABLE COMPANY("  \
		//	"ID				INT		PRIMARY KEY     NOT NULL," \
		//	"NAME           TEXT    NOT NULL," \
		//	"AGE            INT     NOT NULL," \
		//	"ADDRESS        CHAR(50)," \
		//	"SALARY         REAL );";
		return executeSQL(message);
	}
	bool dropTable(std::string name) { return executeSQL("DROP TABLE " + name); }
	bool executeSQL(std::string msg, const bool _printVallback = true);//Executes given message in its native SQL format
	bool open(std::string path);
	bool isOpen() { return _open; }

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
	int callback(int argc, char **argv, char **azColName);
	std::vector<std::string> getColumnNames(std::string tableName);

private:
	bool _open;
	sqlite3* database;
	void printCallback();

	//Callback
	std::vector<std::pair<std::string, std::string>> callbackColumns;
	char* error;
};