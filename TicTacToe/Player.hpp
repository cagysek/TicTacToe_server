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
#include <thread>

#include "LogManager.hpp"

using namespace std;

class Player
{
    public:
        char *ip;
        int socket;
        int game_id;
    
        int connected;
    
        int score;
        int game_indicator;
    
        int invalid_message_counter;
    
        bool ping_status;
    
        string name;
        string msg_in;
        string msg_out;
    
        string state;
    
        bool want_rematch;
    
        thread ping_thread;
        bool is_thread_running;
        bool is_exists;
    
        Player(char *ip, int socket);
        ~Player();
    
        void set_name(string new_name)     {name = new_name; };
};


#endif /* Player_hpp */
