# BeeperMusic
A simple library to play backgroiund tones on the beeper. Tested with ESP8266

## Notes
Notes are encoded as abcdefg for the first octave, and ABCDEFG for the second. S is for silence. The note's value (i. e. relative duration) shall be stated right after the note. For example c2 means breve C (do). Notes are separated by the comma, new line, space or other character specified in the bool BuzzPlayer::isSeparator(char c)

```c++
bool BuzzPlayer::isSeparator(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case ':':
    case ',':
    case '.':
    case '-':
    case ';':
        return (true);
    default:
        return (false);
    }

    return (false);
}
```

## Usage


```c++
void setup()
{
    // Load melody to play
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

    // Set basic playing speed
    buzzPlayer.setTempo(1200);

    // Play melody in the foreground
    buzzPlayer.playMelody();

    // Play melody in the background
    buzzPlayer.playMelodyBg();
}

void loop()
{
    // Check is the background melody still playing
	if (buzzPlayer.isPlaying())
	{
		Serial.println("Oh G-sh! It's still playing!!!");
	}
	else
	{
		Serial.println("At last it's done...");
	}

	delay(1000);
}
```
