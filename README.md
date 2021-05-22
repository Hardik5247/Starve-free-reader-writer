# Starvation-free reader writer problem

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This repo contains working code for starvation free readers writers problem (3rd reader writers problem) implemented for assignment of **CSN-232: Operating Systems** and is submitted by **Hardik Thami, 19114035**

## Basics

### First Readers-Writers Problem
The first reader writer problem involves giving priority to readers. In other words, no reader shall be kept waiting if the share is currently opened for reading. 
**Thus, potentially starving writers**. 

### Second Readers-Writers Problem
The second reader writer problem involved giving priority to readers. In other words, no writer, once added to the queue, shall be kept waiting longer than absolutely necessary. **Thus, potentially starving readers**. This problem acknowledges presence of multiple writers.

### Third Readers-Writers Problem (Starve Free Solution)
The third reader writer problem involves giving neither priority to readers nor writers. 
**No thread shall be allowed to starve**

## Use

### Linux or C solution
`g++ StarveFreeReaderWriter.c -lpthread -o algo_output && ./algo_output && rm algo_output`
