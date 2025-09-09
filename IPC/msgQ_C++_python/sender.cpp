#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256
#define QUEUE_NAME "/my_message_queue"

int main() {
  mqd_t mq;
  struct mq_attr attr;

  // Set message queue attributes
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;  // Maximum number of messages in the queue
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  // Create message queue
  mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
  if (mq == (mqd_t)-1) {
    perror("mq_open");
    exit(1);
  }

  // Send messages
  float current_value = 0.00;
  while (current_value <= 1.00) {
    // Update current value
    current_value += 0.01;

    // Send message
    char message[MAX_MSG_SIZE];
    sprintf(message, "%.2f", current_value);
    mq_send(mq, message, strlen(message) + 1, 0);

    usleep(500000);
  }

  // Close message queue
  mq_close(mq);

  return 0;
}
