//
//  Game.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "Player.hpp"
#include "GameLogic.hpp"
#include <map>
#include "LogManager.hpp"

class Game
{
    public:
        Player *p1;
        Player *p2;
        int id;
        int size = 3;
    
        GameLogic *gameLogic;
    
        Game(int id, Player *p1, Player *p2);
        Player* get_opponent(Player* pl);
        Player* get_player_1()          {return p1; };
        Player* get_player_2()          {return p2; };
    
        ~Game();
};



#endif /* Game_hpp */
