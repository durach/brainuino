#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define LED_PANEL_WIDTH 4
#define LED_PANEL_HEIGHT 1
#define LED_PANEL_PIN_CS 10
#define LED_PANEL_BRIGHTNESS 2

#define LED_PANEL_CHAR_WIDTH 3

#define LEF_PANEL_BRAIN_TABLE_1_CELL 0
#define LEF_PANEL_BRAIN_TABLE_2_CELL 3

class LedPanel {

  private:
    Max72xxPanel _matrix = Max72xxPanel(LED_PANEL_PIN_CS, LED_PANEL_WIDTH, LED_PANEL_HEIGHT); 
    String xxx = "xxxxxxxxxx"; // some code overwrites bytes in the string here #3 and #7
    byte _time_y_offset;
    byte _time_x_offset;

    void _drawBegin();
    void _drawEnd();
    byte _getCellOffsetX(byte cell);
    void _drawNumber1(byte cell);
    void _drawNumber2(byte cell);
    void _drawNumber3(byte cell);
    void _drawNumber4(byte cell);
    
  public:
    LedPanel();
    void setup();  
    void test();
    void drawTime(double value);
    void drawString(String s);
    void drawBrainTable1();
    void drawBrainTable2();
};

