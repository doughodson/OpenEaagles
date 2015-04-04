//------------------------------------------------------------------------------
// Standard Event Tokens
//
// ### DO NOT include this file; this file is included as part of
//     Component.h (i.e., the 'enum' is contained in Component )
//
// ### This file MUST match eventTokens.epp
//
//    'Key' events (values 0 to MAX_KEY_EVENT) are passed up the container tree if not used (see Component::event()).
//       Key values   0 to 127 are keyboard (ASCII) events
//       Key values 128 to (USER_KEY_EVENT-1) reserved for the OpenEaagles toolkit
//       Key values USER_KEY_EVENT to MAX_KEY_EVENT available for user events
//
//    Non-key events --
//       Event values (MAX_KEY_EVENT+1) to (USER_EVENTS-1) reserved for the OpenEaagles toolkit
//       Event values above USER_EVENT are available for user events
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Event_Tokens_H__
#define __Eaagles_Basic_Event_Tokens_H__

// ---
// Key events -- Are passed up to an object's container if not used.
// ---

enum {
// Keyboard events
    CLR_KEY               =   1,
    BACK_SPACE            =   8,
    TAB_KEY               =   9,
    ENTER_KEY             =  13,
    ESC_KEY               =  27,
    FORWARD_SPACE         =  28,
    UP_ARROW_KEY          =  30,
    DOWN_ARROW_KEY        =  31,
    SPACE_BAR             =  32,  // Helpful in case don't want to test against ' '.
    DELETE_KEY            = 127,
    BUTTON_HIT            = 128,  // Button name passed as String
    INPUT_RIGHT_EDGE      = 129,
    INPUT_LEFT_EDGE       = 130,
    ON_SINGLE_CLICK       = 131,
    ON_DOUBLE_CLICK       = 132,
    ON_CANCEL             = 133,
    ON_MOTION             = 134,
    LEFT_ARROW_KEY        = BACK_SPACE,
    RIGHT_ARROW_KEY       = FORWARD_SPACE,

// "Special" Keyboard events (#s in the comments are according to GlutDisplay::specialFuncCB):
    PAGE_UP_KEY           = 136,  // GLUT_KEY_PAGE_UP    = fltk::PageUpKey   = 104 (='h')
    PAGE_DOWN_KEY         = 137,  // GLUT_KEY_PAGE_DOWN  = fltk::PageDownKey = 105 (='i')
    HOME_KEY              = 138,  // GLUT_KEY_HOME       = fltk::HomeKey     = 106 (='j')
    END_KEY               = 139,  // GLUT_KEY_END        = fltk::EndKey      = 107 (='k')
    INSERT_KEY            = 140,  // GLUT_KEY_INSERT     = fltk::InsertKey   = 108 (='l')

// Function keys
    F1_KEY                = 141,
    F2_KEY                = 142,
    F3_KEY                = 143,
    F4_KEY                = 144,
    F5_KEY                = 145,
    F6_KEY                = 146,
    F7_KEY                = 147,
    F8_KEY                = 148,
    F9_KEY                = 149,
    F10_KEY               = 150,
    F11_KEY               = 151,
    F12_KEY               = 152,

// MFD bezel keys
    OSB_T1                = 200,  // Left(T1) to right (T10)
    OSB_T2                = 201,
    OSB_T3                = 202,
    OSB_T4                = 203,
    OSB_T5                = 204,
    OSB_T6                = 205,
    OSB_T7                = 206,
    OSB_T8                = 207,
    OSB_T9                = 208,
    OSB_T10               = 209,

    OSB_R1                = 210,  // top (R1) to bottom (R10)
    OSB_R2                = 211,
    OSB_R3                = 212,
    OSB_R4                = 213,
    OSB_R5                = 214,
    OSB_R6                = 215,
    OSB_R7                = 216,
    OSB_R8                = 217,
    OSB_R9                = 218,
    OSB_R10               = 219,

    OSB_B1                = 220,  // Left(B1) to right (B10)
    OSB_B2                = 221,
    OSB_B3                = 222,
    OSB_B4                = 223,
    OSB_B5                = 224,
    OSB_B6                = 225,
    OSB_B7                = 226,
    OSB_B8                = 227,
    OSB_B9                = 228,
    OSB_B10               = 229,

    OSB_L1                = 230,  // top (L1) to bottom (L10)
    OSB_L2                = 231,
    OSB_L3                = 232,
    OSB_L4                = 233,
    OSB_L5                = 234,
    OSB_L6                = 235,
    OSB_L7                = 236,
    OSB_L8                = 237,
    OSB_L9                = 238,
    OSB_L10               = 239,


    USER_KEY_EVENT        = 512,

    MAX_KEY_EVENT         = 999,

// --- --- ---
// Regular events -- are not passed up to the container
// --- --- ---

// ---
// Basic events
// ---
    SHUTDOWN_EVENT        = 1001,  // Shutdown notification event

// ---
// BasicGL events
// ---
    ON_ENTRY              = 1201,      // Entry (active)
    ON_EXIT               = 1202,      // Exit  (inactive)
    ON_RETURN             = 1203,      // Return
    SELECT                = 1204,      // Subcomponent selection

    UPDATE_INSTRUMENTS    = 1210,      // For all instruments to update themselves with new data
    UPDATE_VALUE          = 1211,
    UPDATE_VALUE1         = UPDATE_VALUE,
    UPDATE_VALUE2         = 1212,
    UPDATE_VALUE3         = 1213,
    UPDATE_VALUE4         = 1214,
    UPDATE_VALUE5         = 1215,
    UPDATE_VALUE6         = 1216,
    UPDATE_VALUE7         = 1217,
    UPDATE_VALUE8         = 1218,
    UPDATE_VALUE9         = 1219,

    SET_VISIBILITY        = 1221,
    SET_COLOR             = 1222,
    SET_LINEWIDTH         = 1223,
    SET_POSITION          = 1224,
    SET_WIDTH             = 1225,
    SET_HIGHLIGHT         = 1226,
    SET_UNDERLINE         = 1227,
    SET_REVERSED          = 1228,
    SET_SPECIAL           = 1229,
    SET_JUSTIFICATION     = 1230,
    SET_FLASHRATE         = 1231,
    SET_LINE              = 1232,
    SET_COLUMN            = 1233,
    SET_MATERIAL          = 1234,
    SET_TEXTURE           = 1235,

// ---
// Simulation events
// ---
    RESET_EVENT           = 1301,  // Reset event
    FREEZE_EVENT          = 1302,  // Freeze event: passes requested freeze state as Boolean
    FREEZE_EVENT_ALL      = 1303,  // Freeze all event: passes requested freeze state as Boolean
    KILL_EVENT            = 1304,  // kill event: passes optional 'killed by' player
    CRASH_EVENT           = 1305,  // crash event: passes 'collided with' player or zero if terrain
    JETTISON_EVENT        = 1306,  // Jettison external store event
    RF_EMISSION           = 1307,  // R/F emission event
    RF_EMISSION_RETURN    = 1308,  // R/F emission return event
    DESIGNATOR_EVENT      = 1309,  // Passes Designator message
    DATALINK_MESSAGE      = 1310,  // Passes Datalink message

    ON_OWNSHIP_CONNECT    = 1311,  // To new 'ownship' player
    ON_OWNSHIP_DISCONNECT = 1312,  // To old 'ownship' player at disconnection

    SCAN_START            = 1313,  // Scan pattern started
    SCAN_END              = 1314,  // Scan pattern ended

    WPN_RELOAD            = 1315,  // Reload weapons

    RF_REFLECTED_EMISSION  = 1316, // Reflected R/F emission event
    RF_REFLECTIONS_REQUEST = 1317, // Request reflected R/F emissions
    RF_REFLECTIONS_CANCEL = 1318,  // Cancel reflected R/F emissions

    IR_QUERY              = 1319,  // IR query message
    IR_QUERY_RETURN       = 1320,  // IR query return message
    SAT_COMM_MSG          = 1321,  // Sat Comm message
    DE_EMISSION           = 1322,  // Directed Energy emission message
    REFUEL_EVENT          = 1323,  // Refuel event

// ---
// HOTAS switches
// ---
    SENSOR_RTS            = 1400,  // Sensor - Return to search
    TGT_STEP_EVENT        = 1401,  // Target step (reject)
    TGT_DESIGNATE         = 1402,  // Target designate

    WPN_REL_EVENT         = 1405,  // Passes requested switch state as Boolean
                                         // or no argument for one-shot event

    TRIGGER_SW_EVENT      = 1406,  // Passes requested switch state as Boolean
                                         // or no argument for one-shot event

    TMS_FWD_EVENT         = 1407,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    TMS_AFT_EVENT         = 1408,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    TMS_LEFT_EVENT        = 1409,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    TMS_RIGHT_EVENT       = 1410,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event

    DMS_FWD_EVENT         = 1411,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    DMS_AFT_EVENT         = 1412,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    DMS_LEFT_EVENT        = 1413,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    DMS_RIGHT_EVENT       = 1414,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event

    CMS_FWD_EVENT         = 1415,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    CMS_AFT_EVENT         = 1416,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    CMS_LEFT_EVENT        = 1417,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    CMS_RIGHT_EVENT       = 1418,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event

    PINKY_SW_EVENT        = 1419,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    NWS_SW_EVENT          = 1420,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event
    CURSOR_ZERO_EVENT     = 1421,  // Passes requested switch state as Boolean
                                        // or no argument for one-shot event

    CURSOR_X_EVENT        = 1422,  // Passes requested position state as LCreal
    CURSOR_Y_EVENT        = 1423,  // Passes requested position state as LCreal

    USER_EVENTS           = 2000   // First user defined event

};

#endif
