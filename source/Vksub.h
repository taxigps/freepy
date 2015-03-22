/**********************************************************************/
/*                                                                    */
/*      VKSUB.C - Windows 95 FAKEIME                                  */
/*                                                                    */
/*      Copyright (c) 1994-1995  Microsoft Corporation                */
/*                                                                    */
/**********************************************************************/

#define VK_0    0x030
#define VK_1    0x031
#define VK_2    0x032
#define VK_3    0x033
#define VK_4    0x034
#define VK_5    0x035
#define VK_6    0x036
#define VK_7    0x037
#define VK_8    0x038
#define VK_9    0x039
#define VK_A    0x041
#define VK_B    0x042
#define VK_C    0x043
#define VK_D    0x044
#define VK_E    0x045
#define VK_F    0x046
#define VK_G    0x047
#define VK_H    0x048
#define VK_I    0x049
#define VK_J    0x04A
#define VK_K    0x04B
#define VK_L    0x04C
#define VK_M    0x04D
#define VK_N    0x04E
#define VK_O    0x04F
#define VK_P    0x050
#define VK_Q    0x051
#define VK_R    0x052
#define VK_S    0x053
#define VK_T    0x054
#define VK_U    0x055
#define VK_V    0x056
#define VK_W    0x057
#define VK_X    0x058
#define VK_Y    0x059
#define VK_Z    0x05A

#define VK_OEM_NUMBER   0x90            // NumLock
#define VK_OEM_SCROLL   0x91            // ScrollLock
#define VK_OEM_SHIFT    0x92            // ShiftLock
#define VK_OEM_0        0x0B9           //
#define VK_OEM_1        0x0BA           // ';:' for US
#define VK_OEM_PLUS     0x0BB           // '+' any country
#define VK_OEM_COMMA    0x0BC           // ',' any country
#define VK_OEM_MINUS    0x00BD          // '-' any country
#define VK_OEM_PERIOD   0x0BE           // '.' any country
#define VK_OEM_2        0x0BF           // '/?' for US
#define VK_OEM_3        0x0C0           // '`~' for US
#define VK_OEM_4        0x0DB           // '[{' for US
#define VK_OEM_5        0x0DC           // '\|' for US
#define VK_OEM_6        0x0DD           // ']}' for US
#define VK_OEM_7        0x0DE           // ''"' for US
#define VK_OEM_8        0x0DF           // none for US
#define VK_OEM_9        0x0E0           // next available
#define VK_OEM_AX       0x0E1           // AX key
#define VK_OEM_BSLASH   0x0E2           // additional Back Slash key for AX.
