//
//  Network.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include <stdio.h>
#include "Player.hpp"

class Network
{
    public:
        static void waitForPlayer();
    private:
        static struct sockaddr_in client;
    
    
};


#endif /* Network_hpp */
