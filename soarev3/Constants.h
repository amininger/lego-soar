/*
 * Constants.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "ev3_constants.h"

// Debug Constants

#define PRINT_EV3_DEBUG 1

// Timing Constants

#define EV3_SEND_STATUS_FPS 20
#define EV3_SEND_ACK_FPS 20
#define SOAR_SEND_COMMAND_FPS 20
#define SOAR_DC_WAIT 0

// Defining devices that messages can be sent to
#define NO_DEV 0
#define MANAGER_DEV 1
#define BRICK_DEV 2
#define OUTPUT_MAN_DEV 3
#define INPUT_MAN_DEV 4

#define INPUT_1_DEV 11
#define INPUT_2_DEV 12
#define INPUT_3_DEV 13
#define INPUT_4_DEV 14

#define OUTPUT_1_DEV 21
#define OUTPUT_2_DEV 22
#define OUTPUT_3_DEV 23
#define OUTPUT_4_DEV 24


#define COMMAND_SUCCESS 0
#define COMMAND_FAILURE -1

// Constants for Manager
#define STATUS_MESSAGE 1
#define COMMAND_MESSAGE 3
#define ACK_MESSAGE 2

// Constants for Brick
#define BRICK_COMMAND_SET_LED 1

// Constants for Motors
#define MOTOR_DIR_FORWARD 1
#define MOTOR_DIR_BACKWARD 2

#define MAX_MOTOR_SPEED 100

#define MOTOR_COMMAND_START opOUTPUT_START
#define MOTOR_COMMAND_STOP opOUTPUT_STOP
#define MOTOR_COMMAND_SET_POWER opOUTPUT_POWER
#define MOTOR_COMMAND_SET_SPEED opOUTPUT_SPEED
#define MOTOR_COMMAND_SET_DIRECTION opOUTPUT_POLARITY

#define LARGE_MOTOR_TYPE 7
#define MEDIUM_MOTOR_TYPE 8

// Constants for Sensor
#define CHANGE_MODE_COMMAND 1
#define CREATE_ANALOG_SENSOR_COMMAND 2
#define DELETE_ANALOG_SENSOR_COMMAND 3

#define SENSOR_CAT_NONE 1
#define SENSOR_CAT_SERIAL 2
#define SENSOR_CAT_ANALOG 3

// Constants for Color Sensor
#define EV3_COLOR_SENSOR_TYPE 29

#define COLOR_SENSOR_MODE_REFLECT 0
#define COLOR_SENSOR_MODE_AMBIENT 1
#define COLOR_SENSOR_MODE_COLOR 2

// Constants for Touch Sensor
#define EV3_TOUCH_SENSOR_TYPE 125

// Constants for IR Remote
#define EV3_IR_REMOTE_SENSOR_TYPE 33

#define NUM_IR_CHANNELS 4
#define NUM_IR_REMOTE_BUTTONS 4

#define EV3_IR_REMOTE_PROXIMITY_MODE 0
#define EV3_IR_REMOTE_BEACON_MODE 1
#define EV3_IR_REMOTE_REMOTE_MODE 2

#endif /* CONSTANTS_H_ */
