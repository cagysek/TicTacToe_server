//
//  LogManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 15/12/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "LogManager.hpp"

void LogManager::log(string file, string function, string text)
{
    cout << LogManager::curr_datetime() << " [" << file << "]" << " " << function << " : " << text << endl;
}


string LogManager::curr_datetime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    
    return buf;
}
