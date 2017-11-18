/***********************************************************
********* MTE 201_Code****  ********************************
********* Date: 2017 / 11 /18 ******************************
********* PROJECT NAME: 201 Project ************************
***********************************************************/
#include <SoftwareSerial.h>
#include <U8g2lib.h>//For Display

#define CDS_ONE A1
#define SDA     A4
#define SCL     A5
#define LED_POWER   3
#define STATE_LED   4
#define PUSH_BUTTON 5

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

float distance_negative_one = 0, distance_ccw = 0, distance_cw = 0, distance_plus_one = 0; 
const uint8_t distance_max_angle = 82;  

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);//display declaration

void setup() {
  // put your setup code here, to run once:
  pinMode (CDS_ONE, INPUT); 
  pinMode(LED_POWER, OUTPUT); 
  pinMode(PUSH_BUTTON, INPUT); 
  pinMode(STATE_LED, OUTPUT); 
  
  current_one = 1024 - analogRead(CDS_ONE); 
  prev_one = 1024 - analogRead(CDS_ONE); 
  digitalWrite(LED_POWER,HIGH);
  digitalWrite(STATE_LED,HIGH); 
  Serial.begin(115200); 
  u8g2.begin();

// Example for string msg, see more funcs from u8g2lib
//  u8g2.clearBuffer();
//  u8g2.setFont(u8g2_font_logisoso24_tr);
//  u8g2.drawStr(0,32,"CaliTool");
//  u8g2.sendBuffer();
//  delay(2000);
  //Example for display value
  updateDataOnScreen(30.45f);
  //software_Serial.begin(9600); 

}

//Display float value
void updateDataOnScreen(float value_){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso24_tr);
  char outputString[] ="00.00";
  dtostrf(value_, 2, 2, outputString);
  u8g2.drawStr(0,32,outputString);
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.drawStr(90,32,"mm");
  u8g2.sendBuffer();
}

void loop() {
  // put your main code here, to run repeatedly:
   prev_one = current_one; 
   current_one = 1024 - analogRead(CDS_ONE);
   
 // cds_two = analogRead(CDS_TWO); 
  //Serial.print("prev_one: "); 
  //Serial.print(prev_one); 
  //Serial.print("\r\n"); 
  //Serial.print("current_one: ");
  //Serial.println(current_one); 
  //Serial.print("\r\n"); 

//850
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

/*
 if (cds_one_light)
 {
   if (current_one < 870 && (current_one > prev_one + TOL || current_one < prev_one - TOL))
   {
       //count_one++;  
       cds_one_light = false;
       distance_ccw = 30 * sin(1.1 * (distance_max_angle - min_count ) * PI / 180.0); 
       distance_cw = 30 * sin(1.1 * (min_count ) * PI / 180.0); 
   }
 }
 */
 distance_ccw = 30 * sin(1.1 * (distance_max_angle - (min_count + 1) ) * PI / 180.0); 
 distance_cw = 30 * sin(1.1 * (min_count ) * PI / 180.0); 
 Serial.println(current_one); 
 Serial.println(min_count);
 //software_Serial.println(current_one); 
 //Serial.println(current_one); 
 Serial.println( (distance_ccw) ,4); 
 //Serial.print("mm");
 //Serial.print("\r\n"); 
 //Serial.print( (distance_cw) ,4); 
 //Serial.print("mm"); 
 //Serial.print("\r\n"); 
}
