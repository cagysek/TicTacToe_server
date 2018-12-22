//
//  GameLogic.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 20/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "GameLogic.hpp"

GameLogic::GameLogic()
{
    this->board = initialize_board();
}

/**
 *  Initialize game board
 */
int** GameLogic::initialize_board()
{
    int** new_board = new int*[SIZE];
    
    for ( int i = 0 ; i < SIZE ; i++ )
    {
        new_board[i] = new int[SIZE];
        for ( int j = 0 ; j < SIZE ; j++ )
        {
            new_board[i][j] = 0;
        }
    }
    
    return new_board;
}

/**
 *  Reset all cells to 0
 */
void GameLogic::reset_board()
{
    for ( int i = 0 ; i < SIZE ; i++ )
    {
        for ( int j = 0 ; j < SIZE ; j++ )
        {
            board[i][j] = 0;
        }
    }
}

/**
 *  Resolve player turn
 */
int GameLogic::turn(int row, int column, Player* pl)
{
    if (row < 0 || column < 0 || row >= SIZE || column >= SIZE)
    {
        return -1;
    }
    
    if ( pl->game_indicator == turn_indicator )
    {
        if ( board[row][column] == 0 )
        {
            board[row][column] = pl->game_indicator;
            
            turn_indicator = -1;
            return 0;
        }
        else
        {
            //already marked
            return 1;
        }
    }
    else
    {
        //not on turn
        return 2;
    }
}

/**
 *  Check gameboard and determine about winner or if is possible to continue in game
 */
int GameLogic::check_board()
{
    int winner;
    // any of the rows is same
    for ( int i = 0 ; i < SIZE ; i++ )
    {
        if ( board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0 )
        {
            return 1;
        }
    }
    
    // any of the columns is same
    for( int i = 0 ; i < SIZE ; i++ )
    {
        if ( board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0 )
        {
            return 1;
        }
    }
    
    // 1st diagonal is same
    if( board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0 )
    {
        return 1;
    }
    
    // 2nd diagonal is same
    if( board[0][2] == board[1][1] && board[1][1] == board[2][0] && board [0][2] != 0 )
    {
        return 1;
    }
    
    // if any empty box on board then play on
    for( int i = 0; i < SIZE ; i++ )
    {
        for( int j=0 ; j < SIZE ; j++ )
        {
            if( board[i][j] == 0 )
            {
                return 0;
            }
        }
    }
    
    winner = -1; // all boxes full and nobody won tie has occurred
    return winner;
}

/**
 *  Get value on specific coordinates
 */
int GameLogic::get_value(int row, int column)
{
    return board[row][column];
}


