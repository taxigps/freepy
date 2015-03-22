/*
 * Copyright (C) 1999.4  Li ZhenChun
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License; or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that is will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, M A 02139, USA.
 *
 * Author: Li ZhenChun  email: zhchli@163.net or zhchli@126.com
 * 
 */

/**********************************************************************/
/*                                                                    */
/* These tables are for ImeProcessKey(). And IMEProcessKey function   */
/* refer this table and deside the VKey is needed for FAKEIME or not. */
/*                                                                    */
/**********************************************************************/
#define NOIME 
#include "windows.h"

BYTE bNoComp[] = {      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-1F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-2F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-3F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-4F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-5F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-6F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-7F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-8F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-9F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-AF
                        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,        // B0-BF
                        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-CF
                        0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,        // D0-DF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-EF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-FF

BYTE bComp[] = {        0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,        // 10-1F
                        1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,        // 20-2F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-3F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-4F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-5F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-6F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-7F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-8F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-9F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-AF
                        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,        // B0-BF
                        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-CF
                        0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,        // D0-DF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-EF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-FF
