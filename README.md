# Custom Memory Allocator in C

This project implements a simple custom memory allocator in C that simulates how dynamic memory management works under the hood. It provides basic functionality similar to `malloc` and `free` and tracks metadata about allocated and freed memory blocks.

> 🎥 **Reference Video**: This implementation is based on [this YouTube tutorial](https://www.youtube.com/watch?v=sZ8GJ1TiMdk&t=166s), which explains how memory allocators work at a low level.

This project is a simple custom heap memory allocator written in C. It simulates how memory allocation and deallocation work internally using metadata structures to manage allocated and freed memory chunks. The allocator supports operations like `heap_alloc`, `heap_free`, merging of adjacent free blocks, and tracking all allocations.

## Features

- Manual memory management simulation
- Allocates and frees memory chunks of custom sizes
- Tracks allocated and freed memory using chunk metadata
- Merges adjacent freed chunks to reduce fragmentation
- Supports inspection of current heap state (allocated and freed lists)

## Project Structure
.
├── alloc.c # Main source file with allocator implementation 
├── Makefile # Makefile to compile the project
└── README.md # Project documentation (this file)

## TODO
1. Re-merge the fragmented freed memory chunks without overriding allocated memory
2. Change allocator function from linear allocator to free-list based allocator
