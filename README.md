# Music Box Guide

Heavily inspired by this [twitter post](https://x.com/xchikex/status/1829906860251758615)

Music box image here

I made a music box using 3 main component. RFID reader, mp3 player speaker module and Arduino. I was heavily inspired by a twitter post made by the user @xchikex & @cyousen2039. While they didn't provide any code or schematic, they provided a crucial detail that they used the RFID RC522 module. I then wanted to make this same contraption but instead make a disc representing some of my favourite songs. Similar to how a yo-kai watch medal works but 10 times more complicated. This was my first time using RFID and it wasn't as bad as I thought.

This branch will only include the instructions, schematic and code. I will not provide the song files nor the 3D CAD models of the disc as I quite literally traced the album art in fusion and probably get in copyright trouble if I share it. 

## Intro
This is a modified code of my music box intended for general music box use. This program should work with any arduino board. This code uses SPI, SoftwareSerial and hardware Serial. 

The way the program works is if the RFID reader finds an RFID tag and if it matches the song ID inside the program, it will send a command to the mp3 player module to output the corresponding song file that is stored on an SD card. The song will keep playing as long as the RFID tag is continously being scanned by the reader and stops if the tag is removed. Features included is the ability to pause and resume if you remove and scan the same tag.

# Instructions
## Components used
This is a list of the components I used to make my project work. 
- Arduino Uno
- RFID RC522
- DFPlayer Mini
- SD Card (32GB is the maximum storage size supported)
- Mifare NFC Sticker

## Music box schematic
Below is the schematic I used to make the music box.
![image](https://github.com/user-attachments/assets/38e2b7f7-81da-4c73-a753-0261d115e450)

Few things to note:
1. It is highly recommended that you use an external power supply especially for the DFPlayer Mini.
   
   *Microcontrollers like an Arduino are not design to output enough current for power hungry device like motors and speakers. Too much current draw can "brick" a component for good so be warned!*
2. Datasheet for the DFPlayer Mini suggest using a "resistor in series" between the microcontroller and DFPlayer if the microcontroller output is 5V.
   
   *However, I think they meant voltage divider circuit because DFPlayer uses 3V3 and Arduino uses 5V and this resistor will only limit the current going in the player. The circuit still works even with this current limiting resistor. But what do I know? I'm not an engineer.*
3. You may noticed in some of my videos that I have a 100uf capacitor parallel to 5V and GND. I use it as a decoupling capacitor and it basically takes spiky uneven voltage line and smoothens it. This capacitor is not necessary for the music box to function but theorically could help if you have a noisy power signal. It's a small precaution I take.

## Libraries used
Make sure the following libraries are installed:
- DFRobotDFPlayerMini by DFRobot

![image](https://github.com/user-attachments/assets/93b4da7f-732a-4bc6-8b1d-9b05384df708)

- MFRC522 by GithubCommunity

![image](https://github.com/user-attachments/assets/f54009ed-f50c-47d8-b9e8-d6ded8fb05f4)

## Getting the RFID tag 
You will need to obtain the RFID tags name first using the RFID RC522 reader. I recommend copying the names down somewhere like a spreadsheet.

Two options to get the RFID tag name.

Option 1 - Only RFID RC522 available:

- Follow this YouTube tutorial I used and follow their schematic and code. https://www.youtube.com/watch?v=lg8HRY8q004

Option 2 - Both RFID RC522 and DFPlayer Mini available:

- Build the circuit from the schematic above then use the code provided in this branch and uncomment the following lines in the sketch (ctrl-f is your best friend here):
  
  `//Serial.print("USER ID tag :");`

  `//Serial.print(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " ");`

  `//Serial.print(myRFID.uid.uidByte[i], HEX);`

  `//Serial.println();`
- Follow [After uploading sketch](https://github.com/lenpai0/Music_Box/blob/main/README.md#after-uploading-the-sketch) and open serial monitor to view feedback

## Storing files in SD card
You will need to source a sound file to use. Both .mp3 and .WAV are supported with the DFPlayer. 

This image is taken from the DFPlayer datasheet. Although there are no folder or files starting from 0, the code will still work. Note that DFPlayer Mini can support SD card up to 32GB. The sound folder limit is 0-99 and file limit is 0-255. So you can hold a lot of sound files.
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

## Before uploading the sketch
Download `music_box.ino`. Update the sketch to match your RFID tag name. Look for the lines like `content.substring(1) == "A1 2F 90 49"` and replace the tag ID. If you have more than two sound files, be sure to add additional code and follow the layout. 

i.e. The next lines of code for song3 would look something like this
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

## After uploading the sketch
Open serial monitor to see the feedback.

![image](https://github.com/user-attachments/assets/626dc969-4cf1-4701-b55a-dc0a978ed55f)

## Resourced used
- RFID code and guide used: https://www.youtube.com/watch?v=lg8HRY8q004
- DFPlayer Mini code and guide used: https://www.youtube.com/watch?v=7WiSeQxb1bU 

## Usage
If you find yourself using this code to add to your project or make a video/tutorial with it, I simply ask that you credit me @lenpai0 and provide a link to this page. Thanks and enjoy!
