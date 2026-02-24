
# CSE_ESPMutex - API Documentation

Version 0.0.6, +05:30 08:09:27 PM 24-02-2026, Tuesday

## Index

- [CSE\_ESPMutex - API Documentation](#cse_espmutex---api-documentation)
  - [Index](#index)
  - [Templates](#templates)
  - [Classes](#classes)
  - [Class `CSE_ESPMutex`](#class-cse_espmutex)
    - [Variables](#variables)
    - [`CSE_ESPMutex()`](#cse_espmutex)
    - [`~CSE_ESPMutex()`](#cse_espmutex-1)
    - [`deinit()`](#deinit)
    - [`lock()`](#lock)
    - [`unlock()`](#unlock)
    - [`getValue()`](#getvalue)
    - [`setValue()`](#setvalue)
    - [`getName()`](#getname)
    - [`setName()`](#setname)
    - [`isValid()`](#isvalid)
    - [`isOwner()`](#isowner)

## Templates

- `T` : Type of the value variable to be protected by the mutex. It can be any data type, including primitive types (e.g., `int`, `float`, `bool`) and user-defined types (e.g., `struct`, `class`).

## Classes

- `CSE_ESPMutex` : The main class the wraps all functions and variables of this library. This is a header-only library and therefore does not have a CPP source file.

## Class `CSE_ESPMutex`

### Variables

- Private:
  - `SemaphoreHandle_t mutex` : The FreeRTOS mutex handle used to protect the shared variable.
  - `String name` : The name of the mutex variable. This is an optional variable that can be used for debugging purposes. It does not affect the functionality of the mutex.
  - `T value` : The variable protected by the mutex. This is the shared variable that can be accessed and modified by multiple tasks while ensuring mutual exclusion.
  - `bool initialized` : A flag to indicate whether the mutex has been initialized successfully. This is used to prevent operations on an uninitialized mutex.
  - `TaskHandle_t owner` : The handle of the task that currently owns the mutex. This is used to ensure that only the owning task can release the mutex and access the protected variable.

### `CSE_ESPMutex()`

This constructor creates a new `CSE_ESPMutex` object. There are two overloads for this constructor.

#### Syntax 1

```cpp
CSE_ESPMutex<T> myValue (T initialValue = T()); // Format

CSE_ESPMutex<int> myValue (0); // Example
```

##### Parameters

- `T initialValue` : The type of variable you want to create. If you want an integer, use `int`. If you want a boolean, use `bool` and so on. The values are initialized by default. But if you want to initialize the variable explicitly, you can pass the initial value as an argument.

##### Returns

- `CSE_ESPMutex` object.

#### Syntax 2

```cpp
CSE_ESPMutex<T> myValue (String name, T initialValue = T()); // Format

CSE_ESPMutex<int> myValue ("myValue", 0); // Example
```

##### Parameters

- `String name` : The name of the mutex variable. This is an optional parameter that can be used for debugging purposes. It does not affect the functionality of the mutex.
- `T initialValue` : The type of variable you want to create. If you want an integer, use `int`. If you want a boolean, use `bool` and so on. The values are initialized by default. But if you want to initialize the variable explicitly, you can pass the initial value as an argument.

##### Returns

- `CSE_ESPMutex` object.

### `~CSE_ESPMutex()`

Destroys the `CSE_ESPMutex` object with the help of `deinit()`.

#### Syntax

```cpp
myValue.~CSE_ESPMutex(); // Example
```

##### Parameters

- None

##### Returns

- None

### `deinit()`

Performs all deinitializations related to the mutex. If a mutex is not valid, the deinitialization is skipped. The function will first try to acquire a lock on the mutex before doing anything. If a lock can not be acquired within the timeout period, the operation is skipped. This is to prevent freeing a mutex when it is currently being used another task.

After a lock is acquired, the `initialized` variable is set to `false` to prevent any further operations on the mutex. The `owner` is reset to `nullptr` to release the ownership. Then the mutex is released and deleted.

#### Syntax

```cpp
myValue.deinit (uint32_t timeout = portMAX_DELAY); // Format

myValue.deinit(); // Example
```

##### Parameters

- `uint32_t timeout` : The maximum time to wait for acquiring a lock on the mutex before skipping the deinitialization. The default value is `portMAX_DELAY`, which means it will wait indefinitely until a lock is acquired.

##### Returns

- `bool` : `true` if the deinitialization was successful, `false` otherwise. The deinitialization can fail if the mutex is currently being used by another task and a lock can not be acquired within the timeout period.

### `lock()`

Acquires a lock on the mutex. If the caller already owns the mutex, then there is no need to acquire the lock again. In that case, `true` is returned immediately. If the caller does not own the mutex, it will try to acquire a lock on it. If a lock can be acquired within the timeout period, `true` is returned. Otherwise, `false` is returned. The ownership is updated when a lock is successfully acquired.

#### Syntax

```cpp
myValue.lock (uint32_t timeout = portMAX_DELAY); // Format

myValue.lock(); // Example
```

##### Parameters

- `uint32_t timeout` : The maximum time to wait for acquiring a lock on the mutex before giving up. The default value is `portMAX_DELAY`, which means it will wait indefinitely until a lock is acquired.

##### Returns

- `bool` : `true` if a lock was successfully acquired, `false` otherwise.

### `unlock()`

Releases the lock on the mutex. Only the owner of the mutex can release it. If the caller is not the owner, the operation is skipped and `false` is returned. If the caller is the owner, the lock is released, the ownership is reset to `nullptr` and `true` is returned.

#### Syntax

```cpp
myValue.unlock(); // Example
```

##### Parameters

- None

##### Returns

- `bool` : `true` if the lock was successfully released, `false` otherwise. The lock can not be released if the caller is not the owner of the mutex.

### `getValue()`

Returns the value of the variable protected by the mutex. The value is only returned if the mutex is initialized, the caller is the owner of the mutex or a lock can be acquired on it. The user can request to acquire a lock on the mutex by passing `toLock` as `true`. If the caller already owns the mutex, then there is no need to acquire the lock again and the value is returned directly. When the value can not be accessed for any reason, a default value of type `T` is returned.

#### Syntax

```cpp
myValue.getValue (bool toLock, uint32_t timeout = portMAX_DELAY); // Format

myValue.getValue (true); // Example
```

##### Parameters

- `bool toLock` : Whether to acquire a lock on the mutex before accessing the value. If `true`, the function will try to acquire a lock on the mutex. If a lock can be acquired within the timeout period, the value is returned. Otherwise, a default value of type `T` is returned. If `false`, the function will not try to acquire a lock and will only return the value if the caller is the owner of the mutex.
- `uint32_t timeout` : The maximum time to wait for acquiring a lock on the mutex before giving up. The default value is `portMAX_DELAY`, which means it will wait indefinitely until a lock is acquired.

##### Returns

- `T` : The value of the variable protected by the mutex. If the value can not be accessed for any reason, a default value of type `T` is returned.

### `setValue()`

Sets the value of the variable protected by the mutex. The value is only set if the mutex is initialized, the caller is the owner of the mutex or a lock can be acquired on it. The user can request to acquire a lock on the mutex by passing `toLock` as `true`. If the caller already owns the mutex, then there is no need to acquire the lock again and the value is set directly. When the value can not be accessed for any reason, `false` is returned.

#### Syntax

```cpp
myValue.setValue (bool toLock, T newValue, uint32_t timeout = portMAX_DELAY); // Format

myValue.setValue (true, 42); // Example
```

##### Parameters

- `bool toLock` : Whether to acquire a lock on the mutex before accessing the value. If `true`, the function will try to acquire a lock on the mutex. If a lock can be acquired within the timeout period, the value is set and `true` is returned. Otherwise, `false` is returned. If `false`, the function will not try to acquire a lock and will only set the value if the caller is the owner of the mutex.
- `T newValue` : The new value to be set for the variable protected by the mutex.
- `uint32_t timeout` : The maximum time to wait for acquiring a lock on the mutex before giving up. The default value is `portMAX_DELAY`, which means it will wait indefinitely until a lock is acquired.

##### Returns

- `bool` : `true` if the value was successfully set, `false` otherwise. The value can not be set if the caller is not the owner of the mutex or a lock can not be acquired on it within the timeout period.

### `getName()`

Returns the name of the mutex variable. If the mutex is not initialized, "NOT INITIALIZED" string is returned. The actual name is only returned if the caller is the owner of the mutex or a lock can be acquired on it.

#### Syntax

```cpp
myValue.getName(); // Example
```

##### Parameters

- None

##### Returns

- `String` : The name of the mutex variable. If the mutex is not initialized, "NOT INITIALIZED" string is returned. The actual name is only returned if the caller is the owner of the mutex or a lock can be acquired on it. Otherwise, an empty string is returned.

### `setName()`

Sets the name of the mutex variable. The name is only set if the mutex is initialized, the caller is the owner of the mutex or a lock can be acquired on it. When the name can not be set for any reason, `false` is returned.

#### Syntax

```cpp
myValue.setName (String mutexName); // Format

myValue.setName ("myValue"); // Example
```

##### Parameters

- `String mutexName` : The new name to be set for the mutex variable.

##### Returns

- `bool` : `true` if the name was successfully set, `false` otherwise. The name can not be set if the caller is not the owner of the mutex or a lock can not be acquired on it within the timeout period.

### `isValid()`

Checks if the mutex is initialized and valid.

#### Syntax

```cpp
myValue.isValid(); // Example
```

##### Parameters

- None

##### Returns

- `bool` : `true` if the mutex is initialized and valid, `false` otherwise.

### `isOwner()`

Checks if the caller is the owner of the mutex.

#### Syntax

```cpp
myValue.isOwner(); // Example
```

##### Parameters

- None

##### Returns

- `bool` : `true` if the caller is the owner of the mutex, `false` otherwise.

