#include <iostream>
#include <sys/msg.h>
#include <cstring>

struct msg_buffer {
    long msg_type;
    char msg_text[250];
};

int main() {
    // Sử dụng key cố định
    key_t key = 12345;

    // Tạo message queue hoặc kết nối đến message queue đã tồn tại
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    while (1)
    {
    // Nhận thông điệp
    msg_buffer msg;
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }

    std::cout << "Received message: " << msg.msg_text << std::endl;
    }
    return 0;
}
