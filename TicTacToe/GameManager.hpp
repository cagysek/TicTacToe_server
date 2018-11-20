//
//  GameManager.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 19/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include <map>
#include <stack>


#include "Player.hpp"
#include "Game.hpp"

using namespace std;

class GameManager
{
    public:
        static void create_unlogged_player(char *ip, int client_socket);
        static Player* get_logged_player_by_socket(int socked_id);
        void log_player(int client_socket, string name);
        static Player* get_unlogged_player(int id);
        Player* get_logged_player(string name);
        static stack<Player*> players_queue;
    
    private:
        static map<int, Player*> unlogged_players;
        static map<string, Player*> logged_players;
        map<int, Game*> running_games;
};



#endif /* GameManager_hpp */
