#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>


//SoftwareSerial mySerial(0, 1); // RX, TX
DFPlayerMini_Fast myDFPlayer;


void setup()
{
  Serial.begin(9600);
//  mySerial.begin(9600);
// 
////  myDFPlayer.begin(mySerial);
//
//  // if (!myDFPlayer.begin(mySoftwareSerial)) {
//  if (!myDFPlayer.begin(Serial)) {
//    //Use softwareSerial to communicate with mp3.
//            for (;;)
//            ; // Don't proceed, loop forever
//  }

  myDFPlayer.begin(Serial);

 

  myDFPlayer.volume(20); //set volume low
  myDFPlayer.play(1); //play mp3 file with leading identifier "0001"
  delay(5000); //wait 100ms
  myDFPlayer.pause(); 
}

void loop() {
  
}

//void loop()
//{
//  myDFPlayer.volume(20); //set volume low
//  myDFPlayer.play(1); //play mp3 file with leading identifier "0001"
//  delay(1000); //wait 100ms
//  myDFPlayer.volume(1); //set volume low
//
//  delay(100); //wait 100ms
//
//  myDFPlayer.volume(30); //crank that stuff!
//
//  delay(100); //wait another 100ms
//
//  myDFPlayer.EQSelect(1); //set EQ as pop
//
//  delay(100); //wait another 100ms
//
//  myDFPlayer.EQSelect(5); //set EQ as bass
//
//  delay(100); //wait another 100ms
//
//  myDFPlayer.pause(); //pause player
//
//  delay(100); //wait another 100ms
//
//  myDFPlayer.resume(); //resume player
//
//  delay(100); //wait another 100ms
//
//  myDFPlayer.loop(2); //loop mp3 file with leading identifier "0002"
//
//  while(1); //halt
//}
