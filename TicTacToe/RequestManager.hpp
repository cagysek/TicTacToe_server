//
//  RequestManager.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 19/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef RequestManager_hpp
#define RequestManager_hpp

#include <stdio.h>
#include "Player.hpp"
#include "GameManager.hpp"

class RequestManager
{
    public:
        static void resolve(Player *pl, std::string msg);
    
};



#endif /* RequestManager_hpp */
