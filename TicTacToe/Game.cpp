//
//  Game.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Game.hpp"

Game::Game(int id, Player *p1, Player *p2)
{
    this->id = id;
    this->p1 = p1;
    this->p2 = p2;
}
