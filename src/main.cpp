#include <mbed.h>

PwmOut led(PA_9);
DigitalOut myleds[]={(PD_12), (PD_13), (PD_14),(PD_15)};//green, orange, red, blue
InterruptIn bt (PA_0);
Timer timer;

enum led_states{RED, GREEN, YELLOW,BLUE}; //States enumeration
int led_state = BLUE; // Defining first state as green

enum states{DOWN, UP, FALL, MIN, RISE, MAX};
int state = UP;

float i=0; // PWM LED intensity
float t1=0,t2=0,tbt=0;

void tbegin();
void tend();
void fsm_action_trigger();
void fsm_state_modifier();

void pwm_test();
void clsled();
void led_test();

int main() {
    bt.rise(&tbegin);
    bt.fall(&tend);
    /*
    myleds[1]=!myleds[1];
    if(tbt<2){
        myleds[3]=!myleds[3];
    }
    else {
        myleds[2]=!myleds[2];
    }
    */
}

void tbegin(){//rise, press button
    timer.start();
    t1=timer.read();
    myleds[0]=1;//para teste
}
    
void tend(){//fall, release button
    timer.stop();
    t2=timer.read();
    myleds[0]=0;//para teste
    tbt=t2-t1;
    //main();
    fsm_state_modifier();
}

void fsm_state_modifier(){
    if((state==UP)&&(tbt<1)){
        state = DOWN;
        fsm_action_trigger();
    }
    else if((state==DOWN)&&(tbt<1)){
        state = UP;
        fsm_action_trigger();
    }
    else if((state==DOWN)&&(tbt>=1)&&(i>0.0)){
        state = FALL;
        fsm_action_trigger();
    }
    else if((state==UP)&&(tbt>=1)&&(i<0)){
        state = RISE;
        fsm_action_trigger();
    }
    else if((state==FALL)&&(bt==1)&&(i==0.0)){
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
    else if ((state==MIN)&&(bt==0)&&(i==0.0)){
        state = UP;
        fsm_action_trigger();
    }
}

void fsm_action_trigger(){
    switch(state){
        case DOWN:
            led_test();
            break;
        case UP:
            led_test();
            break;
        case FALL:
            led_test();
            break;
        case MIN:
            led_test();
            break;
        case RISE:
            led_test();
            break;
        case MAX:
            led_test();
            break;

    }
}

///////////////////////////////////////////////////////////////////////
void clsled(){
    int j;
    for(j=0;j<4;j++){
        myleds [j]=0;
    }
}

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
>>>>>>> state_structure
