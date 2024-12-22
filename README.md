
# Computer Club Manager

This is a C++ program designed to manage events in a computer club. It processes input files describing the club's configuration and events, then outputs the timeline of events and statistics for each table at the end of the day.

## Features

- Handles club opening and closing times.
- Manages clients arriving, leaving, and waiting for tables.
- Tracks table usage and calculates daily revenue.
- Reports errors when input data is invalid or when rules are violated (e.g., client arrives outside working hours, tries to use an occupied table, etc.).

## Prerequisites

- **Linux**: Ensure `gcc` or `clang` is installed.
- **Windows**: Install `MinGW` or `Cygwin` for GCC support.

## Compilation

### Linux
To compile the program on Linux:
```bash
g++ -std=c++17 -o club_manager club_manager.cpp
```

### Windows
To compile the program on Windows using MinGW:
```bash
g++ -std=c++17 -o club_manager.exe club_manager.cpp
```

Ensure the `g++` executable is in your system's PATH.

## Execution

Provide the path to the input file as an argument when running the program.

### Linux
```bash
./club_manager test_file.txt
```

### Windows
```cmd
club_manager.exe test_file.txt
```

## Input Format

The input file should contain:

1. Number of tables in the club.
2. Opening and closing times in HH:MM format.
3. Price per hour.
4. Events in chronological order.

### Example Input
```
3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
...
```

## Output

The program outputs:

1. The opening time.
2. Events as they occur.
3. Errors for invalid actions.
4. Closing time and summary statistics for each table.

### Example Output
```
09:00
08:48 1 client1
08:48 13 NotOpenYet
...
19:00
1 70 05:58 
2 30 02:18
3 90 08:01
```

## Testing

Ensure the input file is formatted correctly. For testing purposes, use the provided example or create your own scenarios.

## Notes

- The program assumes all input times are in chronological order.
- Output uses a 24-hour format with zero-padding for consistency.
