/*
 * app.c
 *
 *  Created on: Oct 9, 2024
 *      Author: samson
 *      This is my own work.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "app.h"
#include "hardware.h"

/*
 * decimal to binary conversion
 * ---------------------------
 * Converts binary numbers to binary
 *
 *
 */
void decimal2Binary(int input, char output[]) {
	int result = -1;
	int bit = strlen(output);

	// While the result is not 0
	while (result != 0) {
		// Get quotient and remainder
		result = input/2;
		int remainder = input % 2;
		// Set a 1 or 0 bit
		output[bit] = remainder ? '1' : '0';
		// Next bit
		--bit;
		input = result;
	}
}

/*
 * Binary to decimal conversion
 * ---------------------------
 * Converts binary numbers to a value readable by a 7-segment display
 *
 * returns decimal numbers
 */
int binary2Decimal(char input[]) {
	int output = 0;
	int power = 1;

	for (int x = strlen(input) - 1; x >= 0; --x) {
		output = output + ((input[x] - 48) * power);
		power = power * 2;
	}
	return output;
}

void binary2Hex(int input, char output[]) {

		char digit[2] = "0\0";
		int nibbles = strlen(output);

		while (nibbles > 0) {
			// convert nibble to hex
			sprintf(&digit[0], "%1x", input & 0x0F);
			output[nibbles - 1] = digit[0];
			// Next nibble
			--nibbles;
			input = input >> 4;
		}
}

/*
 * BCD to 7-segment display decoder
 * ---------------------------
 * Converts binary numbers to a value readable by a 7-segment display
 *
 * returns decimal numbers
 */
int bcd2sevenSegmentDecoder(int ABCD) {

	// ABCD is the input BCD number
	char output[4] = {"000\0"};
	decimal2Binary(ABCD, output);
	int A = output[0] - 48;
	int B = output[1] - 48;
	int C = output[2] - 48;
	int D = output[3] - 48;

	// a is the LSB (Least significant bit)
	// * is AND but coded as & in C
	// + is OR but coded as | in C
	// a = (~B * ~D) + C + (B * D) + A
	int a = (~B & 0x01 & ~D & 0x01) | C | (B & D) | A;

	// b = ~B + (~C * ~D) + (C * D)
	int b = (~B & 0x01) | (~C & 0x01 & ~D & 0x01) | (C & D);

	// c = ~C + D + B
	int c = (~C & 0x01) | D | B;

	// d = (~B * ~D) + (~B * C) + (B * ~C *D) + (C * ~D) + A
	int d = (~B & 0x01 & ~D & 0x01) | (~B & 0x01 & C) | (B & (~C & 0x01) & D) | (C & ~D & 0x01) | A;

	// e = (~B * ~D) + (C * ~D)
	int e = (~B & 0x01 & ~D & 0x01) | (C & ~D & 0x01);

	// f = (~C * ~D) + (B * ~C) + (B * ~D) + A
	int f = (~C & 0x01 & ~D & 0x01) | (B & ~C & 0x01) | (B & ~D & 0x01) | A;

	// g = (~B * C) + (B * ~C) + A + (B * ~D)
	int g = (~B & 0x01 & C) | (B & ~C & 0x01) | A | (B & ~D & 0x01);

	char values[9] = {"00000000\0"};
	values[0] = 48;
	values[1] = g + 48;
	values[2] = f + 48;
	values[3] = e + 48;
	values[4] = d + 48;
	values[5] = c + 48;
	values[6] = b + 48;
	values[7] = a + 48;
	values[8] = 0;

	return binary2Decimal(values);

}


/*
 * Decrease 7 segment decoder
 * ---------------------------
 * Decreases the input and conducts the bcd2sevenSegmentDecoder
 *
 * displays timer decreasing from output
 */
void decrease7Segment(int input) {
	char output2[3] = {"00\0"};
	while (input > 0) {
			printf("Timer at %d\n", input);
			binary2Hex(bcd2sevenSegmentDecoder(input / 10), &output2[0]);
			printf("MSB LED is %s\n", &output2[0]);
			binary2Hex(bcd2sevenSegmentDecoder(input % 10), &output2[0]);
			printf("LSB LED is %s\n", &output2[0]);

			// stub function that would write to the hardware
			writeToSevenSegment(input);
			printf("\n\n");

			// Decrease input
			input--;
			sleep(1);
		}
}

/*
 * Counts up the input for the stopwatch
 * ---------------------------
 * Starting from zero, it increases the timer until the user stops it
 *
 * displays counter increasing
 */
void increaseStopwatch() {
	char output3[3] = {"00\0"};
	int count = 0;
	int value = 1;
	while (value == 1) {
			printf("Counter at %d\n", count);
			binary2Hex(bcd2sevenSegmentDecoder(count / 10), &output3[0]);
			printf("MSB LED is %s\n", &output3[0]);
			binary2Hex(bcd2sevenSegmentDecoder(count % 10), &output3[0]);
			printf("LSB LED is %s\n", &output3[0]);

			// stub function that would write to the hardware
			writeToSevenSegment(count);
			printf("\n\n");

			count++;
			sleep(1);
		}
}

int main(int argc, char **argv) {

	int counter = 0;
	int user = -1;

	// Allow user to switch between modes
	while (user != 0) {
		printf("\n0 - exit\n1 - timer\n2 - stopwatch\n");
		printf("Enter number: ");
		scanf("%d", &user);

		// Timer mode
		if (user == 1) {
			char input[10];
			counter = 0;

			printf("Press Enter to increase the counter. Type 'exit' to quit.\n");

			while (1) {
				// Gets user input, press enter to increase counter
		        fgets(input, sizeof(input), stdin);

				// Check if the user typed "exit"
				if (strncmp(input, "exit", 4) == 0) {
					break;
				}

				// Increment the counter everytime user presses enter
				counter++;
				printf("Counter: %d\n", counter);
			}

			printf("Final counter value: %d\n\n", counter);
			decrease7Segment(counter);
		}

		// Stopwatch mode
		if (user == 2) {
			printf("\nBeginning Timer\n");
			increaseStopwatch();
		}

	}

	return( 0 );
}


