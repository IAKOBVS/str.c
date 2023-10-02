#ifndef JSTR_CTYPE_TABLE_H
#define JSTR_CTYPE_TABLE_H 1

#include "jstr-macros.h"

P_JSTR_BEGIN_DECLS

typedef enum {
	JSTR_ISALNUM = (1),
	JSTR_ISALPHA = (JSTR_ISALNUM << 1),
	JSTR_ISCNTRL = (JSTR_ISALPHA << 1),
	JSTR_ISDIGIT = (JSTR_ISCNTRL << 1),
	JSTR_ISGRAPH = (JSTR_ISDIGIT << 1),
	JSTR_ISLOWER = (JSTR_ISGRAPH << 1),
	JSTR_ISPRINT = (JSTR_ISLOWER << 1),
	JSTR_ISPUNCT = (JSTR_ISPRINT << 1),
	JSTR_ISSPACE = (JSTR_ISPUNCT << 1),
	JSTR_ISUPPER = (JSTR_ISSPACE << 1),
	JSTR_ISXDIGIT = (JSTR_ISUPPER << 1),
	JSTR_ISBLANK = (JSTR_ISXDIGIT << 1),
	/* [_A-Za-z0-9] */
	JSTR_ISWORD = (JSTR_ISBLANK << 1)
} jstr_ctype_ty;

JSTR_MAYBE_UNUSED
static const uint16_t pjstr_table_ctype[256] = {
	/* NUL, control codes */
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	/* \t */
	JSTR_ISCNTRL | JSTR_ISSPACE | JSTR_ISBLANK,
	/* \n \v \f \r */
	JSTR_ISCNTRL | JSTR_ISSPACE,
	JSTR_ISCNTRL | JSTR_ISSPACE,
	JSTR_ISCNTRL | JSTR_ISSPACE,
	JSTR_ISCNTRL | JSTR_ISSPACE,
	/* control codes */
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	JSTR_ISCNTRL,
	/* space */
	JSTR_ISPRINT | JSTR_ISSPACE | JSTR_ISBLANK,
	/* !"#$^&'()*+,-,/ */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	/* 0-9 */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISDIGIT | JSTR_ISXDIGIT | JSTR_ISWORD,
	/* :;<=>?@ */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	/* A-F */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISXDIGIT | JSTR_ISWORD,
	/* G-Z */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISUPPER | JSTR_ISWORD,
	/* [\]^_' */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	/* a-f */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISXDIGIT | JSTR_ISWORD,
	/* g-z */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISALNUM | JSTR_ISALPHA | JSTR_ISLOWER | JSTR_ISWORD,
	/* {|}~ */
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	JSTR_ISPRINT | JSTR_ISGRAPH | JSTR_ISPUNCT,
	/* DEL */
	JSTR_ISCNTRL,
};

JSTR_MAYBE_UNUSED
static const unsigned char pjstr_table_tolower[256] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	/* A-Z */
	65 - 'A' + 'a',
	66 - 'A' + 'a',
	67 - 'A' + 'a',
	68 - 'A' + 'a',
	69 - 'A' + 'a',
	70 - 'A' + 'a',
	71 - 'A' + 'a',
	72 - 'A' + 'a',
	73 - 'A' + 'a',
	74 - 'A' + 'a',
	75 - 'A' + 'a',
	76 - 'A' + 'a',
	77 - 'A' + 'a',
	78 - 'A' + 'a',
	79 - 'A' + 'a',
	80 - 'A' + 'a',
	81 - 'A' + 'a',
	82 - 'A' + 'a',
	83 - 'A' + 'a',
	84 - 'A' + 'a',
	85 - 'A' + 'a',
	86 - 'A' + 'a',
	87 - 'A' + 'a',
	88 - 'A' + 'a',
	89 - 'A' + 'a',
	90 - 'A' + 'a',
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
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
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	185,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	193,
	194,
	195,
	196,
	197,
	198,
	199,
	200,
	201,
	202,
	203,
	204,
	205,
	206,
	207,
	208,
	209,
	210,
	211,
	212,
	213,
	214,
	215,
	216,
	217,
	218,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
};

JSTR_MAYBE_UNUSED
static const unsigned char pjstr_table_toupper[256] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	/* a-z */
	97 - 'a' + 'A',
	98 - 'a' + 'A',
	99 - 'a' + 'A',
	100 - 'a' + 'A',
	101 - 'a' + 'A',
	102 - 'a' + 'A',
	103 - 'a' + 'A',
	104 - 'a' + 'A',
	105 - 'a' + 'A',
	106 - 'a' + 'A',
	107 - 'a' + 'A',
	108 - 'a' + 'A',
	109 - 'a' + 'A',
	110 - 'a' + 'A',
	111 - 'a' + 'A',
	112 - 'a' + 'A',
	113 - 'a' + 'A',
	114 - 'a' + 'A',
	115 - 'a' + 'A',
	116 - 'a' + 'A',
	117 - 'a' + 'A',
	118 - 'a' + 'A',
	119 - 'a' + 'A',
	120 - 'a' + 'A',
	121 - 'a' + 'A',
	122 - 'a' + 'A',
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
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
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	185,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	193,
	194,
	195,
	196,
	197,
	198,
	199,
	200,
	201,
	202,
	203,
	204,
	205,
	206,
	207,
	208,
	209,
	210,
	211,
	212,
	213,
	214,
	215,
	216,
	217,
	218,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
};

P_JSTR_END_DECLS

#endif /* JSTR_CTYPE_TABLE_H */
