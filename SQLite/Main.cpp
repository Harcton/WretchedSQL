#include <iostream>
#include <vector>
#include <string>
#include "Database.h"


#define HIGHSCORES_TABLE_NAME "highscores"


int main(int argc, char* argv[])
{
	Database database;
	if (database.open("database.db"))
	{
		//database.dropTable(HIGHSCORES_TABLE_NAME);

		if (!database.containsTable(HIGHSCORES_TABLE_NAME))
		{//Create table
			DataColumn id("ID", "INT", true, true);
			DataColumn name("name", "TEXT", false, true);
			DataColumn score("score", "FLOAT", false, true);
			database.createTable(HIGHSCORES_TABLE_NAME, &id, &name, &score);
		}
		
		if (true)
		{
			database.insertInto(HIGHSCORES_TABLE_NAME, 1, "Geddy", 123.5f);
			database.insertInto(HIGHSCORES_TABLE_NAME, 2, "Billy", 123);
			database.insertInto(HIGHSCORES_TABLE_NAME, 3, "Beefcake", 124);
			database.insertInto(HIGHSCORES_TABLE_NAME, 4, "Alex", 125);
			database.insertInto(HIGHSCORES_TABLE_NAME, 5, "Keijo", 61);
			database.insertInto(HIGHSCORES_TABLE_NAME, 6, "Neil", -1.1f);
		}

		//Insert a fictional player score into the highscores table
		int playerID(0);
		std::string playerName("Teo Hiltunen -TTV14SP");
		float playerScore(9000.1f);
		if (!database.containsRecord(HIGHSCORES_TABLE_NAME, "ID", playerID))
			database.insertInto(HIGHSCORES_TABLE_NAME, playerID, playerName, playerScore);		
	}
	
	getchar();
	return 0;
}