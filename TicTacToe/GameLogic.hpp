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
    static const int SIZE = 3;
    
    public:
        GameLogic();
    
        int turn_indicator;
        int** initialize_board();
    
        int turn(int row, int column, Player* pl);
    
    
        int check_board();
    
    private:
        int** board;
    
};






#endif /* GameLogic_hpp */
