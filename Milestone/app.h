/*
 * app.h
 *
 *  Created on: Oct 9, 2024
 *      Author: samso
 */

#ifndef APP_H_
#define APP_H_

// Seven Segment Decoder
void decimal2Binary(int input, char output[]);
int binary2Decimal(char output[]);
int bcd2sevenSegmentDecoder(int ABCD);
void binary2Hex(int input, char output[]);

void decrease7Segment(int input);

void increaseCounter();





#endif /* APP_H_ */
