#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
* API set up path for file python in function main() of C++
*/
void setupPythonSysPath();

/*
* API set mode of pin GPIO
*/
void DIO_SetModePin(int pin_number, const char* modepin);

/*
* API set up GPIO mode
*/
void DIO_SetUp(const char* mode);

/*
* API set GPIO output
*/
void DIO_WritePin(int pin_number, const char* state);

/*
* API get GPIO input
*/
int DIO_ReadPin(int pin_number);

/*
* API get GPIO input
*/
void DIO_cleanup();

#ifdef __cplusplus
}

#endif
#endif // GPIO_H