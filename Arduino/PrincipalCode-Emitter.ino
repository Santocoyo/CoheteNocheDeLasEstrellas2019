#include <Wire.h>
#include "i2c.h"
#include "i2c_BMP280.h"
#include <MPU9255.h>
const double SLP = 101600;
const double dp = 0;

BMP280 bmp280;
MPU9255 mpu;

const double lapse = 1 / 1.157;
const double CK = 273.15;
const double alfa = 0.0065;
bool calibracion = false;
bool activacion=false;
bool abertura=false;

float Altura0 = 0;


void setup()
{
    Serial.begin(9600);

    Serial.print("Probe BMP280: ");
    if (bmp280.initialize()) Serial.println("Sensor found");
    else
    {
        Serial.println("BMP missing");
        while (1) {}
    }
    if(mpu.init()){
      Serial.println("MPU missing");
    }
    else{
      Serial.println("Initialization succesful!");
    }

    // onetime-measure:
    bmp280.setEnabled(0);
    bmp280.triggerMeasurement();
}

void loop()
{
    mpu.read_acc();
    mpu.read_gyro();
    mpu.read_mag();
    unsigned long currentMillis = millis();
    bmp280.awaitMeasurement();

    float temperature;
    bmp280.getTemperature(temperature);

    float pascal;
    bmp280.getPressure(pascal);

    if(currentMillis>20000 && calibracion==false){
      Altura0 = (pow(SLP/(pascal-dp), lapse)-1)*(15+CK)/alfa;
      calibracion = true;
    }

    float Altura = (pow(SLP/(pascal-dp), lapse)-1)*(15+CK)/alfa - Altura0;

    activacion=Serial.read()>0;
    if(activacion==true && abertura==false){
      abertura=true;
    }
    
    bmp280.triggerMeasurement();

    Serial.print("Height: ");
    Serial.print(Altura);
    Serial.print("m; Pressure: ");
    Serial.print(pascal);
    Serial.print(" Pa; T: ");
    Serial.print(temperature);
    Serial.print(" C");
    Serial.print("AX: ");
    Serial.print(mpu.ax);
    Serial.print(" AY: ");
    Serial.print(mpu.ay);
    Serial.print(" AZ: ");
    Serial.print(mpu.az);
    Serial.print("    GX: ");
    Serial.print(mpu.gx);
    Serial.print(" GY: ");
    Serial.print(mpu.gy);
    Serial.print(" GZ: ");
    Serial.print(mpu.gz);
    Serial.print("    MX: ");
    Serial.print(mpu.mx);
    Serial.print(" MY: ");
    Serial.print(mpu.my);
    Serial.print(" MZ: ");
    Serial.print(mpu.mz);
    Serial.print(" Activ: ");
    Serial.print(activacion);
    Serial.print(" Abierto ");
    Serial.print(abertura);
    Serial.println();
    delay(100);
}
