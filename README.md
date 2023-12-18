# File Copy and Move Utility

This C program provides a file and directory copy/move utility with support for filtering by file extensions. The program uses the nftw (File Tree Walk) function to traverse the source directory and perform copy or move operations on files and directories based on specified criteria.

## Features

* Copy or move files and directories from a source directory to a destination directory.
* Filter files based on allowed file extensions.
* Supports both single file and directory copy/move operations.
* Utilizes the nftw function for efficient directory traversal.

## Usage

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
