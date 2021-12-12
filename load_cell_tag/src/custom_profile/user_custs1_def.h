/**
 ****************************************************************************************
 *
 * @file user_custs1_def.h
 *
 * @brief Custom Server 1 (CUSTS1) profile database definitions.
 *
 * Copyright (C) 2016-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_DEF_H_
#define _USER_CUSTS1_DEF_H_

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom Server 1 (CUSTS1) profile database definitions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm_db_128.h"
#include "user_empty_peripheral_template.h"

/*
 * DEFINES
 ****************************************************************************************
 */

// Service 1 of the custom server 1
#define DEF_SVC1_UUID_128                {0x59, 0x5a, 0x08, 0xe4, 0x86, 0x2a, 0x9e, 0x8f, 0xe9, 0x11, 0xbc, 0x7c, 0x98, 0x43, 0x42, 0x18}

#define DEF_SVC1_LC_VAL_UUID_128         {0xAB, 0xEE, 0xFF, 0xC0, 0xEE, 0xFF, 0xC0, 0xEE, 0xFF, 0xC0, 0xEE, 0xFF, 0xC0, 0xEE, 0xFF, 0xC0}
#define DEF_SVC1_LC_BASE_UUID_128        {0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0xAB, 0xCD, 0xEF, 0xFE}
#define DEF_SVC1_LC_NUM_VALS_UUID_128    {0xAD, 0xDE, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA}
#define DEF_SVC1_LC_TS_UUID_128     		 {0xBE, 0xBA, 0xDD, 0xBA, 0xBE, 0xBA, 0xDD, 0xBA, 0xBE, 0xBA, 0xDD, 0xBA, 0xBE, 0xBA, 0xDD, 0xBA}

#define DEF_SVC1_CTRL_POINT_UUID_128     {0x20, 0xEE, 0x8D, 0x0C, 0xE1, 0xF0, 0x4A, 0x0C, 0xB3, 0x25, 0xDC, 0x53, 0x6A, 0x68, 0x86, 0x2D}
#define DEF_SVC1_BUTTON_STATE_UUID_128   {0x9E, 0xE7, 0xBA, 0x08, 0xB9, 0xA9, 0x48, 0xAB, 0xA1, 0xAC, 0x03, 0x1C, 0x2E, 0x0D, 0x29, 0x6C}

#define DEF_SVC1_LC_VAL_CHAR_LEN         4
#define DEF_SVC1_LC_BASE_CHAR_LEN        4
#define DEF_SVC1_LC_NUM_VALS_CHAR_LEN    4
#define DEF_SVC1_LC_TS_CHAR_LEN     		 4
#define DEF_SVC1_CTRL_POINT_CHAR_LEN     1
#define DEF_SVC1_BUTTON_STATE_CHAR_LEN   1

#define DEF_SVC1_LC_VAL_USER_DESC            "Load Cell Value"
#define DEF_SVC1_LC_BASE_USER_DESC           "Load Cell Baseline Value"
#define DEF_SVC1_LC_NUM_VALS_USER_DESC       "Number of Load Cell Values Read"
#define DEF_SVC1_LC_TS_USER_DESC        		 "Load Cell Timestamp Value"
#define DEF_SVC1_CONTROL_POINT_USER_DESC     "Control Point"
#define DEF_SVC1_BUTTON_STATE_USER_DESC      "Button State"

/// Custom1 Service Data Base Characteristic enum
enum
{
    // Custom Service 1
    SVC1_IDX_SVC = 0,

	  SVC1_IDX_LC_VAL_CHAR,
    SVC1_IDX_LC_VAL_VAL,
    SVC1_IDX_LC_VAL_USER_DESC,
	
		SVC1_IDX_LC_BASE_CHAR,
    SVC1_IDX_LC_BASE_VAL,
    SVC1_IDX_LC_BASE_USER_DESC,
	
		SVC1_IDX_LC_NUM_VALS_CHAR,
    SVC1_IDX_LC_NUM_VALS_VAL,
    SVC1_IDX_LC_NUM_VALS_USER_DESC,
	
		SVC1_IDX_LC_TS_CHAR,
    SVC1_IDX_LC_TS_VAL,
    SVC1_IDX_LC_TS_USER_DESC,
	
    SVC1_IDX_CONTROL_POINT_CHAR,
    SVC1_IDX_CONTROL_POINT_VAL,
    SVC1_IDX_CONTROL_POINT_USER_DESC,

    SVC1_IDX_BUTTON_STATE_CHAR,
    SVC1_IDX_BUTTON_STATE_VAL,
    SVC1_IDX_BUTTON_STATE_NTF_CFG,
    SVC1_IDX_BUTTON_STATE_USER_DESC,

    CUSTS1_IDX_NB
};

/// @} USER_CONFIG

#endif // _USER_CUSTS1_DEF_H_
