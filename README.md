![Language](https://img.shields.io/badge/language-C++-00599C?style=for-the-badge&logo=c%2B%2B)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/status-Active-success?style=for-the-badge)
# High-Performance Search Engine (C++)

A high-efficiency text search engine built from scratch in C++. This project demonstrates low-level memory management and advanced data structure optimization to perform sub-microsecond keyword searches over large text corpora.

## Key Features

* **Custom Memory Arena:** Implemented a pre-allocated memory pool (`Arena`) to eliminate `malloc/free` overhead and improve CPU cache locality.
* **Inverted Index (Trie):** Utilizes a custom Trie data structure for O(L) search time complexity (where L is word length), independent of document set size.
* **KMP Algorithm:** Implements the Knuth-Morris-Pratt pattern matching algorithm to generate context-aware text snippets.
* **Boolean Search:** Supports fast retrieval of documents containing specific keywords.

## Technical Stack

* **Language:** C++17
* **Build System:** CMake
* **Core Concepts:** Pointers, Memory Management, Data Structures (Trie), Algorithms (KMP).

## Performance

* **Indexing Speed:** ~0.1 seconds for standard datasets.
* **Search Latency:** < 1 microsecond (0 us) for indexed keywords.
* **Memory Efficiency:** Uses a contiguous memory block to minimize fragmentation.

## How to Run

1.  **Configure and Build:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

2.  **Run:**
    * **Windows:**
        ```bash
        Debug\engine.exe
        ```
    * **Linux/Mac:**
        ```bash
        ./engine
        ```

    *(Note: Ensure your `data` folder is in the same directory as the executable).*

## Future Improvements
* **Relevance Ranking:** Currently, the engine performs a boolean search (found/not found). I plan to implement a frequency-based scoring system to rank the most relevant documents at the top.
* **Disk Serialization:** Right now, the engine rebuilds the index every time it runs. I want to save the Trie structure to a binary file so it can load instantly without re-indexing.
* **Multithreading:** I plan to use C++ threads to process multiple files simultaneously during the indexing phase to make it faster for large datasets.
* **Fuzzy Search:** Implementing Levenshtein distance to handle typos in user queries.
