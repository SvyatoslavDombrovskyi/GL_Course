#include "timer_pwm.h"

static value_Hertz_t valid_after_PSC;
static uint8_t duty;

static value_Hertz_t MIN_HERTZ = 5;
static value_Hertz_t MAX_HERTZ = 100;
static value_Hertz_t STEP_HERTZ = 5;

void timer_pwm_init(timer_pwm_t* self, value_Hertz_t actual, value_Hertz_t value_after_PSC)
{
    self->actual = actual;
    self->valid_CCR = 0;
    valid_after_PSC = value_after_PSC;    
    self->valid_ARR = value_after_PSC / actual - 1;
    duty = 10;
    timer_pwm_change_CCR(self);
}

void timer_pwm_add_step(timer_pwm_t* self, value_ARR_t* valid_ARR)
{
    if (self->actual + STEP_HERTZ <= MAX_HERTZ)
    {
        self->actual += STEP_HERTZ;
    }  
    
    timer_pwm_change_ARR(self, valid_ARR);
}

void timer_pwm_remove_step(timer_pwm_t* self, value_ARR_t* valid_ARR)
{
    if (self->actual - STEP_HERTZ >= MIN_HERTZ)
    {
        self->actual -= STEP_HERTZ;
    }  
    
    timer_pwm_change_ARR(self, valid_ARR);
}

void timer_pwm_add_duty(timer_pwm_t* self)
{
    if (duty + 5 <= 100)
    {
        duty += 5;
    }

    timer_pwm_change_CCR(self);
}

void timer_pwm_remove_duty(timer_pwm_t* self)
{
    if (duty - 5 >= 0)
    {
        duty -= 5;
    }

    timer_pwm_change_CCR(self);
}

void timer_pwm_change_ARR(timer_pwm_t* self, volatile value_ARR_t* valid_ARR)
{
    self->valid_ARR = valid_after_PSC / self->actual;
    *valid_ARR = self->valid_ARR - 1;
}

void timer_pwm_change_CCR(timer_pwm_t* self)
{
    self->valid_CCR = self->valid_ARR * duty / 100;    
}

void timer_pwm_run(timer_pwm_t* self, volatile value_CCR_t* this_CCR)
{
    *this_CCR = self->valid_CCR;
}

void timer_pwm_stop(volatile value_CCR_t* this_CCR)
{
    *this_CCR = 0;
}
