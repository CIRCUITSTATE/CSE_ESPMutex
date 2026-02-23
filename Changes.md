
# Changes

#
### **+05:30 12:15:54 AM 24-02-2026, Tuesday**

  - Removed the critical section from the `unlock()` method.
    - It requires another mutex to work.
  - New Version 🆕 `0.0.4`.

#
### **+05:30 11:33:50 PM 23-02-2026, Monday**

  - Improved the logic of `getValue()` and `setValue()`.
    - A value is only written or read if,
      - The mutex is initialized.
      - & The caller is the owner of the mutex.
      - & A lock is acquired as per request before accessing the value variable.
    - Uses the `lock()` and `unlock()` methods now instead of direct API calls.
  - Improved the `lock()`.
    - If the caller already owns the mutex, then there is no need to acquire the lock again.
    - `true` is returned if that's the case.
  - Improved the `unlock()`.
    - Added `bool` return type to indicate success/failure.
    - Critical section is used to prevent context switching when releasing the mutex and setting the `owner` to `nullptr`.
  - New Version 🆕 `0.0.3`.

#
### **+05:30 10:48:40 PM 23-02-2026, Monday**

  - Added `initialized` variable to track the initialization status of the mutex.
    - Mutex is checked for NULL before running any operations on it.
  - Added `owner` to track the owner task/thread of the mutex.
    - Only the owner handle matches the caller handle, the mutex can be released by the caller.
    - Owner handle is saved when a lock is acquired.
  - Deleted the copy constructor and assignment operator to prevent copying of the mutex object.
  - `localValue` is now initialized in `getValue()`.
  - `getName()` and `setName()` now check,
    - If the mutex is initialized.
    - If the caller is the owner of the mutex.
    - Acquire a lock before accessing the name variable.
  - Added `isOwner()` method to check if the caller is the owner of the mutex.
  - Added `isValid()` method to check if the mutex is initialized.
  - Updated Readme.
  - Tested and working.
  - New Version 🆕 `0.0.2`.

#
### **+05:30 08:38:52 PM 22-02-2026, Sunday**

  - Added `Basic.ino` example.
  - Updated Readme.
  - Updated library specification files.
  - Initial commit.
  - New Version 🆕 `0.0.1`.
