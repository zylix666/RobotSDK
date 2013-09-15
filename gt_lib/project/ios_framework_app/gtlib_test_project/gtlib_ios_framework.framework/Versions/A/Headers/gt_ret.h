/*
 Copyright (c) LEADERG Co., Ltd. All rights reserved.
 Free of charge for noncommercial purposes.
 Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.
 */

#ifndef __GT_RET_H__
#define __GT_RET_H__

#ifdef __cplusplus
extern "C" 
{
#endif
   
/* Return value */
typedef enum {
    GT_RET_MAX                    =  4,      // Maximum.
    GT_OK_DONE_ALREADY            =  3,      // Done already.
    GT_OK_INIT_ALREADY            =  2,      // Init already.
    GT_OK                         =  1,      // Success.
    GT_ERROR                      =  0,      // Error.
    GT_ERROR_NO_MATCH             = -1,      // No match parameter.
    GT_ERROR_NOT_DEFINED          = -2,      // Not defined yet.
    GT_ERROR_INVALID_ACC_TYPE     = -3,      // Invalid acceleration type.
    GT_ERROR_OPEN_FILE            = -4,      // Open file fail.
    GT_ERROR_SET_LOCALE           = -5,      // Set locale fail.
    GT_ERROR_AXIS_COUNT           = -6,      // Axis count mismatch.
    GT_ERROR_DEVICE_ID            = -7,      // Device id mismatch.
    GT_ERROR_DEVICE_NAME          = -8,      // Device name mismatch.
    GT_ERROR_DEVICE_TYPE          = -9,      // Device type mismatch.
    GT_ERROR_BUSY                 = -10,     // Busy and in use.
    GT_ERROR_KEY                  = -11,     // Key error.
    GT_ERROR_NO_MEMORY            = -12,     // Not enough memory.
    GT_ERROR_RANGE                = -13,     // Out of range.
    GT_ERROR_MOTION_LOCK          = -14,     // Motion is locked.
    GT_ERROR_PARAM                = -15,     // Parameter is wrong.
    GT_ERROR_NOT_OPENED           = -16,     // File is not opened.
    GT_ERROR_NOT_INIT             = -17,     // Not init.
    GT_ERROR_NOT_SUPPORT_FUNCTION = -18,     // Not support this function yet.
    GT_ERROR_NOT_SUPPORT_FORMAT   = -19,     // Not support this format yet.
    GT_ERROR_NULL_POINTER         = -20,     // NULL pointer.
    GT_ERROR_NOT_CLOSED           = -21,     // File is not closed.
    GT_ERROR_EMPTY                = -22,     // Empty.
    GT_ERROR_NOT_SUPPORT_DEVICE   = -23,     // Not support this device yet.
    GT_ERROR_BUFFER_OVERRUN       = -24,     // Buffer overrun.
    GT_ERROR_MAGIC                = -25,     // Magic number is wrong.
    GT_ERROR_VERSION              = -26,     // Version is wrong.
    GT_ERROR_NOT_SUPPORT          = -27,     // Not support.
    GT_ERROR_NOT_SUPPORT_TYPE     = -28,     // Not support type.
    GT_ERROR_ZERO                 = -29,     // Error because of "Zero" value.
    GT_ERROR_TIMEOUT              = -30,     // Timeout.
    GT_ERROR_LICENSE              = -31,     // License.
    GT_ERROR_FULL                 = -32,     // Full.
    GT_ERROR_ALREADY_OPENED       = -33,     // Already opened.
    GT_ERROR_ALIGNMENT            = -34,     // Alignment.
    GT_ERROR_CRC                  = -35,     // CRC.
    GT_ERROR_TYPE                 = -36,     // Type.
    GT_ERROR_NOT_FOUND            = -37,     // No found.
    GT_ERROR_COLOR_MODE           = -38,     // Color mode.
    GT_ERROR_NEW_LOCK             = -39,     // New lock.
    GT_ERROR_NEW_OBJECT           = -40,     // New object.
    GT_ERROR_PARAMETER_0          = -100,    // Parameter 0 is wrong.
    GT_ERROR_PARAMETER_1          = -101,    // Parameter 1 is wrong.
    GT_ERROR_PARAMETER_2          = -102,    // Parameter 2 is wrong.
    GT_ERROR_PARAMETER_3          = -103,    // Parameter 3 is wrong.
    GT_ERROR_PARAMETER_4          = -104,    // Parameter 4 is wrong.
    GT_ERROR_PARAMETER_5          = -105,    // Parameter 5 is wrong.
    GT_ERROR_PARAMETER_6          = -106,    // Parameter 6 is wrong.
    GT_ERROR_PARAMETER_7          = -107,    // Parameter 7 is wrong.
    GT_ERROR_PARAMETER_8          = -108,    // Parameter 8 is wrong.
    GT_ERROR_PARAMETER_9          = -109,    // Parameter 9 is wrong.
    GT_ERROR_PARAMETER_10         = -110,    // Parameter 10 is wrong.
    GT_RET_MIN                    = -111     // Minimum.
} GT_RET_t;

#ifdef __cplusplus
}
#endif

#endif /* __GT_RET_H__ */
