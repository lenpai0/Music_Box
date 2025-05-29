# Music Box Guide

Heavily inspired by this [twitter post](https://x.com/xchikex/status/1829906860251758615) from user @xchikex & @cyousen2039.

<img src="https://github.com/user-attachments/assets/84d9c411-e8ce-44d7-b01f-54059618e28d" width = "700">

Pictures of the music box

<img src="https://github.com/user-attachments/assets/ef5abdcc-180a-49ff-803e-d900b0fa6908" width = "700">



I made a music box using three main components. RFID reader, mp3 player speaker module and Arduino Uno. I was heavily inspired by a twitter post made by the user @xchikex & @cyousen2039. While they didn't provide any code or schematic, they did provide a crucial detail that they had used the RFID RC522 module. I been wanting to make something similar ever since but instead make my own 3D-printed hand-painted disc representing some of my favourite songs. Similar to how Yo-Kai Watch medals works but 10 times more complicated. I should mentioned I used magnets for the satisfying snapping sound but also to hold the disc so it's always in front of the RFID reader. This was also my first time using RFID. 

# Intro
This is a modified code of my music box intended for general music box use. This program should work with any Arduino board. This code uses SPI, SoftwareSerial and hardware serial with hardware serial used for monitoring feedback. 

The way the program works is if the RFID reader finds an RFID tag and if it matches the song ID inside the program, it will send a command to the mp3 player module to output the corresponding song file that is stored on an SD card. The song will keep playing as long as the RFID tag is continously being scanned by the reader and stops if the tag is removed. Feature included in the code is the ability to pause and resume if you remove and scan the same tag.

# Guide
## Components used
This is a list of the components I used to make my project work. 
- Arduino Uno
- RFID RC522
- DFPlayer Mini
- 3 watt speaker
- SD Card (32GB is the maximum storage size supported)
- Mifare NFC Sticker

## Music box schematic
Below is the schematic I used to make the music box.
![image](https://github.com/user-attachments/assets/658f7880-a122-4f69-a29c-42a18f97eebd)

Few things to note:
1. It is highly recommended that you use an external power supply especially for the DFPlayer Mini.
   
   *Microcontrollers like an Arduino are not design to output enough current for power hungry device like motors and speakers. Too much current draw can "brick" a component for good so be warned!*
2. SPK_1 connects to the positive polarity of the speaker and SPK_2 connects to the negative polarity
3. Datasheet for the DFPlayer Mini suggest using a "resistor in series" between the microcontroller and DFPlayer if the microcontroller output is 5V.
   
   *However, I think they meant voltage divider circuit because DFPlayer uses 3V3 and Arduino uses 5V and this resistor will only limit the current going in the player. The circuit still works even with this current limiting resistor. But what do I know? I'm not an engineer.*
4. You may noticed in some of my pictures/videos that I have a 100uf capacitor parallel to 5V and GND. I use it as a decoupling capacitor and it basically takes spiky uneven voltage line and smoothens it. This capacitor is not necessary for the music box to function but theorically could help if you have a noisy power signal. It's a small precaution I take.

## Libraries used
Make sure the following libraries are installed:
- DFRobotDFPlayerMini by DFRobot

![image](https://github.com/user-attachments/assets/93b4da7f-732a-4bc6-8b1d-9b05384df708)

- MFRC522 by GithubCommunity

![image](https://github.com/user-attachments/assets/f54009ed-f50c-47d8-b9e8-d6ded8fb05f4)

## Getting the RFID tag 
You will need to obtain the RFID tags name using the RFID RC522 reader. 

Two options to get the RFID tag name both using the RFID reader.

Option 1 - Only RFID RC522 available:

- Follow this YouTube tutorial I used and follow their schematic and code. https://www.youtube.com/watch?v=lg8HRY8q004

Option 2 - Both RFID RC522 and DFPlayer Mini available:

- Build the circuit from the schematic above then use the code provided in this branch. Uncomment the following lines in the sketch (ctrl-f is your best friend here):
  
  `//Serial.print("USER ID tag :");`

  `//Serial.print(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " ");`

  `//Serial.print(myRFID.uid.uidByte[i], HEX);`

  `//Serial.println();`
- Upload the sketch. Follow `After uploading sketch` below and open serial monitor to view feedback

Either way, you should be expecting a message that says something like `USER ID tag : XX XX XX XX` I recommend copying the RFID tag somewhere like a spreadsheet.

## Storing files in SD card
You will need to source sound files to use. Both .mp3 and .WAV are supported with the DFPlayer. 

This image is taken from the DFPlayer datasheet. Although there are no folder or files starting from 0, the code will still work. Note that DFPlayer Mini can support SD card up to 32GB. The sound folder limit is 0-99 and file per folder limit is 0-255. So you can hold a lot of sound files.

![image](https://github.com/user-attachments/assets/542fef3a-64e7-40d1-b774-1c11a23e1066)

Once you have sound files to use, you must follow the naming convention. 
1. Inside the SD Card, create a folder named "00"

![image](https://github.com/user-attachments/assets/d659550a-e85a-436d-bb83-0ba071bd0803)

2. Drag and drop the sound files into this folder

![image](https://github.com/user-attachments/assets/51ba3d97-fa3f-4d3f-8f64-8fe04e64ae7f)

3. Rename all the sound files starting from "000"

![image](https://github.com/user-attachments/assets/edb250e1-7b13-492b-8bac-ac873ab24d80)

I also recommend making a copy of this folder as well as writing down the names of the sound files and their number to reference later.

Remember to eject the SD card before removing.

## Before uploading sketch
Download `music_box.ino`. Update the sketch to match your RFID tag name. Look for the lines like `content.substring(1) == "A1 2F 90 49"` and replace the tag ID. If you have more than two sound files, be sure to add additional code and follow the layout. 

i.e. The next lines of code for song3 would look something like this and so on for additional sound files
```
else if (content.substring(1) == "xx xx xx xx" && spkrStatus != 3) { //change here the UID
    //resume feature
    if (songStatus == 3) {
      myDFPlayer.start(); //this resumes from the paused position on the mp3 player

    } else {
      //Serial.println("Playing yy / z"); // used for feedback. uncomment and change to match song
      myDFPlayer.playFolder(0, 2);  //folder, file
    }

    spkrStatus = 3;
    songStatus = 3;
  }
```

You can also change the pins for SDA, RST, software TX and software RX if needed. Keep in mind the available software serial pins for other Arduino boards https://docs.arduino.cc/learn/built-in-libraries/software-serial/

## After uploading sketch
Open serial monitor to see the feedback.

![image](https://github.com/user-attachments/assets/626dc969-4cf1-4701-b55a-dc0a978ed55f)

Upon start up, you will see one of the two messages.

1. `"im ready"` states that initialization for all the components are successful and ready to run
2. `"Not initialized: 1. Check the DFPlayer Mini connections 2. Insert an SD card"` Something is wrong with the DFPlayer Mini. Follow the two troubleshooting points. In my experience, sometimes power cycling the Arduino without changing anything can fix it (power cycling is a fancy industry term for turning something off and on lmao)

## Demo
This is a demo of the project using the same code posted here. Here is a [YouTube link](https://www.youtube.com/watch?v=7Qq8ZaPxRZM) to the same video below

https://github.com/user-attachments/assets/7b9280ac-cf74-4e77-b26d-84fbd12c0a22

Song used for demo
- [ビノミ (Binomi) / MARETU](https://www.youtube.com/watch?v=fGizrX4JjPg)
- [flos / R Sound Design](https://www.youtube.com/watch?v=bUbOc97FpUA)

## Problem experienced
- External power supply is a must if I want this to function longer than 3 minutes of using. What I mean is I would accidentally move my 5V wire on my breadboard and what I can assume, caused the voltage level supplying DFPlayer to become unstable which makes everything stop working. I know this because I would bump into the box and then hear a small "popping" sound from the speaker that's usually associated when starting up. Power cycling the Arduino will fix whenever that happens. 

- Moving the wires between the DFPlayer and speaker might cause the speaker to suddenly sound distorted. I would get too scared of the noise thinking something will break that I would instantly unplug the Arduino and plug back which resolves the issue.

- SD card/SD card reader is sometimes finicky and likes to randomly corrupt itself whenever I plug in my SD card to my computer forcing me to format. This is why I always keep a backup file for my SD card.

- This is the only project I had problems where my Arduino would randomly encounter "Unrecognized device" problems after a few use. Windows would suddenly not recognize the port no matter how many times I reset the board, unplug and plug the board, or update drivers. I have a good USB connector and the only way I can fix this is by resetting my computer. I'm not sure if I had installed something that is interfering the serial communication between the Arduino and Windows or if the libraries I'm using are doing something funky at the start of boot up that I am not aware of.

## Vocaloid disc I made 
   - binomi / MARETU https://www.thingiverse.com/thing:7042155/files

## Resourced used
- RFID code and guide used: https://www.youtube.com/watch?v=lg8HRY8q004
- DFPlayer Mini code and guide used: https://www.youtube.com/watch?v=7WiSeQxb1bU 

## Usage
If you find yourself using this code to add to your project or make a video/tutorial with it, I simply ask that you credit me @lenpai0 and provide a link to this page. Thanks and enjoy!
