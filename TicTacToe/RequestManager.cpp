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
    char delimeter = ';';
    size_t i = msg.find(delimeter, 0);
    string type = msg;
    
    //type of message
    type = msg.substr(0, i);
    
    //rest of message
    msg = msg.substr(i + 1);
    
    if ( strcmp(type.c_str(), "NAME") == 0 )
    {
        i = msg.find(delimeter, 0);
        string name = msg.substr(0, i);
        pl->set_name(name.data());
        cout << "Player with socket: " << pl->socket << " renamed to " << pl->name << endl;
    }
    else if ( strcmp(type.c_str(), "FIND_GAME") == 0 )
    {
        GameManager::want_play(pl);
    }
    else if ( strcmp(type.c_str(), "TURN") == 0 )
    {
        try {
            i = msg.find(delimeter, 0);
            int row = stoi(msg.substr(0, i));
            
            msg = msg.substr(i + 1);
            i = msg.find(delimeter, 0);
            int column = stoi(msg.substr(0, i));
            
            cout << "Player " << pl->name << " move is row: " << row << " column: " << column << endl;
            GameManager::turn(pl, row, column);
        } catch (...) {
            cout << "Invalid input." << endl;
        }
        
    }
    else
    {
        cout << "Action " << type << "is not defined";
    }
}
