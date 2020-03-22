#include <SoftwareSerial.h>

#define DEBUG_ 0

/* Direction def */
#define FWD   0
#define BWD   1

/* Pinout macros */
#define DIR_L 4
#define PWM_L 5
#define PWM_R 6
#define DIR_R 7
#define RX    10
#define TX    11

/* Global vars AP init & GPIO controller */
const String apSsid = "RcSiegeCreep";
const String apPwd = "hello!!";
String lastEspRead = "";
uint8_t carSpeed = 150; /* Must be bounded between 0 (slowest) and 255 (fastest) */ 

SoftwareSerial softSerial(RX, TX); // RX, TX

void setup()
{
  /* Serials init */
  Serial.begin(19200);
  softSerial.begin(19200);

  /* ESP8266 AP init */
  /* Setting AP's config */
  Serial.println(espExec("AT+CWSAP=\"" + apSsid + "\",\"" + apPwd + "\",1,4"));
  delay(500);
  /* Setting to AP mode only */
  Serial.println(espExec("AT+CWMODE=2"));
  delay(500);
  /* Enabling CIPMUX for multiple connections */
  Serial.println(espExec("AT+CIPMUX=1"));
  delay(500);
  /* Starting an UDP server */
  Serial.println(espExec("AT+CIPSTART=0,\"UDP\",\"192.168.4.1\",4445,4445,2")); /* Modify UDP to TCP if TCP socket is needed */
  delay(500);

  /* Pins init */
  pinMode(DIR_L, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);
  digitalWrite(DIR_L, FWD);
  digitalWrite(DIR_R, FWD);
}

void loop()
{
  /* Continuously read from ESP8266 to see if there's 
  a command to process */
  lastEspRead = espRead();
  if (lastEspRead.length() > 0)
  {
    /* Actual command starts after ':' */
    int cmdIdx = lastEspRead.indexOf(":");
    String cmd = lastEspRead.substring(cmdIdx + 1);
  #ifndef DEBUG_
    Serial.println("[DEBUG]: Command received: " + cmd);
  #endif
    gpioControllerExec(cmd);
    lastEspRead = "";
  }
}

/**
 * @brief: Send a command to ESP8266 and read its' response
 * @param[in]: cmd - an AT command
 * @return: the status/response of ESP8266
 */
String espExec(String cmd)
{
  softSerial.println(cmd);
  return espRead();
}

/**
 * @brief: Read the status/response from ESP8266
 * @return: the status/response of ESP8266
 */
String espRead()
{
  String espRetStr = "";
  while (softSerial.available())
  {
    char espRetChar = softSerial.read();
    espRetStr.concat(espRetChar);
  }
  return espRetStr;
}

/**
 * @brief: Execute an RC car command using the GPIOs
 * @param[in]: cmd - the command to execute
 */
void gpioControllerExec(String cmd) {
  int cmdEnum = cmd.toInt();
#ifndef DEBUG_
  Serial.print("[DEBUG]: CmdEnum: " + cmdEnum);
#endif
  switch (cmdEnum)
  {
    case 1:
      {
        goFF();
        break;
      }
    case 2:
      {
        goFR();
        break;
      }
    case 3:
      {
        goFL();
        break;
      }
    case 4:
      {
        goB();
        break;
      }
    case 5:
      {
        goBR();
        break;
      }
    case 6:
      {
        goBL();
        break;
      }
    case 7:
      {
        goL();
        break;
      }
    case 8:
      {
        goR();
        break;
      }
    case 9:
      {
        Stop();
        break;
      }
    default:
      {
        break;
      }
  }
}

/**
 * @brief: Go forward
 */
void goFF()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: FULL FWD");
#endif
  digitalWrite(DIR_L, FWD);
  analogWrite(PWM_L, carSpeed);
  analogWrite(PWM_R, carSpeed);
  digitalWrite(DIR_R, FWD);
}

/**
 * @brief: Go forward and slightly to the right
 */
void goFR()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: FWD RIGHT");
#endif
  digitalWrite(DIR_L, FWD);
  analogWrite(PWM_R, carSpeed);
  analogWrite(PWM_L, 0);
  digitalWrite(DIR_R, FWD);
}

/**
 * @brief: Go forward and slightly to the left
 */
void goFL()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: FWD LEFT");
#endif
  digitalWrite(DIR_L, FWD);
  analogWrite(PWM_R, 0);
  analogWrite(PWM_L, carSpeed);
  digitalWrite(DIR_R, FWD);
}

/**
 * @brief: Go backward
 */
void goB()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: FULL BACK");
#endif
  digitalWrite(DIR_L, BWD);
  analogWrite(PWM_L, 255 - carSpeed);
  analogWrite(PWM_R, 255 - carSpeed);
  digitalWrite(DIR_R, BWD);
}

/**
 * @brief: Go backawrd and slightly to the right
 */
void goBR()
{
  #ifndef DEBUG_
    Serial.println("[DEBUG]: BACK RIGHT");
  #endif
  digitalWrite(DIR_L, BWD);
  analogWrite(PWM_L, 255 - carSpeed);
  analogWrite(PWM_R, 0);
  digitalWrite(DIR_R, FWD);
}

/**
 * @brief: Go backward and slightly to the left
 */
void goBL() {
#ifndef DEBUG_
  Serial.println("[DEBUG]: BACK LEFT");
#endif
  digitalWrite(DIR_L, FWD);
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 255 - carSpeed);
  digitalWrite(DIR_R, BWD);
}

/**
 * @brief: Turn left
 */
void goL() {
#ifndef DEBUG_
  Serial.println("[DEBUG]: FULL LEFT");
#endif
  digitalWrite(DIR_R, BWD);
  analogWrite(PWM_R, 255 - carSpeed);
  analogWrite(PWM_L, carSpeed);
  digitalWrite(DIR_L, FWD);
}

/**
 * @brief: Turn right
 */
void goR()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: FULL RIGHT");
#endif
  digitalWrite(DIR_R, FWD);
  analogWrite(PWM_R, carSpeed);
  analogWrite(PWM_L, 255 - carSpeed);
  digitalWrite(DIR_L, BWD);
}

/**
 * @brief: Stop by writing 0 to all PWM pins
 */
void Stop()
{
#ifndef DEBUG_
  Serial.println("[DEBUG]: STOP");
#endif
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);
  digitalWrite(DIR_R, FWD);
  digitalWrite(DIR_L, FWD);
}
