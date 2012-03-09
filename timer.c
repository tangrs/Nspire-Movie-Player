#include <os.h>
#include "inc.h"

static unsigned timer_load, timer_saved_control, timer_saved_load;

void setup_timer(int fps) {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
	if (is_cx) {
        timer_saved_control = *control;
        timer_saved_load = *load;
        *control = 0; // disable timer
        *control = 0b01100011; // disabled, TimerMode N/A, int, no prescale, 32-bit, One Shot (for the *value > 0 test) -> 32khz
        *control = 0b11100011; // enable timer
        timer_load = 32000/fps;
    }
}

void restore_timer() {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
	if (is_cx) {
        *control = 0; // disable timer
        *control = timer_saved_control & 0b01111111; // timer still disabled
        *load = timer_saved_load;
        *control = timer_saved_control; // enable timer
    }
}

void tick_timer() {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
	if (is_cx) {
	    *load = timer_load;
	}
	while (*value > 0)
			idle();
}