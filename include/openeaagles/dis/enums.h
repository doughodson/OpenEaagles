//--------------------------------------------------------------
// IEEE Standard for Distributed Interactive Simulation (DIS)
// Application Protocols - Enumerations for DIS PDU's
// Standard: 1278.1a
//--------------------------------------------------------------
#ifndef __Eaagles_Network_Dis_Enum_H__
#define __Eaagles_Network_Dis_Enum_H__

namespace Eaagles {
namespace Network {
namespace Dis {

//--------------------------------------------------------------
// Beam Function 
//--------------------------------------------------------------

#define BEAM_FUNCTION_OTHER				0
#define BEAM_FUNCTION_SEARCH				1
#define BEAM_FUNCTION_HEIGHT_FINDER			2
#define BEAM_FUNCTION_ACQUISITION			3
#define BEAM_FUNCTION_TRACKING				4
#define BEAM_FUNCTION_ACQUISITION_TRACK			5
#define BEAM_FUNCTION_COMMAND_GUIDANCE			6
#define BEAM_FUNCTION_ILLUMINATION			7
#define BEAM_FUNCTION_RANGE_FINDER			8
#define BEAM_FUNCTION_MISSILE_BEACON			9
#define BEAM_FUNCTION_MISSILE_FUZE			10
#define BEAM_FUNCTION_ACTIVE_MISSILE_SEEKER		11
#define BEAM_FUNCTION_JAMMER				12
#define BEAM_FUNCTION_IFF				13
#define BEAM_FUNCTION_NAVIGATIONAL			14
#define BEAM_FUNCTION_METEROLOGICAL			15
#define BEAM_FUNCTION_DATA_TRANSMISSION			16
#define BEAM_FUNCTION_NAVIGATIONAL_DIRECTIONAL		17

//--------------------------------------------------------------
// System Function
//--------------------------------------------------------------

#define SYSTEM_FUNCTION_OTHER                             0
#define SYSTEM_FUNCTION_MULTI_FUNCTION                    1
#define SYSTEM_FUNCTION_EW                                2
#define SYSTEM_FUNCTION_HEIGHT_FINDING                    3
#define SYSTEM_FUNCTION_FIRE_CONTROL                      4
#define SYSTEM_FUNCTION_ACQUISITION                       5
#define SYSTEM_FUNCTION_TRACKING                          6
#define SYSTEM_FUNCTION_GUIDANCE                          7
#define SYSTEM_FUNCTION_FIRE_POINT_LOCATION               8
#define SYSTEM_FUNCTION_RANGING                           9
#define SYSTEM_FUNCTION_RADAR_ALTIMETER                   10
#define SYSTEM_FUNCTION_IMAGING                           11
#define SYSTEM_FUNCTION_MOTION_DETECTION                  12
#define SYSTEM_FUNCTION_NAVIGATION                        13
#define SYSTEM_FUNCTION_AAA_FIRE_CONTROL                  17
#define SYSTEM_FUNCTION_DATA_TRANSMISSION                 29
#define SYSTEM_FUNCTION_IFF                               34
#define SYSTEM_FUNCTION_ALL_OFF                           200
#define SYSTEM_FUNCTION_SYSTEM_OFF                        201


//--------------------------------------------------------------
// Detonation Result 
//--------------------------------------------------------------

#define DETONATION_RESULT_OTHER				0
#define DETONATION_RESULT_ENTITY_IMPACT			1
#define DETONATION_RESULT_ENTITY_PROXIMATE_DETONATION	2
#define DETONATION_RESULT_GROUND_IMPACT			3
#define DETONATION_RESULT_GROUND_PROXIMATE_DETONATION	4
#define DETONATION_RESULT_DETONATION			5
#define DETONATION_RESULT_NONE				6
#define DETONATION_RESULT_HE_HIT_SM			7
#define DETONATION_RESULT_HE_HIT_MED			8
#define DETONATION_RESULT_HE_HIT_LARGE			9
#define DETONATION_RESULT_ARMOR_PIERCING		10
#define DETONATION_RESULT_DIRT_BLAST_SM			11
#define DETONATION_RESULT_DIRT_BLAST_MED		12
#define DETONATION_RESULT_DIRT_BLAST_LARGE		13
#define DETONATION_RESULT_WATER_BLAST_SM		14
#define DETONATION_RESULT_WATER_BLAST_MED		15
#define DETONATION_RESULT_WATER_BLAST_LARGE		16
#define DETONATION_RESULT_AIR_HIT			17
#define DETONATION_RESULT_BUILDING_HIT_SM		18
#define DETONATION_RESULT_BUILDING_HIT_MED		19
#define DETONATION_RESULT_BUILDING_HIT_LARGE		20
#define DETONATION_RESULT_MINE_CLEARING_LINE		21
#define DETONATION_RESULT_ENVIRONMENT_OBJECT_IMPACT	22
#define DETONATION_RESULT_ENVIRONMENT_OBJECT_PROX	23
#define DETONATION_RESULT_WATER_IMPACT			24
#define DETONATION_RESULT_AIR_BURST			25

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
