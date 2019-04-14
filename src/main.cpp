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
float i_step=0.2; //PWM intensty step change
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
    if(i==0.0){//test
        myleds[GREEN]=0;
        myleds[RED]=1;
    }
    else if (i==1.0){//test
        myleds[GREEN]=1;
        myleds[RED]=0;
    }
    if(t1 != t2){
        //myleds[GREEN]=!myleds[GREEN];//test
        fsm_state_modifier();
    } 
}

void fsm_state_modifier(){
    if((tbt<1.0)&&(bt==0)&&(state==UP)){//ok
        state = DOWN;
        //do_down();
        fsm_action_trigger();
    }
    else if((tbt<1.0)&&(bt==0)&&(state==DOWN)){//ok
        state = UP;
        //do_up();
        fsm_action_trigger();
    }
    else if((tbt>=1.0)&&(state==UP)&&(i<1.0)){//ok
        state = RISE;
        //do_rise();
        fsm_action_trigger();
    }
    else if((tbt>=1.0)&&(state==DOWN)&&(i>0.0)){
        state = FALL;
        //do_fall();
        fsm_action_trigger();
    }
    else if((state==FALL)&&(bt==1)&&(i==0.0)){
        state = MIN;
        //do_min();
        fsm_action_trigger();
    }
    else if((state==RISE)&&(bt==1)&&(i==1.0)){
        state = MAX;
        //do_max();
        fsm_action_trigger();
    }
    else if ((state==MAX)&&(bt==0)&&(i==1.0)){
        state = DOWN;
        //do_down();
        fsm_action_trigger();
    }
    else if ((state==MIN)&&(bt==0)&&(i==0.0)){
        state = UP;
        //do_up();
        fsm_action_trigger();
    }
        else if((bt==0)&&(state==RISE)&&(i<1.0)){
        state = UP;
        //do_min();
        fsm_action_trigger();
    }
        else if((bt==0)&&(state==FALL)&&(i>0.0)){
        state = DOWN;
        //do_min();
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
    myleds[BLUE]=1;
}

void do_down(){
    clsled();
    myleds[ORANGE]=1;
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
    if(((i_step<0.0)&&(i==0.0))||((i_step>0.0)&&(i==1.0))){
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
