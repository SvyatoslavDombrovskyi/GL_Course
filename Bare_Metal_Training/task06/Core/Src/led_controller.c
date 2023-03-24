#include "led_controller.h"

static const uint32_t STEP_COUNT = 4096;
static const uint32_t OSCILLATOR_FREQUENCY_KHZ = 25000;

void led_controller_init(led_controller_t *self, uint8_t deviceID)
{
    self->deviceID = deviceID;
}

void led_controller_on_all(led_controller_t *self)
{
    self->TxBuffer[0] = 0xFA; // LED_ON_L
    self->TxBuffer[1] = 0x00;
    sendI2C(self);

    self->TxBuffer[0] = 0xFB; // LED_ON_H
    self->TxBuffer[1] = 0x10;
    sendI2C(self);

    self->TxBuffer[0] = 0xFC; // LED_OFF_L
    self->TxBuffer[1] = 0x00;
    sendI2C(self);

    self->TxBuffer[0] = 0xFD; // LED_OFF_H
    self->TxBuffer[1] = 0x00;
    sendI2C(self);
}

void led_controller_off_all(led_controller_t* self)
{
    self->TxBuffer[0] = 0xFA; // LED_ON_L
    self->TxBuffer[1] = 0x00;
    sendI2C(self);

    self->TxBuffer[0] = 0xFB; // LED_ON_H
    self->TxBuffer[1] = 0x00;
    sendI2C(self);

    self->TxBuffer[0] = 0xFC; // LED_OFF_L
    self->TxBuffer[1] = 0x00;
    sendI2C(self);

    self->TxBuffer[0] = 0xFD; // LED_OFF_H
    self->TxBuffer[1] = 0x10;
    sendI2C(self);
}

void led_controller_sleep_enable(led_controller_t* self)
{
    self->TxBuffer[0] = 0x00; // SLEEP LOW POWER MODE (1)
    self->TxBuffer[1] = 0x11;
    sendI2C(self);    
}

void led_controller_sleep_disable(led_controller_t* self)
{
    self->TxBuffer[0] = 0x00; // SLEEP NORMAL MODE (0)
    self->TxBuffer[1] = 0x01;
    sendI2C(self);     
}

void led_controller_set_PWM_frequency(led_controller_t *self, uint32_t frequency)
{
    uint8_t prescaler = (OSCILLATOR_FREQUENCY_KHZ * 1000 / (4096 * frequency)) - 1;

    //prescaler = 0xD0;

    // MODE0
    self->TxBuffer[0] = 0x00; // SLEEP LOW POWER MODE (1)
    self->TxBuffer[1] = 0x11;
    sendI2C(self);

    self->TxBuffer[0] = 0xFE; // PRE_SCALE SET
    self->TxBuffer[1] = prescaler;
    sendI2C(self);

    self->TxBuffer[0] = 0x00; // RESTART AND SLEEP NORMAL (0)
    self->TxBuffer[1] = 0x81;
    sendI2C(self);    
}

void set_PWM_DelayAndDuty(led_controller_t *self, uint8_t delay, uint8_t duty, uint8_t led_number)
{
    uint32_t delay_step = STEP_COUNT * delay / 100 - 1;
    uint32_t delay_and_duty_step = STEP_COUNT * (delay + duty) / 100 - 1;

    uint8_t led_on_l = delay_step & 0xFF;
    uint8_t led_on_h = (delay_step >> 8) & 0xFF;

    uint8_t led_off_l = delay_and_duty_step & 0xFF;
    uint8_t led_off_h = (delay_and_duty_step >> 8) & 0xFF;

    self->TxBuffer[0] = 6 + 4 * led_number; // LED_ON_L
    self->TxBuffer[1] = led_on_l;
    sendI2C(self);

    self->TxBuffer[0] = 6 + 4 * led_number + 1; // LED_ON_H
    self->TxBuffer[1] = led_on_h;
    sendI2C(self);

    self->TxBuffer[0] = 6 + 4 * led_number + 2; // LED_OFF_L
    self->TxBuffer[1] = led_off_l;
    sendI2C(self);

    self->TxBuffer[0] = 6 + 4 * led_number + 3; // LED_OFF_H
    self->TxBuffer[1] = led_off_h;
    sendI2C(self);
}
