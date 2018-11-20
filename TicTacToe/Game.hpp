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


class Game
{
    public:
        Player *p1;
        Player *p2;
        int id;
    
        Game(int id, Player *p1, Player *p2);
    
};



#endif /* Game_hpp */
