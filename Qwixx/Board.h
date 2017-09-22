// Header file for Board class
// Author: David Larkin, 20070186
#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream>;
#include <vector>;
using namespace std;

class Board
{
public:
	Board();
	void printBoard();
	void crossOffNumber(const int choice, const int row);
	vector<vector<int>>* getBoard();
	void removeRow(const int row);
	int amountCrossedOff(const int row);
	bool canLockRow(const int row);
	bool lastNumberCrossedOff(const int row);
	bool validCrossOff(const int row, int choice);
	bool findChoice(vector<vector<int>>* board, const int row, const int choice);
private:
	//filled vector, 0 represents lock
	//numbers 20, 30, 40, 50 to represent colours - 20: Red, 30: Yellow, 40: Green, 50: Blue
	vector<vector<int>> spaces{ 
								{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 20 },
								{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 30 },
								{ 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0, 40 },
								{ 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0, 50 } };
};

#endif // !BOARD_H
