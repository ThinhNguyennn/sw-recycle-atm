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

    // Đăng ký hàm xử lý tín hiệu khi nhận được ngắt (Ctrl+C)
    signal(SIGINT, signal_handler);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // Số lượng tin nhắn tối đa trong hàng đợi
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Mở hoặc tạo hàng đợi tin nhắn
    msg_queue = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0666, &attr);
    if (msg_queue == (mqd_t)-1) {
        perror("Lỗi khi mở hàng đợi tin nhắn");
        exit(1);
    }

    while (!g_signal_received) {
        // Nhận tin nhắn từ hàng đợi
        if (mq_receive(msg_queue, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror("Lỗi khi nhận tin nhắn");
            exit(1);
        }

        // Chuyển đổi chuỗi thành số float
        float received_number = strtof(buffer, NULL);

        // In giá trị float ra màn hình
        std::cout << "Received number: " << received_number << std::endl;

        // Ngủ một khoảng ngắn
        usleep(500000);
    }

    // Đóng hàng đợi tin nhắn
    mq_close(msg_queue);

    return 0;
}
