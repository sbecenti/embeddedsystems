/*
 * hardware.h
 *
 *  Created on: Oct 9, 2024
 *      Author: samso
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

int writeToSevenSegment(int value);

int pressTimerButton(int button);

int pressStopwatchButton(int button);

int pressCounterButton(int button);

int clearSevenSegmentDisplay();

#endif /* HARDWARE_H_ */
