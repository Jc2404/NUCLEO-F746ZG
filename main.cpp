#include "mbed.h"

DigitalOut myled(LED1);
DigitalOut myled2(LED2);

int main() {
        while(true) {
                myled = 1; // LED is ON
                wait(0.2); // 200 ms
                myled = 0; // LED is OFF
                myled2 = 1;
                wait(0.2); // 200 ms
                myled2 = 0;
                wait(1.0); // 1 sec
        }
}