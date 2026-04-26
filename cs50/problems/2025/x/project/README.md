# CODEX
#### Video Demo:  https://youtu.be/g9_eJ7_KgjI
#### Description:

Description

Codex is a command-line tool designed for encoding and decoding data in different formats.
It currently supports Base64 and Hexadecimal encoders, allowing you to process files or raw strings directly through the terminal.

In addition to the CLI executable, the project also provides a static library that enables developers to import the codec functionalities directly into other applications.

Usage
Usage: [mode] [options] [encoder/decoder] [input file] [output file]

Example:
codex --decode base64 input_file.txt output_file.jpg

Modes

-e / --encode
Encode bytes using the specified algorithm.

-d / --decode
Decode data using the specified algorithm.
(Not every encoder allows decoding — for example, hash functions like SHA256.)

Options

-r Recursive mode

On encode, the output file will concatenate data continuously.

On decode, multiple output files will be generated with incremental names.

File type auto-detection is mandatory in recursive mode.

-s String input mode

The provided string will be parsed and treated as the raw input data.

-t Terminal output mode

Redirects the output to stdout instead of writing to a file.

-x Disable file type auto-detection

If no magic signature is found, the fallback extension is .txt.

Input: Path to the input file or a raw string
Output: Path to the output file to be created

Encoders

Base64

Hexadecimal

(More encoders will be added in future versions.)

Concepts

LSB (Least Significant Bit): the bit with the smallest value in a byte.

MSB (Most Significant Bit): the bit with the highest value in a byte.

Base64

In Base64, 3 bytes (24 bits) are represented by 4 characters, resulting in about 33% more file size.
Despite the overhead, it’s widely used for transmitting binary data over text-based protocols (like email or JSON) because it’s safe and compatible with text systems.

The bits are reconstructed using bit-shift operations:

The first byte is formed by the first character (shifted left by two) plus the two MSB bits of the second character.

The second byte is formed by the six LSB bits of the second character plus the two MSB bits of the third.

The third byte is formed by the two LSB bits of the third character plus the six LSB bits of the fourth.

Base64 uses the character set:
A–Z, a–z, 0–9, +, /

Hexadecimal

The hexadecimal format is simpler: each byte (8 bits) is represented by two characters, causing a 100% size increase compared to the original binary data.
However, it is human-readable and easily converted back to binary.

Each nibble (4 bits) represents half of a byte:

The first character corresponds to the 4 most significant bits (MSB).

The second character corresponds to the 4 least significant bits (LSB).

The hexadecimal character set is:
0–9, A–F

Because of its direct binary mapping, HEX is ideal for debugging, manual inspection, and binary data visualization.

To Do

Implement additional encoders/decoders

Add hash function support (e.g., SHA, MD5)

Add basic compression support
