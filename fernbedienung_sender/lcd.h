#ifndef _LCD_H_
#define _LCD_H_ 1

extern uint8_t toggleLCD;

void initLCD( void );
void showNum(uint16_t num);
void splitNum(uint16_t num);

#endif
