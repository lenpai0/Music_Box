/*

  MODIFIED 9/30/2026

  Music Box V2

  Github instruction with schematic: https://github.com/lenpai0/Music_Box

  HUGE QUALITY OF LIFE UPDATE!!!
  Simplified a LOT of the process so that the only thing you need to change is 1 line of code and 
  it will just WORK! When scanning any tag, it will print the UID in serial if it hasn't been 
  scanned preivously. If the tag matches with the songID array, it will send the command to play 
  the song index. You can mix and match different tag and length. Tested with MiFare, NTAG213 and
  NTAG215. 
  
  Please find char *songID[] on line 72 to add your tags.

  Author note's: im looking back at v1 and oh my god i was cringing at my old code and how wildly 
  inefficient it was. im so sorry to the people who used this code and had figure out what was going 
  on.

  //previous description
  This is a modified program of my music box code. This code should work for any Arduino board.
  This program uses SPI and SoftwareSerial to talk to RFID RC522 and DFPlayer Mini. Serial
  monitor is used for monitoring feedback.

  Steps before uploading this program:
  
    Make sure you have the following libraries installed  
    - DFRobotDFPlayerMini by DFRobot
    - MFRC522 by GithubCommunity

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

//pins for RFID RC522 SDA and RST (these can be changed to other GPIO pins)
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
unsigned char spkrStatus = 0;  // 0 = not playing, 1 = playing song1, 2 = playing song2 etc.
unsigned char songStatus = 0;  // using this to save song states so we can resume the same song

/* new!! */
// please update this to match your UID tag ie: song 0 -> "01 23 45 67" song 1 -> "89 AB CD EF 01". (you can mix and match different tags. Tested with MiFare NTAG213 NTAG215)
char *songID[] = { "01 23 45 67", "89 AB CD EF 01"};

unsigned char songLen = sizeof(songID) / sizeof(songID[0]);

String prevContent = "";

void setup() {
  // Initialize Arduino serial
  Serial.begin(115200);

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
  myDFPlayer.volume(10);        // Volume
  myDFPlayer.EQ(0);            // Normal equalization
  Serial.println("ready\n");
}

void loop() {

  unsigned char i = 0;

  // Wait for RFID cards to be scanned

  // this handles no tag detected. idk why i need two of these functions to work
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

  //parsing string we received from RFID reader
  String content = "";

  for (byte i = 0; i < myRFID.uid.size; i++) {
    content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(myRFID.uid.uidByte[i], HEX));
  }

  content.toUpperCase();

  //Show UID on serial monitor
  if (content != prevContent) {
    Serial.print("USER ID tag :");  
    Serial.println(content);        
    prevContent = content;
  }


  //loop through entire songid to find matching id
  while (i < songLen) {
    if (content.substring(1) == songID[i] && spkrStatus != (i + 1)) {
      playMusic(i);
      break;
    }
    i++;
  }

  // in an attempt to delay to prevent serial flooding. might not actually need this after adjusting code
  delay(200);
}

void playMusic(unsigned char songIndex) {
  unsigned char i;
  i = songIndex + 1;

  //resume feature
  if (songStatus == i) {
    myDFPlayer.start();  //this resumes from the paused position on the mp3 player

  } else {
    Serial.print("Playing song: ");  // used for feedback.
    Serial.println(songIndex);
    myDFPlayer.playFolder(0, songIndex);  //folder, file
  }

  spkrStatus = i;
  songStatus = i;
}
