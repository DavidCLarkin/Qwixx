// Board Class
// Author: David Larkin, 20070186

#include <iostream>;
#include "Board.h";
#include <algorithm>;

using namespace std;

Board::Board()
{
}

/**
Method to print the board from the header file multi-dimensional vector. Helper int called notice
included so that, whenever the row is finished, it adds a new line for clarity
**/
void Board::printBoard()
{
	for (unsigned int i = 0; i < spaces.size(); i++)
	{
		for (unsigned int j = 0; j < spaces[i].size()-1; j++) // - 1 so you don't print the number identifier
		{
			cout << spaces[i][j] << " ";
		}

		if (spaces[i].at(12) == 20) // Check for indicator of colour 20 : Red
			cout << "Red";
		if (spaces[i].at(12) == 30) // 30 : Yellow
			cout << "Yellow";
		if (spaces[i].at(12) == 40) // 40 : Green
			cout << "Green";
		if(spaces[i].at(12) == 50) // 50 : Blue
			cout << "Blue";
		cout << endl;
	}
}

/*
Method to cross off a number in the board
*/
void Board::crossOffNumber(const int row, const int choice)
{
	vector<vector<int>>* pSpaces = getBoard();

	if (findChoice(pSpaces, row, choice)) // If number is in board, replace it with -1 (crossed off number)
	{
		replace(pSpaces->at(row).begin(), pSpaces->at(row).end(), choice, -1);
	}
	printBoard();
	cout << "\n";
}

vector<vector<int>>* Board::getBoard()
{
	vector<vector<int>>* pSpaces = &spaces;
	return pSpaces;
}

/**
Method to remove a certain row, to be used when someone crosses off 5
or more of one row, and crosses off the Lock (0) on their Board
**/
void Board::removeRow(const int row)
{
	if (getBoard()->size() > row)
		getBoard()->erase(getBoard()->begin() + row);
	else
		return;
}

/*Get how many numbers are crossed off in a row
*/
int Board::amountCrossedOff(const int row)
{
	int numCrossed = count(getBoard()->at(row).begin(), getBoard()->at(row).end(), -1); //Counts the amount of crossed off numbers

	return numCrossed;
}

/*Checks if you can lock a row for everyone
*/
bool Board::canLockRow(const int row)
{
	if (amountCrossedOff(row) >= 5 && lastNumberCrossedOff(row))
	{
		crossOffNumber(row, 11); //cross off the lock
		return true;
	}

	return false;
}

/*
See if the last number in a row is crossed off
*/
bool Board::lastNumberCrossedOff(const int row)
{
	if (getBoard()->at(row).at(10) == -1) //last number in vector, beside the lock
		return true;
	return false;
}

/*
Method to check for a valid move/crossing off of a number
*/
bool Board::validCrossOff(const int row, int choice)
{
	vector<vector<int>>* temp = getBoard();
	if (temp->at(row).at(choice-2) != -1)
	{
		//if everything to the right of choice is NOT -1, then valid
		if(row < temp->size() / 2) //accounting for removed rows
		{ 
			while (choice <= 10)
			{
				if (temp->at(row).at(choice - 2) == -1) //if any number is a -1, then false
					return false;
				else
					choice++; //increment to test every index
			}
			return true;
		}
		else if (row > temp->size() / 2 - 1) //if last 2 rows
		{
			while (choice >= 0)
			{
				if (temp->at(row).at(12 - choice) == -1) //12 - choice inverts the number so it gives the correct index
					return false;
				else
					choice--;
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}

/*
Helper method to find the number "choice" in the board
*/
bool Board::findChoice(vector<vector<int>>* board, const int row, const int choice)
{
	if (find(board->at(row).begin(), board->at(row).end(), choice) != board->at(row).end())
	{
		return true;
	}
	return false;
}