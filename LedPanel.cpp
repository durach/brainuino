
#include "config.h"
#include "LedPanel.h"

LedPanel::LedPanel() {
   
}

void LedPanel::setup() {
    
  _matrix.setIntensity(LED_PANEL_BRIGHTNESS); 

  _matrix.setPosition(0, 0, 0);
  _matrix.setPosition(1, 0, 1);
  _matrix.setPosition(2, 0, 2);
  _matrix.setPosition(3, 0, 3);

  _matrix.setRotation(0, 1);
  _matrix.setRotation(1, 1);
  _matrix.setRotation(2, 1);
  _matrix.setRotation(3, 1);
}

void LedPanel::test() {
  _matrix.fillScreen(LOW);  
  _matrix.drawChar(0, 0, '1', HIGH, LOW, 1);
  _matrix.drawChar(10, 0, '2', HIGH, LOW, 1);
  _matrix.write();
}

