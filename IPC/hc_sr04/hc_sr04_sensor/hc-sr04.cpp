#include "GPIO.h"
#include <iostream>
#include <Python.h>
#include <sys/time.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define QUEUE_NAME "/my_message_queue"

unsigned long micros() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000000 + currentTime.tv_usec;
}

bool is_running = true;

// Hàm xử lý tín hiệu khi nhận được ngắt (Ctrl+C)
void signal_handler(int signum) {
    is_running = false;
}

int main() {
    Py_Initialize();

    // Set up Python sys path
    setupPythonSysPath();

    // Set up GPIO mode
    DIO_SetUp("BOARD");

    // Define pin numbers
    int trigPin = 23; // GPIO 23 (BOARD mode)
    int echoPin = 24; // GPIO 24 (BOARD mode)

    // Set GPIO pin mode
    DIO_SetModePin(trigPin, "output");
    DIO_SetModePin(echoPin, "input");

    // (Ctrl+C)
    signal(SIGINT, signal_handler);

    //set up msgQ
    mqd_t msg_queue;
    char buffer[MSG_BUFFER_SIZE];
    int number = 5;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Mở hoặc tạo hàng đợi tin nhắn
    msg_queue = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr);
    if (msg_queue == (mqd_t)-1) {
        perror("Lỗi khi mở hàng đợi tin nhắn");
        exit(1);
    }

    while (is_running) {
        // Send a short pulse to trigger the sensor
        DIO_WritePin(trigPin, "HIGH");
        struct timespec sleepTime;
        sleepTime.tv_sec = 0;
        sleepTime.tv_nsec = 10000; // Wait for 10 microseconds
        nanosleep(&sleepTime, NULL);
        DIO_WritePin(trigPin, "LOW");

        // Wait for the echo to start
        while (DIO_ReadPin(echoPin) == 0) {
        }

        // Record the start time of the echo
        int startTime = micros();

        // Wait for the echo to end
        while (DIO_ReadPin(echoPin) == 1) {
        }

        // Record the end time of the echo
        int endTime = micros();

        // Calculate the duration of the echo in microseconds
        int pulseDuration = endTime - startTime;

        // Calculate the distance based on the speed of sound (343 m/s)
        // and divide by 2 since the sound travels to the object and back
        float distance = pulseDuration * 0.0343 / 2;

        // Print the distance in centimeters
        std::cout << "Distance: " << distance << " cm" << std::endl;

         // Chuyển đổi số float thành chuỗi và gửi vào hàng đợi
        snprintf(buffer, MSG_BUFFER_SIZE, "%f", distance);

        // Gửi tin nhắn vào hàng đợi
        if (mq_send(msg_queue, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Lỗi khi gửi tin nhắn");
            exit(1);
        }

        // In tin nhắn ra màn hình để theo dõi
        std::cout << "Sent message: " << buffer << std::endl;

        // Ngủ một khoảng ngắn
        usleep(500000);

        // Check if the program should be terminated
        if (!is_running) {
            break;
        }
    }

    // Giải phóng chân IO
    DIO_cleanup();

    Py_Finalize();

    // Đóng hàng đợi tin nhắn
    mq_close(msg_queue);

    // In thông báo kết thúc
    std::cout << "Chương trình đã kết thúc." << std::endl;

    return 0;
}

