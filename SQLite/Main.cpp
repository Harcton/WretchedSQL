#include <iostream>
#include <vector>
#include <string>
#include "Database.h"
/*
TODO: insert, select, delete and update values from database
*/




//template<typename T>
//T adder(T v)
//{
//	return v;
//}
//template<typename T, typename... Args>
//T adder(T first, Args... args)
//{
//	return first + adder(args...);
//}
//long sum = adder(1, 2, 3, 8, 7);


int main(int argc, char* argv[])
{
	Database database;
	if (database.open("test.db"))
	{
		database.dropTable("table2");

		DataType val1("integer", "INT", false, true);
		DataType val2("floatingPointNumber", "FLOAT", false, true);
		DataType val3("string", "TEXT", false, true);

		database.createTable("table2", &val1, &val2 , &val3);
		database.insertInto("table2", 1, 2.0f, "three");
	}
	
	getchar();
	return 0;
}


/*

//Dropping a table
message = "DROP TABLE COMPANY";

//Inserting into a table
message = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

//Creating a new table
message = "CREATE TABLE COMPANY("  \
"ID				INT		PRIMARY KEY     NOT NULL," \
"NAME           TEXT    NOT NULL," \
"AGE            INT     NOT NULL," \
"ADDRESS        CHAR(50)," \
"SALARY         REAL );";

*/