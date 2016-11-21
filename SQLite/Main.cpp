#include <iostream>
#include <vector>
#include <string>
#include "Database.h"
#define HIGHSCORES_TABLE_NAME "highscores"
#define NAME_COLUMN "name"
#define SCORE_COLUMN "score"


int main(int argc, char* argv[])
{
	Database database;
	std::string dbName;
	std::cout << "\nEnter database name:\n>";
	std::cin >> dbName;
	std::string table;
	std::cout << "\nEnter table name:\n>";
	std::cin >> table;

	if (database.open(dbName))
	{
		//database.dropTable(table);

		if (!database.containsTable(table))
		{//Create table
			//DataColumn id("ID", "INT", true, true);
			DataColumn name(NAME_COLUMN, "TEXT", false, true);
			DataColumn score(SCORE_COLUMN, "FLOAT", false, true);
			database.createTable(table, /*&id,*/ &name, &score);
		}
		
		bool enterScore(true);
		while (enterScore)
		{
			std::cout << "\n-----------------\nAdd score entry?\n>";
			std::string ans;
			std::cin >> ans;
			if (ans == "yes" || ans == "y" || ans == "true")
				enterScore = true;
			else
				break;

			//Insert a fictional player score into the highscores table
			std::string playerName("def");
			float playerScore(0);
			std::cout << "\nEnter player name:\n>";
			std::cin >> playerName;
			std::cout << "\nEnter score:\n>";
			std::cin >> playerScore;

			if (database.getRecordCount(table) < 3)
			{//Top 3 not recorded yet
				database.insertInto(table, playerName, playerScore);
			}
			else
			{//Check if fits in top 3
				std::vector<std::string> scores(database.getValuesInColumn(table, SCORE_COLUMN));
				for (unsigned i = 0; i < scores.size(); i++)
				{
					if (std::atoi(scores[i].c_str()) < playerScore)
					{

					}
				}
			}
		}
		
		std::vector<std::string> names(database.getValuesInColumn(table, NAME_COLUMN));
		std::vector<std::string> scores(database.getValuesInColumn(table, SCORE_COLUMN));
		std::cout << "\n\nHIGHSCORES:";
		for (unsigned i = 0; i < names.size(); i++)
			std::cout << "\n" << names[i] + ": " << scores[i];
	}
	
	getchar();
	return 0;
}