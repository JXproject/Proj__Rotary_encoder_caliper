/***********************************************************
********* MTE 201_Code****  ********************************
********* Date: 2017 / 11 /18 ******************************
********* PROJECT NAME: 201 Project ************************
***********************************************************/
#include <SoftwareSerial.h>
#include <U8g2lib.h>

#define CDS_ONE A1
#define SDA     A4
#define SCL     A5
#define LED_POWER      3
#define STATE_LED      4
#define PUSH_BUTTON    5
#define DISPLAY_BUTTON 2

//SoftwareSerial software_Serial = SoftwareSerial(SOFTWARE_TX, SOFTWARE_RX); 

bool cds_one_light = false;    //shade on the slot 
uint8_t count_one = 0; 
uint16_t prev_one =0; 
uint16_t current_one = 0; 
const uint8_t TOL = 5; 
uint16_t min_Val = 1000 ;  
bool min_point = false; 
uint16_t min_count = 0; 
bool high_pass = false; 
bool skipWelcome = false;

float distance_negative_one = 0, distance_ccw = 0, distance_cw = 0, distance_plus_one = 0; 
const uint8_t distance_max_angle = 82;  

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);//display declaration

void setup() {
  // put your setup code here, to run once:
  pinMode (CDS_ONE, INPUT); 
  pinMode(LED_POWER, OUTPUT); 
  pinMode(PUSH_BUTTON, INPUT_PULLUP); 
  pinMode(DISPLAY_BUTTON, INPUT_PULLUP); 
  pinMode(STATE_LED, OUTPUT); 
  //Start
  current_one = 1024 - analogRead(CDS_ONE); 
  prev_one = 1024 - analogRead(CDS_ONE); 
  digitalWrite(LED_POWER,HIGH);
  digitalWrite(STATE_LED,HIGH); 
  Serial.begin(115200); 
  u8g2.begin();
  
  //welcome
  while(!skipWelcome)
    welcomeInterface();
  delay(50);
  u8g2.clearBuffer();
  delay(50); 

}


//Display float value
void updateDataOnScreen(float value_){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso24_tr);
  char outputString[] ="00.000";
  dtostrf(value_, 2, 3, outputString);
  u8g2.drawStr(0,32,outputString);
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.drawStr(90,32,"mm");
  u8g2.sendBuffer();
}



void loop() {
  // put your main code here, to run repeatedly:
   prev_one = current_one; 
   current_one = 1024 - analogRead(CDS_ONE);

   if (digitalRead(DISPLAY_BUTTON) == 0)
   {
     //Serial.println("display_but"); 
     distance_ccw = 30 * sin(1.1 * (distance_max_angle - (min_count + 1) ) * PI / 180.0); 
     distance_cw = 30 * sin(1.1 * (min_count ) * PI / 180.0); 
     updateDataOnScreen(distance_ccw);
   }
    
 // cds_two = analogRead(CDS_TWO); 
  //Serial.print("prev_one: "); 
  //Serial.print(prev_one); 
  //Serial.print("\r\n"); 
  //Serial.print("current_one: ");
  //Serial.println(current_one); 
  //Serial.print("\r\n"); 

//850
    if (digitalRead(PUSH_BUTTON) == 0)
   {
     software_Reset(); 
   }else
   {
            if (prev_one < 820)
          {
            cds_one_light = false;
            if (prev_one <= min_Val)
            {
              min_Val = prev_one; 
              min_point = false; 
            }
            else if (!min_point)
            {
              min_point = true; 
              min_Val = 800; 
              if (high_pass)
              {
                min_count++; 
                high_pass = false; 
              }
            }
          }
          
          else
          {
            cds_one_light = true;
            high_pass = true; 
          }
        
         Serial.println(current_one); 
         //Serial.println(min_count);
         //software_Serial.println(current_one); 
         //Serial.println(current_one); 
         //Serial.println( (distance_ccw) ,4); 
        
        
         //Serial.print("mm");
         //Serial.print("\r\n"); 
         //Serial.print( (distance_cw) ,4); 
         //Serial.print("mm"); 
         //Serial.print("\r\n"); 
   }
  
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
 //asm volatile ("  jmp 0");
  cds_one_light = false;    //shade on the slot 
  count_one = 0; 
  prev_one =0; 
  current_one = 0; 
  min_Val = 1000 ;  
  min_point = false; 
  min_count = 0; 
  high_pass = false; 
  skipWelcome = false;
  
  distance_negative_one = 0;
  distance_ccw = 0;
  distance_cw = 0;
  distance_plus_one = 0; 
  current_one = 1024 - analogRead(CDS_ONE); 
  prev_one = 1024 - analogRead(CDS_ONE); 
  displayText("RESET->0");
  digitalWrite(STATE_LED,LOW); 
  delay(500);
  digitalWrite(STATE_LED,HIGH);
  delay(300); 
  digitalWrite(STATE_LED,LOW); 
  delay(200);
  digitalWrite(STATE_LED,HIGH); 
  displayText("Rotate>>>");
}  



//Test
void welcomeInterface(){
    displayText("Roli_Cali");
    LEDdelayWithBTNCheck(STATE_LED, 20, 100);
    displayText("Roli_Cali");
    LEDdelayWithBTNCheck(STATE_LED, 20, 100);
    displayText("Jerome");
    LEDdelayWithBTNCheck(STATE_LED, 20, 25);
    displayText("Tsugumi");
    LEDdelayWithBTNCheck(STATE_LED, 20, 25);
    displayText("Jack");
    LEDdelayWithBTNCheck(STATE_LED, 20, 25);
    displayText("Nathan");
    LEDdelayWithBTNCheck(STATE_LED, 20, 25);
    displayText("$(TAT)$");
    LEDdelayWithBTNCheck(STATE_LED, 20, 50);
}

void displayText(char msg[]){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso22_tr);
    u8g2.drawStr(0,27,msg);
    u8g2.sendBuffer();
}
//
void LEDdelayWithBTNCheck(int GPIO_, int checkDelayTime_, int NumCycle_){
  digitalWrite(GPIO_,LOW); 
  delay(200);
  for(int i=0; i<NumCycle_&&!skipWelcome; i++)
  {
    isBTNpushed();
    digitalWrite(GPIO_,HIGH); 
    delay(checkDelayTime_);
  }
}

bool isBTNpushed(){
  if(digitalRead(PUSH_BUTTON) == 0 || digitalRead(DISPLAY_BUTTON) == 0)
      skipWelcome = true;
}
