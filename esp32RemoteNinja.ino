#include "Freenove_WS2812_Lib_for_ESP32.h"

#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include "Ultrasonic.h"
#include <ESP32Servo.h>
#include "const.h"

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);
Ultrasonic ultrasonic(UltrasonicPin);
// Required to replace with your xbox address
// XboxSeriesXControllerESP32_asukiaaa::Core
// xboxController("44:16:22:5e:b2:d4");

// any xbox controller
XboxSeriesXControllerESP32_asukiaaa::Core xboxController("0c:35:26:36:58:36");

Servo myservoLeftFoot;
Servo myservoLeftLeg;
Servo myservoRightFoot;
Servo myservoRightLeg;


uint8_t xboxBtnLB       = 0;
uint8_t xboxBtnRB       = 0;
uint8_t xboxBtnLS       = 0;
uint8_t xboxBtnRS       = 0;

uint8_t xboxBtnA        = 0;
uint8_t xboxBtnB        = 0;
uint8_t xboxBtnX        = 0;
uint8_t xboxBtnY        = 0;

uint8_t xboxBtnDirUp    = 0;
uint8_t xboxBtnDirDown  = 0;
uint8_t xboxBtnDirLeft  = 0;
uint8_t xboxBtnDirRight = 0;

float   xboxJoyLHorz    = 0.0;
float   xboxJoyLVert    = 0.0;
uint8_t xboxJoyLBtn     = 0;
float   xboxJoyRHorz    = 0.0;
float   xboxJoyRVert    = 0.0;
uint8_t xboxJoyRBtn     = 0;

unsigned long receivedAt = 0; 

typedef enum eMode_t
{
  eDrive,
  eWalk
} eMode_t;

eMode_t eMode = eDrive;

typedef enum eStand_t
{
  eUndefined,
  eLeftLeg,
  eRightLeg
} eStand_t;

eStand_t eStand =  eUndefined;

void setModeDrive()
{
  if(eMode == eWalk)
  {
    Serial.println("Mode == Drive"); 
    eMode = eDrive;
    setServoDrive();
  }
}

void setModeWalk()
{
  if(eMode == eDrive)
  {
    Serial.println("Mode == Walk");
    eMode = eWalk;
    setServoWalk();
  }
}

void setStatLedRed()
{
  //Serial.println("Led Red");
  strip.setLedColorData(0, 255,   0,   0);
  strip.setLedColorData(1, 255,   0,   0);
  strip.setLedColorData(2, 255,   0,   0);
  strip.show();
}

void setStatLedObject()
{
  strip.setLedColorData(0, 255,   0,   0);
  strip.setLedColorData(1,   0, 255,   0);
  strip.setLedColorData(2, 255,   0,   0);
  strip.show();
}

void setStatLedYellow()
{
  //Serial.println("Led Yellow");
  strip.setLedColorData(0, 255, 240,   0);
  strip.setLedColorData(1, 255, 240,   0);
  strip.setLedColorData(2, 255, 240,   0);
  strip.show();  
}

void setStatLedGreen()
{
  //Serial.println("Led Green");
  strip.setLedColorData(0,   0, 255,   0);
  strip.setLedColorData(1,   0, 255,   0);
  strip.setLedColorData(2,   0, 255,   0);
  strip.show();    
}

void setStatLedBlue()
{
  //Serial.println("Led Blue");
  strip.setLedColorData(0,   0,   0, 255);
  strip.setLedColorData(1,   0,   0, 255);
  strip.setLedColorData(2,   0,   0, 255);
  strip.show();   
}

void setMotorOff()
{
  Serial.println("Motor aus! ");
  myservoLeftFoot.detach();
  myservoRightFoot.detach();  
  myservoLeftLeg.detach();
  myservoRightLeg.detach();
}

void setMotorOn()
{
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
}

void doWaveRight()
{
  myservoRightLeg.write(20);
  delay(200);
  myservoRightLeg.write(120);
  delay(200);
  myservoRightLeg.write(90);
}

void doWaveLeft()
{
  myservoLeftLeg.write(20);
  delay(200);
  myservoLeftLeg.write(120);
  delay(200);
  myservoLeftLeg.write(90);
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");
  // Start Xontroller 
  xboxController.begin();
  // Servos
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
  myservoLeftLeg.attach(ServoLeftLegPin, 544, 2400);
  myservoRightLeg.attach(ServoRightLegPin, 544, 2400);
  // Start Status-LED
  strip.begin();
  strip.setBrightness(50);  
  setStatLedBlue();
 
  eMode = eWalk;
  setServoWalk();
}

void setServoWalk()
{
  myservoLeftLeg.write(90);
  myservoRightLeg.write(90);
}

void setServoDrive()
{
  myservoLeftLeg.write( 180);
  myservoRightLeg.write(  0);
}


int iDriveLeftMotorSpeed = 0; 
int iDriveRightMotorSpeed = 0;
long range = 999;
void loop()
{
  xboxController.onLoop();

  if (xboxController.isConnected())
  {
    if (xboxController.isWaitingForFirstNotification())
    {
      Serial.println("waiting for first notification");
      setStatLedBlue();
    }
    else
    {
      setStatLedGreen();
      receivedAt = xboxController.getReceiveNotificationAt();
      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
      xboxJoyLHorz = (float)xboxController.xboxNotif.joyLHori; // / joystickMax;
      xboxJoyLVert = (float)xboxController.xboxNotif.joyLVert; // / joystickMax;
      xboxJoyRHorz = (float)xboxController.xboxNotif.joyRHori; // / joystickMax;
      xboxJoyRVert = (float)xboxController.xboxNotif.joyRVert; // / joystickMax;
      xboxJoyLBtn  = xboxController.xboxNotif.trigLT;    // Left Lower Front Button
      xboxJoyRBtn  = xboxController.xboxNotif.trigRT;    //Right Lower Front Button

      xboxBtnDirUp    = xboxController.xboxNotif.btnDirUp;
      xboxBtnDirDown  = xboxController.xboxNotif.btnDirDown;
      xboxBtnDirLeft  = xboxController.xboxNotif.btnDirLeft;
      xboxBtnDirRight = xboxController.xboxNotif.btnDirRight;
      
      xboxBtnA = xboxController.xboxNotif.btnA;
      xboxBtnB = xboxController.xboxNotif.btnB;
      xboxBtnX = xboxController.xboxNotif.btnX;
      xboxBtnY = xboxController.xboxNotif.btnY;

      xboxBtnLB = xboxController.xboxNotif.btnLB;    // Left Upper Front Button
      xboxBtnRB = xboxController.xboxNotif.btnRB;    // Right Upper Front Button 
      xboxBtnLS = xboxController.xboxNotif.btnLS;    // Joystick Left  pressed 
      xboxBtnRS = xboxController.xboxNotif.btnRS;    // Joystick Right pressed
/*
      Serial.println("--------------------------------------------------------------------------");
      Serial.println("Joy-L-Horz"+String(xboxJoyLHorz));
      Serial.println("Joy-L-Vert"+String(xboxJoyLVert));
      Serial.println("Joy-R-Horz"+String(xboxJoyRHorz));
*/      
      //Serial.println("Joy-R-Vert"+String(xboxJoyRVert) + "Joy-R-Horz"+String(xboxJoyRHorz) );
      //Serial.println("Joy-L-Vert"+String(xboxJoyLVert) + "Joy-L-Horz"+String(xboxJoyLHorz) );


//Servo myservoLeftFoot;
//Servo myservoLeftLeg;
//Servo myservoRightFoot;

      //////////////////////////////////////////////////////////////////////////////
      if(eMode == eDrive)
      {
        if((xboxJoyLVert<30000) and ((xboxJoyLHorz<20000) or (xboxJoyLHorz>40000)))
        { // Robot vorwärts, 
          // ein Motor läuft vorwärts der andere Rückwärts, dies ist durch die gespiegelte Einbaulage bedingt
          // die Null-Lage des Joysticks liegt bei ca 31000
          iDriveLeftMotorSpeed  = map(xboxJoyLVert,30000,0,95,180);
          iDriveRightMotorSpeed = map(xboxJoyLVert,30000,0,85,0);    
          Serial.println("Robot vorwärts");
        }
        if((xboxJoyLVert>35000)  and ((xboxJoyLHorz<20000) or (xboxJoyLHorz<40000)))
        { // Robot rückwärts
          iDriveLeftMotorSpeed  = map(xboxJoyLVert,35000,65535,85,0);
          iDriveRightMotorSpeed = map(xboxJoyLVert,35000,65535,95,180);  
          Serial.println("Robot rückwärts");
        }
        if((xboxJoyLVert>30000) and (xboxJoyLVert<35000) and ((xboxJoyLHorz>30000) and (xboxJoyLHorz<35000)))
        { // Robot steht, keine Joystickbewegung 
          iDriveLeftMotorSpeed  = 90;  
          iDriveRightMotorSpeed = 90;       
          Serial.println("Robot stopp");
        }
        if(((xboxJoyLVert>25000) and (xboxJoyLVert<40000)) and (xboxJoyLHorz<30000))
        {  // Robot dreht auf der Stelle (hoffentlich) - Joystick nur horz nach links - keine vertikale Bewegung 
           //iDriveLeftMotorSpeed = 90 + 40 * map(xboxJoyLVert,30000,0,0,1);
           //iDriveRightMotorSpeed = 90 - 40 * map(xboxJoyLVert,30000,0,0,1);
           iDriveLeftMotorSpeed  = 180;
           iDriveRightMotorSpeed =  90; // keine Bewegung
           Serial.println("Robot dreht links");
        }
        if(((xboxJoyLVert>25000) and (xboxJoyLVert<40000)) and (xboxJoyLHorz>35000))
        {  // Robot dreht auf der Stelle (hoffentlich) - Joystick nur horz nach rechts - keine vertikale Bewegung 
           //iDriveLeftMotorSpeed = 90 - 15 * map(xboxJoyLVert,35000,65535,0,1);
           //iDriveRightMotorSpeed = 90 + 15 * map(xboxJoyLVert,35000,65535,0,1);
           iDriveLeftMotorSpeed  =  90; // keine Bewegung
           iDriveRightMotorSpeed =   5; 
           Serial.println("Robot dreht rechts ");
        }

//      range = ultrasonic.MeasureInCentimeters();
//      if(range > 15)
//      {
          myservoLeftFoot.write(iDriveLeftMotorSpeed);
          myservoRightFoot.write(iDriveRightMotorSpeed);
//      }
//      else
//      {
//        Serial.println("Objekt zu nah!");
//        setStatLedObject();
//        setMotorOff();
//      }
      }
      else
      { // eMode == eWalk) --------------------------------------------------------------------------------------------------
        if(xboxJoyRVert < 30000)
        { // rechter Joystick wird nach oben bewegt
          if(eStand==eLeftLeg)
          {
            // iDriveLeftMotorSpeed = map(xboxJoyLVert,30000,0,85,0);
            iDriveLeftMotorSpeed = 150;
            Serial.println("Robot Walk dreht auf dem linken Bein nach links"+String(iDriveLeftMotorSpeed));
          }
          if(eStand==eRightLeg)
          {
            //iDriveRightMotorSpeed = map(xboxJoyLVert,30000,0,95,180); 
            iDriveRightMotorSpeed = 25;
            Serial.println("Robot Walk dreht auf dem rechten Bein nach rechts"+String(iDriveRightMotorSpeed));
          }
        }
        if(xboxJoyRVert > 35000)
        { // rechter Joystick wird nach unten bewegt
          if(eStand==eLeftLeg)
          {
            // iDriveLeftMotorSpeed  = map(xboxJoyLVert,35000,65535,95,180);  
            iDriveLeftMotorSpeed =  25; 
            Serial.println("Robot Walk dreht auf dem linken Bein nach rechts"+String(iDriveLeftMotorSpeed));
          }
          if(eStand==eRightLeg)
          {
            // iDriveRightMotorSpeed = map(xboxJoyLVert,35000,65535,85,0);
            iDriveRightMotorSpeed = 150;
            Serial.println("Robot Walk dreht auf dem rechten Bein nach links"+String(iDriveRightMotorSpeed));
          }
        }
        if((xboxJoyRVert > 30000) and (xboxJoyRVert < 35000))
        { // Mittelposition des Joysticks 
          // => keine Rotation des Robots im Walk-Mode
          // => keine Fortbewegung im Drive-Mode
          iDriveLeftMotorSpeed  = 90;
          iDriveRightMotorSpeed = 90;
          Serial.println("Robot Walk stoppt");
        }
        myservoLeftFoot.write(iDriveLeftMotorSpeed);
        myservoRightFoot.write(iDriveRightMotorSpeed);
        if(xboxBtnDirLeft>0)
        {
          if(eMode == eWalk)
          {
            Serial.println("Btn-Left  "+String(xboxBtnDirLeft));
            myservoLeftLeg.write(130);
            myservoRightLeg.write(RATL);
            eStand = eLeftLeg;
          }
        }
        if(xboxBtnDirRight>0)
        {
          if(eMode == eWalk)
          {
            Serial.println("Btn-Right "+String(xboxBtnDirRight));
            myservoRightLeg.write(50);
            myservoLeftLeg.write(LATR);
            eStand = eRightLeg;
          }
        }
        if(xboxBtnLB>0)
        {
          Serial.println("xboxBtnLB "+String(xboxBtnLB)); //-------
          doWaveLeft();
        }
        if(xboxBtnRB>0)
        {
          Serial.println("xboxBtnRB "+String(xboxBtnRB)); //---------
          doWaveRight();
        }
      }
      if(xboxJoyLBtn>0)
      {
        Serial.println("Joy-L-Btn "+String(xboxJoyLBtn));
      }
      if(xboxJoyRBtn>0)
      {
        Serial.println("Joy-R-Btn "+String(xboxJoyRBtn));
      }


      if(xboxBtnDirUp>0)
      {
        Serial.println("Btn-Up    "+String(xboxBtnDirUp));
      }
      if(xboxBtnDirDown>0)
      {
        Serial.println("Btn-Down  "+String(xboxBtnDirDown));
      }
      if(xboxBtnA>0)
      {
        Serial.println("Btn-A     "+String(xboxBtnA));
        setModeWalk();
      }
      if(xboxBtnB>0)
      {
        Serial.println("Btn-B     "+String(xboxBtnB));
        setModeDrive();
      }
      if(xboxBtnX>0)
      {
        Serial.println("Btn-X     "+String(xboxBtnX));
        setMotorOn(); 
        eMode = eWalk;
        eStand =  eUndefined;
        setServoWalk();
        int i = 0;
        while(i<20)
        {
          Serial.println(" ");
          i=i+1;
        }
      }
      if(xboxBtnY>0)
      {
        Serial.println("Btn-Y     "+String(xboxBtnA));
      }

      if(xboxBtnLS>0)
      {
        Serial.println("xboxBtnLS "+String(xboxBtnLS)); 
      }
      if(xboxBtnRS>0)
      {
        Serial.println("xboxBtnRS "+String(xboxBtnRS));
      }
      //Serial.println("--------------------------------------------------------------------------");
    }
  }
  else
  {
    if(receivedAt > 0)
    {
      Serial.println("not connected");
      if ((millis() - receivedAt)>500)
      {  // 0.5 seconds no contact to the controller
        Serial.println("TimeOut - ReceivedAt");
        setStatLedRed();
        setMotorOff();
      } 
 /*     if (xboxController.getCountFailedConnection() > 2)  // Abfrage scheint nicht zu funktionieren, daher der Vergleich millis mit receivedAt
      {
        setStatLedRed();
        setMotorOff();
      } */
      else
      {
        setStatLedYellow();
      }
    }
    else
    {
      Serial.println("Never had contact!");
    }
  }
}