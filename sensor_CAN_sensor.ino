#include <SPI.h>
#include <Wire.h>
#include <mcp2515.h>

#include <ADS1115_WE.h>
#include <Adafruit_MAX31865.h>

#define ADC_I2C_ADDRESS 0x48

MCP2515 mcp2515(8);                                   //init CAN Controller
Adafruit_MAX31865 transOil = Adafruit_MAX31865(9);         //init Oil Temp Sensor
Adafruit_MAX31865 engOil = Adafruit_MAX31865(10);    //init Coolant Temp Sensor
ADS1115_WE adc = ADS1115_WE(ADC_I2C_ADDRESS);         //init ADC for Pressure Sensor

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;

float engineOilTemp = 0;
float transOilTemp = 0;

unsigned long lastSentMessage = millis();

#define RREF      425.3 //430.0 standart
#define RNOMINAL  100.0

void setup() {
  Wire.begin();
  Serial.begin(2000000);
  if(!adc.init())
    Serial.println("ADS1115 not connected!");
  engOil.begin(MAX31865_3WIRE);
  transOil.begin(MAX31865_3WIRE);

  canMsg1.can_id  = 0x000;
  canMsg1.can_dlc = 2;
  
  canMsg2.can_id  = 0x001;
  canMsg2.can_dlc = 2;

  canMsg3.can_id  = 0x002;
  canMsg3.can_dlc = 2;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  adc.setVoltageRange_mV(ADS1115_RANGE_6144);
}


void loop() {

  engineOilTemp = engOil.temperature(RNOMINAL, RREF);
  transOilTemp = transOil.temperature(RNOMINAL, RREF);

  Serial.print("Engine Oil: "); Serial.println(engineOilTemp);
  Serial.print("Transmission Oil: "); Serial.println(transOilTemp);

  engOilTempErrorCheck();
  transOilTempErrorCheck();

  float voltage;
  float BAR_Pressure;

  voltage = readChannel(ADS1115_COMP_0_GND);
  BAR_Pressure = 2.585 * ((voltage / 1000) - 0.5);
  Serial.print("Öldruck Spannung:");
  Serial.println(voltage);
  Serial.print("Öldruck:");
  Serial.println(BAR_Pressure, 3);

  if((millis() - lastSentMessage) > 100){
    lastSentMessage = millis();
    int16_t scaledEngOil = (int16_t)((engineOilTemp + 60.0) * 100.0);
    int16_t scaledtransOil = (int16_t)((transOilTemp + 60.0) * 100.0);
    int16_t scaledPressure = (int16_t)(BAR_Pressure * 1000.0);

    canMsg1.data[0] = (scaledEngOil >> 8) & 0xFF;  // High byte
    canMsg1.data[1] = scaledEngOil & 0xFF;         // Low byte
    mcp2515.sendMessage(&canMsg1);

    canMsg2.data[0] = (scaledtransOil >> 8) & 0xFF;  // High byte
    canMsg2.data[1] = scaledtransOil & 0xFF;         // Low byte
    mcp2515.sendMessage(&canMsg2);

    canMsg3.data[0] = (scaledPressure >> 8) & 0xFF;  // High byte
    canMsg3.data[1] = scaledPressure & 0xFF;         // Low byte
    mcp2515.sendMessage(&canMsg3);

  }

}

float readChannel(ADS1115_MUX channel){
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while(adc.isBusy()){delay(0);}
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}

void engOilTempErrorCheck(){
  
  // Check and print any faults
  uint8_t fault = engOil.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    engOil.clearFault();

  }

}
void transOilTempErrorCheck(){
 
  // Check and print any faults
  uint8_t fault = transOil.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    transOil.clearFault();
  }
}
