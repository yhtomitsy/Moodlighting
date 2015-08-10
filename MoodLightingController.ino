#include <Adafruit_NeoPixel.h>

#define PIN 6 // signal pin
#define huePin A0 // hue knob pin
#define brightnessPin A1 // brightness know pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800); // declaring neopixel object

uint8_t brightness = 1;
String commandString = "";
uint8_t r=0,g=0,b=0;
uint8_t prevr=0, prevg=0, prevb=0, prevbrightness=0;

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  
  strip.begin(); // begin pixels
  strip.show(); // set pixels off
  
  pinMode(huePin, INPUT); // set analog pin as input
  pinMode(brightnessPin, INPUT); // set analog pin as input
}

void loop() {
  char c = ' ';
  if (Serial1.available()){
    c= char(Serial1.read());
    commandString += c;
  }
  if (c == '#'){
    parseData();
    commandString = "";
    Serial1.end();
  }
  
  //Serial.println(commandString);
  if(r != prevr || g != prevg || b != prevb){
    glow();
    //Serial.println("color change");
    prevr = r;
    prevg = g;
    prevb = b;
  }
  if(c == '#') Serial1.begin(38400);
}

void glow(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show();
}

void parseData(){
  if(commandString[0] == '$'){
    // get starting indices
    uint8_t startIndex = commandString.indexOf("$");
    uint8_t stopIndex = commandString.indexOf(",", startIndex);
    
    // clear rgb values
    r=0;
    g=0;
    b=0;
    
    // get r value
    String comandBuffer  = commandString.substring(startIndex+1, stopIndex);
    
    for (uint8_t i = 0; i < comandBuffer.length(); i++){
      r = (r*10) + (comandBuffer[i] - '0');
    }
    
    // get indices
    startIndex = stopIndex;
    stopIndex = commandString.indexOf(",", startIndex+1);
    
    // get g value
    comandBuffer  = commandString.substring(startIndex+1, stopIndex);
    
    for (uint8_t i = 0; i < comandBuffer.length(); i++){
      g = (g*10) + (comandBuffer[i] - '0');
    }
    
    // get indices
    startIndex = stopIndex;
    stopIndex = commandString.indexOf("#", startIndex+1);
    
    // get b value
    comandBuffer  = commandString.substring(startIndex+1, stopIndex);
    for (uint8_t i = 0; i < comandBuffer.length(); i++){
      b = (b*10) + (comandBuffer[i] - '0');
    }
  }
}
