/** \file
 * The SPWaW Library - SPWaW OOB type codes.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <spwawlib_spwoob_types.h>



/*! SPWAW OOB unit class */
static const char *SPWOOB_UCLASS_names[SPWOOB_UCLASS_LIMIT+1] = {
	"infantry",                                  		/*!< SPWOOB_UCLASS_INF    */
	"cavalry",                                   		/*!< SPWOOB_UCLASS_CAV    */
	"armor",                                     		/*!< SPWOOB_UCLASS_ARM    */
	"APC",                                       		/*!< SPWOOB_UCLASS_APC    */
	"artillery",                                 		/*!< SPWOOB_UCLASS_ART    */
	"anti-tank",                                 		/*!< SPWOOB_UCLASS_AT     */
	"anti-air",                                  		/*!< SPWOOB_UCLASS_AA     */
	"marine unit",                               		/*!< SPWOOB_UCLASS_MAR    */
	"recon",                                     		/*!< SPWOOB_UCLASS_REC    */
	"C&C",                                       		/*!< SPWOOB_UCLASS_CC     */
	"transport",                                 		/*!< SPWOOB_UCLASS_TRN    */
	"air force",                                 		/*!< SPWOOB_UCLASS_AIR    */
	"navy",                                      		/*!< SPWOOB_UCLASS_NVY    */
	"fortification",                             		/*!< SPWOOB_UCLASS_FRT    */
	"other",                                     		/*!< SPWOOB_UCLASS_OTH    */
};

/*! SPWAW OOB unit class lookup function
 *
 * \param e	SPWAW OOB unit class
 * \return	Pointer to const string with SPWAW OOB unit class name
 */
const char *
SPWOOB_UCLASS_lookup (SPWOOB_UCLASS e)
{
	return (SPWOOB_UCLASS_names[e]);
}



/*! SPWAW OOB unit class: code strings */
static const char *SPWOOB_UCLASS_codes[SPWOOB_UCLASS_LIMIT+1] = {
	"INF",                                       		/*!< SPWOOB_UCLASS_INF    */
	"CAV",                                       		/*!< SPWOOB_UCLASS_CAV    */
	"ARM",                                       		/*!< SPWOOB_UCLASS_ARM    */
	"APC",                                       		/*!< SPWOOB_UCLASS_APC    */
	"ART",                                       		/*!< SPWOOB_UCLASS_ART    */
	"AT",                                        		/*!< SPWOOB_UCLASS_AT     */
	"AA",                                        		/*!< SPWOOB_UCLASS_AA     */
	"MAR",                                       		/*!< SPWOOB_UCLASS_MAR    */
	"REC",                                       		/*!< SPWOOB_UCLASS_REC    */
	"CC",                                        		/*!< SPWOOB_UCLASS_CC     */
	"TRN",                                       		/*!< SPWOOB_UCLASS_TRN    */
	"AIR",                                       		/*!< SPWOOB_UCLASS_AIR    */
	"NVY",                                       		/*!< SPWOOB_UCLASS_NVY    */
	"FRT",                                       		/*!< SPWOOB_UCLASS_FRT    */
	"OTH",                                       		/*!< SPWOOB_UCLASS_OTH    */
};

/*! SPWAW OOB unit class code lookup function
 *
 * \param e	SPWAW OOB unit class
 * \return	pointer to const string with SPWAW OOB unit class code
 */
const char *
SPWOOB_UCLASS_lookup_code (SPWOOB_UCLASS e)
{
	return (SPWOOB_UCLASS_codes[e]);
}



/*! SPWAW OOB weapon class */
static const char *SPWOOB_WCLASS_names[SPWOOB_WCLASS_LIMIT+1] = {
	"SPWOOB_WCLASS_UNKNOWN",                     		/*!< SPWOOB_WCLASS_UNKNOWN      */
	"SPWOOB_WCLASS_INF_PRM",                     		/*!< SPWOOB_WCLASS_INF_PRM      */
	"SPWOOB_WCLASS_INF_SEC",                     		/*!< SPWOOB_WCLASS_INF_SEC      */
	"SPWOOB_WCLASS_TEAM",                        		/*!< SPWOOB_WCLASS_TEAM         */
	"SPWOOB_WCLASS_AA",                          		/*!< SPWOOB_WCLASS_AA           */
	"SPWOOB_WCLASS_SML_GUN",                     		/*!< SPWOOB_WCLASS_SML_GUN      */
	"SPWOOB_WCLASS_MDM_GUN",                     		/*!< SPWOOB_WCLASS_MDM_GUN      */
	"SPWOOB_WCLASS_LRG_GUN",                     		/*!< SPWOOB_WCLASS_LRG_GUN      */
	"SPWOOB_WCLASS_FLAME_INF",                   		/*!< SPWOOB_WCLASS_FLAME_INF    */
	"SPWOOB_WCLASS_FLAME_VHC",                   		/*!< SPWOOB_WCLASS_FLAME_VHC    */
	"SPWOOB_WCLASS_NAVAL",                       		/*!< SPWOOB_WCLASS_NAVAL        */
	"SPWOOB_WCLASS_AIRCRAFT",                    		/*!< SPWOOB_WCLASS_AIRCRAFT     */
	"SPWOOB_WCLASS_SAM",                         		/*!< SPWOOB_WCLASS_SAM          */
	"SPWOOB_WCLASS_AT",                          		/*!< SPWOOB_WCLASS_AT           */
	"SPWOOB_WCLASS_LRG_CLSTR",                   		/*!< SPWOOB_WCLASS_LRG_CLSTR    */
	"SPWOOB_WCLASS_SML_CLSTR",                   		/*!< SPWOOB_WCLASS_SML_CLSTR    */
	"SPWOOB_WCLASS_ARM",                         		/*!< SPWOOB_WCLASS_ARM          */
	"SPWOOB_WCLASS_ASM",                         		/*!< SPWOOB_WCLASS_ASM          */
	"SPWOOB_WCLASS_NAPALM",                      		/*!< SPWOOB_WCLASS_NAPALM       */
	"*unknown value*"                            		/*!< SPWOOB_WCLASS__NONE        */
};

/*! SPWAW OOB weapon class lookup function
 *
 * \param e	SPWAW OOB weapon class
 * \return	pointer to const string with SPWAW OOB weapon class name
 */
const char *
SPWOOB_WCLASS_lookup (SPWOOB_WCLASS e) {
	return (SPWOOB_WCLASS_names[e]);
}

/*! SPWAW OOB weapon class translation function
 *
 * \param r	raw SPWAW OOB weapon class value
 * \return	SPWAW OOB weapon class
 */
SPWOOB_WCLASS
SPWOOB_WCLASS_xlt (BYTE r)
{
	SPWOOB_WCLASS	e;

	switch (r) {
		case 0:
			e = SPWOOB_WCLASS_UNKNOWN;
			break;
		case 1:
			e = SPWOOB_WCLASS_INF_PRM;
			break;
		case 2:
			e = SPWOOB_WCLASS_INF_SEC;
			break;
		case 3:
			e = SPWOOB_WCLASS_TEAM;
			break;
		case 4:
			e = SPWOOB_WCLASS_AA;
			break;
		case 5:
			e = SPWOOB_WCLASS_SML_GUN;
			break;
		case 6:
			e = SPWOOB_WCLASS_MDM_GUN;
			break;
		case 7:
			e = SPWOOB_WCLASS_LRG_GUN;
			break;
		case 8:
			e = SPWOOB_WCLASS_FLAME_INF;
			break;
		case 9:
			e = SPWOOB_WCLASS_FLAME_VHC;
			break;
		case 10:
			e = SPWOOB_WCLASS_NAVAL;
			break;
		case 11:
			e = SPWOOB_WCLASS_AIRCRAFT;
			break;
		case 12:
			e = SPWOOB_WCLASS_SAM;
			break;
		case 13:
			e = SPWOOB_WCLASS_AT;
			break;
		case 14:
			e = SPWOOB_WCLASS_LRG_CLSTR;
			break;
		case 15:
			e = SPWOOB_WCLASS_SML_CLSTR;
			break;
		case 16:
			e = SPWOOB_WCLASS_ARM;
			break;
		case 17:
			e = SPWOOB_WCLASS_ASM;
			break;
		case 18:
			e = SPWOOB_WCLASS_NAPALM;
			break;
		default:
			e = SPWOOB_WCLASS__NONE;
			break;
	}
	return (e);
}



/*! SPWAW OOB unit type */
static const char *SPWOOB_UTYPE_names[SPWOOB_UTYPE_LIMIT+1] = {
	"light infantry",                            		/*!< SPWOOB_UTYPE_LT_INF         */
	"infantry",                                  		/*!< SPWOOB_UTYPE_INF            */
	"medium infantry",                           		/*!< SPWOOB_UTYPE_MD_INF         */
	"heavy infantry",                            		/*!< SPWOOB_UTYPE_HV_INF         */
	"motorized infantry",                        		/*!< SPWOOB_UTYPE_MOT_INF        */
	"armored infantry",                          		/*!< SPWOOB_UTYPE_ARM_INF        */
	"engineers",                                 		/*!< SPWOOB_UTYPE_ENG            */
	"motorized engineers",                       		/*!< SPWOOB_UTYPE_MOT_ENG        */
	"armored engineers",                         		/*!< SPWOOB_UTYPE_ARM_ENG        */
	"machine gun",                               		/*!< SPWOOB_UTYPE_MG             */
	"heavy machine gun",                         		/*!< SPWOOB_UTYPE_HV_MG          */
	"ski troops",                                		/*!< SPWOOB_UTYPE_SKI            */
	"airborne infantry",                         		/*!< SPWOOB_UTYPE_AIR_INF        */
	"bicycle infantry",                          		/*!< SPWOOB_UTYPE_BCYCLE         */
	"elite infantry",                            		/*!< SPWOOB_UTYPE_ELITE          */
	"sniper",                                    		/*!< SPWOOB_UTYPE_SNIPER         */
	"special forces",                            		/*!< SPWOOB_UTYPE_SPECOP         */
	"guerilla forces",                           		/*!< SPWOOB_UTYPE_GUERILLA       */
	"conscripts",                                		/*!< SPWOOB_UTYPE_CONSCR         */
	"second line infantry",                      		/*!< SPWOOB_UTYPE_SLINF          */
	"cavalry",                                   		/*!< SPWOOB_UTYPE_CAV            */
	"armed halftrack",                           		/*!< SPWOOB_UTYPE_AHT            */
	"light tank",                                		/*!< SPWOOB_UTYPE_LT_TNK         */
	"medium tank",                               		/*!< SPWOOB_UTYPE_MD_TNK         */
	"heavy tank",                                		/*!< SPWOOB_UTYPE_HV_TNK         */
	"very heavy tank",                           		/*!< SPWOOB_UTYPE_VHV_TNK        */
	"main battle tank",                          		/*!< SPWOOB_UTYPE_MB_TNK         */
	"cruiser tank",                              		/*!< SPWOOB_UTYPE_CR_TNK         */
	"close support tank",                        		/*!< SPWOOB_UTYPE_CS_TNK         */
	"infantry tank",                             		/*!< SPWOOB_UTYPE_INF_TNK        */
	"mineclear tank",                            		/*!< SPWOOB_UTYPE_MINECLR_TNK    */
	"engineer tank",                             		/*!< SPWOOB_UTYPE_ENG_TNK        */
	"flame tank",                                		/*!< SPWOOB_UTYPE_FLM_TNK        */
	"special tank",                              		/*!< SPWOOB_UTYPE_SPEC_TNK       */
	"lend lease tank",                           		/*!< SPWOOB_UTYPE_LL_TNK         */
	"captured tank",                             		/*!< SPWOOB_UTYPE_CAPT_TNK       */
	"assault gun",                               		/*!< SPWOOB_UTYPE_ASSAULT        */
	"halftrack",                                 		/*!< SPWOOB_UTYPE_HT             */
	"wheeled APC",                               		/*!< SPWOOB_UTYPE_WH_APC         */
	"tracked APC",                               		/*!< SPWOOB_UTYPE_TR_APC         */
	"heavy tracked APC",                         		/*!< SPWOOB_UTYPE_HV_TR_APC      */
	"light mortar",                              		/*!< SPWOOB_UTYPE_LT_MORT        */
	"mortar",                                    		/*!< SPWOOB_UTYPE_MORT           */
	"heavy mortar",                              		/*!< SPWOOB_UTYPE_HV_MORT        */
	"SP mortar",                                 		/*!< SPWOOB_UTYPE_SP_MORT        */
	"SP gun",                                    		/*!< SPWOOB_UTYPE_SP_GUN         */
	"infantry gun",                              		/*!< SPWOOB_UTYPE_INF_GUN        */
	"howitzer",                                  		/*!< SPWOOB_UTYPE_HWTZR          */
	"SP artillery",                              		/*!< SPWOOB_UTYPE_SP_ART         */
	"offboard artillery",                        		/*!< SPWOOB_UTYPE_OB_ART         */
	"rocket launcher",                           		/*!< SPWOOB_UTYPE_RKT            */
	"SP rocket launcher",                        		/*!< SPWOOB_UTYPE_SP_RKT         */
	"offboard rocket launcher",                  		/*!< SPWOOB_UTYPE_OB_RKT         */
	"captured SP gun",                           		/*!< SPWOOB_UTYPE_CAPT_SPG       */
	"AT infantry",                               		/*!< SPWOOB_UTYPE_INF_AT         */
	"AT gun",                                    		/*!< SPWOOB_UTYPE_AT             */
	"wheeled AT gun",                            		/*!< SPWOOB_UTYPE_WH_AT          */
	"SP AT gun",                                 		/*!< SPWOOB_UTYPE_SP_AT          */
	"tank destroyer",                            		/*!< SPWOOB_UTYPE_TD             */
	"heavy tank destroyer",                      		/*!< SPWOOB_UTYPE_HV_TD          */
	"turreted tank destroyer",                   		/*!< SPWOOB_UTYPE_TTD            */
	"flak",                                      		/*!< SPWOOB_UTYPE_FLAK           */
	"heavy flak",                                		/*!< SPWOOB_UTYPE_HV_FLAK        */
	"wheeled flak",                              		/*!< SPWOOB_UTYPE_WH_FLAK        */
	"SP flak",                                   		/*!< SPWOOB_UTYPE_SP_FLAK        */
	"amphibian unit",                            		/*!< SPWOOB_UTYPE_AMPHIB         */
	"heavy amphibian unit",                      		/*!< SPWOOB_UTYPE_HV_AMPHIB      */
	"amphibian tank",                            		/*!< SPWOOB_UTYPE_AMPHIB_TNK     */
	"marines",                                   		/*!< SPWOOB_UTYPE_MRNS           */
	"scout",                                     		/*!< SPWOOB_UTYPE_SCOUT          */
	"scout vehicle",                             		/*!< SPWOOB_UTYPE_VEH_SCOUT      */
	"scout APC",                                 		/*!< SPWOOB_UTYPE_APC_SCOUT      */
	"forward observer",                          		/*!< SPWOOB_UTYPE_FO             */
	"FO vehicle",                                		/*!< SPWOOB_UTYPE_VEH_FO         */
	"tracked FO",                                		/*!< SPWOOB_UTYPE_TR_FO          */
	"motorcycle",                                		/*!< SPWOOB_UTYPE_MCYCLE         */
	"recon halftrack",                           		/*!< SPWOOB_UTYPE_RHT            */
	"light armored car",                         		/*!< SPWOOB_UTYPE_LT_ARMCAR      */
	"armored car",                               		/*!< SPWOOB_UTYPE_ARMCAR         */
	"heavy armored car",                         		/*!< SPWOOB_UTYPE_HV_ARMCAR      */
	"recon tank",                                		/*!< SPWOOB_UTYPE_RE_TNK         */
	"force HQ",                                  		/*!< SPWOOB_UTYPE_HQ             */
	"platoon HQ",                                		/*!< SPWOOB_UTYPE_PLT_HQ         */
	"command tank",                              		/*!< SPWOOB_UTYPE_CC_TNK         */
	"prime mover",                               		/*!< SPWOOB_UTYPE_PRMV           */
	"medium truck",                              		/*!< SPWOOB_UTYPE_MD_TRK         */
	"heavy truck",                               		/*!< SPWOOB_UTYPE_HV_TRK         */
	"barge carrier",                             		/*!< SPWOOB_UTYPE_BCARR          */
	"wagon",                                     		/*!< SPWOOB_UTYPE_WAGON          */
	"team",                                      		/*!< SPWOOB_UTYPE_TEAM           */
	"gun team",                                  		/*!< SPWOOB_UTYPE_GTEAM          */
	"pack animal",                               		/*!< SPWOOB_UTYPE_PACK           */
	"aircraft",                                  		/*!< SPWOOB_UTYPE_AIR            */
	"level bomber",                              		/*!< SPWOOB_UTYPE_AIRB           */
	"cargo aircraft",                            		/*!< SPWOOB_UTYPE_CRG_AIR        */
	"glider",                                    		/*!< SPWOOB_UTYPE_GLIDER         */
	"helicopter",                                		/*!< SPWOOB_UTYPE_HELI           */
	"attack helicopter",                         		/*!< SPWOOB_UTYPE_AHELI          */
	"observer helicopter",                       		/*!< SPWOOB_UTYPE_OHELI          */
	"landing barge",                             		/*!< SPWOOB_UTYPE_BARGE          */
	"assault boat",                              		/*!< SPWOOB_UTYPE_ABOAT          */
	"landing craft support",                     		/*!< SPWOOB_UTYPE_LCS            */
	"landing craft gunboat",                     		/*!< SPWOOB_UTYPE_LCG            */
	"ship",                                      		/*!< SPWOOB_UTYPE_SHIP           */
	"naval bombardment",                         		/*!< SPWOOB_UTYPE_NAVB           */
	"fort",                                      		/*!< SPWOOB_UTYPE_FORT           */
	"gun emplacement",                           		/*!< SPWOOB_UTYPE_EMP_FORT       */
	"naval fortification",                       		/*!< SPWOOB_UTYPE_NAV_FORT       */
	"utility vehicle",                           		/*!< SPWOOB_UTYPE_UTIL           */
	"crew",                                      		/*!< SPWOOB_UTYPE_CREW           */
	"ammo carrier",                              		/*!< SPWOOB_UTYPE_AMMOC          */
	"ammo dump",                                 		/*!< SPWOOB_UTYPE_AMMOD          */
	"cave",                                      		/*!< SPWOOB_UTYPE_CAVE           */
	"*unknown value*"                            		/*!< SPWOOB_UTYPE__NONE          */
};

/*! SPWAW OOB unit type lookup function
 *
 * \param e	SPWAW OOB unit type
 * \return	pointer to const string with SPWAW OOB unit type name
 */
const char *
SPWOOB_UTYPE_lookup (SPWOOB_UTYPE e) {
	return (SPWOOB_UTYPE_names[e]);
}

/*! SPWAW OOB unit type translation function
 *
 * \param r	raw SPWAW OOB unit type value
 * \return	SPWAW OOB unit type
 */
SPWOOB_UTYPE
SPWOOB_UTYPE_xlt (BYTE r)
{
	SPWOOB_UTYPE	e;

	switch (r) {
		case 0:
			e = SPWOOB_UTYPE_FORT;
			break;
		case 1:
			e = SPWOOB_UTYPE_INF;
			break;
		case 2:
			e = SPWOOB_UTYPE_INF_AT;
			break;
		case 3:
			e = SPWOOB_UTYPE_CAV;
			break;
		case 4:
			e = SPWOOB_UTYPE_MG;
			break;
		case 5:
			e = SPWOOB_UTYPE_MORT;
			break;
		case 6:
			e = SPWOOB_UTYPE_AT;
			break;
		case 7:
			e = SPWOOB_UTYPE_PLT_HQ;
			break;
		case 8:
			e = SPWOOB_UTYPE_FLAK;
			break;
		case 9:
			e = SPWOOB_UTYPE_HWTZR;
			break;
		case 10:
			e = SPWOOB_UTYPE_OB_ART;
			break;
		case 11:
			e = SPWOOB_UTYPE_ARMCAR;
			break;
		case 12:
			e = SPWOOB_UTYPE_LT_TNK;
			break;
		case 13:
			e = SPWOOB_UTYPE_MB_TNK;
			break;
		case 14:
			e = SPWOOB_UTYPE_HV_TNK;
			break;
		case 15:
			e = SPWOOB_UTYPE_CS_TNK;
			break;
		case 16:
			e = SPWOOB_UTYPE_HV_FLAK;
			break;
		case 17:
			e = SPWOOB_UTYPE_FLM_TNK;
			break;
		case 18:
			e = SPWOOB_UTYPE_ASSAULT;
			break;
		case 19:
			e = SPWOOB_UTYPE_SP_AT;
			break;
		case 20:
			e = SPWOOB_UTYPE_ENG;
			break;
		case 21:
			e = SPWOOB_UTYPE_SP_ART;
			break;
		case 22:
			e = SPWOOB_UTYPE_SP_FLAK;
			break;
		case 23:
			e = SPWOOB_UTYPE_WH_APC;
			break;
		case 24:
			e = SPWOOB_UTYPE_HT;
			break;
		case 25:
			e = SPWOOB_UTYPE_TR_APC;
			break;
		case 26:
			e = SPWOOB_UTYPE_UTIL;
			break;
		case 27:
			e = SPWOOB_UTYPE_MD_TRK;
			break;
		case 28:
			e = SPWOOB_UTYPE_HV_TRK;
			break;
		case 29:
			e = SPWOOB_UTYPE_SKI;
			break;
		case 30:
			e = SPWOOB_UTYPE_WH_FLAK;
			break;
		case 31:
			e = SPWOOB_UTYPE_OB_RKT;
			break;
		case 32:
			e = SPWOOB_UTYPE_VEH_SCOUT;
			break;
		case 33:
			e = SPWOOB_UTYPE_AMPHIB;
			break;
		case 34:
			e = SPWOOB_UTYPE_HV_AMPHIB;
			break;
		case 35:
			e = SPWOOB_UTYPE_AMPHIB_TNK;
			break;
		case 36:
			e = SPWOOB_UTYPE_MINECLR_TNK;
			break;
		case 37:
			e = SPWOOB_UTYPE_ENG_TNK;
			break;
		case 38:
			e = SPWOOB_UTYPE_SP_MORT;
			break;
		case 39:
			e = SPWOOB_UTYPE_SP_GUN;
			break;
		case 40:
			e = SPWOOB_UTYPE_HV_INF;
			break;
		case 41:
			e = SPWOOB_UTYPE_BARGE;
			break;
		case 42:
			e = SPWOOB_UTYPE_ABOAT;
			break;
		case 43:
			e = SPWOOB_UTYPE_LCS;
			break;
		case 44:
			e = SPWOOB_UTYPE_AIR;
			break;
		case 45:
			e = SPWOOB_UTYPE_SNIPER;
			break;
		case 46:
			e = SPWOOB_UTYPE_LCG;
			break;
		case 47:
			e = SPWOOB_UTYPE_HELI;
			break;
		case 48:
			e = SPWOOB_UTYPE_BCARR;
			break;
		case 49:
			e = SPWOOB_UTYPE_MCYCLE;
			break;
		case 50:
			e = SPWOOB_UTYPE_AHELI;
			break;
		case 51:
			e = SPWOOB_UTYPE_LT_INF;
			break;
		case 52:
			e = SPWOOB_UTYPE_WH_AT;
			break;
		case 53:
			e = SPWOOB_UTYPE_OHELI;
			break;
		case 54:
			e = SPWOOB_UTYPE_CREW;
			break;
		case 55:
			e = SPWOOB_UTYPE_HQ;
			break;
		case 56:
			e = SPWOOB_UTYPE_AMMOC;
			break;
		case 57:
			e = SPWOOB_UTYPE_VEH_FO;
			break;
		case 58:
			e = SPWOOB_UTYPE_SCOUT;
			break;
		case 59:
			e = SPWOOB_UTYPE_APC_SCOUT;
			break;
		case 60:
			e = SPWOOB_UTYPE_TD;
			break;
		case 61:
			e = SPWOOB_UTYPE_HV_TD;
			break;
		case 62:
			e = SPWOOB_UTYPE_INF_GUN;
			break;
		case 63:
			e = SPWOOB_UTYPE_INF_TNK;
			break;
		case 64:
			e = SPWOOB_UTYPE_CR_TNK;
			break;
		case 65:
			e = SPWOOB_UTYPE_CC_TNK;
			break;
		case 66:
			e = SPWOOB_UTYPE_AMMOD;
			break;
		case 67:
			e = SPWOOB_UTYPE_SHIP;
			break;
		case 68:
			e = SPWOOB_UTYPE_EMP_FORT;
			break;
		case 69:
			e = SPWOOB_UTYPE_FO;
			break;
		case 70:
			e = SPWOOB_UTYPE_SPECOP;
			break;
		case 71:
			e = SPWOOB_UTYPE_GUERILLA;
			break;
		case 72:
			e = SPWOOB_UTYPE_AIR_INF;
			break;
		case 73:
			e = SPWOOB_UTYPE_ELITE;
			break;
		case 74:
			e = SPWOOB_UTYPE_BCYCLE;
			break;
		case 75:
			e = SPWOOB_UTYPE_WAGON;
			break;
		case 76:
			e = SPWOOB_UTYPE_TEAM;
			break;
		case 77:
			e = SPWOOB_UTYPE_GLIDER;
			break;
		case 78:
			e = SPWOOB_UTYPE_CRG_AIR;
			break;
		case 79:
			e = SPWOOB_UTYPE_TTD;
			break;
		case 80:
			e = SPWOOB_UTYPE_LT_MORT;
			break;
		case 81:
			e = SPWOOB_UTYPE_HV_MG;
			break;
		case 82:
			e = SPWOOB_UTYPE_HV_MORT;
			break;
		case 83:
			e = SPWOOB_UTYPE_GTEAM;
			break;
		case 84:
			e = SPWOOB_UTYPE_PACK;
			break;
		case 85:
			e = SPWOOB_UTYPE_TR_FO;
			break;
		case 86:
			e = SPWOOB_UTYPE_NAV_FORT;
			break;
		case 87:
			e = SPWOOB_UTYPE_CAVE;
			break;
		case 88:
			e = SPWOOB_UTYPE_LT_ARMCAR;
			break;
		case 89:
			e = SPWOOB_UTYPE_HV_ARMCAR;
			break;
		case 90:
			e = SPWOOB_UTYPE_HV_TR_APC;
			break;
		case 91:
			e = SPWOOB_UTYPE_MD_TNK;
			break;
		case 92:
			e = SPWOOB_UTYPE_VHV_TNK;
			break;
		case 93:
			e = SPWOOB_UTYPE_CAPT_TNK;
			break;
		case 94:
			e = SPWOOB_UTYPE_CAPT_SPG;
			break;
		case 95:
			e = SPWOOB_UTYPE_RE_TNK;
			break;
		case 96:
			e = SPWOOB_UTYPE_AHT;
			break;
		case 97:
			e = SPWOOB_UTYPE_PRMV;
			break;
		case 98:
			e = SPWOOB_UTYPE_SPEC_TNK;
			break;
		case 99:
			e = SPWOOB_UTYPE_ARM_INF;
			break;
		case 100:
			e = SPWOOB_UTYPE_ARM_ENG;
			break;
		case 101:
			e = SPWOOB_UTYPE_MOT_INF;
			break;
		case 102:
			e = SPWOOB_UTYPE_MOT_ENG;
			break;
		case 103:
			e = SPWOOB_UTYPE_CONSCR;
			break;
		case 104:
			e = SPWOOB_UTYPE_RHT;
			break;
		case 105:
			e = SPWOOB_UTYPE_LL_TNK;
			break;
		case 106:
			e = SPWOOB_UTYPE_MRNS;
			break;
		case 107:
			e = SPWOOB_UTYPE_SLINF;
			break;
		case 108:
			e = SPWOOB_UTYPE_MD_INF;
			break;
		case 109:
			e = SPWOOB_UTYPE_RKT;
			break;
		case 110:
			e = SPWOOB_UTYPE_SP_RKT;
			break;
		case 111:
			e = SPWOOB_UTYPE_NAVB;
			break;
		case 112:
			e = SPWOOB_UTYPE_AIRB;
			break;
		default:
			e = SPWOOB_UTYPE__NONE;
			break;
	}
	return (e);
}



/*! SPWAW OOB formation status */
static const char *SPWOOB_FSTAT_names[SPWOOB_FSTAT_LIMIT+1] = {
	"average",                                   		/*!< SPWOOB_FSTAT_D     */
	"recon",                                     		/*!< SPWOOB_FSTAT_R     */
	"dedicated artillery",                       		/*!< SPWOOB_FSTAT_A     */
	"elite",                                     		/*!< SPWOOB_FSTAT_E     */
	"elite recon",                               		/*!< SPWOOB_FSTAT_ER    */
	"elite dedicated artillery",                 		/*!< SPWOOB_FSTAT_EA    */
	"second line",                               		/*!< SPWOOB_FSTAT_S     */
	"second line recon",                         		/*!< SPWOOB_FSTAT_SR    */
	"second line dedicated artillery",           		/*!< SPWOOB_FSTAT_SA    */
	"*unknown value*"                            		/*!< SPWOOB_FSTAT__NONE */
};

/*! SPWAW OOB formation status lookup function
 *
 * \param e	SPWAW OOB formation status
 * \return	pointer to const string with SPWAW OOB formation status name
 */
const char *
SPWOOB_FSTAT_lookup (SPWOOB_FSTAT e) {
	return (SPWOOB_FSTAT_names[e]);
}

/*! SPWAW OOB formation status: code strings */
static const char *SPWOOB_FSTAT_codes[SPWOOB_FSTAT_LIMIT+1] = {
	"D",                                         		/*!< SPWOOB_FSTAT_D     */
	"R",                                         		/*!< SPWOOB_FSTAT_R     */
	"A",                                         		/*!< SPWOOB_FSTAT_A     */
	"E",                                         		/*!< SPWOOB_FSTAT_E     */
	"ER",                                        		/*!< SPWOOB_FSTAT_ER    */
	"EA",                                        		/*!< SPWOOB_FSTAT_EA    */
	"S",                                         		/*!< SPWOOB_FSTAT_S     */
	"SR",                                        		/*!< SPWOOB_FSTAT_SR    */
	"SA",                                        		/*!< SPWOOB_FSTAT_SA    */
};

/*! SPWAW OOB formation status code lookup function
 *
 * \param e	SPWAW OOB formation status
 * \return	pointer to const string with SPWAW OOB formation status code
 */
const char *
SPWOOB_FSTAT_lookup_code (SPWOOB_FSTAT e) {
	return (SPWOOB_FSTAT_codes[e]);
}

/*! SPWAW OOB formation status translation function
 *
 * \param r	raw SPWAW OOB formation status value
 * \return	SPWAW OOB formation status
 */
SPWOOB_FSTAT
SPWOOB_FSTAT_xlt (BYTE r)
{
	SPWOOB_FSTAT	e;

	switch (r) {
		case 0:
			e = SPWOOB_FSTAT_D;
			break;
		case 1:
			e = SPWOOB_FSTAT_R;
			break;
		case 2:
			e = SPWOOB_FSTAT_A;
			break;
		case 10:
			e = SPWOOB_FSTAT_E;
			break;
		case 11:
			e = SPWOOB_FSTAT_ER;
			break;
		case 12:
			e = SPWOOB_FSTAT_EA;
			break;
		case 20:
			e = SPWOOB_FSTAT_S;
			break;
		case 21:
			e = SPWOOB_FSTAT_SR;
			break;
		case 22:
			e = SPWOOB_FSTAT_SA;
			break;
		default:
			e = SPWOOB_FSTAT__NONE;
			break;
	}
	return (e);
}



/*! SPWAW OOB formation type */
static const char *SPWOOB_FTYPE_names[SPWOOB_FTYPE_LIMIT+1] = {
	"SPWOOB_FTYPE_UNKNOWN",                      		/*!< SPWOOB_FTYPE_UNKNOWN    */
	"platoon",                                   		/*!< SPWOOB_FTYPE_PLT        */
	"company",                                   		/*!< SPWOOB_FTYPE_COM        */
	"naval",                                     		/*!< SPWOOB_FTYPE_NAV        */
	"force HQ",                                  		/*!< SPWOOB_FTYPE_FHQ        */
	"*unknown value*"                            		/*!< SPWOOB_FTYPE__NONE      */
};

/*! SPWAW OOB formation type lookup function
 *
 * \param e	SPWAW OOB formation type
 * \return	pointer to const string with SPWAW OOB formation type name
 */
const char *
SPWOOB_FTYPE_lookup (SPWOOB_FTYPE e) {
	return (SPWOOB_FTYPE_names[e]);
}

/*! SPWAW OOB formation type translation function
 *
 * \param r	raw SPWAW OOB formation type value
 * \return	SPWAW OOB formation type
 */
SPWOOB_FTYPE
SPWOOB_FTYPE_xlt (BYTE r)
{
	SPWOOB_FTYPE	e;

	switch (r) {
		case 0:
			e = SPWOOB_FTYPE_UNKNOWN;
			break;
		case 1:
			e = SPWOOB_FTYPE_PLT;
			break;
		case 2:
			e = SPWOOB_FTYPE_COM;
			break;
		case 4:
			e = SPWOOB_FTYPE_NAV;
			break;
		case 5:
			e = SPWOOB_FTYPE_FHQ;
			break;
		default:
			e = SPWOOB_FTYPE__NONE;
			break;
	}
	return (e);
}



/*! SPWAW OOB formation purchase screen type */
static const char *SPWOOB_FPSCR_names[SPWOOB_FPSCR_LIMIT+1] = {
	"default",                                   		/*!< SPWOOB_FPSCR_DEF     */
	"armor",                                     		/*!< SPWOOB_FPSCR_ARM     */
	"artillery",                                 		/*!< SPWOOB_FPSCR_ART     */
	"infantry",                                  		/*!< SPWOOB_FPSCR_INF     */
	"miscellaneous",                             		/*!< SPWOOB_FPSCR_MISC    */
	"*AI*",                                      		/*!< SPWOOB_FPSCR_AI      */
	"force HQ",                                  		/*!< SPWOOB_FPSCR_FHQ     */
	"*unknown value*"                            		/*!< SPWOOB_FPSCR__NONE   */
};

/*! SPWAW OOB formation purchase screen type lookup function
 *
 * \param e	SPWAW OOB formation purchase screen type
 * \return	pointer to const string with SPWAW OOB formation purchase screen type name
 */
const char *
SPWOOB_FPSCR_lookup (SPWOOB_FPSCR e) {
	return (SPWOOB_FPSCR_names[e]);
}

/*! SPWAW OOB formation purchase screen type translation function
 *
 * \param r	raw SPWAW OOB formation purchase screen type value
 * \return	SPWAW OOB formation purchase screen type
 */
SPWOOB_FPSCR
SPWOOB_FPSCR_xlt (BYTE r)
{
	SPWOOB_FPSCR	e;

	switch (r) {
		case 0:
			e = SPWOOB_FPSCR_DEF;
			break;
		case 1:
			e = SPWOOB_FPSCR_ARM;
			break;
		case 2:
			e = SPWOOB_FPSCR_ART;
			break;
		case 3:
			e = SPWOOB_FPSCR_INF;
			break;
		case 4:
			e = SPWOOB_FPSCR_MISC;
			break;
		case 5:
			e = SPWOOB_FPSCR_AI;
			break;
		case 10:
			e = SPWOOB_FPSCR_FHQ;
			break;
		default:
			e = SPWOOB_FPSCR__NONE;
			break;
	}
	return (e);
}



/*! SPWAW OOB movement class */
static const char *SPWOOB_MOVCL_names[SPWOOB_MOVCL_LIMIT+1] = {
	"default",                                   		/*!< SPWOOB_MOVCL_DEF    */
	"wheel",                                     		/*!< SPWOOB_MOVCL_WHL    */
	"wheel (AT)",                                		/*!< SPWOOB_MOVCL_WAT    */
	"track",                                     		/*!< SPWOOB_MOVCL_TRK    */
	"ship",                                      		/*!< SPWOOB_MOVCL_NAV    */
	"*unknown value*"                            		/*!< SPWOOB_MOVCL__NONE  */
};

/*! SPWAW OOB movement class lookup function
 *
 * \param e	SPWAW OOB movement class
 * \return	pointer to const string with SPWAW OOB movement class name
 */
const char *
SPWOOB_MOVCL_lookup (SPWOOB_MOVCL e) {
	return (SPWOOB_MOVCL_names[e]);
}

/*! SPWAW OOB movement class translation function
 *
 * \param r	raw SPWAW OOB movement class value
 * \return	SPWAW OOB movement class
 */
SPWOOB_MOVCL
SPWOOB_MOVCL_xlt (BYTE r)
{
	SPWOOB_MOVCL	e;

	switch (r) {
		case 0:
			e = SPWOOB_MOVCL_DEF;
			break;
		case 2:
			e = SPWOOB_MOVCL_WHL;
			break;
		case 3:
			e = SPWOOB_MOVCL_WAT;
			break;
		case 4:
			e = SPWOOB_MOVCL_TRK;
			break;
		case 7:
			e = SPWOOB_MOVCL_NAV;
			break;
		default:
			e = SPWOOB_MOVCL__NONE;
			break;
	}
	return (e);
}



