
//============================================================================================//
/*
  Filename: Basic.ino
  Description: Demonstrates the usage of the CSE_ESPMutex library by sharing a counter varaible
  between two concurrent tasks. The counter is incremented and printed by the two tasks at
  different intervals.
  Framework: ESP32-Arduino, PlatformIO
  Author: Vishnu Mohanan (@vishnumaiea, @vizmohanan)
  Maintainer: CIRCUITSTATE Electronics (@circuitstate)
  Version: 0.1
  License: MIT
  Source: https://github.com/CIRCUITSTATE/CSE_ESPMutex
  Last Modified: +05:30 22:47:48 PM 23-02-2026, Monday
 */
//============================================================================================//

#include <Arduino.h>
#include "CSE_ESPMutex.h"

#define  PORT_SERIAL        Serial0 // Use UART0 for communication

//============================================================================================//

// Basic example demonstrating using CSE_ESPMutex to protect a shared
// variable and to guard a critical section (Serial prints) between
// two FreeRTOS tasks on ESP32.

CSE_ESPMutex<int> counter ("Counter", 0);

//============================================================================================//

// Task that increments the shared counter by 1.
void task_1 (void *pvParameters) {
  (void) pvParameters;
  while (1) { // Infinite loop for the task
    if (counter.lock()) { // Try to get a lock
      // Get current counter value. No need to get a lock at here since we already have that.
      int counter_local = counter.getValue (false);

      PORT_SERIAL.println ("task_1: Current Counter Value = " + String (counter_local));
      counter_local += 1; // Increment the local copy of the counter

      // Set the new counter value. No need to get a lock here again.
      counter.setValue (false, counter_local);

      PORT_SERIAL.println ("task_1: Incremented Counter Value = " + String (counter_local));
      PORT_SERIAL.println();
      counter.unlock(); // Release the lock
    }
    vTaskDelay (pdMS_TO_TICKS (200));
  }
}

//============================================================================================//

// Task that increments the shared counter by 10.
void task_2 (void *pvParameters) {
  (void) pvParameters;
  while (1) { // Infinite loop for the task
    if (counter.lock()) { // Try to get a lock
      // Get current counter value. No need to get a lock at here since we already have that.
      int counter_local = counter.getValue (false);

      PORT_SERIAL.println ("task_2: Current Counter Value = " + String (counter_local));
      counter_local += 10; // Increment the local copy of the counter

      // Set the new counter value. No need to get a lock here again.
      counter.setValue (false, counter_local);

      PORT_SERIAL.println ("task_2: Incremented Counter Value = " + String (counter_local));
      PORT_SERIAL.println();
      counter.unlock(); // Release the lock
    }
    vTaskDelay (pdMS_TO_TICKS (100));
  }
}

//============================================================================================//

void setup() {
  PORT_SERIAL.begin (115200);
  
  PORT_SERIAL.println();
  PORT_SERIAL.println ("CSE_ESPMutex Basic Example");

  // Create two tasks pinned to different cores (ESP32). If you prefer
  // the tasks to run on any core, use xTaskCreate() instead.
  xTaskCreatePinnedToCore (task_1, "task_1", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore (task_2, "task_2", 4096, NULL, 1, NULL, 1);
}

//============================================================================================//

void loop() {
  // Nothing needed here; tasks run in the background.
  vTaskDelay (pdMS_TO_TICKS (10000));
}

//============================================================================================//
