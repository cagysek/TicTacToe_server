//
//  Player.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Player.hpp"

Player::Player(char *ip, int socket)
{
    this->ip = ip;
    this->socket = socket;
    this->name = "UnKnow";
    this->want_rematch = false;
    this->connected = 0;
    this->score = 0;
}

void set_name(Player *pl, std::string name)
{
    pl->name = name;
}

Player::~Player()
{
    cout << "Player deleted" << endl;
}
