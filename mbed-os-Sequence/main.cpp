#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

Timeout button_debounce_timeout;
Timeout button_debounce_timeout2;
float debounce_time_interval = 0.2;

InterruptIn button(USER_BUTTON);

Ticker cycle_ticker;
float cycle_time_interval = 0.6;
int OldSequence[3] = {1, 2, 3};
volatile int *Sequence = nullptr;
volatile int LightState = 1;
volatile int N = 8;
volatile int i = 0;
volatile int l = 0;
volatile bool Stop = false;

void onButtonStopDebouncing(void);
void onButtonStopDebouncing2(void);

void onButtonPress(void)
{
    button.rise(NULL);
    button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time_interval);

    if (i < N && Stop == false) {
        Sequence[i] = LightState;
        i++;
    }
    else if (i == N || Stop == true) {
        // free(Sequence);
        // If freed, further button press would stop the beautiful light switch.
    }

}

void onButtonRepeat()
{
    button.rise(NULL);
    Stop = true;
}

void onButtonStopDebouncing(void)
{
        button.rise(onButtonRepeat);
        button_debounce_timeout2.attach(onButtonStopDebouncing2, debounce_time_interval);
}

void onButtonStopDebouncing2(void)
{
        button.rise(onButtonPress);
}

void onCycleTicker(void)
{
        if (i < N && Stop == false) {
            LightState = OldSequence[l];
            l = (l+1)%3;
        }
        else if (i == N || Stop == true) {
            LightState = Sequence[l];
            l = (l+1)%i;
        }

        if (LightState==1) {
                led1 = true;
                led2 = false;
                led3 = false;
        }
        else if (LightState==2) {
                led1 = false;
                led2 = true;
                led3 = false;
        }
        else if (LightState==3) {
                led1 = false;
                led2 = false;
                led3 = true;
        }
}


int main()
{       
        Sequence = (int *)malloc(N * sizeof(int));
        if (Sequence == NULL) {
            free(Sequence);
            return 1;
        }
        button.rise(onButtonPress);
        cycle_ticker.attach(onCycleTicker, cycle_time_interval);
        // Even more important code could be placed here
}