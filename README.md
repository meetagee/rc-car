# Remote Controlled Car a.k.a RC Siege Creep

This is a personal project of mine in which I implemented a low-budget remote controlled car (or a remote controlled siege creep if you play Dota 2 ( ͡° ͜ʖ ͡°))

### Materials used:
- [Arduino Uno (Atmega328p)](https://hshop.vn/products/arduino-uno-r3)
- [ESP3266 Wifi Shield](https://hshop.vn/products/arduino-esp8266-wifi-shield)
- [L298 Motor Driver](https://hshop.vn/products/mach-dieu-khien-dong-co-dc-l298)
- [Motors, wheels, etc.](https://hshop.vn/products/khung-xe-robot-black-cat)
- [A pair of 18650 batteries](https://hshop.vn/products/pin-sac-18650panasonic-3-7v-3700mah)
- 6 dupont M-F wires

### Directory:
````bash
├── RcCarClient // Client-side code where a simple interface is implemented for controlling the car
│   ├── build
│   │   ├── RcCarClientNc
│   │   └── RcCarClientSocket
│   └── src
│       ├── RcCarClientNc.c // Netcat version
│       └── RcCarClientSocket.c // Socket (UDP/TCP) version
└── RcCarController // Server-side code where all the GPIO setup, control logic, command handling happen
    └── RcCarController.ino
````
### Usage:
1) [Load an AT firmware to the ESP8266](https://www.instructables.com/id/ESP8266-ESP-12E-UART-Wireless-WIFI-Shield-TTL-Conv/)
2) Modify apSsid & apPwd to your preference in RcCarController.ino and upload Arduino code to the board
3) Compile client-side code for remote control
````bash
# Assuming we're at the working directory
cd RcCarClient/build
gcc -W -Wall ../src/RcCarClientSocket.c -lncurses -o RcCarClientSocket # Change 'Socket' -> 'Nc' if you wanna try out Netcat version ( ͡° ͜ʖ ͡°) - I mean they're pretty much the same eh... I only used it for testing
````
4) Connect to the wifi hotstop (apSsid - apPwd that you set up in step 2)
5) LET'S GO
````bash
# Assuming we're at the working directory
cd RcCarClient/build
./RcCarClientSocket # Or ./RcCarClientNc
````
