import posix_ipc
import time

MAX_MSG_SIZE = 256
QUEUE_NAME = "/my_message_queue"

# Mở message queue
mq = posix_ipc.MessageQueue(QUEUE_NAME, posix_ipc.O_RDONLY)

# Khởi tạo biến current_value
current_value = 0.00

# Đọc các message
while current_value <= 1.00:
  # Đọc message
  message, _ = mq.receive()

  # Cập nhật current_value
  current_value = float(message.decode('utf-8').strip('\x00'))

  # In giá trị hiện tại
  print(f"Received value: {current_value}")
  
  time.sleep(0.5)

# Đóng message queue
mq.close()
