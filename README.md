# ESP32 Button-Controlled Tetris Game

This project implements a physical game controller using an ESP32 and five pushbuttons to play a browser-based game of Tetris.

The ESP32 reads button input using GPIO interrupts, debounces the signals with FreeRTOS timers, and sends real-time events over a WebSocket connection to a local HTML5 Tetris game hosted from the same board via SPIFFS.

---

## Demo

https://www.youtube.com/watch?v=28kBbpH67Zk

---

## Features

- ESP-IDF implementation (no Arduino dependencies)
- Interrupt-driven GPIO input with debounce logic
- FreeRTOS-based timers and task queues for responsive control
- WebSocket server hosted on ESP32 for inputting to browser
- Tetris game (HTML/CSS/JS) served from onboard SPIFFS
- Live gameplay using only embedded hardware and browser

---

## What I Learned

- How to implement GPIO interrupts and debounce using FreeRTOS timers
- How to integrate ESP32 with WebSockets to communicate with a frontend
- Hosting web applications from SPIFFS on an embedded system
- Designing an input system with event queues and interrupts

---

The Tetris game interface used in this project is adapted from the open-source project [JS Tetris](https://github.com/cztomczak/jstetris) by Czarek Tomczak, licensed under the BSD 3-Clause License.
A copy of the original license is included in [`spiffs/src/LICENSE.txt`](spiffs/src/LICENSE.txt).

You can view the original project here:  
https://github.com/cztomczak/jstetris

## Author

Cale Conrad

Electrical Engineering – University of Nebraska-Lincoln
