// *****************************************************************************
// ***************************    C Source Code     ****************************
// *****************************************************************************
//   DESIGNER NAME:  Bruce Link
//
//         VERSION:  1.0
//
//       FILE NAME:  demo_melodies.h
//
//-----------------------------------------------------------------------------
// DESCRIPTION
//    This header defines several demo melodies as arrays of musical notes with
//    their corresponding frequencies (in Hz) and durations (in milliseconds).
//    Each melody is represented as an array of `song_struct_t` structures,
//    which include the note name, frequency, and duration.
//
//    Included melodies are:
//      - Imperial March
//      - Mario Theme
//      - Fur Elise (simplified intro)
//
//    The note frequencies used are defined in "pitches.h", based on equal
//    temperament tuning with A4 = 440 Hz.
//
//    These melodies are intended for use with tone-generating functions on
//    microcontrollers and passive buzzers or speakers.
//
//-----------------------------------------------------------------------------
// DISCLAIMER
//    This code was developed for educational purposes as part of the CSC202 
//    course and is provided "as is" without warranties of any kind, whether 
//    express, implied, or statutory.
//
//    The author and organization do not warrant the accuracy, completeness, or
//    reliability of the code. The author and organization shall not be liable
//    for any direct, indirect, incidental, special, exemplary, or consequential
//    damages arising out of the use of or inability to use the code, even if
//    advised of the possibility of such damages.
//
//    Use of this code is at your own risk, and it is recommended to validate
//    and adapt the code for your specific application and hardware 
//    requirements.
//
// Copyright (c) 2025 by Bruce Link
//    You may use, edit, run or distribute this file as long as the above
//    copyright notice remains
// *****************************************************************************
//******************************************************************************

#ifndef __DEMO_MELODIES_H__
#define __DEMO_MELODIES_H__

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdint.h>
#include "pitches.h"

typedef struct 
{
    const char *note;         // Note name as string
    uint16_t freq;            // Frequency in Hz (REST = 0)
    uint16_t duration;        // Duration in milliseconds
} song_struct_t;

#define IMPERIAL_MARCH_LENGTH 36

const song_struct_t imperial_march[IMPERIAL_MARCH_LENGTH] = 
{
    {"A4",  NOTE_A4, 500}, {"A4",  NOTE_A4, 500}, {"A4",  NOTE_A4, 500},
    {"F4",  NOTE_F4, 350}, {"C5",  NOTE_C5, 150}, {"A4",  NOTE_A4, 500},
    {"F4",  NOTE_F4, 350}, {"C5",  NOTE_C5, 150}, {"A4",  NOTE_A4, 650},

    {"E5",  NOTE_E5, 500}, {"E5",  NOTE_E5, 500}, {"E5",  NOTE_E5, 500},
    {"F5",  NOTE_F5, 350}, {"C5",  NOTE_C5, 150}, {"G#4", NOTE_GS4, 500},
    {"F4",  NOTE_F4, 350}, {"C5",  NOTE_C5, 150}, {"A4",  NOTE_A4, 650},

    {"A5",  NOTE_A5, 375}, {"A4",  NOTE_A4, 375}, {"A4",  NOTE_A4, 375},
    {"A5",  NOTE_A5, 375}, {"G#5", NOTE_GS5, 250}, {"G5",  NOTE_G5, 250},
    {"F#5", NOTE_FS5, 500}, {"F#5", NOTE_FS5, 500}, {"R",  REST,     1500},

    {"A#4", NOTE_AS4, 250}, {"D#5", NOTE_DS5, 500}, {"A4",  NOTE_A4, 500},
    {"F#5", NOTE_FS5, 375}, {"D5",  NOTE_D5, 125}, {"D#5", NOTE_DS5, 125},
    {"D5",  NOTE_D5, 125}, {"C#5", NOTE_CS5, 250}, {"C5",  NOTE_C5, 250}
};


#define MARIO_LENGTH 37

const song_struct_t mario[MARIO_LENGTH] = 
{
  {"E5", NOTE_E5, 125}, {"E5", NOTE_E5, 125}, {"R", REST, 125}, 
  {"E5", NOTE_E5, 125}, {"R", REST, 125}, {"C5", NOTE_C5, 125},
  {"E5", NOTE_E5, 125}, {"R", REST, 125}, {"G5", NOTE_G5, 375},
  {"R", REST, 375}, {"G4", NOTE_G4, 375},

  {"C5", NOTE_C5, 125}, {"R", REST, 250}, {"G4", NOTE_G4, 250},
  {"E4", NOTE_E4, 250}, {"A4", NOTE_A4, 375}, {"B4", NOTE_B4, 375},
  {"A#4", NOTE_AS4, 125}, {"A4", NOTE_A4, 250},

  {"G4", NOTE_G4, 125}, {"E5", NOTE_E5, 375}, {"G5", NOTE_G5, 250},
  {"A5", NOTE_A5, 375}, {"F5", NOTE_F5, 125}, {"G5", NOTE_G5, 250},

  {"R", REST, 250}, {"E5", NOTE_E5, 125}, {"C5", NOTE_C5, 250},
  {"D5", NOTE_D5, 125}, {"B4", NOTE_B4, 250}
};

// Fur Elise (simplified intro)
#define FUR_ELISE_LENGTH 31

const song_struct_t fur_elise[FUR_ELISE_LENGTH] = {
    {"E5", NOTE_E5, 250}, {"D#5", NOTE_DS5, 250}, {"E5", NOTE_E5, 250},
    {"D#5", NOTE_DS5, 250}, {"E5", NOTE_E5, 250}, {"B4", NOTE_B4, 250},
    {"D5", NOTE_D5, 250}, {"C5", NOTE_C5, 250},

    {"A4", NOTE_A4, 500}, {"R", REST, 250},

    {"C4", NOTE_C4, 250}, {"E4", NOTE_E4, 250}, {"A4", NOTE_A4, 250},
    {"B4", NOTE_B4, 500}, {"R", REST, 250},

    {"E4", NOTE_E4, 250}, {"G#4", NOTE_GS4, 250}, {"B4", NOTE_B4, 250},
    {"C5", NOTE_C5, 500}, {"R", REST, 250},

    {"E4", NOTE_E4, 250}, {"E5", NOTE_E5, 250}, {"D#5", NOTE_DS5, 250},
    {"E5", NOTE_E5, 250}, {"D#5", NOTE_DS5, 250}, {"E5", NOTE_E5, 250},
    {"B4", NOTE_B4, 250}, {"D5", NOTE_D5, 250}, {"C5", NOTE_C5, 250},

    {"A4", NOTE_A4, 500}
};


#endif // __DEMO_MELODIES_H__
