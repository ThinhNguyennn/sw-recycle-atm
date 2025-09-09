#ifndef MAINHELPER_H
#define MAINHELPER_H

#include "ATM_FSM.h"
#include "fsm.h"
#include "FSMHelper.h"
#include <iostream>
#include <sys/msg.h>
#include <cstring>

/**************************************************************************************
  * 
  * Wordspace: To set a MsgQ attribute
  *
***************************************************************************************/
struct msg_buffer_receiver {
    long msg_type;
    char msg_text[11]; 
};

/*----------------------------- DEFINE FUNCTION -----------------------------------*/   
std::string data_change_page();
std::string receive_message_detect();
void Send_label_detect(const char* message);

void Process_page_Changeloop(std::string current_page);
void Detect_object_loop(std::string detect_message);

#endif //MAINHELPER_H