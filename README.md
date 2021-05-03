# Starvation-free reader writer problem

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This repo contains pseudocode for starvation free readers writers problem implemented for assignment of **CSN-232: Operating Systems** and is submitted by **Hardik Thami, 19114035**

## Documentation

- Global Variables:

```
  semaphore accessMutex;        // Initialized to 1
  semaphore readersMutex;       // Initialized to 1
  semaphore orderMutex;         // Initialized to 1

  unsigned int readers = 0;     // Number of readers accessing the resource
```

- Readers algorithm pseudocode:

```
  Wait(orderMutex);           // Remember our order of arrival

  Wait(readersMutex);         // We will manipulate the readers counter
  if (readers == 0)           // If there are currently no readers (we came first)...
    Wait(accessMutex);        // ...requests exclusive access to the resource for readers
  readers++;                  // Note that there is now one more reader
  
  Signal(orderMutex);         // Release order of arrival semaphore (we have been served)
  Signal(readersMutex);       // We are done accessing the number of readers for now

  ReadResource();             // Here the reader can read the resource at will

  Wait(readersMutex);         // We will manipulate the readers counter
  readers--;                  // We are leaving, there is one less reader
  if (readers == 0)           // If there are no more readers currently reading...
    Signal(accessMutex);      // ...release exclusive access to the resource
  Signal(readersMutex);       // We are done accessing the number of readers for now
```
- Writers algorithm pseudocode:

```
  Wait(orderMutex);           // Remember our order of arrival
  Wait(accessMutex);          // Request exclusive access to the resource
  Signal(orderMutex);         // Release order of arrival semaphore (we have been served)

  WriteResource();            // Here the writer can modify the resource at will

  Signal(accessMutex);        // Release exclusive access to the resource
```
