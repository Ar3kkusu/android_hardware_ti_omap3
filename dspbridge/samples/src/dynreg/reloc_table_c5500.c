/*
 *  Copyright 2001-2008 Texas Instruments - http://www.ti.com/
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/* Tables generated for c5500 */

#define HASH_FUNC(zz) (((((zz) +1) * UINT32_C(1971)) >> 11) & 63)
#define HASH_L(zz) ((zz) >> 8)
#define HASH_I(zz) ((zz) & 0xFF)

static const uint_least16_t rop_map1[] = {
	0,
	1,
	2,
	136,
	137,
	5,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	17,
	18,
	19,
	20,
	21,
	4,
	138,
	16384,
	16385,
	16386,
	16387,
	16388,
	16389,
	16390,
	32,
	16392,
	16393,
	16394,
	16395,
	16396,
	16397,
	16398,
	16399,
	16400,
	16401,
	16402,
	16403,
	16404,
	16405,
	16406,
	6,
	15,
	16,
	16391,
	40,
	120,
	65535,
	122,
	123,
	124,
	65535,
	65535,
	65535,
	65535,
	65535,
	65535,
	65535
};

static const int_least16_t rop_map2[] = {
	-256,
	-255,
	-254,
	-237,
	5652,
	5892,
	12054,
	-233,
	-232,
	-231,
	-230,
	-229,
	-228,
	-227,
	-226,
	12319,
	12576,
	-248,
	-247,
	-246,
	-245,
	-244,
	-253,
	-235,
	-223,
	-222,
	-221,
	-220,
	-219,
	-218,
	-217,
	12813,
	-215,
	-214,
	-213,
	-212,
	-211,
	-210,
	-209,
	13104,
	-207,
	-206,
	-205,
	-204,
	-203,
	-202,
	-201,
	-251,
	-250,
	-249,
	-216,
	-242,
	-241,
	-1,
	-240,
	-239,
	-238,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1
};

static const uint_least16_t rop_action[] = {
	2560,
	2304,
	2304,
	2432,
	2432,
	2560,
	2176,
	2304,
	2560,
	3200,
	3328,
	3584,
	3456,
	2304,
	4208,
	2161,
	2164,
	2196,
	2432,
	2176,
	2304,
	2176,
	2304,
	3200,
	3328,
	3200,
	3328,
	2116,
	2128,
	2128,
	2144,
	2244,
	2304,
	6656,
	7680,
	8704,
	9728,
	10752,
	11776,
	12800,
	13824,
	14848,
	15872,
	16896,
	17920,
	18944,
	0,
	0,
	0,
	0,
	1536,
	1536,
	1536,
	5632,
	512,
	0
};

static const uint_least16_t rop_info[] = {
	0,
	35,
	35,
	35,
	47,
	35,
	35,
	35,
	35,
	39,
	39,
	39,
	39,
	35,
	34,
	35,
	43,
	43,
	47,
	43,
	47,
	47,
	47,
	43,
	43,
	47,
	47,
	43,
	43,
	47,
	43,
	47,
	35,
	64,
	64,
	128,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	64,
	128,
	201,
	197,
	74,
	78,
	208,
	196,
	200,
	192,
	192,
	66
};