# File System Manipulation Tool

A simple Linux command-line tool for copying or moving files and directories based on specified extensions.

## Features

- **Copy (`-cp`):** Copies files and directories from a source directory to a destination directory.
- **Move (`-mv`):** Moves files and directories from a source directory to a destination directory.
- **Filter by Extension:** Optionally filter files based on specified extensions.

## Usage

### Prerequisites

- Linux environment
- GCC compiler

### Build

```bash
gcc dircmx.c -o dircmx
```

### Run

```bash
./dircmx <source_directory> <destination_directory> <option> [extensions]
```

* <source_directory>: Path to the source directory.
* <destination_directory>: Path to the destination directory.
* <option>: Choose either -cp (copy) or -mv (move).
* [extensions] (optional): Specify file extensions to filter (e.g., .txt .jpg).

## Example

```bash
./file_manipulator /path/to/source /path/to/destination -cp .txt .jpg
```

This example command copies all .txt and .jpg files from the source directory to the destination directory.

Adjust the paths, options, and extensions according to your specific use case when providing examples in your documentation.
