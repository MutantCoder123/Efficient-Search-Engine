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

## Demo & Performance

Here is an actual runtime capture of the engine indexing the dataset and performing real-time searches.

**Performance Highlights:**
* **Indexing Time:** ~78ms for the full dataset
* **Memory Usage:** ~2.8 MB (using custom Arena allocator)
* **Search Latency:** 0 µs (Microseconds)

```text
--- SEARCH ENGINE v1 ---
[SYS] Arena Ready: 50 MB
[SYS] Reading files...
[SYS] Indexing done in 78751 us
[SYS] Ram used: 2820240 bytes

Enter query (exit to quit): software
Found in 2 docs (0 us):
 -> [Doc 1] data/AI_taking_works.txt
    Snippet: ...nd professional work is done. Software powered by AI can analyze large amounts of data, ...
 -> [Doc 3] data/linux_guide.txt
    Snippet: ...another key feature of Linux. Software is installed using package managers that automati...

Enter query (exit to quit): call
Found in 1 docs (0 us):
 -> [Doc 5] data/sherlock.txt
    Snippet: ...or address.
       "There will call upon you to-night, at a quarter to eight o'clock,...
```

## 📂 Project Structure

```text
├── data/          # Text datasets for searching
├── arena.h        # Custom Memory Arena (Pool Allocator) for O(1) allocations
├── kmp.h          # Knuth-Morris-Pratt Algorithm for substring matching
├── trie.h         # Trie data structure for efficient prefix indexing
├── main.cpp       # Entry point, REPL loop, and file loading logic
└── CMakeLists.txt # Build configuration
```

## Build & Run

### Prerequisites
Before running, ensure you have the following installed:
* **C++ Compiler:** GCC, Clang, or MSVC (supporting C++17 or later)
* **CMake:** Version 3.10+
* **Build System:** Make (Linux/Mac) or Ninja (Windows)

### Installation Steps

1.  **Clone the repository**
    ```bash
    git clone [https://github.com/MutantCoder123/Efficient-Search-Engine.git](https://github.com/MutantCoder123/Efficient-Search-Engine.git)
    cd Efficient-Search-Engine
    ```

2.  **Build the Project**
    I use an out-of-source build to keep the directory clean.
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
    *(Note: On Windows with MSVC, use `cmake --build .` instead of `make`)*

3.  **Run the Engine**
     **Important:** Run the executable from the project root so it can find the `data/` folder.

    ```bash
    # 1. Go back to the root directory
    cd ..

    # 2. Run the executable from here
    # Linux / Mac:
    ./build/main

    # Windows (Git Bash / PowerShell):
    ./build/main.exe
    ```
    *(Note: Ensure your `data` folder is in the same directory as the executable).*
    
## Future Improvements
* **Relevance Ranking:** Currently, the engine performs a boolean search (found/not found). I plan to implement a frequency-based scoring system to rank the most relevant documents at the top.
* **Disk Serialization:** Right now, the engine rebuilds the index every time it runs. I want to save the Trie structure to a binary file so it can load instantly without re-indexing.
* **Multithreading:** I plan to use C++ threads to process multiple files simultaneously during the indexing phase to make it faster for large datasets.
* **Fuzzy Search:** Implementing Levenshtein distance to handle typos in user queries.

## 📄 License
This project is open-source and available under the [MIT License](LICENSE).
