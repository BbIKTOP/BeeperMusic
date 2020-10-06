#include <Arduino.h>
#include <FS.h>

#include "BuzzPlayer.h"

int loadFileAsString(char *fileName, char *buffer, int bufferSize);

BuzzPlayer buzzPlayer(15);

void setup()
{
	Serial.begin(115200);
	Serial.println("\n\nStarting\n");

	SPIFFS.begin();

	char melodyBuffer[2048] = {0};
	loadFileAsString("/melody.txt", melodyBuffer, 2048);

	Serial.println("Started");

	buzzPlayer.setMelody((const char *)"a4,e4,a4,e4,a4,g#4,g#4,"
									   "s4,"
									   "g#4,e4,g#4,e4,g#4,a4,a4,"
									   "s4,"
									   "a4,e4,a4,e4,a4,g#4,g#4,"
									   "s4,"
									   "g#4,e4,g#4,e4,g#4,a2,"
									   "a2,"
									   "b4,b8,b8,b4,"
									   "b4,C4,C8,C8,C4,"
									   "C4,C4,b4,a4,g#4,a4,a4,"
									   "s4,"
									   "a4,b4,b8,b8,b4,"
									   "b4,C4,C8,C8,C4,"
									   "C4,C4,b4,a4,g#4,a2,"
									   "s8,"
									   "g#2,a2,"
									   "s2");
	//buzzPlayer.printMelody();
	// buzzPlayer.playMelody();

	/*
	 buzzPlayer.setMelody((const char *)"a-g-a-e-c-e-c-s-"
	 "a-g-a-e-c-e-c-s-"
	 "a-b-c-b-c-c-a-d-a-d-d-g-a-a-g-a-s-"
	 "a-g-a-e-c-e-a-s-"
	 "a-g-a-e-c-e-a-s-"
	 "a-b-c4b4-c-c-a-d-a-d-d-g-a-a-g-a-b-c-e-d-e-c-a-c-g-"
	 "e-d-e-c-a-c-g--e-f#-g-f#-e-e");
	 */

	buzzPlayer.setTempo(1200);
	buzzPlayer.setMelody(melodyBuffer);
	// buzzPlayer.printMelody();
	buzzPlayer.playMelody();

	Serial.println("Playing in BG");
	buzzPlayer.setTempo(400);
	//buzzPlayer.printMelody();
	buzzPlayer.playMelodyBg();
}

void loop()
{
	static int done = 0;

	if (buzzPlayer.isPlaying())
	{
		Serial.println("Oh G-sh! It's still playing!!!");
	}
	else
	{
		if (!done)
		{
			done = 1;
			Serial.println("At last it's done...");
		}
	}

	delay(5000);
}
