//
//  LogManager.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 15/12/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef LogManager_hpp
#define LogManager_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

using namespace std;
class LogManager
{
public:
    static void log(string filename, string function_name, string text);
    static string curr_datetime();
    
    
private:
};

#endif /* LogManager_hpp */
