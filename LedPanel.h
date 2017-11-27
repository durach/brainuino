#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define LED_PANEL_WIDTH 4
#define LED_PANEL_HEIGHT 1
#define LED_PANEL_PIN_CS 10
#define LED_PANEL_BRIGHTNESS 2

#define LED_PANEL_CHAR_WIDTH 5

class LedPanel {

  private:
    Max72xxPanel _matrix = Max72xxPanel(LED_PANEL_PIN_CS, LED_PANEL_WIDTH, LED_PANEL_HEIGHT); 
    String xxx = "xxxxxxxxxx"; // some code overwrites bytes in the string here #3 and #7
    
  public:
    LedPanel();
    void setup();  
    void test();
    void drawTime(double value);
    void drawString(String s);
};

