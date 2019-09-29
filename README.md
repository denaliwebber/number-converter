Denali Webber- CS 315

Program: Number Converter and Bit Twiddler Command Line Utility.

Description: This program can convert between number systems, unsigned int, signed int, hexadecimal, and binary. Value entered must fit into 32 bits, and may contain bit width and range specifications as seen below in the examples. 

How to compile: gcc -o nt nt.c

How to run: ./nt -number to be converted-
				- use 0b for binary conversion
				- use 0x for hex conversion
				- use - for negative signed int
				- use a -r with two numbers separated with a comma and no space to set a range
				- use a -b followed by a space and a number between 4-32 that is a multiple of 4 to specify the bit width

	examples:
				$ ./nt 713
				0000 0000 0000 0000 0000 0010 1100 1001 (base 2) 
				0b00000000000000000000001011001001 (base 2) 
				0x000002C9 (base 16)
				713 (base 10 unsigned)
				713 (base 10 signed)

				$ nt -713
				1111 1111 1111 1111 1111 1101 0011 0111 (base 2) 
				0b11111111111111111111110100110111 (base 2)
				0xFFFFFD37 (base 16)
				4294966583 (base 10 unsigned)
				-713 (base 10 signed)

				$ nt 0b11100011
				0000 0000 0000 0000 0000 0000 1110 0011 (base 2)
				0b00000000000000000000000011100011 (base 2)
				0x000000E3 (base 16)
				227 (base 10 unsigned)  
				227 (base 10 signed)

				$ nt 0xE3
				0000 0000 0000 0000 0000 0000 1110 0011 (base 2)  
				0b00000000000000000000000011100011 (base 2) 
				0x000000E3 (base 16)
				227 (base 10 unsigned) 
				227 (base 10 signed)

				$ nt -b 16 713
				0000 0010 1100 1001 (base 2) 
				0b0000001011001001 (base 2) 
				0x02C9 (base 16)
				713 (base 10 unsigned)
				713 (base 10 signed)  

				$ nt -b 4 -r 4,7 0b000010100000
				1010 (base 2)
				0b1010 (base 2)
				0xA (base 16)
				10 (base 10 unsigned)
				-6 (base 10 signed)
