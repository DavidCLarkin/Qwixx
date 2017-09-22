// Header file for Player
// Author: David Larkin, 20070186
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h";
#include <string>;

using namespace std;

class Player
{
public:
	Player(Board board, Board copyBoard, unsigned int missedTurns = 0, int score = 0, bool activePlayer = false, string name = "");
	~Player();
	int* getMissedTurns();
	int* getScore();
	bool* getActivePlayer();
	Board* getBoard();
	string* getName();
	Board* getCopyBoard();
	void setScore(const int score);
	void setMissedTurns(const int missedTurns);
	void setActivePlayer(const bool activePlayer);
	void calculateScore();
	void setCopyBoard(Board board);
	void setName(string name);

	// Way to compare scores of players for the end of the game
	struct ByScore
	{
		bool operator ()(const Player *a, const Player *b) const
		{
			return a->m_Score < b->m_Score;
		}
	};

private:
	friend ostream& operator <<(ostream &stream, Player &player);
	int m_MissedTurns;
	int m_Score;
	string m_Name;
	Board m_Board;
	Board m_CopyBoard;
	bool m_activePlayer;
};

#endif // !PLAYER_H

