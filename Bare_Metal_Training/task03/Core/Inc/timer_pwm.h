#pragma once

#include <stdint.h>

typedef uint32_t value_CCR_t;
typedef uint32_t value_ARR_t;

typedef uint16_t value_Hertz_t;

typedef struct timer_pwm_s
{
    value_Hertz_t actual;
    value_CCR_t valid_CCR;
    value_ARR_t valid_ARR;
} timer_pwm_t;


void timer_pwm_init(timer_pwm_t* self, value_Hertz_t actual, value_Hertz_t value_after_PSC);
void timer_pwm_add_step(timer_pwm_t* self, value_ARR_t* valid_ARR);
void timer_pwm_remove_step(timer_pwm_t* self, value_ARR_t* valid_ARR);
void timer_pwm_add_duty(timer_pwm_t* self);
void timer_pwm_remove_duty(timer_pwm_t* self);
void timer_pwm_change_ARR(timer_pwm_t* self, volatile value_ARR_t* valid_ARR);
void timer_pwm_change_CCR(timer_pwm_t* self);
void timer_pwm_run(timer_pwm_t* self, volatile value_CCR_t* this_CCR);
void timer_pwm_stop(volatile value_CCR_t* this_CCR);

