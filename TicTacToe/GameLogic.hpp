//
//  GameLogic.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 20/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef GameLogic_hpp
#define GameLogic_hpp

#include <stdio.h>
#include "Player.hpp"

using namespace std;

class GameLogic
{
    
    
    public:
        GameLogic();
        static const int SIZE = 3;
        int turn_indicator;
        int** initialize_board();
    
        int turn(int row, int column, Player* pl);
    
        void reset_board();
        int check_board();
        int get_value(int row, int column);
    
        Player* last_winner;
    
    private:
        int** board;
    
};






#endif /* GameLogic_hpp */
