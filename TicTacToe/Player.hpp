//
//  Player.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <iostream>

using namespace std;

class Player
{
    public:
        char *ip;
        int socket;
        char *message_in;
        std::string name;
        int game_id;
    
        Player(char *ip, int socket);
    
        void set_name(string new_name)     {name = new_name; };
};


#endif /* Player_hpp */
