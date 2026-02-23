//============================================================================================//
/*
  Filename: CSE_ESPMutex.h
  Description: Header file for CSE_ESPMutex library.
  Allows you to create mutexes for ESP32 microcontroller with ease.
  Framework: ESP32-Arduino, PlatformIO
  Author: Vishnu Mohanan (@vishnumaiea, @vizmohanan)
  Maintainer: CIRCUITSTATE Electronics (@circuitstate)
  Version: 0.0.2
  License: MIT
  Source: https://github.com/CIRCUITSTATE/CSE_ESPMutex
  Last Modified: +05:30 22:55:52 PM 23-02-2026, Monday
 */
//============================================================================================//

#ifndef CSE_ESPMUTEX_H
#define CSE_ESPMUTEX_H

#include <Arduino.h>

//============================================================================================//

template <typename T>
class CSE_ESPMutex {
  private:
    SemaphoreHandle_t mutex; // The mutex handle
    String name = ""; // Name of the mutex
    T value; // The variable that the mutex is protecting. You can set and get this variable using the provided methods.
    bool initialized = false; // Mutex is initialized or not
    TaskHandle_t owner = nullptr; // FOr tracking the owner task/thread of the mutex

  public:
    // Constructor with optional initial value.
    CSE_ESPMutex (T initialValue = T()) : value (initialValue) {
      mutex = xSemaphoreCreateMutex();
      initialized = (mutex != NULL); // Check if the mutex was created successfully
    }

    //--------------------------------------------------------------------------------------------//

    // Constructor with name and optional initial value.
    CSE_ESPMutex (String mutexName, T initialValue = T()) : name (mutexName), value (initialValue) {
      mutex = xSemaphoreCreateMutex();
      initialized = (mutex != NULL); // Check if the mutex was created successfully
    }

    // Disable copying. This prevents any accidental copying of the mutex which can lead to issues
    // with multiple instances trying to manage the same mutex.
    CSE_ESPMutex (const CSE_ESPMutex&) = delete;
    CSE_ESPMutex& operator= (const CSE_ESPMutex&) = delete;

    //--------------------------------------------------------------------------------------------//

    // Destructor to clean up the mutex.
    ~CSE_ESPMutex() {
      if (mutex != NULL) {
        vSemaphoreDelete (mutex);
      }
    }

    //--------------------------------------------------------------------------------------------//

    // Get a manual lock on the mutex. This is useful when you want to perform multiple operations
    // on the shared variable atomically. Remember to call `unlock()` after you're done.
    bool lock (uint32_t timeout = portMAX_DELAY) {
      if (!initialized) return false;

      if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
        owner = xTaskGetCurrentTaskHandle(); // Update the owner
        return true;
      }
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Release the already acquired lock on the mutex.
    void unlock() {
      if (!initialized) return;
      // Only the owner should release the mutex.
      if (owner != xTaskGetCurrentTaskHandle()) return;
      owner = nullptr;
      xSemaphoreGive (mutex);
    }

    //--------------------------------------------------------------------------------------------//

    // Get the value of the the protected variable. If `toLock` is true, it will attempt to lock the
    // mutex before getting the value. If you already have a lock on the mutex, you can set `toLock`
    // to `false` to get the value without locking.
    T getValue (bool toLock, uint32_t timeout = portMAX_DELAY) {
      T localValue = T();
      
      if (!initialized && toLock) return localValue;

      if (toLock) { // Try to get a lock before accessing the value
        if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
          localValue = value; // Copy the value to a local variable while we have the lock
          xSemaphoreGive (mutex); // Release the lock immediately after copying the value
        }
      }
      else {
        localValue = value; // If no need to lock, just return the value directly.
      }
      return localValue;
    }

    //--------------------------------------------------------------------------------------------//

    // Set value of the protected variable. If `toLock` is true, it will attempt to lock the mutex
    // before setting the value. If you already have a lock on the mutex, you can set `toLock` to
    // `false` to set the value directly.
    bool setValue (bool toLock,T newValue, uint32_t timeout = portMAX_DELAY) {
      if (!initialized && toLock) return false;

      if (toLock) { // Try to get a lock before accessing the value
        if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
          value = newValue; // Set the new value while we have the lock
          xSemaphoreGive (mutex); // Release the lock immediately after setting the value
          return true;
        }
      }
      else {
        value = newValue; // If no need to lock, just set the value directly.
        return true;
      }
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Get the name of the mutex.
    String getName() {
      if (!initialized) return name;
      
      if (owner == xTaskGetCurrentTaskHandle()) return name; // Already owned by the caller

      String localName;

      if (xSemaphoreTake (mutex, portMAX_DELAY) == pdTRUE) {
        localName = name;
        xSemaphoreGive (mutex);
      }
      return localName;
    }

    //--------------------------------------------------------------------------------------------//

    // Set the name of the mutex.
    bool setName (String mutexName) {
      if (!initialized) return false;

      // Acquire a lock before setting the name.
      if (xSemaphoreTake (mutex, portMAX_DELAY) == pdTRUE) {
        name = mutexName;
        xSemaphoreGive (mutex); // Release the lock immediately after setting the name
        return true;
      }
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Check if the mutex is initialized.
    bool isValid() const { return initialized; }

    //--------------------------------------------------------------------------------------------//

    // Return true if the calling task currently owns the mutex
    bool isOwner() const {
      return owner == xTaskGetCurrentTaskHandle();
    }
};

//============================================================================================//

#endif // CSE_ESPMUTEX_H