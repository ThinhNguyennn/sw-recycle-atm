import posix_ipc
import time
import signal

MAX_MSG_SIZE = 256
QUEUE_NAME = "/message_queue_sensor"

def handle(signum,frame):
    exit(0)

signal.signal(signal.SIGINT, handle)

while True:
    try:
        # Mở message queue
        mq = posix_ipc.MessageQueue(QUEUE_NAME, posix_ipc.O_RDONLY)
        # Đọc message
        message, _ = mq.receive()
        # Determine garbage level based on average brightness
        if message == "the level of garbage is full":
            # In giá trị hiện tại
            print(f"Received value: {message}")
            time.sleep(0.5)
        else:
            # In giá trị hiện tại
            print(f"Received value: {message}")
            time.sleep(0.5)
    except KeyboardInterrupt:
        break

# Đóng message queue
mq.close()