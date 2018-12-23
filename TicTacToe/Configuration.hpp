//
//  Configuration.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 22/12/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef Configuration_hpp
#define Configuration_hpp

#include <stdio.h>
#include <string>
#include <stdio.h>

using namespace std;

class Configuration
{
public:
    string ip;
    int port;
    int max_games;
    
    Configuration(string ip, int port, int max_games);
    
};

#endif /* Configuration_hpp */
