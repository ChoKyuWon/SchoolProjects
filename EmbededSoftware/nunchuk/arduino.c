#include <TM1637Display.h>

int dustpin = 4;
int rgbpin[3]={10,11,12};
int biffpin = 13;
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
int d = 0;
#define CLK 2
#define DIO 3
TM1637Display display(CLK, DIO);

unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 3000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
int concentration = 0;

int rgb_output = 0;

int isseg = 1;
int isrgb = 1;

void setup() 
{
    // Set UART comm and starttime
    
    Serial.begin(9600);
    //get the current time
    starttime = millis();
    
    // pinmode for Dust sensor
    pinMode(dustpin,INPUT);
    
    // pinmode for RGB
    for(int i = 0; i<3; i++){
      pinMode(rgbpin[i], OUTPUT);
    }
    pinMode(biffpin, OUTPUT);
}

void loop() 
{
    display.setBrightness(0x0f);
    
    if(Serial.available())
    {
      int command = Serial.read();
      Serial.print("recive ");Serial.println(command);
      if(command == '1'){
        isseg ^= 1;
        Serial.print("seg mode change, isseg is ");Serial.println(isseg);
      }
      else if (command == '0'){
        isrgb ^= 1;
        Serial.print("rgb mode change, isseg is ");Serial.println(isrgb);
      }
    }
    duration = pulseIn(dustpin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
    
    if ((millis()-starttime) > sampletime_ms)
    //if the sampel time == 30s
    {
        //Dust Input
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
        lowpulseoccupancy = 0;
        starttime = millis();
    }
    if(isseg == 1){
        display.setBrightness(0x0f);
        d = (int)concentration % 10000;
        data[3] = display.encodeDigit(d % 10);
        data[2] = display.encodeDigit((d/10) % 10);
        data[1] = display.encodeDigit((d/100) % 10);
        data[0] = display.encodeDigit((d/1000) % 10);
    }
    else{
        display.setBrightness(0x0);
        for(int k =0; k< 4; k++){
            data[k] = 0xff;
        }
    }
    if(isrgb == 1){
        // RGB output
        if(concentration < 1000) {rgb_output=0;digitalWrite(biffpin, HIGH);}
        else if(concentration < 5000){ rgb_output=1;digitalWrite(biffpin, HIGH);}
        else if(concentration > 0) {rgb_output=2;digitalWrite(biffpin, LOW);}
    }
    else{
        rgb_output = -1;
    }
    switch(rgb_output){
      case 0: setColor(0,0,255); break;
      case 1: setColor(0,255,0); break;
      case 2: setColor(255,0,0); break;
      case -1: setColor(0,0,0); break;
    }
    display.setSegments(data);
}


void setColor(int r, int g, int b)
{
  analogWrite(rgbpin[0], r);
  analogWrite(rgbpin[1], g);
  analogWrite(rgbpin[2], b); 
}
