// Header file for AIPlayer
// Author: David Larkin, 20070186

#pragma once
#ifndef AIPLAYER_H
#define AIPlayer_H

#include "Board.h";
#include <string>;
#include "Player.h";

using namespace std;

class AIPlayer : public Player
{
public:
	using Player::Player;
};
#endif // !AIPLAYER_H