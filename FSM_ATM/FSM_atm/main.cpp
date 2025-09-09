#include "fsm.h"
#include "FSMHelper.h"
#include "mainhelper.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    ATM_recycle ATM_recycle;
    std::string receivedData;

    std::cout << "Current state: " << ATM_recycle.GetCurrentState() << std::endl;

    while (1) {
        std::string current_page = data_change_page();
        std::cout <<  current_page << std::endl;
        //std::string detect_object = detect_message();

        Process_page_Changeloop(current_page);
        //Detect_object_loop(detect_object);
    }
    return 0;
}
