/*
  TextScroller.h
  - A text scroller for the Arduino LCD (I2C version)
  - with user new text injections via serial port
  Created by Gui, December 2022.
  Released into the public domain.
  < Many thanks to Cristian D. Szwarc for creating and sharing SerialCommander >
  <                  source: https://github.com/cristianszwarc/SerialCommander >
*/
#ifndef TextScroller_h
#define TextScroller_h

#include <Arduino.h>

class MyText
{
  public:
    MyText(String a); // the constructor ---- example: ("Hello")
    unsigned int getLength();
    unsigned int getPointer();
    void setPointer(unsigned int p);
    void incrementPointer();
    String getParsingChar();
    void scrollToLeft(String r);
    bool allParsed();
    String getText();
    void setText(String t);
  private:
    String _myStr;
    unsigned int _myPointer;
};

#endif