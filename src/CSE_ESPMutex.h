
//============================================================================================//
/*
  Filename: CSE_ESPMutex.h
  Description: Header file for CSE_ESPMutex library.
  Allows you to create mutexes for ESP32 microcontroller with ease.
  Framework: ESP32-Arduino, PlatformIO
  Author: Vishnu Mohanan (@vishnumaiea, @vizmohanan)
  Maintainer: CIRCUITSTATE Electronics (@circuitstate)
  Version: 0.0.1
  License: MIT
  Source: https://github.com/CIRCUITSTATE/CSE_ESPMutex
  Last Modified: +05:30 20:52:21 PM 22-02-2026, Sunday
 */
//============================================================================================//

#ifndef CSE_ESPMUTEX_H
#define CSE_ESPMUTEX_H

#include <Arduino.h>

//============================================================================================//

template <typename T>
class CSE_ESPMutex {
  private:
    SemaphoreHandle_t mutex;
    String name = "";
    T value;

  public:
    // Constructor with optional initial value
    CSE_ESPMutex (T initialValue = T()) : value (initialValue) {
      mutex = xSemaphoreCreateMutex();
    }

    //--------------------------------------------------------------------------------------------//

    // Constructor with name and optional initial value
    CSE_ESPMutex (String mutexName, T initialValue = T()) : name (mutexName), value (initialValue) {
      mutex = xSemaphoreCreateMutex();
    }

    //--------------------------------------------------------------------------------------------//

    // Destructor to clean up the mutex
    ~CSE_ESPMutex() {
      if (mutex != NULL) {
        vSemaphoreDelete (mutex);
      }
    }

    //--------------------------------------------------------------------------------------------//

    // Get a manual lock on the mutex. This is useful when you want to perform multiple operations
    // on the shared variable atomically. Remember to call `unlock()` after you're done.
    bool lock (uint32_t timeout = portMAX_DELAY) {
      return xSemaphoreTake (mutex, timeout) == pdTRUE;
    }

    //--------------------------------------------------------------------------------------------//

    // Release the already acquired lock on the mutex.
    void unlock() {
      xSemaphoreGive (mutex);
    }

    //--------------------------------------------------------------------------------------------//

    // Get the value of the the protected variable. If `toLock` is true, it will attempt to lock the
    // mutex before getting the value. If you already have a lock on the mutex, you can set `toLock`
    // to false to get the value without locking.
    T getValue (bool toLock, uint32_t timeout = portMAX_DELAY) {
      T localValue;
      if (toLock) {
        if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
          localValue = value;
          xSemaphoreGive (mutex);
        }
      }
      else {
        localValue = value;
      }
      return localValue;
    }

    //--------------------------------------------------------------------------------------------//

    // Set value of the protected variable. If `toLock` is true, it will attempt to lock the mutex
    // before setting the value. If you already have a lock on the mutex, you can set `toLock` to
    // false to set the value directly.
    bool setValue (bool toLock,T newValue, uint32_t timeout = portMAX_DELAY) {
      if (toLock) {
        if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
          value = newValue;
          xSemaphoreGive (mutex);
          return true;
        }
      }
      else {
        value = newValue;
        return true;
      }
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Get the name of the mutex.
    String getName() {
      return name;
    }

    //--------------------------------------------------------------------------------------------//

    // Set the name of the mutex.
    void setName (String mutexName) {
      name = mutexName;
    }
};

//============================================================================================//


#endif // CSE_ESPMUTEX_H