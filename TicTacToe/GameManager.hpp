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
#include "GameLogic.hpp"
#include "ResponseManager.hpp"

using namespace std;

class GameManager
{
    public:
        static Player* get_logged_player_by_socket(int socked_id);
    
        static Player* get_unlogged_player(int id);
    
        static void create_unlogged_player(char *ip, int client_socket);
        static void want_play(Player *pl);
        static void turn(Player *pl, int row, int column);
    
        Player* get_logged_player(string name);
        void log_player(int client_socket, string name);

    static Game* get_running_game(int id);
    static void rematch(Player* pl);
    
    private:
        static map<int, Player*> unlogged_players;
        static map<string, Player*> logged_players;
    
        static map<int, Game*> running_games;
        static stack<Player*> players_queue;
    
        static void create_game(Player* pl1, Player* pl2);
        static Player* find_opponent();
    
        static int game_id_generator;
        static void resolve_result(Player* pl, int result);
};



#endif /* GameManager_hpp */
