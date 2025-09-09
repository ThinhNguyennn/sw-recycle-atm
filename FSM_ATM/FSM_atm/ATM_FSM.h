#include "fsm.h"
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>

/**************************************************************************************
* 
*                WORDSPACE FOR DATATYPE OF FINITE STATE MACHINE
* 
***************************************************************************************/
    
    /*!
     * @brief error's type of the FSM in the project ATM Recycle
     *        The enum datatype contains the errors of the peripherals
     */
    typedef enum {
      no_error,
      sensor_garbage_lv_max_error,
      camera_garbage_lv_max_error,
      sensor_camera_garbage_lv_max_error,
      sensor_error,
      camera_error,
      garbage_lv_max_error
    } INIT_t;

    /*!
     * @brief type of message queue SEND 
     *        set the struct of message buffer sending
     */
    struct msg_buffer_send {
        long msg_type;
        char msg_text[160];
    };
    
class ATM_recycle: public fsmlite::fsm<ATM_recycle> {
    //! grant base class access to private transition_table
    friend class fsmlite::fsm<ATM_recycle>;

public:

    /*!
     * @brief States of the FSM in the project ATM Recycle
     *        The enum datatype contains the states of the FSM
     */
      typedef enum {
        INIT,                         //!< State of beginning project
        LOGIN,                        //!< State of Login
        ERROR_DEVICE_GARBAGELV,       //!< State of processing when another device disconnected or garbage level is MAX
        INFORMATION,                  //!< State of beginning Running substate with the information of users      
        GARBAGE,                      //!< State of information of detect garbage 
        PLUS_POINT,                   //!< State of stored point
        CONTACT                       //!< State of contact between user and admin
      }states_t;

    ATM_recycle(state_type init_state = INFORMATION) : fsm(init_state) {}

    /*!
     * @brief Simple event declare as below
     *        This contains only simple struct as an event of main Finite State Machine
     */
    //! @brief Event checking again when we reset from ERROR state to INIT state
    struct Recheck_on_Evt {};
    //! @brief Event checking failed garbage level max, or devices are disconected
    struct error_device_garbagelv_Evt {};
    //! @brief Event complete checking device and garbage level and moving to LOGIN state
    struct Check_ID_Evt {};
    //! @brief Event checking ID is correct and moves to PROCESSING state
    struct correct_ID_Evt {};
    //! @brief Event checking ID is wrong and re-enter the ID
    struct wrong_ID_Evt {};
    //! @brief Event Logging out when the users want to out of process
    struct Logout_Evt {};

    /*!
     * @brief Simple event declare as below
     *         This contains only simple struct as an event of SubStateMachine [PROCESSING]
     */

    //! @brief Event changing page INFORMATIONtoGARBAGE in main window
    struct Change_page_INFORMATIONtoGARBAGE_Evt {};
    //! @brief Event changing page INFORMATIONtoPLUSPOINT in main window
    struct Change_page_INFORMATIONtoPLUSPOINT_Evt {};
    //! @brief Event changing page INFORMATIONtoCONTACT in main window
    struct Change_page_INFORMATIONtoCONTACT_Evt {};

    //! @brief Event changing page GARBAGEtoiNFORMATION in main window
    struct Change_page_GARBAGEtoiNFORMATION_Evt {};
    //! @brief Event changing page GARBAGEtoPLUSPOINT in main window
    struct Change_page_GARBAGEtoPLUSPOINT_Evt {};
    //! @brief Event changing page GARBAGEtoCONTACT in main window
    struct Change_page_GARBAGEtoCONTACT_Evt {};
    
    //! @brief Event changing page PLUSPOINTtoGARBAGE in main window
    struct Change_page_PLUSPOINTtoGARBAGE_Evt {};
    //! @brief Event changing page PLUSPOINTtoINFORMATION in main window
    struct Change_page_PLUSPOINTtoINFORMATION_Evt {};
    //! @brief Event changing page PLUSPOINTtoCONTACT in main window
    struct Change_page_PLUSPOINTtoCONTACT_Evt {};

    //! @brief Event changing page CONTACTtoPLUSPOINT in main window
    struct Change_page_CONTACTtoGARBAGE_Evt {};
    //! @brief Event changing page CONTACTtoPLUSPOINT in main window
    struct Change_page_CONTACTtoPLUSPOINT_Evt {};
    //! @brief Event changing page CONTACTONtoINFORMATION in main window
    struct Change_page_CONTACTONtoINFORMATION_Evt {};

public:  

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
    INIT_t Checking_devices_garbagelv_Act();

/**************************************************************************************
 * function name: Reset_ClickOn_INIT()
 * Function: Turning back to the INIT state and rechecking the device and garbage level
 * state: ERROR_DEVICE_GARBAGELV state
 * Return: None
***************************************************************************************/
    void Reset_ClickOn_INIT();

/**************************************************************************************
 * function name: error_device_garbagelv_Act()
 * Function: Check the error connecting of devices and the level of garbage is Max
 * state: ERROR_DEVICE_GARBAGELV state
 * Return: INIT_t
***************************************************************************************/
    INIT_t error_device_garbagelv_Act();
    
/**************************************************************************************
 * function name: Check_ID_Act()
 * Function: Check the ID of user
 * state: LOGIN state
 * Return: bool
***************************************************************************************/
    bool Check_ID_Act();
    
/**************************************************************************************
 * function name: Log_out_Act()
 * Function: Logging out when the user want to out the processing
 * state: PROCESSING state
 * Return: void
***************************************************************************************/
    void Log_outBtn_Act();

/**************************************************************************************
 * function name: INFORMATION_Act()
 * Function: the main action in the INFORMATION state
 * state: INFORMATION state
 * Return: None
***************************************************************************************/
    void INFORMATION_Act();
    
/**************************************************************************************
 * function name: GARBAGE_Act()
 * Function: the main action in the GARBAGE state
 * state: GARBAGE state
 * Return: None
***************************************************************************************/
    void GARBAGE_Act();

/**************************************************************************************
 * function name: PLUS_POIT_Act()
 * Function: the main action in the PLUS_POIT state
 * state: PLUS_POIT state
 * Return: None
***************************************************************************************/
    void PLUS_POINT_Act();

/**************************************************************************************
 * function name: CONTACT_Act()
 * Function: the main action in the CONTACT state
 * state: CONTACT state
 * Return: None
***************************************************************************************/
    void CONTACT_Act();

private:
    using m = ATM_recycle;  // for brevity
    
    using transition_table = table<
        //                 Start                       Event                                          Target                                Action                              Guard
        //---------+-------------------+----------------------------------------------------+-----------------------------------+-------------------------------------------+-----------------+
            row<            INIT,                     error_device_garbagelv_Evt,              ERROR_DEVICE_GARBAGELV,              &m::error_device_garbagelv_Act              /*none*/ >,
            row<            INIT,                     Check_ID_Evt,                            LOGIN,                               &m::Check_ID_Act                            /*none*/ >,
            row<            ERROR_DEVICE_GARBAGELV,   Recheck_on_Evt,                          INIT,                                &m::Checking_devices_garbagelv_Act          /*none*/ >,
            row<            LOGIN,                    correct_ID_Evt,                          INFORMATION,                         &m::INFORMATION_Act                         /*none*/ >,
            row<            LOGIN,                    wrong_ID_Evt,                            LOGIN,                               &m::Check_ID_Act                            /*none*/ >,
            row<            INFORMATION,              Change_page_INFORMATIONtoGARBAGE_Evt,    GARBAGE,                             &m::GARBAGE_Act                             /*none*/ >,
            row<            INFORMATION,              Change_page_INFORMATIONtoPLUSPOINT_Evt,  PLUS_POINT,                          &m::PLUS_POINT_Act                          /*none*/ >,
            row<            INFORMATION,              Change_page_INFORMATIONtoCONTACT_Evt,    CONTACT,                             &m::CONTACT_Act                             /*none*/ >,
            row<            GARBAGE,                  Change_page_GARBAGEtoiNFORMATION_Evt,    INFORMATION,                         &m::INFORMATION_Act                         /*none*/ >,
            row<            GARBAGE,                  Change_page_GARBAGEtoPLUSPOINT_Evt,      PLUS_POINT,                          &m::PLUS_POINT_Act                          /*none*/ >,
            row<            GARBAGE,                  Change_page_GARBAGEtoCONTACT_Evt,        CONTACT,                             &m::CONTACT_Act                             /*none*/ >,
            row<            PLUS_POINT,               Change_page_PLUSPOINTtoGARBAGE_Evt,      GARBAGE,                             &m::GARBAGE_Act                             /*none*/ >,
            row<            PLUS_POINT,               Change_page_PLUSPOINTtoINFORMATION_Evt,  INFORMATION,                         &m::INFORMATION_Act                         /*none*/ >,
            row<            PLUS_POINT,               Change_page_PLUSPOINTtoCONTACT_Evt,      CONTACT,                             &m::CONTACT_Act                             /*none*/ >,
            row<            CONTACT,                  Change_page_CONTACTtoGARBAGE_Evt,        GARBAGE,                             &m::GARBAGE_Act                             /*none*/ >,
            row<            CONTACT,                  Change_page_CONTACTtoPLUSPOINT_Evt,      PLUS_POINT,                          &m::PLUS_POINT_Act                          /*none*/ >,
            row<            CONTACT,                  Change_page_CONTACTONtoINFORMATION_Evt,  INFORMATION,                         &m::INFORMATION_Act                         /*none*/ >,
            row<            INFORMATION,              Logout_Evt,                              LOGIN,                               &m::Check_ID_Act                            /*none*/ >,
            row<            GARBAGE,                  Logout_Evt,                              LOGIN,                               &m::Check_ID_Act                            /*none*/ >,
            row<            PLUS_POINT,               Logout_Evt,                              LOGIN,                               &m::Check_ID_Act                            /*none*/ >,
            row<            CONTACT,                  Logout_Evt,                              LOGIN,                               &m::Check_ID_Act                            /*none*/ >>;
};