// Qwixx.cpp : Defines the entry point for the console application.
// Was in the middle of implementing AI, but didn't get it finished, to test the game simply input anything except
// 'Y' or 'y' at the starting prompt

#include "stdafx.h"
#include "Dice.h";
#include <vector>;
#include <ctime>;
#include "Board.h";
#include "Player.h";
#include <algorithm>;
#include <string>;
#include "AIPlayer.h";


vector<Dice*> setupDies();
void rollDies(vector<Dice*>&);
int addWhiteDies(vector<Dice*>&);
int getRowInput();
int getWhiteDie(vector<Dice*> &dies);
char secondActiveTurn(int &currentPlayer, vector<Dice*> &dies, vector<Player*> &players);
void firstPassiveTurn(vector<Player*> &players, int &whiteDiceAddition, int &incrementMissedTurn, int &currentPlayer);

int main()
{
	srand(static_cast<unsigned int>(time(NULL))); //seed random

	//Setting up variables
	int currentPlayer = 0;
	char answer, AIanswer;
	bool playing = true;
	bool badInput = true;
	bool usingAI = false;

	vector<Dice*> dies = setupDies(); // Make 6 new dies

	for (;;)
	{
		cout << "Do you want to play versus AI?" << endl; // Was in the middle of creating AI
		if (cin >> AIanswer)
		{
			if (AIanswer == 'Y' || AIanswer == 'y')
			{
				usingAI = true;
				break;
			}
			else
				break;
		}
		else 
		{
			cout << "Input (Y/N) for Yes/No" << endl;
			cin.clear();
			cin.ignore(5, '\n');
		}
	}

	int numPlayers;
	while (badInput)
	{
		cout << "Please enter the amount of players: (2-5)" << endl;
		cin >> numPlayers;
		if (numPlayers >= 2 && numPlayers <= 5)
			badInput = false;
		else
			cout << "Please enter a number of players (2-5)" << endl;
	}

	string name;
	vector<Player*> players;
	if (!usingAI)
	{
		for (unsigned int i = 0; i < numPlayers; i++)
			players.push_back(new Player(Board(), Board(), 0, 0));

		for (unsigned int i = 0; i < players.size(); i++)
		{
			cout << "Enter your player alias: " << "(" << i+1 << ")";
			cin >> name;
			players.at(i)->setName(name);
		}
	}
	else
	{
		players.push_back(new Player(Board(), Board(), 0, 0)); // Player
		for (unsigned int i = 1; i < numPlayers; i++)
		{
			players.push_back(new AIPlayer(Board(), Board(), 0, 0, "AI "+i)); // AI
		}

		cout << "Enter your name: ";
		cin >> name;
		players.at(0)->setName(name);
	}


	while (playing != false)
	{
		if (usingAI)
		{
			cout << "AI";
			for (int i = 0; i < players.size(); i++)
				cout << *players.at(i)->getName() + " ";
			break;
				
		}
		else
		{
			//players.at(currentPlayer)->getCopyBoard()->printBoard(); 
			int incementMissedTurn = 0;															// If this value reaches 2, increment players' missed turn variable
			int rowReturn = 0;																	// Used to test if a user passes each turn on Passive Turn
			Board* playerBoard = players.at(currentPlayer)->getBoard();
			players.at(currentPlayer)->setActivePlayer(true);									// Set active player to current player
			rollDies(dies);																		// Roll each dice
			int whiteDiceAddition = addWhiteDies(dies);											// Add up the white dice

			firstPassiveTurn(players, whiteDiceAddition, incementMissedTurn, currentPlayer);	// Perform the first passive turn for each player

			if (players.at(currentPlayer)->getActivePlayer())
			{
				answer = secondActiveTurn(currentPlayer, dies, players);						// Perform the second turn for the ACTIVE player only
				if (answer != 'Y' || 'y')														// if skipped, increment value
					incementMissedTurn += 1;
			}

			if (incementMissedTurn > 1)
			{
				players.at(currentPlayer)->setMissedTurns(*players.at(currentPlayer)->getMissedTurns() + 1); // Increment a players missed turns
				cout << "\nPlayer " << currentPlayer + 1 << "(" << *players.at(currentPlayer)->getName() << ") has skipped a turn!" << " They have (" << *players.at(currentPlayer)->getMissedTurns() << ") missed turns!\n" << endl;
			}



			players.at(currentPlayer)->setActivePlayer(false);

			for (unsigned int i = 0; i < players.size(); i++)
			{
				for (int r = 0; r < players.at(i)->getBoard()->getBoard()->size(); r++)		//Check each row
				{
					if (players.at(i)->getBoard()->canLockRow(r))		 // Check if a row is to be locked at the end of the turn
					{
						cout << "\nREMOVING ROW " << r << " FROM THE GAME FOR EVERY PLAYER\n";
						for each(Player* player in players)
						{
							//copy(player->getBoard()->getBoard()[r].begin(), player->getBoard()->getBoard()[r].end(), player->getCopyBoard()->getBoard()->begin()); Tried to copy vectors until game over, but couldnt figure it out
							player->getBoard()->removeRow(r);		//Remove row for each player
						}
					}
				}
			}

			if (*players.at(currentPlayer)->getMissedTurns() >= 4 || playerBoard->getBoard()->size() <= 2)	// if missed turns are 4, or 2 rows are locked, then game over
				playing = false;

			if (currentPlayer == players.size() - 1)			// End Of Turn
			{
				currentPlayer = 0;
			}
			else
			{
				currentPlayer++;
			}
		}
	}

	//END OF GAME

	for each(Dice* dice in dies)			// Destroy all dice, not to be used
		delete dice;

	cout << "GAME OVER\n";
	for each(Player* player in players)		// Calculate scores for each player
	{
		player->calculateScore();
	}

	for (unsigned int i = 0; i < players.size(); i++)
	{
		cout << "Player " << i+1 << "(" <<*players.at(i)->getName() << ") score: " << *players.at(i)->getScore() << "\n"; //Print scores of each player
	}

	cout << "\nWINNER: " << *(*max_element(players.begin(), players.end(), Player::ByScore())); //Get the winner by comparator in player header

	for each(Player* player in players)			// Destroy all players, not to be used
		delete player;
	return 0;

}

/*
Method to ask every player (active or not) which row they want to cross a number off
*/
void firstPassiveTurn(vector<Player*> &players, int &whiteDiceAddition, int &incrementMissedTurn, int &currentPlayer)
{
	int row = -1;
	for (int i = 0; i < players.size(); i++)
	{
		cout << "Player " << i + 1 << "(" << *players.at(i)->getName() << "), current Board: \n" << endl;
		players.at(i)->getBoard()->printBoard();
		bool badInput = true;
		while (badInput)
		{
			cout << "Player " << i + 1 << "(" << *players.at(i)->getName() << "), What row would you like to cross off " << whiteDiceAddition << " on? (Enter 0 to Pass Turn, 1 for Row 1 etc.,)" << endl;

			row = getRowInput();		//get the input for row number
			if (row == 0)		//when 0, break regardless and move to next loop
			{
				if (currentPlayer == i) //	BUT if i == currentPlayer, then increment the value for the active player
				{
					incrementMissedTurn++;
				}
				break;
			}
			else if (whiteDiceAddition == 12 && row < 3)		// If top 2 rows, and == 12 (LAST NUMBER IN ROW)
			{
				for (unsigned int r = 0; r < players.at(i)->getBoard()->getBoard()->size(); r++)
				{
					if (players.at(i)->getBoard()->amountCrossedOff(r) > 4) // If 5 or greater numbers already crossed off in the row
					{
						if (players.at(i)->getBoard()->validCrossOff(row - 1, whiteDiceAddition))
						{
							players.at(i)->getBoard()->crossOffNumber(row - 1, whiteDiceAddition);
							badInput = false;
						}
					}
				}
			}

			else if (whiteDiceAddition == 2 && row > 2)		// If bottom 2 rows, and == 2 (LAST NUMBER IN ROW)
			{
				for (unsigned int r = 0; r < players.at(i)->getBoard()->getBoard()->size(); r++)
				{
					if (players.at(i)->getBoard()->amountCrossedOff(r) > 4) // If 5 or greater numbers already crossed off in the row
					{
						if (players.at(i)->getBoard()->validCrossOff(row - 1, whiteDiceAddition))
						{
							players.at(i)->getBoard()->crossOffNumber(row - 1, whiteDiceAddition);
							badInput = false;
						}
					}
				}
			}
			else					// If none of above match, normal move
			{
				if (players.at(i)->getBoard()->validCrossOff(row - 1, whiteDiceAddition))
				{
					players.at(i)->getBoard()->crossOffNumber(row - 1, whiteDiceAddition);
					badInput = false;
				}
			}
		}
	}
	
}

/*	
Method to only ask the Active Player if they want to add a white and colored die together
*/
char secondActiveTurn(int &currentPlayer, vector<Dice*> &dies, vector<Player*> &players)
{
	cout << "Player " << currentPlayer + 1 << "(" << *players.at(currentPlayer)->getName() << "), do you want to add a white and colored die? (Y/N)" << endl;
	bool badInput = true;
	char answer;
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		cout << "Current Board: \n" << endl;

		players.at(currentPlayer)->getBoard()->printBoard();

		cout << "\nAll Dice to choose from: \n" << endl;
		for(int i = 0; i < dies.size(); i++)
		{
			cout << i+1 << ") " << *(dies.at(i)) << endl;
		}

		cout << "\nWhat 2 dice would you like to add together? You must choose 1 White die, and 1 Coloured die. \n" << "An example would be: 1, 4 to add the first White die and the Yellow die" << endl;

		int firstWhiteDie = getWhiteDie(dies);
		int colorDie, addition = 0;
		for (;;)
		{
			cout << "Choose the colored die: (3-6)" << endl;
			if (cin >> colorDie)
			{
				if (colorDie >= 3 && colorDie <= 6)
					break;
			}
			else 
			{
				cout << "Input numbers between 3-6 for the colored dice" << endl;
				cin.clear();
				cin.ignore(5, '\n');
			}
		}

		addition = *(dies.at(firstWhiteDie - 1)->getRoll()) + *(dies.at(colorDie - 1)->getRoll()); // Way to sum up the color and white dies together to get the number to cross off
		cout << "Crossing off " << addition << " on row " << colorDie - 2 << "\n" << endl; // Clarity for the user, - 2 gives correct row

		if (players.at(currentPlayer)->getBoard()->validCrossOff(colorDie - 3, addition))
		{
			players.at(currentPlayer)->getBoard()->crossOffNumber(colorDie - 3, addition);
		}
		else
		{
			cout << "Can't cross that number off, try again" << endl;
			secondActiveTurn(currentPlayer, dies, players);
		}

	}
	else
	{
		cout << "\nMoving to next players turn..." << endl;
	}

	return answer;
}

/*
Method to setup a vector of pointers*/
vector<Dice*> setupDies()
{
	vector<Dice*> dies;
	dies.push_back(new Dice("White", 0));
	dies.push_back(new Dice("White", 0));
	dies.push_back(new Dice("Red", 0));
	dies.push_back(new Dice("Yellow", 0));
	dies.push_back(new Dice("Green", 0));
	dies.push_back(new Dice("Blue", 0));

	return dies;
}

/*
Make a vector of pointers for each individual roll
*/
void rollDies(vector<Dice*> &dies)
{
	for (int i = 0; i < dies.size(); i++)
	{
		dies.at(i)->setRoll(dies.at(i)->roll()); //set the dice roll to the roll method return type
	}

	for each (Dice* die in dies)
	{
		cout << *die << "\n";
	}

}

int addWhiteDies(vector<Dice*> &dies)
{
	int whiteAdd = 0;
	whiteAdd = *(dies.at(0)->getRoll()) + *(dies.at(1)->getRoll());
	return whiteAdd;
}

/*
Deal with input and exceptions for choosing 2 dice to add together
*/
int getWhiteDie(vector<Dice*> &dies)
{
	int firstWhiteDie, secondDieType, addition = 0;
	for (;;) 
	{
		cout << "Choose the white die: (1-2)" << endl;
		if (cin >> firstWhiteDie)
		{
			if (firstWhiteDie == 1 || firstWhiteDie == 2)
				break;
		}
		else 
		{
			cout << "Input 1 or 2 for the first die" << endl;
			cin.clear();
			cin.ignore(5, '\n');
		}
	}
	return firstWhiteDie;
}

int getRowInput()
{
	int row;
	for (;;) {
		cout << "Row: ";
		if (cin >> row) 
		{
			if(row >= 0 && row <= 4)
				break;
		}
		else 
		{
			cout << "Please enter a valid row" << endl;
			cin.clear();
			cin.ignore(5, '\n');
		}
	}

	return row;
}
