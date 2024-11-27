#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define echoPin1 D8
#define trigPin1 D7

#define greenLed D3
#define yellowLed D4
#define redLed D5

// Define LCD parameters
#define LCD_ADDR 0x27  // I2C address of the LCD
#define LCD_COLS 16     // Number of columns in the LCD
#define LCD_ROWS 2      // Number of rows in the LCD

// Initialize the LCD object
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// defines variables for the first sensor
long duration1;
int distance1;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test with LCD and LEDs");
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Turn on backlight
   
}

void loop() {
  // Sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  
  // Print distance to serial monitor
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  
  // Print distance to LCD
  lcd.setCursor(0, 1);
  lcd.print("               ");  // Clear previous distance
  lcd.setCursor(0, 1);
  lcd.print("Vehicle At:");
  lcd.print(distance1);
  lcd.print("m");

  // Control LEDs based on distance
  if (distance1 > 15) {
    lcd.setCursor(0, 0);
    lcd.print("               ");  // Clear previous distance
    lcd.setCursor(0, 0);
    lcd.print("Go Ahead");
    digitalWrite(greenLed, HIGH);  // Turn on green LED
    digitalWrite(yellowLed, LOW);  // Turn off yellow LED
    digitalWrite(redLed, LOW);     // Turn off red LED
  } else if (distance1 <= 15 && distance1 > 10) {
     lcd.setCursor(0, 0);
    lcd.print("               ");  // Clear previous distance
     lcd.setCursor(0, 0);
    lcd.print("Go Slow");
    digitalWrite(greenLed, LOW);   // Turn off green LED
    digitalWrite(yellowLed, HIGH); // Turn on yellow LED
    digitalWrite(redLed, LOW);     // Turn off red LED
  } else {
    lcd.setCursor(0, 0);
    lcd.print("               ");  // Clear previous distance
    lcd.setCursor(0, 0);
    lcd.print("Please Wait");
    digitalWrite(greenLed, LOW);   // Turn off green LED
    digitalWrite(yellowLed, LOW);  // Turn off yellow LED
    digitalWrite(redLed, HIGH);    // Turn on red LED
  }

  delay(1000); // Wait for a moment before the next measurement
}