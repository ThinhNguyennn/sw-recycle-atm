#include <iostream>
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

volatile sig_atomic_t g_signal_received = 0;

// Hàm xử lý tín hiệu khi nhận được ngắt (Ctrl+C)
void signal_handler(int signum) {
    g_signal_received = 1;
}

int main() {
    mqd_t msg_queue;
    char buffer[MSG_BUFFER_SIZE];
    int number = 5;

    // Đăng ký hàm xử lý tín hiệu khi nhận được ngắt (Ctrl+C)
    signal(SIGINT, signal_handler);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // Số lượng tin nhắn tối đa trong hàng đợi
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Mở hoặc tạo hàng đợi tin nhắn
    msg_queue = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr);
    if (msg_queue == (mqd_t)-1) {
        perror("Lỗi khi mở hàng đợi tin nhắn");
        exit(1);
    }

    while (!g_signal_received) {
        // Chuyển đổi số nguyên thành chuỗi để gửi
        snprintf(buffer, MSG_BUFFER_SIZE, "%d", number);

        // Gửi tin nhắn đến hàng đợi
        if (mq_send(msg_queue, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Lỗi khi gửi tin nhắn");
            exit(1);
        }

        // Ngủ một khoảng ngắn
        sleep(1);
    }

    // Đóng hàng đợi tin nhắn
    mq_close(msg_queue);

    return 0;
}
