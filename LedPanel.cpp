
#include "config.h"
#include "LedPanel.h"
#include <Fonts/TomThumb.h>

LedPanel::LedPanel() {
   
}

void LedPanel::setup() {
  
  _matrix.fillScreen(LOW);  
  _matrix.setIntensity(LED_PANEL_BRIGHTNESS); 

  _matrix.setPosition(0, 0, 0);
  _matrix.setPosition(1, 0, 1);
  _matrix.setPosition(2, 0, 2);
  _matrix.setPosition(3, 0, 3);

  _matrix.setRotation(0, 1);
  _matrix.setRotation(1, 1);
  _matrix.setRotation(2, 1);
  _matrix.setRotation(3, 1);

  _time_y_offset = 6;
  _time_x_offset = 8;
}

void LedPanel::test() {
  _drawBegin();
  //drawString("60.00");
  //drawBrainTable1();
  //drawBrainTable2();
  //_drawNumber1(0);
  //_drawNumber2(1);
  _drawNumber3(0);
  _drawNumber4(1);
  _drawEnd();
}

void LedPanel::_drawBegin() {
  _matrix.fillScreen(LOW);  
}

void LedPanel::_drawEnd() {
  _matrix.write();
}

void LedPanel::drawTime(double value) {
  char s[6]; // ##.## and 0 terminator
  dtostrf(value, 5, 2, s);
  drawString(s);
}

void LedPanel::drawString(String s) {

  _matrix.setFont(&TomThumb);
  
  int y = _time_y_offset;
  int x = _time_x_offset;
  
  for (int i = 0; i < s.length(); i++) {

    _matrix.drawChar(x, y, s[i], HIGH, LOW, 1);
    
    x += LED_PANEL_CHAR_WIDTH;  

    // Hack for chosen font (Picopixel) to save matrix space ;)
    if (s[i] == '.') {
      x--;
    } else {
      x++;
    }
  }
}

void LedPanel::drawBrainTable1() {
  _drawNumber1(LEF_PANEL_BRAIN_TABLE_1_CELL);
}

void LedPanel::drawBrainTable2() {
  _drawNumber2(LEF_PANEL_BRAIN_TABLE_2_CELL);
}

void LedPanel::_drawNumber1(byte cell) {
  byte x = _getCellOffsetX(cell);
  byte y = 0;
  byte bg_color = LOW;
  byte fg_color = HIGH;
  _matrix.fillRect(x, y, 8, 8, bg_color);
  _matrix.drawFastHLine(x + 3, y + 0, 2, fg_color);
  _matrix.drawFastHLine(x + 2, y + 1, 3, fg_color);
  _matrix.drawFastHLine(x + 2, y + 2, 3, fg_color);
  _matrix.drawFastHLine(x + 3, y + 3, 2, fg_color);
  _matrix.drawFastHLine(x + 3, y + 4, 2, fg_color);
  _matrix.drawFastHLine(x + 3, y + 5, 2, fg_color);
  _matrix.drawFastHLine(x + 2, y + 6, 4, fg_color);
  _matrix.drawFastHLine(x + 2, y + 7, 4, fg_color);
}

void LedPanel::_drawNumber2(byte cell) {
  byte x = _getCellOffsetX(cell);
  byte y = 0;
  byte bg_color = LOW;
  byte fg_color = HIGH;
  _matrix.fillRect(x, y, 8, 8, bg_color);
  _matrix.drawFastHLine(x + 2, y + 0, 4, fg_color);
  _matrix.drawFastHLine(x + 1, y + 1, 6, fg_color);
  _matrix.drawFastHLine(x + 1, y + 2, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 2, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 3, 2, fg_color);
  
  _matrix.drawFastHLine(x + 5, y + 2, 2, fg_color);  
  _matrix.drawFastHLine(x + 3, y + 4, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 5, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 6, 6, fg_color);
  _matrix.drawFastHLine(x + 1, y + 7, 6, fg_color);
}

void LedPanel::_drawNumber3(byte cell) {
  byte x = _getCellOffsetX(cell);
  byte y = 0;
  byte bg_color = LOW;
  byte fg_color = HIGH;
  _matrix.fillRect(x, y, 8, 8, bg_color);
  _matrix.drawFastHLine(x + 2, y + 0, 4, fg_color);
  _matrix.drawFastHLine(x + 1, y + 1, 6, fg_color);
  _matrix.drawFastHLine(x + 1, y + 2, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 2, 2, fg_color);
  _matrix.drawFastHLine(x + 4, y + 3, 2, fg_color);
  _matrix.drawFastHLine(x + 4, y + 4, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 5, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 5, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 6, 6, fg_color);
  _matrix.drawFastHLine(x + 2, y + 7, 4, fg_color);
}

void LedPanel::_drawNumber4(byte cell) {
  byte x = _getCellOffsetX(cell);
  byte y = 0;
  byte bg_color = LOW;
  byte fg_color = HIGH;
  _matrix.fillRect(x, y, 8, 8, bg_color);
  _matrix.drawFastHLine(x + 5, y + 0, 2, fg_color);
  _matrix.drawFastHLine(x + 4, y + 1, 3, fg_color);
  _matrix.drawFastHLine(x + 3, y + 2, 4, fg_color);
  _matrix.drawFastHLine(x + 2, y + 3, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 3, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 4, 2, fg_color);
  _matrix.drawFastHLine(x + 5, y + 4, 2, fg_color);
  _matrix.drawFastHLine(x + 1, y + 5, 7, fg_color);
  _matrix.drawFastHLine(x + 1, y + 6, 7, fg_color);
  _matrix.drawFastHLine(x + 5, y + 7, 2, fg_color);
}

byte LedPanel::_getCellOffsetX(byte cell) {
  return cell * 8;
}

