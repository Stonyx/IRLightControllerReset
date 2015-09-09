// IR Light Controller Reset
// Copyright (C) 2015 Stonyx
//
// This software is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License.
//
// You can redistribute and/or modify this software for non-commerical purposes under the terms
// of the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
//
// This software is provided "as is" without express or implied warranty.

// Debug related definitions
#define DEBUG
#ifdef DEBUG
  #define DEBUG_SERIAL_BEGIN() Serial.begin(9600)
  #define DEBUG_LOG(string) Serial.print(string)
  #define DEBUG_LOG_LN(string) Serial.println(string)
#else
  #define DEBUG_SERIAL_BEGIN()
  #define DEBUG_LOG(string)
  #define DEBUG_LOG_LN(string)
#endif
#define DEBUG_LOG_FREE_RAM() DEBUG_LOG(F("Free RAM: ")); DEBUG_LOG_LN(FreeRam())

// Include header files
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

// Define counts
#define COLOR_VALUES_COUNT 5
#define MEMORY_SCHEDULE_COUNT 50 // Must be multiples of 2
#define TIMER_SCHEDULE_COUNT 50 // Must be multiples of 5

// Define EEPROM locations
#define ADDRESSES_LOCATION_BEGIN 0
#define ADDRESSES_LOCATION_END (ADDRESSES_LOCATION_BEGIN + 6 + 4 * 4) // Six byte MAC address plus four 4 byte IP addresses
#define TIME_ZONE_LOCATION_BEGIN ADDRESSES_LOCATION_END
#define TIME_ZONE_LOCATION_END (TIME_ZONE_LOCATION_BEGIN + sizeof(TimeZone))
#define COLOR_VALUES_LOCATION_BEGIN TIME_ZONE_LOCATION_END
#define COLOR_VALUES_LOCATION_END (COLOR_VALUES_LOCATION_BEGIN + sizeof(ColorValues) * COLOR_VALUES_COUNT)
#define MEMORY_SCHEDULE_LOCATION_BEGIN COLOR_VALUES_LOCATION_END
#define MEMORY_SCHEDULE_LOCATION_END (MEMORY_SCHEDULE_LOCATION_BEGIN + sizeof(MemorySchedule) * MEMORY_SCHEDULE_COUNT)
#define TIMER_SCHEDULE_LOCATION_BEGIN MEMORY_SCHEDULE_LOCATION_END
#define TIMER_SCHEDULE_LOCATION_END (TIMER_SCHEDULE_LOCATION_BEGIN + sizeof(TimerSchedule) * TIMER_SCHEDULE_COUNT)

// Define Structures
struct TimeZone
{
  signed short offset; // In minutes
  byte dstStartMonth; // 0: Janary ... 11: December
  byte dstStartWeekday; // 0: Sunday ... 6: Saturday
  byte dstStartWeekdayNumber; // 0: first, 1: second, 2: third, 3: fourth, 4: last
  unsigned short dstStartMinutes; // After midnight
  byte padding; // Inserted by the compiler but added explicitly to make sure this doesn't change unexpectedly with a newer compiler version
  signed short dstOffset; // In minutes
  byte dstEndMonth; // 0: Janary ... 11: December
  byte dstEndWeekday; // 0: Sunday ... 6: Saturday
  byte dstEndWeekdayNumber; // 0: first, 1: second, 2: third, 3: fourth, 4: last
  unsigned short dstEndMinutes; // After midnight
  byte morePadding; // Inserted by the compiler but added excplicitly to make sure this doesn't change unexpectedly with a newer compiler version
};
struct ColorValues
{
  byte red;
  byte green;
  byte blue;
  byte white;
};
struct MemorySchedule
{
  byte button;
  byte weekday;
  unsigned long timeSinceMidnight;
  unsigned long duration;
};
struct TimerSchedule
{
  byte button;
  byte weekday;
  unsigned long timeSinceMidnight;
};

// Setup code
void setup()
{
  // Initialize the serial communication for debugging
  DEBUG_SERIAL_BEGIN();
  DEBUG_LOG_LN(F("Starting IR Light Controller sketch ..."));
  DEBUG_LOG_FREE_RAM();
  
  // Log details
  DEBUG_LOG("Resetting saved settings ...");

  // Reset all saved settings
  byte defaultSettings[] = { 0xDE, 0x12, 0x34, 0x56, 0x78, 0x90, 192, 168, 1, 254, 192, 168, 1, 1, 192, 168, 1, 1, 255, 255, 255, 0,
          0xD4, 0xFE, 3, 0, 2, 0x78, 0x00, 0, 0x3C, 0x00, 11, 0, 1, 0x78, 0x00, 0 };
  for (unsigned short i = ADDRESSES_LOCATION_BEGIN; i < TIME_ZONE_LOCATION_END; ++i)
  {
    EEPROM.update(i, defaultSettings[i - ADDRESSES_LOCATION_BEGIN]);
  }
  for (unsigned short i = COLOR_VALUES_LOCATION_BEGIN; i < TIMER_SCHEDULE_LOCATION_END; ++i)
  {
    EEPROM.update(i, 0);
  }
  
  // Log details
  DEBUG_LOG_LN(" done");
  
  // Log free memory
  DEBUG_LOG_FREE_RAM();
}

// Main code
void loop()
{
}
