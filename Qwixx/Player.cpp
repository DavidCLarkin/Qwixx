// Player class
// Author: David Larkin, 20070186

#include <iostream>;
#include "Player.h";

using namespace std;

Player::Player(Board board, Board copyBoard, unsigned int missedTurns, int score, bool activePlayer, string name)
	: m_Board(board), m_MissedTurns(missedTurns), m_Score(score), m_activePlayer(activePlayer), m_Name(name), m_CopyBoard(copyBoard)
{
	m_CopyBoard.getBoard()->resize(m_Board.getBoard()->size());
}

Player::~Player()
{
	//cout << "Destroying player";
}


int* Player::getMissedTurns()
{
	return &m_MissedTurns;
}
int* Player::getScore()
{
	return &m_Score;
}
void Player::setScore(const int score)
{
	m_Score = score;
}
void Player::setMissedTurns(const int missedTurns)
{
	m_MissedTurns = missedTurns;
}

Board* Player::getBoard()
{
	return &m_Board;
}

bool* Player::getActivePlayer()
{
	return &m_activePlayer;
}

void Player::setActivePlayer(const bool activePlayer)
{
	m_activePlayer = activePlayer;
}

string* Player::getName()
{
	return &m_Name;
}

Board* Player::getCopyBoard()
{
	return &m_CopyBoard;
}

void Player::setCopyBoard(Board board)
{
	copy(m_CopyBoard.getBoard()->begin(), m_CopyBoard.getBoard()->end(), board.getBoard()->begin());
}

void Player::setName(string name)
{
	m_Name = name;
}

void Player::calculateScore()
{
	Board* board = this->getBoard();
	int score = 0;

	for (int row = 0; row < board->getBoard()->size(); row++)
	{
		int amountCrossedOff = board->amountCrossedOff(row);
		score += (amountCrossedOff*(amountCrossedOff + 1) / 2); //algorithm for triangular numbers
	}

	score -= m_MissedTurns * 5;

	this->setScore(score);
}

ostream& operator <<(ostream &stream, Player &player)
{
	return stream << "Player(" << "Score: " << *(&player.m_Score) << ", Name: " << *(&player.m_Name) << ")"; //dereferencing so it shows values, not addresses
}
