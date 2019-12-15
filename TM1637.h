	//	Author:Fred.Chu
	//	Date:9 April,2013
	//
	//	Applicable Module:
	//		  4-Digit Display by catalex
	//		  4-Digit Display (D4056A) by catalex
	//   Store: http://www.aliexpress.com/store/1199788
	//
	//	This library is free software; you can redistribute it and/or
	//	modify it under the terms of the GNU Lesser General Public
	//	License as published by the Free Software Foundation; either
	//	version 2.1 of the License, or (at your option) any later version.
	//
	//	This library is distributed in the hope that it will be useful,
	//	but WITHOUT ANY WARRANTY; without even the implied warranty of
	//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	//	Lesser General Public License for more details.
	//
	//	You should have received a copy of the GNU Lesser General Public
	//	License along with this library; if not, write to the Free Software
	//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
	//
	//	Modified record:
	//
	/*******************************************************************************/
	
#ifndef TM1637_h
#define TM1637_h
#include <inttypes.h>
#include <Arduino.h>
	//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
	
#define STARTADDR  0xc0 
	/**** definitions for the clock point of the 4-Digit Display *******/
#define POINT_ON   1
#define POINT_OFF  0
	/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7
//--------------------------------------------------------//
//Special characters index of tube table
#define INDEX_NEGATIVE_SIGN	16
#define INDEX_BLANK			17
/************definitions for type of the 4-Digit Display*********************/
#define D4036B 0
#define D4056A 1

//// Symbols
//// Took from: https://github.com/AlexGyver/WS2812_painter/blob/master/libraries/TM1637_Gyver/TM1637.h
//#define _A 0x77
//#define _B 0x7f
//#define _C 0x39
//#define _D 0x3f
//#define _E 0x79
//#define _F 0x71
//#define _G 0x3d
//#define _H 0x76
//#define _J 0x1e
//#define _L 0x38
//#define _N 0x37
//#define _O 0x3f
//#define _P 0x73
//#define _S 0x6d
//#define _U 0x3e
//#define _Y 0x6e
//#define _a 0x5f
//#define _b 0x7c
//#define _c 0x58
//#define _d 0x5e
//#define _e 0x7b
//#define _f 0x71
//#define _h 0x74
//#define _i 0x10
//#define _j 0x0e
//#define _l 0x06
//#define _n 0x54
//#define _o 0x5c
//#define _q 0x67
//#define _r 0x50
//#define _t 0x78
//#define _u 0x1c
//#define _y 0x6e
//#define _dash 0x40
//#define _under 0x08
//#define _equal 0x48
//#define _empty 0x00
//
//#define _0 0x3f
//#define _1 0x06
//#define _2 0x5b
//#define _3 0x4f
//#define _4 0x66
//#define _5 0x6d
//#define _6 0x7d
//#define _7 0x07
//#define _8 0x7f
//#define _9 0x6f

	class TM1637
	{
	  public:
		uint8_t Cmd_SetData;
		uint8_t Cmd_SetAddr;
		uint8_t Cmd_DispCtrl;
		boolean _PointFlag; 	//_PointFlag=1:the clock point on
		uint8_t _DispType;
    TM1637();
		TM1637(uint8_t, uint8_t);
		void init(uint8_t = D4036B);
		void writeByte(int8_t wr_data);//write 8bit data to tm1637
		void start(void);//send start bits
		void stop(void); //send stop bits
		void display(int8_t DispData[]);
		void display(uint8_t BitAddr,int8_t DispData);
		void display(double Decimal);
		void display(int16_t Decimal);
		void clearDisplay(void);
		void set(uint8_t = BRIGHT_TYPICAL,uint8_t = 0x40,uint8_t = 0xc0);//To take effect the next time it displays.
		void point(boolean PointFlag);//whether to light the clock point ":".To take effect the next time it displays.
		void coding(int8_t DispData[]); 
		int8_t coding(int8_t DispData); 
	  private:
		uint8_t Clkpin;
		uint8_t Datapin;
		uint8_t DecPoint;
		boolean BlankingFlag;
	};
#endif
