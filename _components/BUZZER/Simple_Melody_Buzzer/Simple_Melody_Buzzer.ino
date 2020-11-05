/* 
 * Play duo melody with Arduino
 * ============================
 *  
 * Poor attempt at Nabuo Uematsu's "Victory Fanfare" introduction
 * borrowing Andriy Makukha's system
 * 
 * Author: Andriy Makukha (March 2016)
 * 
 * Based on PlayMelody tutorial:
 *   https://www.arduino.cc/en/Tutorial/PlayMelody
 *   with some improvements.
 */

// GPIOs  ==========================================

/* All you have to do is to connect each of the buzzers to ground and to one of these GPIOs. 
 * You can change GPIOs as you see fit. */
#define BUZZER1 A0
#define BUZZER2 A1

// TONES  ==========================================
#include "pitches.h"
bool playTriggered = false;

// Define a special note, 'R', to represent a rest
#define REST 0
#define REST_COUNT 185

#define TEMPO 35000
#define PAUSE 1000 // length of pause between notes

// MELODY and TIMING  =======================================

// notes in the melody 1
int melody1[] = {
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_GS3, NOTE_AS3,
    NOTE_C3, NOTE_AS3, NOTE_C3};

// note durations: 1 = 1/16 note, 3 = 3/16 note, etc.:
int beats1[] = {
    1, 1, 1, 3, 3, 3,
    2, 1, 9};

// notes in the melody 2
int melody2[] = {
    NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C4, NOTE_GS4, NOTE_AS4,
    NOTE_C3, NOTE_AS3, NOTE_C2};

void setup()
{
    pinMode(BUZZER1, OUTPUT);
    pinMode(BUZZER2, OUTPUT);
}

void rest(long duration)
{
    for (int j = 0; j < REST_COUNT; j++)
    {
        delayMicroseconds(duration);
    }
}

void playTone(long tone_, long duration)
{
    byte state = LOW;
    long elapsed_time = 0;
    if (tone_ > 0)
    {
        tone_ >>= 1;
        while (elapsed_time < duration)
        {
            state = state == HIGH ? LOW : HIGH;
            digitalWrite(BUZZER1, state);
            delayMicroseconds(tone_);
            elapsed_time += tone_;
        }
    }
    else
        rest(duration);
}

void play2Tones(long tone1, long tone2, long duration)
{
    byte s1, s2; // state of the buzzers
    long sum1, sum2, cur, next, n1, n2;
    cur = next = sum1 = sum2 = 0;

    // Init buzzers
    s1 = s2 = LOW;
    digitalWrite(BUZZER1, s1);
    digitalWrite(BUZZER2, s2);

    if (tone1 > 0 && tone2 > 0)
    {
        duration -= PAUSE;
        tone1 >>= 1;
        tone2 >>= 1;
        while (cur < duration)
        {
            next = min(min(sum1 + tone1, sum2 + tone2), duration);
            delayMicroseconds(next - cur);
            if (sum1 + tone1 == next)
            {
                s1 = s1 == HIGH ? LOW : HIGH;
                digitalWrite(BUZZER1, s1);
                sum1 += tone1;
            }
            if (sum2 + tone2 == next)
            {
                s2 = s2 == HIGH ? LOW : HIGH;
                digitalWrite(BUZZER2, s2);
                sum2 += tone2;
            }
            cur = next;
        }
        delayMicroseconds(PAUSE);
    }
    else if (tone1 > 0 || tone2 > 0)
    {
        // TODO: choose appropriate buzzer
        playTone(tone1 > 0 ? tone1 : tone2, duration);
    }
    else
        rest(duration);
}

void loop()
{
    if (playTriggered)
    {
        if (sizeof(melody1) != sizeof(melody2) || sizeof(melody1) != sizeof(beats1))
        {
            playTone(NOTE_E4, 32 * TEMPO);
            playTone(REST, 16 * TEMPO);
            playTone(NOTE_E4, 16 * TEMPO);
            return;
        }

        int n = sizeof(beats1) / 2;
        for (int i = 0; i < n; i++)
        {
            play2Tones(melody1[i], melody2[i], beats1[i] * 4 * TEMPO);
        }
        playTriggered = false;
    }

    if (digitalRead(12) == 1)
    {
        playTriggered = true;
    }
}
