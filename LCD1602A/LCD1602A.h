/*
 * LCD1602A.h
 *
 *  Created on: 13/01/2017
 *      Author: Enrique
 */

#ifndef LCD1602A_H_
#define LCD1602A_H_


/*LCD Configuation*/
#define LCD_8_DATA_LINES	0
#define LCD_4_DATA_LINES	1

#include <stdint.h>

#define CLK_SRC_HZ	((uint64_t)24000000)

#define LOW_NIBBLE		0u
#define HIGH_NIBBLE		1u

#define DISP_DL	0x10u /* 1: 8bits			0: 4bits Data transfer*/
#define DISP_N	0x08u /* 1: 2 Lines			0: 1 Line LCD Lines*/
#define DISP_F	0x04u /* 1: 5x10 LCD font	0: 5x8 LCD Font*/


/********************************************
S = 1: Accompanies display shift			*
S/C = 1: Display shift						*
S/C = 0: Cursor move						*
R/L = 1: Shift to the right					*
R/L = 0: Shift to the left					*
DL = 1: 8 bits, DL = 0: 4 bits				*
N = 1: 2 lines, N = 0: 1 line				*
F = 1: 5 ´ 10 dots, F = 0: 5 ´ 8 dots		*
BF = 1: Internally operating				*
BF = 0: Instructions acceptable				*
*********************************************/

#define DISP_ON_D	0x04u /*1: Disp On 		0: Disp Off*/
#define DISP_ON_C	0x02u /*1: Cursor ON  	0: Cursor OFF*/
#define DISP_ON_B	0x01u /*1: Blink ON 		0: BLink OFF */

#define CURSOR_SHIFT	0x01u /*TRUE*/

/*
I/D = 1: Increment
I/D = 0: Decrement
*/
#define CURSOR_INC		0x02u /*Cursor increment*/
#define CURSOR_DEC		0x00u /*Cursor Decrement*/

/*Disp Commands*/
#define DISP_CLEAR					(uint8_t)0x01u
#define DISP_RETURN_HOME			(uint8_t)0x02u
#define DISP_ENTRY_MODE_SET			(uint8_t)0x04u
#define DISP_ON						(uint8_t)0x08u
#define DISP_CURSORDISP_SHIFT		(uint8_t)0x10u
#define DISP_FUNC_SET				(uint8_t)0x20u
#define DISP_SET_CGRAM_ADDRESS		(uint8_t)0x40u
#define DISP_SET_DDRAM_ADDRESS		(uint8_t)0x80u

#define DISP_DDRAM_ADDRESS			(uint8_t)0x80u /*Set DDRAM Address*/


#define	DISP_INIT_DELAY				(uint8_t)50u
#define DISP_DELAY					(uint8_t)100u
#define ENABLE_WITH					(uint8_t)10u


typedef enum
{
	SHOW_TIME,
	SET_TIME_CFG,
	SET_TIME_HR,
	SET_TIME_MIN,
	SET_DATE_CFG,
	SET_DATE_DAY,
	SET_DATE_MONTH,
	SET_DATE_YEAR,
	SET_COMPLETE,
	INVALID_MENU
}menu_index_type;






typedef enum
{
	ASCII_space				=  0x20u,//		space
	ASCII_ExclMarck 		=  0x21u,//		!
	ASCII_quotes 			=  0x22u,//		"
	ASCII_hashtag 			=  0x23u,//		#
	ASCII_Money 			=  0x24u,//		$
	ASCII_porcent 			=  0x25u,//		%
	ASCII_pound 			=  0x26u,//		&
	ASCII_singlequote 		=  0x27u,//		'
	ASCII_OpenParenthesis 	=  0x28u,//		(
	ASCII_CloseParenthesis 	=  0x29u,//		)
	ASCII_asterick 			=  0x2Au,//		*
	ASCII_plus 				=  0x2Bu,//		+
	ASCII_comma 			=  0x2Cu,//		,
	ASCII_minus 			=  0x2Du,//		-
	ASCII_period 			=  0x2Eu,//		.
	ASCII_slash 			=  0x2Fu,//		/
	ASCII_0					=  0x30u,//		0
	ASCII_1					=  0x31u,//		1
	ASCII_2 				=  0x32u,//		2
	ASCII_3 				=  0x33u,//		3
	ASCII_4 				=  0x34u,//		4
	ASCII_5 				=  0x35u,//		5
	ASCII_6 				=  0x36u,//		6
	ASCII_7 				=  0x37u,//		7
	ASCII_8 				=  0x38u,//		8
	ASCII_9 				=  0x39u,//		9
	ASCII_colom 			=  0x3Au,//		:
	ASCII_smicolom 			=  0x3Bu,//		;
	ASCII_lessthan 			=  0x3Cu,//		<
	ASCII_equal 			=  0x3Du,//		=
	ASCII_graterthan 		=  0x3Eu,//		>
	ASCII_questionmark 		=  0x3Fu,//		?
	ASCII_at 				=  0x40u,//		@
	ASCII_A 				=  0x41u,//		A
	ASCII_B 				=  0x42u,//		B
	ASCII_C 				=  0x43u,//		C
	ASCII_D					=  0x44u,//     D
	ASCII_E					=  0x45u,//     E
	ASCII_F					=  0x46u,//     F
	ASCII_G					=  0x47u,//     G
	ASCII_H					=  0x48u,//     H
	ASCII_I					=  0x49u,//     I
	ASCII_J					=  0x4Au,//     J
	ASCII_K					=  0x4Bu,//     K
	ASCII_L					=  0x4Cu,//     L
	ASCII_M					=  0x4Du,//     M
	ASCII_N					=  0x4Eu,//     N
	ASCII_O					=  0x4Fu,//     O
	ASCII_P					=  0x50u,//     P
	ASCII_Q					=  0x51u,//     Q
	ASCII_R					=  0x52u,//     R
	ASCII_S					=  0x53u,//     S
	ASCII_T					=  0x54u,//     T
	ASCII_U					=  0x55u,//     U
	ASCII_V					=  0x56u,//     V
	ASCII_W					=  0x57u,//     W
	ASCII_X					=  0x58u,//     X
	ASCII_Y					=  0x59u,//     Y
	ASCII_Z					=  0x5Au,//     Z
	ASCII_a					=  0x61u,//     a
	ASCII_b					=  0x62u,//     b
	ASCII_c					=  0x63u,//     c
	ASCII_d					=  0x64u,//     d
	ASCII_e					=  0x65u,//     e
	ASCII_f					=  0x66u,//     f
	ASCII_g					=  0x67u,//     g
	ASCII_h					=  0x68u,//     h
	ASCII_i					=  0x69u,//     i
	ASCII_j					=  0x6Au,//     j
	ASCII_k					=  0x6Bu,//     k
	ASCII_l					=  0x6Cu,//     l
	ASCII_m					=  0x6Du,//     m
	ASCII_n					=  0x6Eu,//     n
	ASCII_o					=  0x6Fu,//     o
	ASCII_p					=  0x70u,//     p
	ASCII_q					=  0x71u,//     q
	ASCII_r					=  0x72u,//     r
	ASCII_s					=  0x73u,//     s
	ASCII_t					=  0x74u,//     t
	ASCII_u					=  0x75u,//     u
	ASCII_v					=  0x76u,//     v
	ASCII_w					=  0x77u,//     w
	ASCII_x					=  0x78u,//     x
	ASCII_y					=  0x79u,//     y
	ASCII_z					=  0x7Au,//     z
	ASCII_OpenKey			=  0x7Bu,//     {
	ASCII_VLine				=  0x7Cu,//     |
	ASCII_CloseKey			=  0x7Du,//     }
	ASCII_rightarrow		=  0x7Eu,//     -->
	ASCII_leftarrow			=  0x7Fu //     <--

}ASCII_Char;

/*Exported Variables and Functions*/

extern menu_index_type SetMenu;
extern uint8_t Disp_Init_Complete;


extern void LCD1602A_Init(void);
extern void LCD1602A_Disp_Main(void);
extern void LCD1602A_Disp_Cfg_Refres(void);


extern ASCII_Char SetTime_Menu[5];
extern ASCII_Char SetDate_Menu[5];

extern uint8_t Disp_On_Var;
extern uint8_t Disp_FuncSet_Var;
extern uint8_t Disp_EntryMode_Var;



#endif /* LCD1602A_H_ */
