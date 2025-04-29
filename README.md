# Music Box Guide

Heavily inspired by this [twitter post](https://x.com/xchikex/status/1829906860251758615)

Music box image here

I made a music box using 3 main component. RFID reader, mp3 player speaker module and Arduino. I was heavily inspired by a twitter post made by the user @xchikex & @cyousen2039. While they didn't provide any code or schematic, they provided a crucial detail that they used the RFID RC522 module. I then wanted to make this same contraption but instead make a disc representing some of my favourite songs. Similar to how a yo-kai watch medal works but 10 times more complicated.

This branch will only include the instructions, schematic and code. I will not provide the song files nor the 3D cad models of the disc as I quite literally traced the album art in fusion and probably get in copyright trouble if I do share it. 

## Intro
This is a modified code of my music box intended for general music box use. This program should work with any arduino board. This code uses SPI, SoftwareSerial and hardware Serial. 

The way the program works is if the RFID reader finds an RFID tag and if it matches the song ID inside the program, it will send a command to the mp3 player module to output the corresponding song file that is stored on an SD card. The song will keep playing as long as the RFID tag is continously being scanned by the reader and stops if the tag is removed. Features included is the ability to pause and resume if you remove and scan the same tag.

## Instructions
This is a list of the components I used to make my project work. 

### Components used
- Arduino Uno
- RFID RC522
- DFPlayer Mini
- Mifare NFC Sticker

### Schematic

## Resourced used
- RFID code and guide used: https://www.youtube.com/watch?v=lg8HRY8q004
- DFPlayer Mini code and guide used: https://www.youtube.com/watch?v=7WiSeQxb1bU 

## Usage
