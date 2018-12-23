//
//  Configuration.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 22/12/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Configuration.hpp"

Configuration::Configuration(string ip, int port, int max_games)
{
    this->ip = ip;
    this->port = port;
    this->max_games = max_games;
}
