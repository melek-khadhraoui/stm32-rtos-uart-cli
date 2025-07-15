# STM32 RTOS Command Shell Project

This project implements a simple **UART command shell interface** on an STM32F401 microcontroller using **FreeRTOS**. It handles LED control and echo functionality through UART commands, demonstrating the use of **RTOS primitives** such as:

- Threads (Tasks)
- Event Flags
- Queues
- Semaphores
- Software Timers

---

## 🧠 Features

- UART-based CLI with the following commands:
  - `LED ON` – turns the LED on
  - `LED OFF` – turns the LED off
  - `TOGGLE LED` – toggles the LED every second
  - `ECHO <message>` – echoes back the message
  - `HELP` – prints list of available commands
- Interrupt-driven UART reception (HAL_UART_Receive_IT)
- FreeRTOS-based task management and communication
- Timer-based LED toggling
- Clean modular code with RTOS components

---

## 📦 Project Structure

| Component | Description |
|----------|-------------|
| `main.c` | Initializes peripherals and sets up RTOS |
| `LED()` | Handles LED control using event flags |
| `Sender()` | Sends messages via UART from the queue |
| `Receiver()` | Parses UART commands and triggers actions |
| `Callback01()` | Software timer callback for LED toggling |

---

## 🛠 Hardware Requirements

- STM32F401 Discovery board (or any STM32F4 MCU)
- Onboard LED (PA5)
- USB-to-Serial (or onboard ST-Link VCP)
- UART2 connected to terminal (115200 baud)

---
## 🧪 Usage

1. Flash the firmware to your STM32
2. Open a terminal (Hercules, Tera Term, PuTTY)
3. Set baud rate to `115200`, 8-N-1
4. Type commands like:
LED ON
TOGGLE LED
ECHO Hello World
HELP


---

## 📁 Build System

- IDE: STM32CubeIDE
- HAL + CMSIS-RTOS (FreeRTOS)
- GCC compiler

---

## 💡 Notes

- Project is auto-generated via STM32CubeMX with additional user code
- UART communication is interrupt-driven and non-blocking
- Command parsing is based on `strncmp`

---

## 👨‍💻 Author

Melek Khadhraoui  
[Embedded Systems Engineering Student]

---

## 🗃 License

This project is provided under the terms described in the LICENSE file.


