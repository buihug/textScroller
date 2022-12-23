#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommander.h>
#include <TextScroller.h>

// initialize objects (section 1)
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27, 16 chars, 2 lines
SerialCommander MyCommander;

// function declarations
void serialPrintOutput();
void printObjOutToLCD();
void userSerialportActivity();

// initialize variables
//   16-char ruler: "----------------"
String initSpacer = "................"; // as long as the LCD number of columns: 16
String myString = "Hello";
String mySpacer = ".......";

// initialize objects (section 2)
MyText objStr(myString); // ("Hello") <---pointer = 0 (all due to be parsed)
MyText objSpa(mySpacer); // ("....") <---pointer = length (all parsed)
MyText objOut(initSpacer); // initial starting string ("................")

void setup()
{
  // Sets pointers
  objStr.setPointer(0);
  objSpa.setPointer(mySpacer.length()); // set as .lenght() for initializing the flip-flop
  Serial.begin(9600); // starts serial port as object
  while (!Serial) { ; } // waits for serial port connection up
  MyCommander.setPort(&Serial);	// sets MyCommander to current serial port
  lcd.init();
  lcd.backlight();
  Serial.println(); // line feed as a spacer
  serialPrintOutput(); // <=== DEBUGGER =====
  printObjOutToLCD(); // prints initial screen to LCD
  delay(500);
}

void loop()
{
  if (MyCommander.available()) // if there is user activity at the Serial port
  {
    userSerialportActivity();
  }
  // ====== (BEGINS) ===== myString<--->mySpacer FLIP-FLOP =====
  // (1) Parsing myString
  if (!objStr.allParsed() && objSpa.allParsed()) // does while NOT all mySpacer parsed
  {
    objOut.scrollToLeft(objStr.getParsingChar()); 
    if (objStr.allParsed()) // all of myString parsed now. Reset mySpacer pointer to 0
    {
      objSpa.setPointer(0); // setting to 0 prompts a parsing myString<--->mySpacer flip-flop
    }
  }
  // (2) Parsing mySpacer
  if (objStr.allParsed() && !objSpa.allParsed()) // does while NOT all mySpacer parsed
  {
    objOut.scrollToLeft(objSpa.getParsingChar());
    if (objSpa.allParsed()) // all of mySpacer parsed now. Reset myString pointer to 0
    {
      objStr.setPointer(0); // setting to 0 prompts a parsing myString<--->mySpacer flip-flop
    }
  } // ====== (ENDS) ===== myString<--->mySpacer FLIP-FLOP =====
  serialPrintOutput(); // <=== DEBUGGER ====
  printObjOutToLCD();
}

void serialPrintOutput() // prints to Serial port monitor (output plus debugging values)
{
  // uncomment for debugging
  // Serial.println(objOut.getText() + "_____STp: " + String(objStr.getPointer()) + "__STl: " + String(objStr.getLength()) + "_____SPp: " + String(objSpa.getPointer()) + "__SPl: " + String(objSpa.getLength()));
  ;
}

void printObjOutToLCD() // prints objOut text to LCD
{
  lcd.setCursor(0,0);
  lcd.print(objOut.getText());
  delay(700);
}

void userSerialportActivity() // adresses user Serial port activity
{  
  char newText[50]; // a buffer to store the input
  // Serial.println();
  Serial.println(F("\n---- Serial Commander ----")); // A welcome msg (optional)
  // menu choice 1
  Serial.println(F("Enter a new text + [ENTER]")); // prompt
  if (MyCommander.readTo(newText, sizeof(newText)))
  {
    Serial.println(F("Injecting new text........"));
    // reset pointers
    if (!objStr.allParsed()) // still parsing myString
    {
      objStr.setPointer(666); // prevents parsing the new myString as yet
      objSpa.setPointer(objSpa.getLength() - 3); // prompts parsing 3 chars of mySpacer to separate old from new text
    } 
    else // still parsing mySpacer
    {
      while (objSpa.getPointer() < 3) // while less than 3 mySpacer chars parsed
      {
        // scroll another mySpacer char
        objOut.scrollToLeft(objSpa.getParsingChar());
        serialPrintOutput();
      }
      // prompts parsing the new myString
      objSpa.setPointer(objSpa.getLength());
      objStr.setPointer(0);
    }
    // reset myString
    objStr.setText(newText); // newText value replaces that of myString
  }
  else // if after 10 seconds no extra activity, then Bye
  {
    Serial.println();
    Serial.println(F("Bye"));
    Serial.println();
  }
}

// Class
//
MyText::MyText(String a)    // Constructor
{
  _myStr = a;
}
// gets length
unsigned int MyText::getLength()
{
  return _myStr.length();
}
// gets pointer
unsigned int MyText::getPointer()
{
  return _myPointer;
}
// sets pointer
void MyText::setPointer(unsigned int p)
{
  _myPointer = p;
}
// gets parsing char
String MyText::getParsingChar()
{
  String pointedChar = _myStr.substring(_myPointer, _myPointer + 1); // gets pointed char
  incrementPointer(); // increments pointer
  return pointedChar;
}
// increments pointer
void MyText::incrementPointer()
{
  _myPointer++; // += 1;
}
// scroller
void MyText::scrollToLeft(String r)
{
  _myStr.remove(0,1); // removes most left char
  _myStr += r; // adds argument char at the right end
}
// gets text
String MyText::getText()
{
  return _myStr;
}
// sets text
void MyText::setText(String t)
{
  _myStr = t;
}
// verifies if all parsed
bool MyText::allParsed()
{
  if (_myPointer >= getLength())
  {
    return true; // all parsed
  } else {
    return false; // chars due to be parsed
  }
}
