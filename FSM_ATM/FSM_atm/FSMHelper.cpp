#include "FSMHelper.h"
#include <iostream>

const char* FsmHelper::toStateName(int state)
{
    //todo: [TaiTN] clean up duplication
    typedef enum {
        INIT,                         //!< State of beginning project
        LOGIN,                        //!< State of Login
        ERROR_DEVICE_GARBAGELV,       //!< State of processing when another device disconnected or garbage level is MAX
        INFORMATION,                  //!< State of beginning Running substate with the information of users      
        GARBAGE,                      //!< State of information of detect garbage 
        PLUS_POINT,                   //!< State of stored point
        CONTACT                       //!< State of contact between user and admin
    }state_t;
    switch (static_cast<state_t>(state))
    {
    case state_t::INIT:
        return "INIT";
    case state_t::LOGIN:
        return "LOGIN";
    case state_t::ERROR_DEVICE_GARBAGELV:
        return "ERROR_DEVICE_GARBAGELV";
    case state_t::INFORMATION:
        return "INFORMATION";
    case state_t::GARBAGE:
        return "GARBAGE";
    case state_t::PLUS_POINT:
        return "PLUS_POINT";
    case state_t::CONTACT:
        return "CONTACT";

    default:
        std::cout << "ERROR UNKNOWN STATE !" << std::endl;
        return "UNKNOWN";
    }
}