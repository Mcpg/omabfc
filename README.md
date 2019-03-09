# Oh my... Another Brainfuck Compiler - omabfc
OMABFC is a Brainfuck compiler written in C that produces NASM-compatible
x64 Linux assembly code.

## Usage
OMABFC is a command-line tool. Available options:

 * **--input-file | -i**: provides input Brainfuck file (required),
 * **--output-file | -o**: provides output file for assembly code (by default code is outputted to stdout),
 * **--version | -v**: prints OMABFC version to stdout.

## Actually building Brainfuck programs into executables
OMABFC only produces assembly code. This means, that it needs
to be manually assembled and linked. The process is fairly simple.

 1. Compile a Brainfuck file: `omabfc -i program.bf -o program.asm`
 2. Assemble it using nasm: `nasm -f elf64 program.asm -o program.o`
 3. Link it: `ld program.o -o program`

And that's it.

## Building OMABFC
You need to get CMake and GCC. Go into root directory of the repostiory with
a terminal, make a directory called `build`, go to it and execute `cmake ..` from it.
Now run `make all`. After the compilation succeeds, `omabfc` file should appear
in the `build` directory.

## License
This project is licensed under the [MIT License](LICENSE).