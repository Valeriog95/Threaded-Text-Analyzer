# Project Requirements: Threaded Text Analyzer

## Objective
The goal is to develop a C application capable of processing large text files to build a Markov Chain-based frequency table and generate pseudo-random text.

## Functional Requirements
### Task 1: Text Analysis
- **Tokenization**: Process input text, treating words and punctuation (., !, ?, ,) as distinct tokens.
- **Data Structure**: Store relationships between words using a nested structure (e.g., Red-Black Trees).
- **Multi-threading**: Implement a concurrent pipeline to speed up analysis on multi-core systems.
- **Circular Logic**: The last word of the text must link back to the first word to ensure a continuous Markov chain.
- **Export**: Generate a CSV file containing word pairs and their relative transition frequencies.

### Task 2: Text Generation
- **Import**: Load a frequency table from a CSV file.
- **Stochastic Generation**: Given a starting word, pick the next word based on the calculated probabilities.
- **Formatting**: Ensure proper capitalization after strong punctuation marks.

## Technical Constraints
- Language: **C** (Standard C11/C17).
- Memory Management: Zero leaks allowed (verified via **Valgrind**).
- Build System: **CMake**.
- Documentation: **Doxygen**-compliant comments.