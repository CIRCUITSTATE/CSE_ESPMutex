//============================================================================================//
/*
  Filename: CSE_ESPMutex.h
  Description: Header file for CSE_ESPMutex library.
  Allows you to create mutexes for ESP32 microcontroller with ease.
  Framework: ESP32-Arduino, PlatformIO
  Author: Vishnu Mohanan (@vishnumaiea, @vizmohanan)
  Maintainer: CIRCUITSTATE Electronics (@circuitstate)
  Version: 0.0.6
  License: MIT
  Source: https://github.com/CIRCUITSTATE/CSE_ESPMutex
  Last Modified: +05:30 01:01:09 AM 24-02-2026, Tuesday
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
      // Try a non-blocking cleanup; if it fails, do nothing to avoid blocking in dtor.
      deinit (0);
    }

    //--------------------------------------------------------------------------------------------//

    // Attempt to cleanly destroy the mutex. Caller MUST ensure no other tasks will
    // touch this object after calling deinit(). Returns true on success.
    bool deinit (uint32_t timeout = portMAX_DELAY) {
      if (!initialized) return true; // Already deinitialized

      // Try to acquire mutex to ensure exclusive access.
      if (!lock (timeout)) return false; // Couldn't get exclusive access

      // We now own the mutex; mark as uninitialized so other callers won't proceed.
      initialized = false;
      owner = nullptr;

      // Release the semaphore handle and delete it.
      xSemaphoreGive (mutex); // Release before delete (we held it)
      vSemaphoreDelete (mutex);
      mutex = NULL;
      return true;
    }

    //--------------------------------------------------------------------------------------------//

    // Get a manual lock on the mutex. This is useful when you want to perform multiple operations
    // on the shared variable atomically. Remember to call `unlock()` after you're done.
    bool lock (uint32_t timeout = portMAX_DELAY) {
      if (!initialized) return false;

      if (owner == xTaskGetCurrentTaskHandle()) {
        return true; // Already owned by the caller
      }

      if (xSemaphoreTake (mutex, timeout) == pdTRUE) {
        owner = xTaskGetCurrentTaskHandle(); // Update the owner
        return true;
      }
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Release the already acquired lock on the mutex.
    bool unlock() {
      if (!initialized) return false;
      
      // Only the owner should release the mutex.
      if (owner == xTaskGetCurrentTaskHandle()) {
        owner = nullptr; // Clear the owner after releasing the mutex
        xSemaphoreGive (mutex);
        return true;
      }
      
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Get the value of the the protected variable. If `toLock` is true, it will attempt to lock the
    // mutex before getting the value. If you already have a lock on the mutex, you can set `toLock`
    // to `false` to get the value without locking.
    T getValue (bool toLock, uint32_t timeout = portMAX_DELAY) {
      T localValue = T();
      
      if (!initialized) return localValue; // Return if the mutex is not initialized

      // If the task already owns the mutex, we don't need to lock it again.
      if (owner == xTaskGetCurrentTaskHandle()) {
        localValue = value; // Copy the value directly
      }
      else if (toLock) { // If we need to a lock before accessing the value
        if (lock (timeout)) { // Acquire the lock
          localValue = value; // Get the value while we have the lock
          unlock(); // Release the lock immediately after getting the value
        }
      }
      
      return localValue;
    }

    //--------------------------------------------------------------------------------------------//

    // Set value of the protected variable. If `toLock` is true, it will attempt to lock the mutex
    // before setting the value. If you already have a lock on the mutex, then the value is set
    // directly without trying to acquire the lock again. In that case, value of `toLock`
    // has no effect.
    bool setValue (bool toLock, T newValue, uint32_t timeout = portMAX_DELAY) {
      if (!initialized) return false; // Return if the mutex is not initialized

      // If the task already owns the mutex, we don't need to lock it again.
      if (owner == xTaskGetCurrentTaskHandle()) {
        value = newValue; // Set the value directly
        return true;
      }
      else if (toLock) { // Try to get a lock before accessing the value
        if (lock (timeout)) { // Acquire the lock
          value = newValue; // Set the new value while we have the lock
          unlock(); // Release the lock immediately after setting the value
          return true;
        }
      }
      
      return false;
    }

    //--------------------------------------------------------------------------------------------//

    // Get the name of the mutex.
    String getName() {
      if (!initialized) return String ("NOT INITIALIZED"); // Return if the mutex is not initialized
      
      if (owner == xTaskGetCurrentTaskHandle()) return name; // Already owned by the caller

      String localName;

      if (lock (portMAX_DELAY)) { // Acquire a lock before getting the name
        localName = name;
        unlock(); // Release the lock immediately after getting the name
      }
      return localName;
    }

    //--------------------------------------------------------------------------------------------//

    // Set the name of the mutex.
    bool setName (String mutexName) {
      if (!initialized) return false;

      if (owner == xTaskGetCurrentTaskHandle()) { // Already owned by the caller
        name = mutexName;
        return true;
      }

      // Acquire a lock before setting the name.
      if (lock (portMAX_DELAY)) {
        name = mutexName;
        unlock(); // Release the lock immediately after setting the name
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