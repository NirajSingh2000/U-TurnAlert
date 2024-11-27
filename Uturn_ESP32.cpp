#define BLYNK_TEMPLATE_ID "TMPL3GWbRJGE3"
#define BLYNK_TEMPLATE_NAME "Rain Alert Notification"
#define BLYNK_AUTH_TOKEN "ZVovvPMvLISI3XPwTVKTsp_5yRaUsqBJ"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

#define I2C_ADDR    0x27

// Define the dimensions of your LCD
#define LCD_COLS    16
#define LCD_ROWS    2

// Create a LiquidCrystal_I2C object
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
#define IR_SENSOR1    26 // Pin for IR sensor 1
#define IR_SENSOR2    27 // Pin for IR sensor 2

int objectCount = 0;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "taki";  // type your wifi name
char pass[] = "11111111";  // type your wifi password
Servo servo1;
 
#define RAIN_SENSOR  4
BlynkTimer timer;
int flag=0;

void notifyOnRain() {
  int isRaining = digitalRead(RAIN_SENSOR);
 
  if (isRaining == LOW && flag == 0) {
    Serial.println("It's Raining!");
    Blynk.logEvent("rain_alert", "It's Raining!");
    servo1.write(90); // Rotate the servo to 90 degrees
    flag = 1;
  } else if (isRaining == HIGH) {
    servo1.write(0); // Move the servo to another position
    Serial.println("It's not Raining.");
    flag = 0;
  }
}

void setup() {
  pinMode(RAIN_SENSOR, INPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(13, OUTPUT); // Initialize built-in LED as an output
  pinMode(12, OUTPUT);
  servo1.attach(2); // Attaching servo to pin 2
  pinMode(IR_SENSOR1, INPUT); // Initialize IR sensor 1 pin as input
  pinMode(IR_SENSOR2, INPUT); // Initialize IR sensor 2 pin as input
  
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight (if available)
  lcd.backlight();

  timer.setInterval(2500L, notifyOnRain); // Set the interval to check for rain
}

void loop() {
  Blynk.run();
  timer.run();
  countObjects();
  delay(1000); // Wait for 1 second before checking again
}

void countObjects() {
  int objectDetected1 = digitalRead(IR_SENSOR1);
  int objectDetected2 = digitalRead(IR_SENSOR2);
  
  // If object detected by sensor 1 and not detected by sensor 2, increment object count
  if (objectDetected1 == LOW && objectDetected2 == HIGH) {
    objectCount++;
  }
  // If object detected by sensor 2 and not detected by sensor 1, decrement object count
  else if (objectDetected1 == HIGH && objectDetected2 == LOW) {
    objectCount--;
  }
  
  // Display object count on LCD
  lcd.clear();
  
  // Print "Welcome" on the LCD first row
  lcd.setCursor(0, 0);
  lcd.print("Welcome");

  lcd.setCursor(0, 1);
  lcd.print("No. of Person ");
  lcd.print(objectCount);
}

BLYNK_WRITE(V1) { // Slider Widget for Servo 1 on V1
  int pos1 = param.asInt(); // Get value from slider
  servo1.write(pos1); // Set servo 1 position
  Blynk.virtualWrite(V2, pos1); // Send slider value to another widget
}

BLYNK_WRITE(V3) { // Button widget on V3
  int value = param.asInt();
  digitalWrite(13, value); // Control built-in LED based on button state
}
BLYNK_WRITE(V4) { // Button widget on V4
  int value = param.asInt();
  digitalWrite(12, value); // Control built-in LED based on button state
}