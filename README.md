
# Word Frequency Analyzer – OS Project

This project analyzes a large dataset to identify the top 10 most frequent words using three different approaches:

* Naïve (sequential)
* Multiprocessing (fork + pipes)
* Multithreading (POSIX threads)

The purpose is to compare their performance and understand how parallelism affects execution time.

---

## Overview

* Reads all words dynamically from a dataset
* Counts word frequencies using three approaches
* Sorts results using merge sort
* Measures and compares execution time
* Includes Amdahl’s Law analysis (see report)

For full details and analysis, refer to the project report (PDF).

---

## Building and Running

### Compile

```bash
gcc main.c -o OSproject -lpthread
```

### Naïve version

```c
Naive(TheWords, count);
```

### Multiprocessing version

```c
MultiProcessing(TheWords, count, 4);   // number of processes
```

### Multithreading version

```c
MultiThreading(TheWords, count, 4);    // number of threads
```

---

## Recommended Repository Structure

```
OSproject/
│── main.c
│── dataset.txt
│── README.md
│── OSproject1.pdf
```

---


