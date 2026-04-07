Threaded Text Analyzer
A high-performance C application designed for probabilistic text analysis and generation. The project implements a Markov Chain model using nested Red-Black Trees to map word transitions and supports high-concurrency processing via a multi-threaded pipeline.

🚀 Key Features
    Task 1: Frequency Analysis
        - Single-threaded and Multi-threaded processing modes.
        - Case-insensitive tokenization with specialized punctuation handling.
        - Circular logic: the last token of the text links back to the first.
        - Optimized CSV export with $O(1)$ total frequency retrieval.
    Task 2: Pseudo-Random Text Generation
        - Generates text based on imported or analyzed frequency tables.
        - Support for "Contextual Start": specify a starting word to guide the generation.
        - Automatic capitalization management following strong punctuation (., !, ?).
        - Integrity check: validates that the sum of frequencies in the input CSV equals 1.0.

This project was developed following a specific set of technical requirements. You can find the full specification in the REQUIREMENTS.md file.

🏗️ Technical Architecture

1. Nested Data Structure
    The core is a Nested Red-Black Tree:
        - Main Tree: Keys are unique words (char*), Values are pointers to a SuccessorContext.
        - Successor Tree: Nested inside each context, storing following words and their occurrences (int*).

2. Multi-threaded Pipeline
    In Multi-thread mode, the analyzer uses a Producer-Consumer pattern:
        - Reader Thread: Streams tokens from the input file.
        - Worker Threads: Concurrently update the shared Red-Black Tree using Mutex Locks to ensure thread-safety and data consistency.

3. Memory Management
    Strict Post-Order Traversal ensures clean deallocation. Verified via Valgrind to guarantee zero memory leaks.

🛠️ Build and Usage

Prerequisites:
    - GCC/Clang
    - CMake (3.10+)
    - Valgrind (optional, for memory checks)

Compilation:

```bash
    mkdir build && cd build
    cmake ..
    make
```

Execution (CLI): 
The application uses argtable3 for robust command-line parsing.

Task 1: Analyze a text file

```bash
    ./build/bin/ThreadedTextAnalyzer input.txt --export=table.csv --multi
```

Task 2: Generate text from a CSV table

```bash
    # Generates 20 words starting from a random punctuation mark
    ./build/bin/ThreadedTextAnalyzer table.csv -g 20 --export=generated.txt
```

Combined: Analyze and immediately generate (result will be print in stdout)

```bash
    # Analyzes input.txt and generates 15 words starting from "previsioni"
    ./build/bin/ThreadedTextAnalyzer input.txt -g 15 -s "previsioni" --single  
```

Parameters:

    input: Path to a .txt (to analyze) or .csv (to load).

    --single / --multi: Toggle between single and multi-threaded analysis.

    -g, --task2 <n>: Mandatory for Task 2. Enables generation of <n> words.

    -s, --start <word>: Optional. Specifies the starting word. If omitted, a random punctuation mark (. ! ?) is used as context.

    -e, --export <file>: Save the frequency table (.csv) or generated text (.txt) to a file.

📊 Evaluation & Testing:

The project includes a comprehensive test suite:

    - Tokenization: Validates punctuation as individual tokens.

    - Integrity: Ensures frequency sums in CSV imports are correct.

    - Memory Safety: 
    ```bash 
        valgrind --leak-check=full ./build/bin/ThreadedTextAnalyzer input.txt -g 10 -s "!"
    ```

📚 Documentation
    The project is fully documented using Doxygen. To generate the HTML documentation:
        1. Ensure `doxygen` is installed.
        2. Run: ```bash make doc```
📝 Authors
Valeriog95 - Architecture & Lead Developer