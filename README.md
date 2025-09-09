# SW Recycle ATM

Một bộ mã mẫu cho hệ thống ATM tái chế, gồm 3 phần chính:
- Ứng dụng giao diện người dùng (PyQt5) trong `SW_20Recycle_20ATM/application_ATMRecycle`
- Mô-đun FSM (C++) trong `SW_20Recycle_20ATM/FSM_ATM/FSM_atm`
- Các ví dụ IPC (Message Queue, HC-SR04, v.v.) trong `SW_20Recycle_20ATM/IPC`

## 1. Kiến trúc & Luồng hoạt động
- Ứng dụng GUI (PyQt5) gửi/nhận thông điệp qua hàng đợi thông điệp (System V IPC) với FSM C++ để:
  - Gửi sự kiện: người dùng nhấn nút chuyển trang (`send_key = 12345`)
  - Nhận trang cần hiển thị: FSM phản hồi tên trang (`receive_key = 56789`)
- FSM C++ chạy vòng lặp, đọc sự kiện/ra quyết định và phản hồi về trang cần hiển thị.

Sơ lược các key trong GUI (xem `application_ATMRecycle/main.py`):
- Gửi nút: key `12345`
- Nhận trang: key `56789`

## 2. Cấu trúc thư mục
```
SW_20Recycle_20ATM/
  application_ATMRecycle/
    main.py                # Điểm chạy GUI PyQt5, kết nối IPC SysV
    ui_main.py, ui_main.ui # File UI đã sinh/thiết kế
    rec_main.py, rec_login.py, *.qrc, *.ui, *.png/*.jpeg
  FSM_ATM/
    FSM_atm/
      main.cpp             # Vòng lặp FSM, xử lý trang
      *.h, *.cpp           # Triển khai FSM và helper
  IPC/
    msgQ_C++_python/       # Ví dụ MQ giao tiếp C++/Python (POSIX MQ)
    MsgQ_C++/              # Ví dụ MQ C
    hc_sr04/               # GPIO/HC-SR04 demo (Linux)
    sender_receiver_loop_C++/
  UML/
    ATM_Recycle.xmi
```

## 3. Yêu cầu hệ thống
Do dự án sử dụng System V IPC (`sysv_ipc` trong Python, `sys/msg.h` trong C++) và một số ví dụ POSIX, môi trường khuyến nghị là Linux (Ubuntu/Debian). Trên Windows (win32) các thư viện này không khả dụng trực tiếp.

- HĐH khuyến nghị: Ubuntu 20.04+/Debian (x86_64, ARM64 đều được)
- Python 3.8+ (khuyến nghị 3.10)
- Trình biên dịch C++: `g++` (gcc 9+)
- Thư viện Python:
  - `PyQt5`
  - `sysv_ipc` (Python binding cho System V IPC)
- Công cụ bổ trợ: `make` (tùy chọn), `cmake` (nếu bạn tự thêm), `qttools5-dev-tools` (nếu muốn biên dịch `.qrc/.ui`)

Cài đặt nhanh trên Ubuntu/Debian:
```bash
sudo apt update && sudo apt install -y \
  python3 python3-pip python3-venv \
  build-essential \
  qtbase5-dev qttools5-dev-tools \
  libx11-dev

# Thư viện Python
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install PyQt5 sysv_ipc
```

Lưu ý:
- `sysv_ipc` chỉ hoạt động trên Unix-like. Với Windows, hãy chạy dự án này trong WSL2/VM Linux.
- Các ví dụ `IPC/msgQ_C++_python` dùng POSIX MQ (`posix_ipc`), cài thêm nếu cần: `pip install posix_ipc` (Linux yêu cầu kernel hỗ trợ POSIX MQ).

## 4. Chạy ứng dụng GUI (PyQt5)
```bash
cd SW_20Recycle_20ATM/application_ATMRecycle
# (khuyến nghị) kích hoạt venv nếu đã tạo
# source ../../.venv/bin/activate
python3 main.py
```
Mặc định ứng dụng:
- Tạo hàng đợi gửi với key `12345` và nhận với key `56789` (System V IPC)
- Khi người dùng nhấn các nút `Information`, `Contact`, `Garbage`, `PlusPoint`, GUI sẽ gửi tên trang sang FSM và thay đổi giao diện theo phản hồi nhận được.

## 5. Biên dịch & chạy FSM (C++)
Ví dụ biên dịch nhanh bằng `g++`:
```bash
cd SW_20Recycle_20ATM/FSM_ATM/FSM_atm
# Biên dịch tất cả *.cpp (điều chỉnh thêm tùy tệp nguồn thực tế)
g++ -std=c++17 -O2 \
  main.cpp FSMHelper.cpp mainhelper.cpp ATM_FSM.cpp \
  -o fsm_app

# Chạy FSM
./fsm_app
```
FSM sẽ:
- In trạng thái hiện tại
- Vòng lặp: đọc dữ liệu trang (hàm `data_change_page()` trong helper), xử lý và phản hồi bằng cách gửi tên trang qua MQ để GUI cập nhật.

Gợi ý chạy kết hợp:
- Mở 2 terminal
  - Terminal 1: chạy `./fsm_app`
  - Terminal 2: chạy `python3 application_ATMRecycle/main.py`

## 6. Giao tiếp IPC (thư mục `IPC`)
- `msgQ_C++_python/`: ví dụ dùng POSIX MQ giữa C++ và Python (`posix_ipc`).
  - Nhận (Python): `receiver.py`
  - Gửi (C++): `sender.cpp` (đã có binary mẫu)
- `MsgQ_C++/`: ví dụ MQ thuần C
- `hc_sr04/hc_sr04_sensor`: ví dụ điều khiển GPIO/HC-SR04 (Linux, cần quyền và phần cứng phù hợp)

Chạy thử nhận POSIX MQ (Linux):
```bash
cd SW_20Recycle_20ATM/IPC/msgQ_C++_python
pip install posix_ipc
python3 receiver.py
```

## 7. Các tệp giao diện/nguồn lực Qt
- `rec_main.py`, `rec_login.py`: mã tài nguyên đã được biên dịch từ `.qrc` (chứa hình ảnh, icon). Không cần sửa trực tiếp; nếu thay đổi tài nguyên, hãy cập nhật `.qrc` và biên dịch lại bằng `pyrcc5`.
- `ui_main.ui` và `ui_main.py`: nếu bạn chỉnh sửa `.ui` bằng Qt Designer, tạo lại `.py` bằng `pyuic5`:
```bash
pyrcc5 rec_main.qrc -o rec_main.py
pyuic5 ui_main.ui -o ui_main.py
```

## 8. Ghi chú khả năng tương thích
- Windows: các thành phần IPC (System V / POSIX MQ) không khả dụng bản địa. Dùng WSL2 hoặc Docker Linux để chạy đầy đủ.
- Linux: đảm bảo quyền truy cập MQ và giới hạn kernel phù hợp (POSIX MQ cần `CONFIG_POSIX_MQUEUE`).

## 9. Khắc phục sự cố
- Không import được `sysv_ipc`/`posix_ipc`: kiểm tra hệ điều hành, đã cài gói `pip`, và đang chạy trên Linux.
- GUI không đổi trang: kiểm tra tiến trình FSM có đang chạy không; kiểm tra key MQ (12345/56789) trùng khớp giữa hai bên; tránh xóa MQ trước khi bên kia đọc.
- Lỗi biên dịch C++: đảm bảo thêm đầy đủ tệp `*.cpp`, header path đúng; dùng `-std=c++17` hoặc phù hợp mã nguồn.

## 10. Giấy phép
Dự án học tập/mẫu. Vui lòng bổ sung license phù hợp nếu phân phối. 