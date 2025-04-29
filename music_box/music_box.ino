/*

  MODIFIED 4/29/2025

  Music Box

  Github instruction with schematic: https://github.com/lenpai0/Music_Box

  This is a modified program of my music box code. This code should work for any Arduino board.
  This program uses SPI and SoftwareSerial to talk to RFID RC522 and DFPlayer Mini. Serial
  monitor is used for monitoring feedback.

  Steps before uploading this program:
  
    Make sure you have the following libraries installed  
    - DFRobotDFPlayerMini by DFRobot
    - MFRC522 by GithubCommunity

    Be sure to change the RFID tag @ line 124 & 137 to match your RFID tag ID

  **WARNING && RECOMMENDATION**
  I recommend using an external power supply especially for the DFPlayer Mini. I had problems where
  I would move the wires 2 nanometers and what I can assume caused the voltage level to drop and 
  make everything stop working. Resetting the board will fix this issue but the power dropping
  can cause components to brick so be aware and dont blame me if something breaks because of this!

  Guide and code used:
  - RFID: https://www.youtube.com/watch?v=lg8HRY8q004
  - DFPlayer Mini: https://www.youtube.com/watch?v=7WiSeQxb1bU

  by Lenpai (YouTube: https://www.youtube.com/@lenpai0 | TikTok: https://www.tiktok.com/@lenpai0)

*/

// libraries used
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <MFRC522.h>

//pins for RFID RC522 SDA and RST (these can be changed other GPIO pins)
#define SDAPIN 10
#define RSTPIN 9

//pins for DFPlayer Mini MP3 using softwareserial library 
//(can change to any GPIO but be sure to double check the board's supported pins https://docs.arduino.cc/learn/built-in-libraries/software-serial/)
#define RXPIN 6
#define TXPIN 5

// Create MFRC522 instance.
MFRC522 myRFID(SDAPIN, RSTPIN);

//using software serial to talk to speaker player module
SoftwareSerial mySoftwareSerial(RXPIN, TXPIN);  // (RX, TX)

// Create DFRobotDFPlayerMini instance.
DFRobotDFPlayerMini myDFPlayer;

// global speaker status
char spkrStatus = 0;  // 0 = not playing, 1 = playing song1, 2 = playing song2 etc.
char songStatus = 0;  // using this to save song states so we can resume the same song

void setup() {
  // Initialize Arduino serial
  Serial.begin(9600);

  //for rfid
  SPI.begin();        // Initiate  SPI bus
  myRFID.PCD_Init();  // Initiate MFRC522
  Serial.println();

  //for dfplayer
  mySoftwareSerial.begin(9600);

  //if dfplayer fails to initialize loop forever until manually reset
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true)
      ;  //ermm just reset board
  }

  myDFPlayer.setTimeOut(500);  // Serial timeout 500ms
  myDFPlayer.volume(5);        // Volume 5
  myDFPlayer.EQ(0);            // Normal equalization
  Serial.println("im ready\n");
}

void loop() {

  // Wait for RFID cards to be scanned

  // this handles no tag detected idk why i need two of these functions to work
  // otherwise it will scan once and refuse to accept it the second scan. keyword probably is "New"
  if (!myRFID.PICC_IsNewCardPresent() && !myRFID.PICC_IsNewCardPresent()) {

    //if the speaker is playing, pause it
    if (spkrStatus) {
      myDFPlayer.pause();
      spkrStatus = 0;
    }
    //Serial.println("PICC_IsNewCardPresent ---> Hell NO!");
    return;
  } else {
    //Serial.println("PICC_IsNewCardPresent ---> YES!");
  }

  // an RFID card has been scanned but no UID
  if (!myRFID.PICC_ReadCardSerial()) return;

  //idk what the next few lines are doing but just dont touch it
  String content = "";

  for (byte i = 0; i < myRFID.uid.size; i++) {
    //Serial.print(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(myRFID.uid.uidByte[i], HEX);
    content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(myRFID.uid.uidByte[i], HEX));
  }

  content.toUpperCase();

  if (content.substring(1) == "A1 2F 90 49" && spkrStatus != 1)  //change here the UID of the card/cards that you want to give access
  {
    //resume feature
    if (songStatus == 1) {
      myDFPlayer.start(); //this resumes from the paused position on the mp3 player

    } else {
      //Serial.println("Playing Binomi / MARETU"); // used for feedback. uncomment and change to match song
      myDFPlayer.playFolder(0, 0);  //folder, file
    }
    spkrStatus = 1;
    songStatus = 1;

  } else if (content.substring(1) == "B1 2D 90 49" && spkrStatus != 2) { //change here the UID of the card/cards that you want to give access
    //resume feature
    if (songStatus == 2) {
      myDFPlayer.start(); //this resumes from the paused position on the mp3 player

    } else {
      //Serial.println("Playing flos / R Sound Design"); // used for feedback. uncomment and change to match song
      myDFPlayer.playFolder(0, 1);  //folder, file
    }

    spkrStatus = 2;
    songStatus = 2;
  }

  // in an attempt to delay to prevent serial flooding. might not actually need this after adjusting code
  delay(200);
}
