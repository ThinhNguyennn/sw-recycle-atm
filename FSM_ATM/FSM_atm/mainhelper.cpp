#include "mainhelper.h"


/*------------------------------- FUNCTION CHANGE PAGE ------------------------------*/

/**************************************************************************************
  * function name: data_change_page()
  * Function: Get infor_page from the app interface
  * state: 4 states of interface
  * Return: std::string
***************************************************************************************/
std::string data_change_page() {
    //!< key 12345 is key id for btn message between FSM and Application
    key_t key = 12345;

    //!< Create a message queue or connect to an existing message queue
    int msgid_receiver = msgget(key, IPC_CREAT | 0666);
    if (msgid_receiver == -1) {
        perror("msgget");
        return "Error getting message queue";
    }

    //!< Receive the message
    msg_buffer_receiver msg;
    if (msgrcv(msgid_receiver, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
    }
    
    std::string result(msg.msg_text, sizeof(msg.msg_text));
    size_t nullPos = result.find_first_of('\0');
    if(nullPos != std::string::npos)
    {
      result.erase(nullPos);
    }
    return result;
}


/**************************************************************************************
  * function name: Process_page_Changeloop(std::string current_page)
  * Function: processing changes page
  * state: PROCESSING state
  * Return: None
***************************************************************************************/
void Process_page_Changeloop(std::string current_page)
{
  ATM_recycle ATM_recycle;
  //!< Changing page when start page = "INFORMATION"
  if ( (current_page == "Contact") && (ATM_recycle.GetCurrentState() == "INFORMATION") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_INFORMATIONtoCONTACT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Information") && (ATM_recycle.GetCurrentState() == "INFORMATION") )
  {
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Garbage") && (ATM_recycle.GetCurrentState() == "INFORMATION") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_INFORMATIONtoGARBAGE_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "PlusPoint") && (ATM_recycle.GetCurrentState() == "INFORMATION") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_INFORMATIONtoPLUSPOINT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } 
  
  //!< Changing page when start page = "GARBAGE"
  else if ( (current_page == "Garbage") && (ATM_recycle.GetCurrentState() == "GARBAGE") )
  {
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Contact") && (ATM_recycle.GetCurrentState() == "GARBAGE") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_GARBAGEtoCONTACT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Information") && (ATM_recycle.GetCurrentState() == "GARBAGE") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_GARBAGEtoiNFORMATION_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "PlusPoint") && (ATM_recycle.GetCurrentState() == "GARBAGE") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_GARBAGEtoPLUSPOINT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  }

  //!< Changing page when start page = "PLUS_POINT"
  else if ( (current_page == "PlusPoint") && (ATM_recycle.GetCurrentState() == "PLUS_POINT") )
  {
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Contact") && (ATM_recycle.GetCurrentState() == "PLUS_POINT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_PLUSPOINTtoCONTACT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Information") && (ATM_recycle.GetCurrentState() == "PLUS_POINT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_PLUSPOINTtoINFORMATION_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Garbage") && (ATM_recycle.GetCurrentState() == "PLUS_POINT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_PLUSPOINTtoGARBAGE_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  }

  //!< Changing page when start page = "CONTACT"
  else if ( (current_page == "Contact") && (ATM_recycle.GetCurrentState() == "CONTACT") )
  {
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "PlusPoint") && (ATM_recycle.GetCurrentState() == "CONTACT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_CONTACTtoPLUSPOINT_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Information") && (ATM_recycle.GetCurrentState() == "CONTACT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_CONTACTONtoINFORMATION_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  } else if ( (current_page == "Garbage") && (ATM_recycle.GetCurrentState() == "CONTACT") )
  {
    ATM_recycle.process_event(ATM_recycle::Change_page_CONTACTtoGARBAGE_Evt());
    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;
  }
}

/*-------------------------- FUNCTION DISPLAY LABEL GARBAGE -------------------------*/

/**************************************************************************************
  * function name: std::string detect_message()
  * Function: get label of garbage from process detection
  * state: GARBAGE STATE
  * Return: std::string
***************************************************************************************/
// std::string receive_message_detect() {
//     //!< key 30042000 is key id for label_object between FSM and object-detection
//     key_t detect_key = 30042000;

//     //!< Create a message queue or connect to an existing message queue
//     int msgid_receiver = msgget(detect_key, IPC_CREAT | 0666);
//     if (msgid_receiver == -1) {
//         perror("msgget");
//         return "Error getting message queue";
//     }

//     //!< Receive message
//     msg_buffer_receiver msg;
//     if (msgrcv(msgid_receiver, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
//         perror("msgrcv");
//     }

//     return msg.msg_text;
// }

/**************************************************************************************
 * send_page_application(const char* message)
 * Function: Send page want to change
 * state: PROCESSING state
 * Return: None
***************************************************************************************/
// void Send_label_detect(const char* message) {
//     //!< key 30042001 is key id for label_object between FSM and Application
//     key_t key = 30042001;

//     //!< Create a message queue or connect to an existing message queue
//     int msgid_sender = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

//     //!< If the queue already exists, do not use the IPC_CREAT flag and do not check for errors
//     if (msgid_sender == -1 && errno == EEXIST) {
//         msgid_sender = msgget(key, 0666);
//     } else if (msgid_sender == -1) {
//         perror("msgget"); //!< if error connect, print the error
//         return;
//     }

//     //!< Send page message to Application
//     msg_buffer_send msg_send;
//     msg_send.msg_type = 1;
//     strncpy(msg_send.msg_text, message, sizeof(msg_send.msg_text) - 1);
//     msg_send.msg_text[sizeof(msg_send.msg_text) - 1] = '\0';  //!< Ensure null-terminated

//     if (msgsnd(msgid_sender, &msg_send, sizeof(msg_send.msg_text), 0) == -1) {
//         perror("msgsnd");
//         return;
//     } else {
//         std::cout << "send complete" << std::endl;
//     }
// }

/**************************************************************************************
  * function name: Detect_object_Changeloop(std::string detect_message)
  * Function: Processing 
  * state: GARBAGE state
  * Return: None
***************************************************************************************/
// void Detect_object_loop(std::string detect_message)
// {
//   if ( (detect_message = "plastic_bottle") || (detect_message = "can") || (detect_message = "glass_bottle") && (ATM_recycle.GetCurrentState() == "GARBAGE") )
//   {
//     Send_label_detect(detect_message);
//   } else 
//   {
//     Send_label_detect("No object detects")
//   }
// }
