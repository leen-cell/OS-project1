#Word Frequency Analyzer – OS Project

This project analyzes a large dataset to find the top 10 most frequent words using three approaches:

Naïve (sequential)

Multiprocessing (fork + pipes)

Multithreading (POSIX threads)

The goal is to compare the performance of each method and observe the impact of parallelism on execution time.

Overview

Reads all words from the dataset into a dynamic structure

Counts word frequencies using different approaches

Sorts results using merge sort

Measures execution time for comparison

Includes Amdahl’s Law analysis in the report

For full details, refer to the project report (PDF).

Building and Running
Compile
gcc main.c -o OSproject -lpthread

Run examples

Naïve:

Naive(TheWords, count);


Multiprocessing:

MultiProcessing(TheWords, count, 4);   // number of processes


Multithreading:

MultiThreading(TheWords, count, 4);    // number of threads

Repository Structure (recommended)
OSproject/
│── main.c
│── dataset.txt
│── README.md
│── OSproject1.pdf
