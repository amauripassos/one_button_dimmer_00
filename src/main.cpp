#include "blink.hpp"

//InterruptIn event (PA_0);
//DigitalOut myleds[]={(PD_12), (PD_13), (PD_14),(PD_15)};
/*DigitalOut myleds[]={(PD_12), (PD_13), (PD_14),(PD_15)};
Ticker ticker;
int blinkld=3;
float tblink=0.5;*/
//Timeout timeout;
//Timer timer;



/*
int i=0,blinkld=1,blk=0,pw=0,ctrl=0;
float tvd=20.0,tam=4.0,tvm=10.0,tbmin=3.0,tbmax=10.0,tb=0.0,tblink=0.5,t2=0.0,t1=0.0;
*/

/*void blink(){
    myleds[blinkld]=!myleds[blinkld];
}*/

DigitalOut myleds[]={(PD_12), (PD_13), (PD_14),(PD_15)};
Ticker ticker;
int blinkld=3;
float tblink=0.5;

int main() {
  ticker.attach(&blink,tblink);
}