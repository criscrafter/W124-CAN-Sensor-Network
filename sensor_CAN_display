#include <SPI.h>
#include <Wire.h>
#include <mcp2515.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_I2C_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

unsigned long lastDisplayUpdate = millis();

struct can_frame canMsg;

MCP2515 mcp2515(10);
Adafruit_SSD1306 OLED1(128, 64, &Wire, OLED_RESET);

float engCoolantTemp = 0;
float engOilPressure = 0;
float transOilTemp = 0;
float engOilTemp = 0;
int   fanPWM = 0;

void setup() { 

  if(!OLED1.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();


  OLED1.clearDisplay();

  OLED1.setTextSize(1);             // Normal 1:1 pixel scale
  OLED1.setTextColor(SSD1306_WHITE);        // Draw white text
  OLED1.setCursor(0,0);             // Start at top-left corner
  OLED1.println(F("CAN Data Display"));
  OLED1.println("- Friedrich Rosenthal");
  OLED1.display();

  delay(1000);
}

void loop() {

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    if (canMsg.can_id == 0x000 && canMsg.can_dlc >= 2) {
      int16_t scaledEngOil = (canMsg.data[0] << 8) | canMsg.data[1];
      engOilTemp = (scaledEngOil / 100.0) - 60.0;
    }

    if (canMsg.can_id == 0x002 && canMsg.can_dlc >= 2) {
      int16_t scaledPressure = (canMsg.data[0] << 8) | canMsg.data[1];
      engOilPressure = (scaledPressure / 1000.0);
    }

    if (canMsg.can_id == 0x001 && canMsg.can_dlc >= 2) {
      int16_t scaledTransOil = (canMsg.data[0] << 8) | canMsg.data[1];
      transOilTemp = (scaledTransOil / 100.0) - 60.0;
    }
    
    if (canMsg.can_id == 0x003 && canMsg.can_dlc >= 2) {
      int16_t scaledEngCoolant = (canMsg.data[0] << 8) | canMsg.data[1];
      engCoolantTemp = (scaledEngCoolant / 100.0) - 60.0;
    }

    if (canMsg.can_id == 0x004) {
       fanPWM = canMsg.data[0];
    }

  }

  if((millis() - lastDisplayUpdate) > 100){
    lastDisplayUpdate = millis();
    OLED1.clearDisplay();

    OLED1.setCursor(0,0);
    OLED1.print("Wassertemp:");
    OLED1.print(engCoolantTemp, 2);
    OLED1.print("C");

    OLED1.setCursor(0,10);
    OLED1.print("Motoroel.:");
    OLED1.print(engOilTemp, 2);
    OLED1.print("C");

    OLED1.setCursor(0,20);
    OLED1.print("Transoel.:");
    OLED1.print(transOilTemp, 2);
    OLED1.print("C");

    OLED1.setCursor(0,30);
    OLED1.print("Oeldruck:");
    OLED1.print(engOilPressure, 3);
    OLED1.print("Bar");
    
    OLED1.setCursor(0,40);
    OLED1.print("Luefter PWM:");
    OLED1.print(fanPWM);    

    OLED1.display();
  }
}
