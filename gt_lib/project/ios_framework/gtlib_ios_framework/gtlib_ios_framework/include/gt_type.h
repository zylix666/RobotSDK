/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_TYPE_H__
#define __GT_TYPE_H__

/* signed */
#define gt_char                 char
#define gt_int8                 signed char
#define gt_short                short
#define gt_int16                short
#define gt_int                  int
#define gt_int32                int
#define gt_long                 long
#define gt_longlong             long long

#if defined(GT_CONFIG_WINDOWS)
#define gt_int64                __int64
#else
#define gt_int64                long long
#endif

#define gt_float                float
#define gt_float32              float
#define gt_float64              double
#define gt_double               double
#define gt_bool                 int

/* unsigned */
#define gt_byte                 unsigned char 
#define gt_uchar                unsigned char
#define gt_uint8                unsigned char
#define gt_ushort               unsigned short
#define gt_uint16               unsigned short
#define gt_uint                 unsigned int
#define gt_uint32               unsigned int
#define gt_ulong                unsigned long
#define gt_ulonglong            unsigned long long
#define gt_uint64               unsigned long long
#define gt_constptr             const void*
typedef void*                   gt_ptr;
typedef size_t                  gt_size;
typedef unsigned char           gt_pixel8;
#define gt_wchar                wchar_t

#if defined(GT_CONFIG_32BIT)
#define gt_word                 unsigned short
#define gt_dword                unsigned int
#endif

#if defined(GT_CONFIG_VC)
typedef SSIZE_T gt_ssize;
#else
typedef key_t gt_key_t;
typedef ssize_t gt_ssize;
#endif

#define GT_STR_SIZE GT_CONFIG_PAGE_SIZE // 1 memory page for faster performance.

/* Define min and max constants for the fixed size numerical types. */
#define GT_CHAR_MAX       127
#define GT_CHAR_MIN	      (-128)

#define GT_INT8_MAX       127
#define GT_INT8_MIN	      (-128)

#define GT_UINT8_MAX	    255
#define GT_UINT8_MIN	    0

#define GT_INT16_MAX	    32767
#define GT_INT16_MIN	    (-32768)

#define GT_UINT16_MAX     65535
#define GT_UINT16_MIN     0

#define GT_INT32_MAX	    2147483647L
#define GT_INT32_MIN	    (-GT_INT32_MAX-1)

#define GT_UINT32_MAX     0xffffffffL
#define GT_UINT32_MIN     0

#if defined(GT_CONFIG_32BIT)
#define GT_INT_MAX	    2147483647L
#define GT_INT_MIN	    (-GT_INT_MAX-1)
#define GT_UINT_MAX	    0xffffffffL
#define GT_SIZE_MAX     0xffffffffL
#define GT_SSIZE_MAX    2147483647L
#elif defined(GT_CONFIG_64BIT)
#define GT_INT_MAX	    9223372036854775807LL
#define GT_INT_MIN	    (-GT_LONG_LONG_MAX-1)
#define GT_UINT_MAX	  (2ULL * LONG_LONG_MAX + 1)
#define GT_SIZE_MAX	    (2ULL * LONG_LONG_MAX + 1)
#define GT_SSIZE_MAX	  9223372036854775807LL
#endif

#define GT_UINT_MIN	    0
#define GT_SIZE_MIN 0
#define GT_SSIZE_MIN 0

#define GT_LONG_MAX       2147483647L
#define GT_LONG_MIN       (-GT_LONG_MAX-1)

#define GT_ULONG_MAX      0xffffffffUL
#define GT_ULONG_MIN      0

#define GT_INT64_MAX	9223372036854775807LL
#define GT_INT64_MIN	(-GT_INT64_MAX-1)

#define GT_UINT64_MAX	(2ULL * GT_INT64_MAX + 1)
#define GT_UINT64_MIN	0

#define GT_LONGLONG_MAX	9223372036854775807LL
#define GT_LONGLONG_MIN	(-GT_LONGLONG_MAX-1)

#define GT_ULONGLONG_MAX	(2ULL * GT_LONGLONG_MAX + 1)
#define GT_ULONGLONG_MIN	0

#define GT_FLOAT_MAX                    3.402823466e+38F
#define GT_FLOAT_MIN                   -3.402823466e+38F
#define GT_FLOAT_POSITIVE_MAX           3.402823466e+38F
#define GT_FLOAT_POSITIVE_MIN           1.175494351e-38F
#define GT_FLOAT_NEGTIVE_MAX           -1.175494351e-38F
#define GT_FLOAT_NEGTIVE_MIN           -3.402823466e+38F

#define GT_FLOAT32_MAX                  3.402823466e+38F
#define GT_FLOAT32_MIN                 -3.402823466e+38F
#define GT_FLOAT32_POSITIVE_MAX         3.402823466e+38F
#define GT_FLOAT32_POSITIVE_MIN         1.175494351e-38F
#define GT_FLOAT32_NEGTIVE_MAX         -1.175494351e-38F
#define GT_FLOAT32_NEGTIVE_MIN         -3.402823466e+38F

#define GT_FLOAT64_MAX                  1.7976931348623158e+308
#define GT_FLOAT64_MIN                 -1.7976931348623158e+308
#define GT_FLOAT64_POSITIVE_MAX         1.7976931348623158e+308
#define GT_FLOAT64_POSITIVE_MIN         2.2250738585072014e-308
#define GT_FLOAT64_NEGTIVE_MAX         -2.2250738585072014e-308 
#define GT_FLOAT64_NEGTIVE_MIN         -1.7976931348623158e+308

#define GT_DOUBLE_MAX                   1.7976931348623158e+308
#define GT_DOUBLE_MIN                  -1.7976931348623158e+308
#define GT_DOUBLE_POSITIVE_MAX          1.7976931348623158e+308
#define GT_DOUBLE_POSITIVE_MIN          2.2250738585072014e-308
#define GT_DOUBLE_NEGTIVE_MAX          -2.2250738585072014e-308 
#define GT_DOUBLE_NEGTIVE_MIN          -1.7976931348623158e+308

#define GT_TB 1099511627776LL
#define GT_GB 1073741824L
#define GT_MB 1048576L
#define GT_KB 1024

#ifndef	TRUE
#define	TRUE	 (1)  // !FALSE
#endif

#ifndef	FALSE
#define	FALSE	(0)
#endif

/* Char coding */
#define gt_ascii                unsigned char
#define gt_big5                 unsigned char
#define gt_gb2312               unsigned char
#define gt_utf8                 unsigned char
#define gt_utf16                unsigned short

/* New line */
#if defined(GT_CONFIG_WINDOWS)
#define GT_NL "\r\n"
#elif defined(GT_CONFIG_MAC)
#define GT_NL "\r"
#else
#define GT_NL "\n"
#endif

/* Axis */
typedef enum {
	GT_X = 0, GT_Y = 1, GT_Z = 2, GT_U = 3,
} GT_AXIS_t;

/* Boolean */
typedef enum {
	GT_UNKNOWN = -1, GT_FALSE = 0, GT_TRUE = 1,
} GT_BOOLEAN_t;

typedef enum {
	GT_OFF = 0, GT_ON = 1,
} GT_ON_OFF_t;

typedef enum {
	GT_NO = 0, GT_YES = 1,
} GT_YES_NO_t;

typedef enum {
	GT_COMPARE_UNKNOWN = -2, GT_LESS = -1, GT_EQUAL = 0, GT_GREAT = 1,
} GT_COMPARE_t;

typedef enum {
	GT_CW = 0,              // 0: Clockwise.
	GT_CCW,                 // 1: Counter clockwise.
} GT_CLOCK_DIRECTION_t;                // Circle direction.

typedef int (*GtCompare_f)(gt_constptr a, gt_constptr b);
typedef int (*GtCompareDataFunction)(gt_constptr a, gt_constptr b, gt_ptr user_data);

typedef struct {
	int x;
	int y;
} GtPoint_t;

typedef struct {
	int x;
	int y;
	int z;
} GtPoint3D_t;

typedef struct {
	int x;
	int y;
	int z;
	int u;
} GtPoint4D_t;

/* Callback function */
typedef int (*GtCallback_t)(int data);
typedef int (*GtCallbackData_t)(gt_ptr data, int dataSize);

/* Bit defination. */
#define GT_BIT_0                      0x1U
#define GT_BIT_1                      0x2U
#define GT_BIT_2                      0x4U
#define GT_BIT_3                      0x8U
#define GT_BIT_4                     0x10U
#define GT_BIT_5                     0x20U
#define GT_BIT_6                     0x40U
#define GT_BIT_7                     0x80U
#define GT_BIT_8                    0x100U
#define GT_BIT_9                    0x200U
#define GT_BIT_10                   0x400U
#define GT_BIT_11                   0x800U
#define GT_BIT_12                  0x1000U
#define GT_BIT_13                  0x2000U
#define GT_BIT_14                  0x4000U
#define GT_BIT_15                  0x8000U
#define GT_BIT_16                 0x10000U
#define GT_BIT_17                 0x20000U
#define GT_BIT_18                 0x40000U
#define GT_BIT_19                 0x80000U
#define GT_BIT_20                0x100000U
#define GT_BIT_21                0x200000U
#define GT_BIT_22                0x400000U
#define GT_BIT_23                0x800000U
#define GT_BIT_24               0x1000000U
#define GT_BIT_25               0x2000000U
#define GT_BIT_26               0x4000000U
#define GT_BIT_27               0x8000000U
#define GT_BIT_28              0x10000000U
#define GT_BIT_29              0x20000000U
#define GT_BIT_30              0x40000000U
#define GT_BIT_31              0x80000000U
#define GT_BIT_32             0x100000000UL
#define GT_BIT_33             0x200000000UL
#define GT_BIT_34             0x400000000UL
#define GT_BIT_35             0x800000000UL
#define GT_BIT_36            0x1000000000UL
#define GT_BIT_37            0x2000000000UL
#define GT_BIT_38            0x4000000000UL
#define GT_BIT_39            0x8000000000UL
#define GT_BIT_40           0x10000000000UL
#define GT_BIT_41           0x20000000000UL
#define GT_BIT_42           0x40000000000UL
#define GT_BIT_43           0x80000000000UL
#define GT_BIT_44          0x100000000000UL
#define GT_BIT_45          0x200000000000UL
#define GT_BIT_46          0x400000000000UL
#define GT_BIT_47          0x800000000000UL
#define GT_BIT_48         0x1000000000000UL
#define GT_BIT_49         0x2000000000000UL
#define GT_BIT_50         0x4000000000000UL
#define GT_BIT_51         0x8000000000000UL
#define GT_BIT_52        0x10000000000000UL
#define GT_BIT_53        0x20000000000000UL
#define GT_BIT_54        0x40000000000000UL
#define GT_BIT_55        0x80000000000000UL
#define GT_BIT_56       0x100000000000000UL
#define GT_BIT_57       0x200000000000000UL
#define GT_BIT_58       0x400000000000000UL
#define GT_BIT_59       0x800000000000000UL
#define GT_BIT_60      0x1000000000000000UL
#define GT_BIT_61      0x2000000000000000UL
#define GT_BIT_62      0x4000000000000000UL
#define GT_BIT_63      0x8000000000000000UL

#define GT_UNUSED(name) ((void)(name))

/* Remark for function call */
#define GT_IN
#define GT_OUT
#define GT_IN_OUT

#endif /* __GT_TYPE_H__ */
