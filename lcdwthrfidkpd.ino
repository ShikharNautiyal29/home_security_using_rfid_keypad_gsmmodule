

/*
 * 
 * Projects sourced gotten from included Arduino examples and combined into
 *  one project
 * 
 * 
 * 10/5/2017
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
          #include <Password.h>
#include <LiquidCrystal_I2C.h>
          #include <Keypad.h> 

#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int servoPin = 6;
int flag=0;
Servo Servo1;
// initialize the library with the numbers of the interface pins

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

          Password password = Password( "1234" );
          const byte ROWS = 4; // Four rows
          const byte COLS = 4; //  columns
          // Define the Keymap
          char keys[ROWS][COLS] = {
          {'1','2','3','A'},
          {'4','5','6','B'},
          {'7','8','9','C'},
          {'*','0','#','D'}
          };
          // Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
          byte rowPins[ROWS] = { A0, A1, 2, 3 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
          byte colPins[COLS] = { 4, 5, 7, 8 };

 // Create the Keypad
          Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

         

void setup() 
{
         
  Servo1.attach(servoPin); 
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad// Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
 // lcd.print("hello, world!");

}
void loop() 
{
 // lcd.setCursor(0,1);
 // lcd.print(millis()/1000);
   // Turn off the display:
  //lcd.noDisplay();
  //delay(500);
  // Turn on the display:
  lcd.display();
  delay(500);
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  lcd.setCursor(0,0);
  Serial.print("UID tag :");
  lcd.print("UID:");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     lcd.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  lcd.print("Message : ");
  content.toUpperCase();
 // if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
  if (content.substring(1) == "87 82 02 C5")
  {
    
    lcd.setCursor(0,1);
    Serial.println("enter password");
    Serial.println();
    lcd.print("enter password");
    lcd.println();
                
while (flag==0)
{
     keypad.getKey();
  }
  Servo1.write(180); 
    delay(3000);
    Servo1.write(-180);
    delay(3000);
    Servo1.write(180);
  flag=0;                
     
 }
//                 else
//                 {
//                  Serial.print("not found");
//                  break;
               
//    
  
 
 else   {
    lcd.setCursor(0,1);
    Serial.println(" Access denied");
    lcd.print(" Access denied  ");
    delay(3000);
  }
} 
//take care of some special events
void keypadEvent(KeypadEvent eKey){
 switch (keypad.getState()){
   case PRESSED:
     Serial.print("Pressed: ");
     Serial.println(eKey);
     switch (eKey){
       case '*': guessPassword(); break;
       case '#': password.reset(); break;
        default:
              password.append(eKey);
    }
 }
}

void guessPassword(){
    Serial.print("Guessing password... ");
    if (password.evaluate()){
          //digitalWrite(ledPin,HIGH);
          Serial.print("ok");
         // digitalWrite(6,HIGH);
                      password.reset(); //resets password after correct entry

    
    flag=1;
    }else{
         // digitalWrite(ledPin,LOW);
          Serial.print("not ok");
          //flag=1;
                       password.reset(); //resets password after correct entry

    }
}
