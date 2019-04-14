#include <mbed.h>

PwmOut led(PA_9);
DigitalOut myleds[]={(PD_12), (PD_13), (PD_14), (PD_15)};//green, orange, red, blue
InterruptIn bt (PA_0);//botao azul
Timer timer;
Ticker ticker;
Timeout timeout;

enum led_states{GREEN, ORANGE, RED,BLUE}; //States enumeration
int led_state = BLUE; // Defining first state as blue

enum states{DOWN, UP, FALL, MIN, RISE, MAX};
int state = UP;

float i=0.0; // PWM LED intensity
float i_step=0.05; //PWM intensty step change
float tbt=0.0; //time button is pressed

void tbegin();
void tend();
void tread();
void fsm_action_trigger();
void fsm_state_modifier();
void do_up();
void do_down();
void do_rise();
void do_fall();
void pwm_change();
void do_min();
void do_max();
void flash_led();
void clsled();

void pwm_test();
void led_test();

int main() {
    fsm_action_trigger();
    bt.rise(&tbegin);
    bt.fall(&tend);
}

void tbegin(){//rise, press button
    timer.reset();
    timer.start();
    ticker.attach(&tread,0.1);
}
    
void tend(){//fall, release button
    timer.stop();
    ticker.detach();
    tbt=timer.read();
    fsm_state_modifier();
}

void tread(){
    float t1 = 0.0, t2 = 0.0;
    t1=timer.read();
    tbt=timer.read();
    t2=timer.read();
    if(t1 != t2){
        fsm_state_modifier();
    } 
}

void fsm_state_modifier(){
    if((tbt<1.0)&&(bt==0)&&(state==UP)){
        state = DOWN;
        fsm_action_trigger();
    }
    else if((tbt<1.0)&&(bt==0)&&(state==DOWN)){
        state = UP;
        fsm_action_trigger();
    }
    else if((tbt>=1.0)&&(state==UP)&&(i<1.0)){
        state = RISE;
        fsm_action_trigger();
    }
    else if((tbt>=1.0)&&(state==DOWN)&&(i>0.0)){
        state = FALL;
        fsm_action_trigger();
    }
    else if((state==FALL)&&(bt==1)&&(i<=0.0)){
        state = MIN;
        fsm_action_trigger();
    }
    else if((state==RISE)&&(bt==1)&&(i==1.0)){
        state = MAX;
         fsm_action_trigger();
    }
    else if ((state==MAX)&&(bt==0)&&(i==1.0)){
        state = DOWN;
        fsm_action_trigger();
    }
    else if ((state==MIN)&&(bt==0)&&(i<=0.0)){
        state = UP;
        fsm_action_trigger();
    }
    else if((bt==0)&&(state==RISE)&&(i<1.0)){
        state = UP;
        fsm_action_trigger();
    }
    else if((bt==0)&&(state==FALL)&&(i>0.0)){
        state = DOWN;
        fsm_action_trigger();
    }
    else{
        fsm_action_trigger();
    }
}

void fsm_action_trigger(){
    switch(state){
         case UP:
            do_up();
            break;
       case DOWN:
            do_down();
            break;
        case RISE:
            do_rise();
            break;
        case FALL:
            do_fall();
            break;
        case MIN:
            do_min();
            break;
        case MAX:
            do_max();
            break;
    }
}

void do_up(){
    clsled();
    led_state = BLUE;
    myleds[led_state]=1;
}

void do_down(){
    clsled();
    led_state = ORANGE;
    myleds[led_state]=1;
}

void do_rise(){
    i_step = fabs(i_step);///SEMPRE POSITIVO
    ticker.attach(&pwm_change,1);
}

void do_fall(){
    i_step = -fabs(i_step); /// SEMPRE NEGATIVO
    ticker.attach(&pwm_change,1);
}

void pwm_change(){
    if(((i_step<0.0)&&(i<=0.0))||((i_step>0.0)&&(i==1.0))){
        tread();
    }
    else {
        i = i + i_step;
        led = i;
        myleds[led_state] = 0;
        timeout.attach(&flash_led,0.2);
    }
}

void do_min(){
    ticker.attach(&flash_led,0.1);
}

void do_max(){
    ticker.attach(&flash_led,0.1);
}

void flash_led(){
    myleds[led_state] = !myleds[led_state];
}

void clsled(){
    int j;
    for(j=0;j<4;j++){
        myleds [j]=0;
    }
}

/////////////////////////////FUNCOES DE TESTE/////////////////////////////


void pwm_test(){
    led=0;
    while(1) {
        led = led + 0.1;
        wait(0.2);
        if(led == 1.0) {
            led = 0;
            myleds[led_state]=!myleds[led_state];
            led_state++;
            if(led_state>3){
                led_state=0;
            }

            wait(1);
        }
    }
}

void led_test(){
    myleds[led_state]=!myleds[led_state];
        led_state++;
        if(led_state>3){
            led_state=0;
        }
}
