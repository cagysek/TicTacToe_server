//
//  MessageManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 19/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "RequestManager.hpp"



void RequestManager::resolve(Player *pl, std::string msg)
{
    pl->msg_in = msg;
    
    vector<string> msg_parts = Utils::split(msg, ";");

    //type of message
    string type = msg_parts[0];
    
    if ( type.compare("NAME") == 0 )
    {
        string name = msg_parts[1];
        GameManager::log_player_resolve(pl->socket, name);
        //GameManager::log_player(pl->socket, name);
        //pl->set_name(name.data());
    }
    else if ( type.compare("FIND_GAME") == 0 )
    {
        GameManager::want_play(pl);
    }
    else if ( type.compare("TURN") == 0 )
    {
        try {
            int row = stoi(msg_parts[1]);
            
            int column = stoi(msg_parts[2]);
            
            cout << "Player " << pl->name << " move is row: " << row << " column: " << column << endl;
            GameManager::turn(pl, row, column);
        } catch (...) {
            cout << "Invalid input." << endl;
        }
    }
    else if ( type.compare("REMATCH") == 0 )
    {
        GameManager::rematch(pl);
    }
    else if ( type.compare("CLOSE_GAME") == 0 )
    {
        GameManager::close_game(pl);
    }
    else if ( type.compare("EXIT") == 0)
    {
        GameManager::exit(pl);
    }
    else
    {
        cout << "Action " << type << " is not defined";
    }
}
