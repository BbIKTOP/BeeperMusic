/*
 * BuzzPlayer.h
 *
 *  Created on: Jun 7, 2019
 *      Author: viktor remennik
 *
 *  Simple class to play tones on beeper/buzzer
 *
 *  Platform: ESP8266/Arduino core
 */

#ifndef BUZZPLAYER_H_
#define BUZZPLAYER_H_

#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <Ticker.h>


class BuzzPlayer
{
	private:
		struct ToneFreq
		{
				char name[5];
				int freq;
				int length;
		};

		int currentMelodyLength = 0;
		int currentMelodyAllocated = 0;

		struct ToneFreq  * currentMelody = NULL;

		char * currentMelodyString=NULL;

		int allocationChunkSize=20; // Allocation chunk size in bytes

		int pin=-1;
		int tempo=1000;

		bool printTone=false;

		// Last note always shall be silent!!!
		struct ToneFreq tonesTable[59]={
				{ "c",    261, 2 }, // Do
				{ "c#",   277, 2 }, // Do#
				{ "d",    294, 2 }, // Re
				{ "d#",   311, 2 }, // Re#
				{ "e",    329, 2 }, // Mi
				{ "f",    349, 2 }, // Fa
				{ "f#",   370, 2 }, // Fa#
				{ "g",    392, 2 }, // Sol
				{ "g#",   415, 2 }, // Sol#
				{ "a",    440, 2 }, // La
				{ "a#",   466, 2 }, // la#
				{ "b",    493, 2 }, // Ti
				{ "C",    523, 2 }, // Do
				{ "C#",   554, 2 }, // Do#
				{ "D",    587, 2 }, // Re
				{ "D#",   622, 2 }, // Re#
				{ "E",    659, 2 }, // Mi
				{ "F",    698, 2 }, // Fa
				{ "F#",   740, 2 }, // Fa#
				{ "G",    784, 2 }, // Sol
				{ "G#",   831, 2 }, // Sol#
				{ "A",    880, 2 }, // La
				{ "A#",   932, 2 }, // La#
				{ "B",    988, 2 }, // Ti

				{ "db",   277, 2 }, // Do#
				{ "eb",   311, 2 }, // Re#
				{ "gb",   370, 2 }, // Fa#
				{ "ab",   415, 2 }, // Sol#
				{ "bb",   466, 2 }, // la#

				{ "Db",   554, 2 }, // Do#
				{ "Eb",   622, 2 }, // Re#
				{ "Gb",   740, 2 }, // Fa#
				{ "Ab",   831, 2 }, // Sol#
				{ "Bb",   932, 2 }, // La#


				{ "do",   261, 2 }, // Do
				{ "do#",  277, 2 }, // Do#
				{ "re",   294, 2 }, // Re
				{ "re#",  311, 2 }, // Re#
				{ "mi",   329, 2 }, // Mi
				{ "fa",   349, 2 }, // Fa
				{ "fa#",  370, 2 }, // Fa#
				{ "sol",  392, 2 }, // Sol
				{ "sol#", 415, 2 }, // Sol#
				{ "la",   440, 2 }, // La
				{ "la#",  466, 2 }, // la#
				{ "ti",   493, 2 }, // Ti
				{ "DO",   523, 2 }, // Do
				{ "DO#",  554, 2 }, // Do#
				{ "RE",   587, 2 }, // Re
				{ "RE#",  622, 2 }, // Re#
				{ "MI",   659, 2 }, // Mi
				{ "FA",   698, 2 }, // Fa
				{ "FA#",  740, 2 }, // Fa#
				{ "SOL",  784, 2 }, // Sol
				{ "SOL#", 831, 2 }, // Sol#
				{ "LA",   880, 2 }, // La
				{ "LA#",  932, 2 }, // La#
				{ "TI",   988, 2 }, // Ti

				{ "s",    0, 2 }  // Silence
		};

		int tonesTableLength=sizeof(tonesTable)/sizeof(struct ToneFreq);

		int currrentBgNote=0;
		Ticker bgTicker;
		static void bgPlayer(BuzzPlayer*);

		bool isSeparator(char);

	public:
		BuzzPlayer(int pin);
		BuzzPlayer(int pin, int tempo);
		~BuzzPlayer();
		void setTempo(int t);
		int setMelody(const char * melodyStr);
		const char * getMelody(void);
		void printMelody(void);
		void playMelody(void);
		void playMelodyBg(void);
		bool isPlaying(void);
		void stopPlay(void);

		void setPrintTone(bool);
		bool getPrintTone(void);
};

#endif /* BUZZPLAYER_H_ */
