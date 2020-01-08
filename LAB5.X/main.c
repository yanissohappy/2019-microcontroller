/*
 * File:   main.c
 * Author: yan
 *
 * Created on 2019年10月20日, 下午 4:48
 */

#include <xc.h>

//extern unsigned int add(unsigned int a, unsigned int b);
extern int divide(unsigned int a, unsigned int b);

void main(void) {
    volatile unsigned int res = divide(166, 12) ;
    volatile unsigned char quotient = (unsigned char)res;
    volatile unsigned char remainder = (unsigned char)res >> 8;
    while(1);
    return;
}
