#pragma once

#include <stdint.h>

typedef struct led_controller_s
{
    uint8_t deviceID;
    uint8_t TxBuffer[8];

} led_controller_t;


void led_controller_init(led_controller_t* self, uint8_t deviceID);
void led_controller_on_all(led_controller_t* self);
void led_controller_off_all(led_controller_t* self);

void led_controller_sleep_enable(led_controller_t* self);
void led_controller_sleep_disable(led_controller_t* self);

void set_PWM_DelayAndDuty(led_controller_t* self, uint8_t delay, uint8_t duty, uint8_t led_number);

void led_controller_set_PWM_frequency(led_controller_t* self, uint32_t frequency);

extern void sendI2C(led_controller_t* self);
extern void delay();