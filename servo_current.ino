#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// initialize pin variables
int sensor_plus = A1;
int sensor_neg = A0;
int sensorValue_plus;
int sensorValue_neg;
double mVolts;

// Low-pass filter
double data_filtered[] = {0, 0, 0, 0};
const int n = 1;

int current; // to check the current


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(60);

  pinMode(INPUT, A0);
  pinMode(INPUT, A1);

  pwm.setPWM(0,0,200);
  delay(2500);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the sensor pins and calculate voltage
  sensorValue_neg = analogRead(sensor_neg);
  sensorValue_plus = analogRead(sensor_plus);
  // 10-bit analog reading => V, V/R => I, A => mA
  mVolts = (sensorValue_plus-sensorValue_neg)*(0.00488 / 1.3) * 1000; // (5v/1024bits)*(1/1.3R)*(1000mA/1A) => mA

  // rolling average
  data_filtered[n-3] = data_filtered[n-2];
  data_filtered[n-2] = data_filtered[n-1];
  data_filtered[n-1] = data_filtered[n];
  data_filtered[n] = (mVolts + data_filtered[n-1] + data_filtered[n-1] + data_filtered[n-3])/4;
  current = data_filtered[n];

  // check the current
  if (current > 200) {
    pwm.setPWM(0,0,325);
  }


  // print the raw and filtered signals
  Serial.print("Min:");
  Serial.print(50);
  Serial.print(",");
  Serial.print("Max:");
  Serial.print(300);
  Serial.print(",");
  Serial.print("data:");
  Serial.print(data_filtered[n]);
  Serial.print(",");
  Serial.print("Raw:");
  Serial.println(mVolts);
}
