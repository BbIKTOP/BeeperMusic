#include "BuzzPlayer.h"

BuzzPlayer::BuzzPlayer(int pin)
{
    this->pin = pin;
}
BuzzPlayer::BuzzPlayer(int pin, int tempo) : BuzzPlayer(pin)
{
    this->tempo = tempo;
}

BuzzPlayer::~BuzzPlayer()
{
    if (pin > 0)
    {
        noTone(pin);
    }
    if (currentMelodyAllocated > 0)
    {
        currentMelodyAllocated = 0;
        currentMelodyLength = 0;
        free(currentMelody);
    }
}

const char *BuzzPlayer::getMelody(void)
{
    return (currentMelodyString);
}
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
int BuzzPlayer::setMelody(const char *melodyStr)
{
    int currentStart = 0;
    int currentEnd = 0;

    int len = strlen(melodyStr);

    currentMelodyLength = 0;

    if (len == 0)
    {
        return (0);
    }

    currentMelodyString = (char *)realloc(currentMelodyString, len + 1);
    if (!currentMelodyString)
    {
        Serial.println("Cannot allocate memory for melody string");
        return (-1);
    }
    strcpy(currentMelodyString, melodyStr);

    // Skip separators/spaces
    while (currentStart < len && isSeparator(melodyStr[currentStart]))
    {
        currentStart++;
    }

    while (currentStart < len)
    {
        // Find current tone end
        for (currentEnd = currentStart;
             currentEnd < len && !isSeparator(melodyStr[currentEnd]);
             currentEnd++)
            ;

        // Serial.printf("Found end of note: Start=%d, end=%d\n", currentStart, currentEnd);

        // Find current beat start in the current tone
        int currentBeatStart;
        for (currentBeatStart = currentEnd - 1;
             currentBeatStart >= currentStart && isdigit(melodyStr[currentBeatStart]);
             currentBeatStart--)
        {
            delay(0);
        }
        currentBeatStart++;

        // Calculate current length value
        int currentBeat = 0;
        if (currentBeatStart >= currentEnd)
        {
            Serial.printf("No beat in note: %d-%d\n", currentStart, currentEnd);
            currentBeat = 1;
        }
        else
        {
            for (int beatStart = currentBeatStart; beatStart < currentEnd; beatStart++)
            {
                currentBeat = currentBeat * 10 + melodyStr[beatStart] - '0';
                delay(0);
            }
        }

        // Calculate current frequency
        int currentToneLen = currentBeatStart - currentStart;
        int foundTone = 0;
        // Last note always shall be silent!!!
        for (foundTone = 0; foundTone < tonesTableLength - 1; foundTone++)
        {
            int currentToneNameLen = strlen(tonesTable[foundTone].name);
            if (currentToneNameLen != currentToneLen)
            {
                continue;
            }
            if (memcmp(&melodyStr[currentStart], tonesTable[foundTone].name,
                       currentToneNameLen) == 0)
            {
                break;
            }
            delay(0);
        }

        //Done calculating, store values

        // Check for allocated space, allocate another chunk if necessary
        if (currentMelodyLength + 1 > currentMelodyAllocated)
        {
            currentMelodyAllocated += allocationChunkSize;
            currentMelody = (struct ToneFreq *)realloc(currentMelody,
                                                       currentMelodyAllocated * sizeof(struct ToneFreq));

            if (currentMelody == NULL)
            {
                Serial.println("Error - cannot allocate memory for melody");
                return (-1);
            }
        }

        delay(0);

        memcpy(&currentMelody[currentMelodyLength], &tonesTable[foundTone],
               sizeof(struct ToneFreq));

        // Set length if any
        if (currentBeat >= 0)
        {
            currentMelody[currentMelodyLength].length = currentBeat;
        }
        currentMelodyLength++;

        currentStart = currentEnd;

        // Skip separators/spaces
        while (currentStart < len && isSeparator(melodyStr[currentStart]))
        {
            currentStart++;
        }

        // Serial.printf("End of loop: Start=%d, end=%d\n", currentStart, currentEnd);

        delay(0);
    }

    return (0);
}

void BuzzPlayer::setPrintTone(bool pt)
{
    printTone = pt;
}

bool BuzzPlayer::getPrintTone(void)
{
    return (printTone);
}

void BuzzPlayer::printMelody(void)
{
    Serial.print("Melody string: \"");
    Serial.print(currentMelodyString);
    Serial.println("\"");

    for (int i = 0; i < currentMelodyLength; i++)
    {
        Serial.println(
            String(i) + ") " + String(currentMelody[i].name) + " (" + String(currentMelody[i].freq) + ") " + String(currentMelody[i].length) + "\t\t(" + String(currentMelody[i].length > 0 ? tempo / currentMelody[i].length : 0) + ")");
    }
}

void BuzzPlayer::playMelody(void)
{
    for (int i = 0; i < currentMelodyLength; i++)
    {
        tone(pin, currentMelody[i].freq);
        if (currentMelody[i].length > 0)
        {
            delay(tempo / currentMelody[i].length);
        }
        noTone(pin);
    }
}

void BuzzPlayer::setTempo(int t)
{
    tempo = t;
}

bool BuzzPlayer::isPlaying(void)
{
    return (currrentBgNote != 0);
}
void BuzzPlayer::stopPlay(void)
{
    currrentBgNote = -1;
}

void BuzzPlayer::bgPlayer(BuzzPlayer *self)
{
    int currentNoteNumber = self->currrentBgNote / 2;
    int currentNotePhase = self->currrentBgNote % 2;

    if (currentNoteNumber >= self->currentMelodyLength || self->currrentBgNote < 0)
    {
        self->currrentBgNote = 0;
        return;
    }

    struct ToneFreq &currentNote = self->currentMelody[currentNoteNumber];

    self->currrentBgNote++;

    int currentNoteDelay = 0;
    if (currentNote.length > 0)
    {
        currentNoteDelay = self->tempo / currentNote.length;
    }
    if (currentNotePhase == 0)
    {
        // Start playback
        tone(self->pin, currentNote.freq);
        self->bgTicker.once_ms<BuzzPlayer *>(currentNoteDelay, bgPlayer, self);
    }
    else
    {
        // Stop playback
        noTone(self->pin);
        self->bgTicker.once_ms<BuzzPlayer *>(0, bgPlayer, self);
    }
}

void BuzzPlayer::playMelodyBg(void)
{
    currrentBgNote = 0;
    bgTicker.once_ms<BuzzPlayer *>(0, bgPlayer, this);
    delay(1); // Small delay to allow it begin
}
