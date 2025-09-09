#include "ATM_FSM.h"

using namespace fsmlite;


/**************************************************************************************
 * send_page_application(const char* message)
 * Function: Send page want to change to Application
 * state: 4 state in PROCESSING
 * Return: None
***************************************************************************************/
void send_page_application(const char* message) {
    //!< key 56789 is key id for page message between FSM and Application
    key_t key = 56789;

    //!< Creating or connect to Queue
    int msgid_sender = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

    //!< If the queue already exists, do not use the IPC_CREAT flag and do not check for errors
    if (msgid_sender == -1 && errno == EEXIST) {
        msgid_sender = msgget(key, 0666);
    } else if (msgid_sender == -1) {    
        perror("msgget");  //!< if error connect, print the error
        return;
    }

    //!< Send page message to Application
    msg_buffer_send msg_send;
    msg_send.msg_type = 1;
    strncpy(msg_send.msg_text, message, sizeof(msg_send.msg_text) - 1);
    msg_send.msg_text[sizeof(msg_send.msg_text) - 1] = '\0';  //!< Ensure null-terminated

    //!< start sending
    if (msgsnd(msgid_sender, &msg_send, sizeof(msg_send.msg_text), 0) == -1) {
        perror("msgsnd");
        return;
    } else {
        std::cout << "send complete" << std::endl;
    }
}

/**************************************************************************************
* 
*                WORDSPACE FOR FUNCTION OF FINITE STATE MACHINE
* 
***************************************************************************************/

/**************************************************************************************
 * function name: Checking_devices_garbagelv_Act()
 * Function: Check the connecting of devices and the level of garbage
 * state: INIT state
 * Return: INIT_t
***************************************************************************************/
INIT_t ATM_recycle::Checking_devices_garbagelv_Act()
{
    return INIT_t::sensor_garbage_lv_max_error;
}

/**************************************************************************************
 * function name: Reset_ClickOn_INIT()
 * Function: Turning back to the INIT state and rechecking the device and garbage level
 * state: ERROR_DEVICE_GARBAGELV state
 * Return: None
***************************************************************************************/
void ATM_recycle::Reset_ClickOn_INIT()
{

}

/**************************************************************************************
 * function name: error_device_garbagelv_Act()
 * Function: Check the error connecting of devices and the level of garbage is Max
 * state: ERROR_DEVICE_GARBAGELV state
 * Return: INIT_t
***************************************************************************************/
INIT_t ATM_recycle::error_device_garbagelv_Act()
{
    return INIT_t::sensor_garbage_lv_max_error;
}

/**************************************************************************************
 * function name: Check_ID_Act()
 * Function: Check the ID of user
 * state: LOGIN state
 * Return: bool
***************************************************************************************/
bool ATM_recycle::Check_ID_Act()
{
    return true;
}

/**************************************************************************************
 * function name: Log_out_Act()
 * Function: Logging out when the user want to out the processing
 * state: PROCESSING state
 * Return: void
***************************************************************************************/
void ATM_recycle::Log_outBtn_Act()
{

}

/**************************************************************************************
 * function name: INFORMATION_Act()
 * Function: the main action in the INFORMATION state
 * state: INFORMATION state
 * Return: None
***************************************************************************************/
void ATM_recycle::INFORMATION_Act()
{
    std::cout << "in information page" << std::endl;

    send_page_application("Information");
}

/**************************************************************************************
 * function name: GARBAGE_Act()
 * Function: the main action in the GARBAGE state
 * state: GARBAGE state
 * Return: None
***************************************************************************************/
void ATM_recycle::GARBAGE_Act()
{
    std::cout << "in garbage page" << std::endl;
    send_page_application("Garbage");
}

/**************************************************************************************
 * function name: PLUS_POIT_Act()
 * Function: the main action in the PLUS_POIT state
 * state: PLUS_POIT state
 * Return: None
***************************************************************************************/
void ATM_recycle::PLUS_POINT_Act()
{
    std::cout << "in plus point page" << std::endl;
    send_page_application("Pluspoint");
}

/**************************************************************************************
 * function name: CONTACT_Act()
 * Function: the main action in the CONTACT state
 * state: CONTACT state
 * Return: None
***************************************************************************************/
void ATM_recycle::CONTACT_Act()
{
    std::cout << "in contact page" << std::endl;
    send_page_application("Contact");
}