

#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10

#define encoder0PinA  2
#define encoder0PinB  3
volatile unsigned int encoder0Pos = 0;

int valA;
int valB;
int valC;
int valD;
int i,k,mainMenuCount; 

byte clk;
byte menuCount = 0;
byte dir = 0;
bool runState = false;
unsigned long zaman=0;
bool mainMenuState=true;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.display();
  delay(1000);
  display.clearDisplay();
 
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  
  setPwmFrequency(6,1);
  setPwmFrequency(9,1);
  setPwmFrequency(10,1);
  setPwmFrequency(11,1);

  attachInterrupt(0, doEncoder, CHANGE); 

}
void staticMenu() { 
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10,10);
  display.println("Back");
  display.setCursor(70,2);
  display.println("Profile");
  display.setCursor(120,2);
  display.println(mainMenuCount+1);
  
  display.setCursor(10, 20);
  display.println("Top :");
  display.setCursor(60, 20);
  display.println(valA);

  display.setCursor(10, 30);
  display.println("Right :");
  display.setCursor(60, 30);
  display.println(valB);

  display.setCursor(10, 40);
  display.println("Left :");
  display.setCursor(60, 40);
  display.println(valC);

  display.setCursor(10, 50);
  display.println("Bottom :");
  display.setCursor(60, 50);
  display.println(valD);



  
  
  display.setCursor(2, (menuCount * 10+10) );
  display.println(">");

  display.display();
} //menu 


void mainMenu() {

  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  display.setCursor(25, 5);
  display.println("Profile 1");


  display.setCursor(25, 20);
  display.println("Profile 2");


  display.setCursor(25, 35);
  display.println("Profile 3");


  display.setCursor(25, 50);
  display.println("TEST");

  display.setCursor(2, (mainMenuCount * 15 +5) );
  display.println(">");

  display.display();
  
  
  } //main menu

void mainMenuCheck() {

  mainMenuCount =  encoder0Pos;
  mainMenuCount = mainMenuCount % 4;
  
  if (digitalRead(4) == LOW){
    while(digitalRead(4)==0) {}
    switch (mainMenuCount) {
      
    case 0: eepromRead();  mainMenuState=false; encoder0Pos=0; break;
    case 1: eepromRead();  mainMenuState=false; encoder0Pos=0; break;
    case 2: eepromRead();  mainMenuState=false; encoder0Pos=0; break;
    case 3: test();  break;
    default: return;
  }

  
  } 
  } //main menu check



void menuCheck() {
  menuCount =  encoder0Pos;
  menuCount = menuCount % 5;

if (digitalRead(4) == LOW ) { while(digitalRead(4)==0) {} 


   switch (menuCount) {
   
   case 0: mainMenuState=true;   encoder0Pos = mainMenuCount ; break;  
   case 1: encoder0Pos = valA; while(digitalRead(4)==1) {valA=encoder0Pos; screenEdit();} encoder0Pos=1; while(digitalRead(4)==0) {} break; 
   case 2: encoder0Pos = valB; while(digitalRead(4)==1) {valB=encoder0Pos; screenEdit();} encoder0Pos=2; while(digitalRead(4)==0) {} break;
   case 3: encoder0Pos = valC; while(digitalRead(4)==1) {valC=encoder0Pos; screenEdit();} encoder0Pos=3; while(digitalRead(4)==0) {} break;
   case 4: encoder0Pos = valD; while(digitalRead(4)==1) {valD=encoder0Pos; screenEdit();} encoder0Pos=4; while(digitalRead(4)==0) {} break;
}
    

  }
  


 


} // menucheck
void screenEdit(){
  display.clearDisplay(); display.setTextColor(WHITE);   
  display.setCursor(80, menuCount*10+10); display.println("<<<"); 
  staticMenu();  ledControl(); 
}

void doEncoder() { 
  if (digitalRead(encoder0PinA) == HIGH) {
        if (digitalRead(encoder0PinB) == LOW && encoder0Pos > 0) {
             encoder0Pos = encoder0Pos - 1;
             dir = 0;
        } else {
        encoder0Pos = encoder0Pos + 1;
        dir = 1;
             }
  } else  {
          if (digitalRead(encoder0PinB) == LOW ) {
              encoder0Pos = encoder0Pos + 1;
              dir = 1;
         } else {
              if (encoder0Pos > 0) {
              encoder0Pos = encoder0Pos - 1;
              dir = 0;
      }
    }
  }
} //doencoder
void zero() {
  
    analogWrite(6,  0);
    analogWrite(9,  0);
    analogWrite(10, 0);
    analogWrite(11, 0);   

} 


  

void ledControl() {

  if (runState == true) {
   

    analogWrite(6,  map(valA, 0, 100, 0, 220));
    analogWrite(9,  map(valB, 0, 100, 0, 220));
    analogWrite(10, map(valC, 0, 100, 0, 220));
    analogWrite(11, map(valD, 0, 100, 0, 220));   


    eepromWrite();


  }  
  } //ledControl
 
void test() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(15, 0);
  display.println("Test...");  
  display.display();
  
  for( k=0; k<5; k++){
  for( i=0; i<150; i++) { analogWrite(6,i);  delay(200); }
  for( i=0; i<150; i++) { analogWrite(9,i);  delay(200); }
  for( i=0; i<150; i++) { analogWrite(10,i); delay(200); }
  for( i=0; i<150; i++) { analogWrite(11,i); delay(200); }

  for( i=150; i>1; i--) { analogWrite(6,i);  delay(200); }
  for( i=150; i>1; i--) { analogWrite(9,i);  delay(200); }
  for( i=150; i>1; i--) { analogWrite(10,i); delay(200); }
  for( i=150; i>1; i--) { analogWrite(11,i); delay(200); }

  }
  } // test 



void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}  //set pwm





void eepromWrite() {

      switch(mainMenuCount) {
      
      case 0:     EEPROM.write(0, valA);
                  EEPROM.write(1, valB);
                  EEPROM.write(2, valC);   
                  EEPROM.write(3, valD);   break;       
     
      case 1:     EEPROM.write(4, valA);
                  EEPROM.write(5, valB);
                  EEPROM.write(6, valC);    
                  EEPROM.write(7, valD);   break;         
  
      case 2:     EEPROM.write(8,  valA);
                  EEPROM.write(9,  valB);
                  EEPROM.write(10, valC);  
                  EEPROM.write(11, valD);  break; 
      default:return;                                   
  
  }
  }//eepromWrite


void eepromRead() {        
        
      switch(mainMenuCount) {


      case 0:     valA = EEPROM.read(0);
                  valB = EEPROM.read(1);
                  valC = EEPROM.read(2);
                  valD = EEPROM.read(3);   break;     

      case 1:     valA = EEPROM.read(4);
                  valB = EEPROM.read(5);
                  valC = EEPROM.read(6);
                  valD = EEPROM.read(7);   break;              

      case 2:     valA = EEPROM.read(8);
                  valB = EEPROM.read(9);
                  valC = EEPROM.read(10);
                  valD = EEPROM.read(11);  break;  
      default:return;   

  }
  } //eepromRead
  

void loop() { 

  
  if (mainMenuState==true) {
  mainMenu();
  mainMenuCheck(); 
  runState = false; 
  zero(); 
  zaman = millis();
  } else {
 
  if(millis()-zaman>60000000) {mainMenuState=true;}  
  runState = true; 

  menuCheck();
  staticMenu();
  ledControl(); }
 
  display.clearDisplay();
  delay(50);

} //loop
