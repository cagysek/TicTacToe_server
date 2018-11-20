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


int GameLogic::turn(int row, int column, Player* pl)
{
    if ( pl->socket == turn_indicator )
    {
        if ( board[row][column] == 0 )
        {
            board[row][column] = pl->socket;
            cout << "Row: " << row << " Column: " << column << " marked for player: " << pl->name << "." << endl;
            
            turn_indicator = -1;
            return 0;
        }
        else
        {
            cout << "Invalid move. Row: " << row << " Column: " << column << " is already marked." << endl;
            turn(row, column, pl);
            return 0;
        }
    }
    else
    {
        cout << "Invalid move. Player " << pl->name << " is not on turn." << endl;
        return -1;
    }
}



int GameLogic::check_board()
{
    int winner;
    // any of the rows is same
    for ( int i = 0 ; i < SIZE ; i++ )
    {
        if ( board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0 )
        {
            winner = board[i][0];
            return winner;
        }
    }
    
    // any of the columns is same
    for( int i = 0 ; i < SIZE ; i++ )
    {
        if ( board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0 )
        {
            winner = board[0][i];
            return winner;
        }
    }
    
    // 1st diagonal is same
    if( board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0 )
    {
        winner = board[0][0];
        return winner;
    }
    
    // 2nd diagonal is same
    if( board[0][2] == board[1][1] && board[1][1] == board[2][0] && board [0][2] != 0 )
    {
        winner = board[0][2];
        return winner;
    }
    
    // if any empty box on board then play on
    for( int i = 0; i <= SIZE ; i++ )
    {
        for( int j=0 ; j <= SIZE ; j++ )
        {
            if( board[i][j] == 0 )
            {
                winner = 0;
                return winner;
            }
        }
    }
    
    winner = -1; // all boxes full and nobody won so A tie has occurred
    return winner;
}



