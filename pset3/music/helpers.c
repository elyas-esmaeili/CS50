// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int numerator = fraction[0] - '0';
    int denumerator = fraction[2] - '0';

    // Duration equals to (X / Y) / (1 / 8 ) = (X * 8) / (Y * 1)
    int dur = (numerator * 8) / denumerator;
    return dur;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int octave;
    char noteLetter, accidental;

    // Set accidental to a trash value
    accidental = '0';
    noteLetter = note[0];
    if ((note[1] == '#') || (note[1] == 'b'))
    {
        accidental = note[1];
        // convert digit char to int
        octave = note[2] - '0';
    }
    else
    {
        octave = note[1] - '0';
    }

    // There are 12 semitones in each octave
    // Base octave is 4
    int n = (octave - 4) * 12;

    if (noteLetter >= 'C' && noteLetter <= 'E')
    {
        // There are 9 semitones between note A and note C
        n += (((noteLetter - 'C') * 2) - 9);
    }
    else if (noteLetter == 'F' || noteLetter == 'G')
    {
        // Because there isn't a black key between E and F
        // have to replace 9 with 10 (9 + 1)
        n += ((noteLetter - 'C') * 2 - 10);
    }
    else
    {
        // this block is for A or B notes
        n += ((noteLetter - 'A') * 2);
    }

    // Check if accidental exist then plus
    // or minus the number of semitones
    if (accidental == '#')
    {
        n++;
    }
    else if (accidental == 'b')
    {
        n--;
    }

    // frequency = 440 (A4 frequency) * 2 ^ (n / 12)
    // which n equals to number of semitones
    float freq = 440 * pow(2, (float)n / 12.0);
    int ret = round(freq);

    return ret;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    //Inputs only a Line ending, get-string() return ""
    //if str equals to "" then strlen(str) equals to ZERO
    if (!strlen(s))
    {
        return true;
    }
    return false;
}
