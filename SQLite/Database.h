#pragma once
#include <string>
#include "sqlite3.h"

struct DataType
{
	friend class Database;
public:
	std::string toStringSQL() { return name + " " + type + " NOT NULL"; }
	void setName(std::string name);
protected:
	std::string name;
	std::string type;
};
struct IntType : public DataType
{
	IntType()
	{
		type = "INT";
	}
	int value;
};
struct FloatType : public DataType
{
	FloatType()
	{
		type = "REAL";
	}
	float value;
};
struct StringType : public DataType
{
	StringType()
	{
		type = "TEXT";
	}
	std::string value;
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


class Database
{
public:
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
	bool executeSQL(std::string msg);//Executes given message in its native SQL format
	bool isOpen() { return open; }
private:

	bool open;
	sqlite3* database;
	char *error;
};