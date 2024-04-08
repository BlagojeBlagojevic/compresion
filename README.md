# File Compression and Decompression with FastLZ

This C program provides functionality for compressing and decompressing files using the FastLZ compression algorithm. The program supports multithreaded compression and decompression to utilize multiple CPU cores effectively.

## Features

- **Compression**: Compress single or multiple files into a compressed format using FastLZ.
- **Decompression**: Decompress previously compressed files to retrieve the original content.
- **Multithreading**: Utilizes OpenMP for parallelizing compression and decompression tasks across multiple CPU cores.
- **Dynamic Memory Management**: Efficiently manages memory allocation and deallocation for handling large files.

## Requirements

- C Compiler
- OpenMP library
- FastLZ library

## Usage

### Compression

To compress a file or multiple files, follow these steps:

1. Compile the program using a C compiler.
2. Run the compiled executable.
3. Choose the compression option.
4. Input the name of the file(s) you want to compress.
5. Input the name for the compressed file(s).

### Decompression

To decompress previously compressed files, follow these steps:

1. Compile the program using a C compiler.
2. Run the compiled executable.
3. Choose the decompression option.
4. Input the name of the compressed file(s).
5. Input the name for the decompressed file(s).

## Example

```bash
$ ./file_compressor
What do you want to do? (1 - Compression, 2 - Decompression, any other key to exit)
> 1
Input name of file:
> input_file.txt
Input name of file you want to save as:
> compressed_file
```

```bash
$ ./file_compressor
What do you want to do? (1 - Compression, 2 - Decompression, any other key to exit)
> 2
Input name of file:
> compressed_file
Input name of file you want to save as:
> decompressed_file.txt
```

## License

This program is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Credits

This program uses the FastLZ compression library. See the [FastLZ GitHub repository](https://github.com/ariya/fastlz) for more information.
