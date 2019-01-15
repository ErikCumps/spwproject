/** \file
 * The SPWaW Library - SPWOOB type codes.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <spwawlib_spwoob_types.h>



/*! SPWOOB unit class */
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

/*! SPWOOB unit class lookup function
 *
 * \param e	SPWOOB unit class
 * \return	Pointer to const string with SPWOOB unit class name
 */
const char *
SPWOOB_UCLASS_lookup (SPWOOB_UCLASS e)
{
	return (SPWOOB_UCLASS_names[e]);
}



/*! SPWOOB unit class: code strings */
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

/*! SPWOOB unit class code lookup function
 *
 * \param e	SPWOOB unit class
 * \return	pointer to const string with SPWOOB unit class code
 */
const char *
SPWOOB_UCLASS_lookup_code (SPWOOB_UCLASS e)
{
	return (SPWOOB_UCLASS_codes[e]);
}



/*! SPWOOB weapon class */
static const char *SPWOOB_WCLASS_names[SPWOOB_WCLASS_LIMIT+1] = {
	"unknown weapon",                            		/*!< SPWOOB_WCLASS_UNKNOWN        */
	"primary infantry weapon",                   		/*!< SPWOOB_WCLASS_INF_PRM        */
	"secondary infantry weapon",                 		/*!< SPWOOB_WCLASS_INF_SEC        */
	"team weapon",                               		/*!< SPWOOB_WCLASS_TEAM           */
	"anti-air weapon",                           		/*!< SPWOOB_WCLASS_AA             */
	"light gun",                                 		/*!< SPWOOB_WCLASS_SML_GUN        */
	"medium gun",                                		/*!< SPWOOB_WCLASS_MDM_GUN        */
	"large gun",                                 		/*!< SPWOOB_WCLASS_LRG_GUN        */
	"infantry flamethrower",                     		/*!< SPWOOB_WCLASS_FLAME_INF      */
	"vehicle flamethrower",                      		/*!< SPWOOB_WCLASS_FLAME_VHC      */
	"naval artillery",                           		/*!< SPWOOB_WCLASS_NAVAL          */
	"aircraft weapon",                           		/*!< SPWOOB_WCLASS_AIRCRAFT       */
	"surface-to-air missile",                    		/*!< SPWOOB_WCLASS_SAM            */
	"anti-tank missile",                         		/*!< SPWOOB_WCLASS_AT             */
	"large cluster bomb",                        		/*!< SPWOOB_WCLASS_LRG_CLSTR      */
	"small cluster bomb",                        		/*!< SPWOOB_WCLASS_SML_CLSTR      */
	"ARM",                                       		/*!< SPWOOB_WCLASS_ARM            */
	"ASM",                                       		/*!< SPWOOB_WCLASS_ASM            */
	"napalm",                                    		/*!< SPWOOB_WCLASS_NAPALM         */
	"auto cannon",                               		/*!< SPWOOB_WCLASS_AUTO_CANNON    */
	"*unknown value*"                            		/*!< SPWOOB_WCLASS__NONE          */
};

/*! SPWOOB weapon class lookup function
 *
 * \param e	SPWOOB weapon class
 * \return	pointer to const string with SPWOOB weapon class name
 */
const char *
SPWOOB_WCLASS_lookup (SPWOOB_WCLASS e) {
	return (SPWOOB_WCLASS_names[e]);
}

/*! SPWOOB weapon class translation function
 *
 * \param r	raw SPWOOB weapon class value
 * \return	SPWOOB weapon class
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
		case 19:
			e = SPWOOB_WCLASS_AUTO_CANNON;
			break;
		default:
			e = SPWOOB_WCLASS_UNKNOWN;
			break;
	}
	return (e);
}



/*! SPWOOB formation status */
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

/*! SPWOOB formation status lookup function
 *
 * \param e	SPWOOB formation status
 * \return	pointer to const string with SPWOOB formation status name
 */
const char *
SPWOOB_FSTAT_lookup (SPWOOB_FSTAT e) {
	return (SPWOOB_FSTAT_names[e]);
}

/*! SPWOOB formation status: code strings */
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

/*! SPWOOB formation status code lookup function
 *
 * \param e	SPWOOB formation status
 * \return	pointer to const string with SPWOOB formation status code
 */
const char *
SPWOOB_FSTAT_lookup_code (SPWOOB_FSTAT e) {
	return (SPWOOB_FSTAT_codes[e]);
}

/*! SPWOOB formation status translation function
 *
 * \param r	raw SPWOOB formation status value
 * \return	SPWOOB formation status
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



/*! SPWOOB formation type */
static const char *SPWOOB_FTYPE_names[SPWOOB_FTYPE_LIMIT+1] = {
	"section",                                   		/*!< SPWOOB_FTYPE_SEC    */
	"platoon",                                   		/*!< SPWOOB_FTYPE_PLT    */
	"company",                                   		/*!< SPWOOB_FTYPE_COM    */
	"combat group",                              		/*!< SPWOOB_FTYPE_GRP    */
	"naval",                                     		/*!< SPWOOB_FTYPE_NAV    */
	"force HQ",                                  		/*!< SPWOOB_FTYPE_FHQ    */
	"*unknown value*"                            		/*!< SPWOOB_FTYPE__NONE  */
};

/*! SPWOOB formation type lookup function
 *
 * \param e	SPWOOB formation type
 * \return	pointer to const string with SPWOOB formation type name
 */
const char *
SPWOOB_FTYPE_lookup (SPWOOB_FTYPE e) {
	return (SPWOOB_FTYPE_names[e]);
}

/*! SPWOOB formation type translation function
 *
 * \param r	raw SPWOOB formation type value
 * \return	SPWOOB formation type
 */
SPWOOB_FTYPE
SPWOOB_FTYPE_xlt (BYTE r)
{
	SPWOOB_FTYPE	e;

	switch (r) {
		case 0:
			e = SPWOOB_FTYPE_SEC;
			break;
		case 1:
			e = SPWOOB_FTYPE_PLT;
			break;
		case 2:
			e = SPWOOB_FTYPE_COM;
			break;
		case 3:
			e = SPWOOB_FTYPE_GRP;
			break;
		case 4:
			e = SPWOOB_FTYPE_NAV;
			break;
		case 5:
			e = SPWOOB_FTYPE_FHQ;
			break;
		default:
			e = SPWOOB_FTYPE_SEC;
			break;
	}
	return (e);
}



/*! SPWOOB formation purchase screen type */
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

/*! SPWOOB formation purchase screen type lookup function
 *
 * \param e	SPWOOB formation purchase screen type
 * \return	pointer to const string with SPWOOB formation purchase screen type name
 */
const char *
SPWOOB_FPSCR_lookup (SPWOOB_FPSCR e) {
	return (SPWOOB_FPSCR_names[e]);
}

/*! SPWOOB formation purchase screen type translation function
 *
 * \param r	raw SPWOOB formation purchase screen type value
 * \return	SPWOOB formation purchase screen type
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
			e = SPWOOB_FPSCR_DEF;
			break;
	}
	return (e);
}



/*! SPWOOB movement class */
static const char *SPWOOB_MOVCL_names[SPWOOB_MOVCL_LIMIT+1] = {
	"default",                                   		/*!< SPWOOB_MOVCL_DEF    */
	"wheel",                                     		/*!< SPWOOB_MOVCL_WHL    */
	"wheel (AT)",                                		/*!< SPWOOB_MOVCL_WAT    */
	"track",                                     		/*!< SPWOOB_MOVCL_TRK    */
	"ship",                                      		/*!< SPWOOB_MOVCL_NAV    */
	"*unknown value*"                            		/*!< SPWOOB_MOVCL__NONE  */
};

/*! SPWOOB movement class lookup function
 *
 * \param e	SPWOOB movement class
 * \return	pointer to const string with SPWOOB movement class name
 */
const char *
SPWOOB_MOVCL_lookup (SPWOOB_MOVCL e) {
	return (SPWOOB_MOVCL_names[e]);
}

/*! SPWOOB movement class translation function
 *
 * \param r	raw SPWOOB movement class value
 * \return	SPWOOB movement class
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
			e = SPWOOB_MOVCL_DEF;
			break;
	}
	return (e);
}



/*! SPWOOB unit type */
static const char *SPWOOB_UTYPE_names[SPWOOB_UTYPE_LIMIT+1] = {
	"infantry",                                  		/*!< SPWOOB_UTYPE_INF              */
	"light infantry",                            		/*!< SPWOOB_UTYPE_LT_INF           */
	"medium infantry",                           		/*!< SPWOOB_UTYPE_MD_INF           */
	"heavy wpns squad",                          		/*!< SPWOOB_UTYPE_HV_INF           */
	"rifle squad",                               		/*!< SPWOOB_UTYPE_RFL_INF          */
	"rifle section",                             		/*!< SPWOOB_UTYPE_RFL_SEC          */
	"SMG section",                               		/*!< SPWOOB_UTYPE_SMG_SEC          */
	"machine gun",                               		/*!< SPWOOB_UTYPE_MG               */
	"MG section",                                		/*!< SPWOOB_UTYPE_MG_SEC           */
	"MG team",                                   		/*!< SPWOOB_UTYPE_MG_TEAM          */
	"LMG section",                               		/*!< SPWOOB_UTYPE_LMG_SEC          */
	"heavy MG squad",                            		/*!< SPWOOB_UTYPE_HV_MG            */
	"special forces",                            		/*!< SPWOOB_UTYPE_SPECOP           */
	"engineers",                                 		/*!< SPWOOB_UTYPE_ENG              */
	"engineer support squad",                    		/*!< SPWOOB_UTYPE_ENG_SPT          */
	"elite infantry",                            		/*!< SPWOOB_UTYPE_ELITE            */
	"airborne infantry",                         		/*!< SPWOOB_UTYPE_AIR_INF          */
	"lancers",                                   		/*!< SPWOOB_UTYPE_LNCR             */
	"sniper",                                    		/*!< SPWOOB_UTYPE_SNIPER           */
	"conscripts",                                		/*!< SPWOOB_UTYPE_CONSCR           */
	"second line infantry",                      		/*!< SPWOOB_UTYPE_SL_INF           */
	"second line support squad",                 		/*!< SPWOOB_UTYPE_SL_SPT           */
	"irregulars",                                		/*!< SPWOOB_UTYPE_IRR_INF          */
	"guerilla forces",                           		/*!< SPWOOB_UTYPE_GUERILLA         */
	"native troops",                             		/*!< SPWOOB_UTYPE_NAT_INF          */
	"police",                                    		/*!< SPWOOB_UTYPE_POLICE           */
	"armored infantry",                          		/*!< SPWOOB_UTYPE_ARM_INF          */
	"light armored infantry",                    		/*!< SPWOOB_UTYPE_LT_ARM_INF       */
	"medium armored infantry",                   		/*!< SPWOOB_UTYPE_MD_ARM_INF       */
	"armored heavy wpns squad",                  		/*!< SPWOOB_UTYPE_HV_ARM_INF       */
	"armored SMG squad",                         		/*!< SPWOOB_UTYPE_ARM_SMG          */
	"armored support squad",                     		/*!< SPWOOB_UTYPE_ARM_SPT          */
	"armored engineers",                         		/*!< SPWOOB_UTYPE_ARM_ENG          */
	"commandos",                                 		/*!< SPWOOB_UTYPE_CMD              */
	"light commandos",                           		/*!< SPWOOB_UTYPE_LT_CMD           */
	"medium commandos",                          		/*!< SPWOOB_UTYPE_MD_CMD           */
	"commandos heavy wpns squad",                		/*!< SPWOOB_UTYPE_HV_CMD           */
	"commando support squad",                    		/*!< SPWOOB_UTYPE_CMD_SPT          */
	"commando engineers",                        		/*!< SPWOOB_UTYPE_CMD_ENG          */
	"commando sniper",                           		/*!< SPWOOB_UTYPE_CMD_SNIPER       */
	"colonial infantry",                         		/*!< SPWOOB_UTYPE_COL_INF          */
	"colonial light infantry",                   		/*!< SPWOOB_UTYPE_COL_LT_INF       */
	"colonial heavy wpns squad",                 		/*!< SPWOOB_UTYPE_COL_HV_INF       */
	"colonial support squad",                    		/*!< SPWOOB_UTYPE_COL_SPT          */
	"Ghurkas",                                   		/*!< SPWOOB_UTYPE_GHRK             */
	"Ghurkas heavy wpns squad",                  		/*!< SPWOOB_UTYPE_HV_GHRK          */
	"Guards infantry",                           		/*!< SPWOOB_UTYPE_GRDS_INF         */
	"light Guards squad",                        		/*!< SPWOOB_UTYPE_LT_GRDS          */
	"medium Guards squad",                       		/*!< SPWOOB_UTYPE_MD_GRDS          */
	"Guards heavy wpns squad",                   		/*!< SPWOOB_UTYPE_HV_GRDS          */
	"Guards SMG squad",                          		/*!< SPWOOB_UTYPE_GRDS_SMG         */
	"Guards support squad",                      		/*!< SPWOOB_UTYPE_GRDS_SPT         */
	"Guards engineers",                          		/*!< SPWOOB_UTYPE_GRDS_ENG         */
	"Legionnaires",                              		/*!< SPWOOB_UTYPE_LGN              */
	"Legionnaires SMG squad",                    		/*!< SPWOOB_UTYPE_LGN_SMG          */
	"Legionnaires support squad",                		/*!< SPWOOB_UTYPE_LGN_SPT          */
	"militia",                                   		/*!< SPWOOB_UTYPE_MLT              */
	"light militia",                             		/*!< SPWOOB_UTYPE_LT_MLT           */
	"militia heavy wpns squad",                  		/*!< SPWOOB_UTYPE_HV_MLT           */
	"militia support squad",                     		/*!< SPWOOB_UTYPE_MLT_SPT          */
	"motorized infantry",                        		/*!< SPWOOB_UTYPE_MOT_INF          */
	"motorized engineers",                       		/*!< SPWOOB_UTYPE_MOT_ENG          */
	"mountain infantry",                         		/*!< SPWOOB_UTYPE_MTN_INF          */
	"mountain light infantry",                   		/*!< SPWOOB_UTYPE_MTN_LT_INF       */
	"mountain heavy wpns sqd",                   		/*!< SPWOOB_UTYPE_MTN_HV_INF       */
	"mountain support squad",                    		/*!< SPWOOB_UTYPE_MTN_SPT          */
	"mountain engineers",                        		/*!< SPWOOB_UTYPE_MTN_ENG          */
	"mountain engineers support squad",          		/*!< SPWOOB_UTYPE_MTN_ENG_SPT      */
	"mountain sniper",                           		/*!< SPWOOB_UTYPE_MTN_SNIPER       */
	"paratroop",                                 		/*!< SPWOOB_UTYPE_PARA             */
	"paratroop light infantry",                  		/*!< SPWOOB_UTYPE_PARA_LT_INF      */
	"paratroop medium infantry",                 		/*!< SPWOOB_UTYPE_PARA_MD_INF      */
	"paratroop SMG squad",                       		/*!< SPWOOB_UTYPE_PARA_SMG         */
	"paratroop MG squad",                        		/*!< SPWOOB_UTYPE_PARA_MG          */
	"paratroop support squad",                   		/*!< SPWOOB_UTYPE_PARA_SPT         */
	"paratroop engineers",                       		/*!< SPWOOB_UTYPE_PARA_ENG         */
	"paratroop sniper",                          		/*!< SPWOOB_UTYPE_PARA_SNIPER      */
	"partisan",                                  		/*!< SPWOOB_UTYPE_PRTS             */
	"partisan squad",                            		/*!< SPWOOB_UTYPE_PRTS_INF         */
	"partisan support squad",                    		/*!< SPWOOB_UTYPE_PRTS_SPT         */
	"partisan pioneers",                         		/*!< SPWOOB_UTYPE_PRTS_ENG         */
	"partisan band",                             		/*!< SPWOOB_UTYPE_PRTS_BAND        */
	"Ranger",                                    		/*!< SPWOOB_UTYPE_RNGR             */
	"light Rangers squad",                       		/*!< SPWOOB_UTYPE_LT_RNGR          */
	"Rangers heavy wpns squad",                  		/*!< SPWOOB_UTYPE_HV_RNGR          */
	"Ranger support squad",                      		/*!< SPWOOB_UTYPE_RNGR_SPT         */
	"Ranger engineers",                          		/*!< SPWOOB_UTYPE_RNGR_ENG         */
	"Ranger sniper",                             		/*!< SPWOOB_UTYPE_RNGR_SNIPER      */
	"ski troops",                                		/*!< SPWOOB_UTYPE_SKI              */
	"light ski infantry",                        		/*!< SPWOOB_UTYPE_LT_SKI           */
	"ski heavy wpns squad",                      		/*!< SPWOOB_UTYPE_HV_SKI           */
	"ski SMG squad",                             		/*!< SPWOOB_UTYPE_SKI_SMG          */
	"ski engineers",                             		/*!< SPWOOB_UTYPE_SKI_ENG          */
	"ski sniper",                                		/*!< SPWOOB_UTYPE_SKI_SNIPER       */
	"Waffen SS",                                 		/*!< SPWOOB_UTYPE_WSS_INF          */
	"Waffen SS heavy wpns squad",                		/*!< SPWOOB_UTYPE_WSS_HV_INF       */
	"Waffen SS rifle squad",                     		/*!< SPWOOB_UTYPE_WSS_RFL          */
	"Waffen SS SMG squad",                       		/*!< SPWOOB_UTYPE_WSS_SMG          */
	"Waffen SS engineers",                       		/*!< SPWOOB_UTYPE_WSS_ENG          */
	"cavalry",                                   		/*!< SPWOOB_UTYPE_CAV              */
	"medium cavalry",                            		/*!< SPWOOB_UTYPE_MD_CAV           */
	"light cavalry tank",                        		/*!< SPWOOB_UTYPE_LT_CAV_TNK       */
	"cavalry tank",                              		/*!< SPWOOB_UTYPE_CAV_TNK          */
	"dismounted cavalry",                        		/*!< SPWOOB_UTYPE_CAV_DIS          */
	"cavalry mounts",                            		/*!< SPWOOB_UTYPE_CAV_MNT          */
	"colonial cavalry",                          		/*!< SPWOOB_UTYPE_COL_CAV          */
	"colonial light cavalry",                    		/*!< SPWOOB_UTYPE_COL_LT_CAV       */
	"tankette",                                  		/*!< SPWOOB_UTYPE_TNY_TNK          */
	"CS tankette",                               		/*!< SPWOOB_UTYPE_CS_TNY_TNK       */
	"tank",                                      		/*!< SPWOOB_UTYPE_TNK              */
	"CS tank",                                   		/*!< SPWOOB_UTYPE_CS_TNK           */
	"main battle tank",                          		/*!< SPWOOB_UTYPE_MB_TNK           */
	"light tank",                                		/*!< SPWOOB_UTYPE_LT_TNK           */
	"medium tank",                               		/*!< SPWOOB_UTYPE_MD_TNK           */
	"heavy tank",                                		/*!< SPWOOB_UTYPE_HV_TNK           */
	"very heavy tank",                           		/*!< SPWOOB_UTYPE_VHV_TNK          */
	"cruiser tank",                              		/*!< SPWOOB_UTYPE_CR_TNK           */
	"heavy cruiser tank",                        		/*!< SPWOOB_UTYPE_HV_CR_TNK        */
	"CS cruiser tank",                           		/*!< SPWOOB_UTYPE_CS_CR_TNK        */
	"infantry tank",                             		/*!< SPWOOB_UTYPE_INF_TNK          */
	"light infantry tank",                       		/*!< SPWOOB_UTYPE_LT_INF_TNK       */
	"CS infantry tank",                          		/*!< SPWOOB_UTYPE_CS_INF_TNK       */
	"assault gun",                               		/*!< SPWOOB_UTYPE_ASSAULT          */
	"airborne lt tank",                          		/*!< SPWOOB_UTYPE_LT_AIR_TNK       */
	"DD tank",                                   		/*!< SPWOOB_UTYPE_DD_TNK           */
	"engineer tank",                             		/*!< SPWOOB_UTYPE_ENG_TNK          */
	"flame tank",                                		/*!< SPWOOB_UTYPE_FLM_TNK          */
	"guntank",                                   		/*!< SPWOOB_UTYPE_MG_TNK           */
	"mine clearing tank",                        		/*!< SPWOOB_UTYPE_MINECLR_TNK      */
	"snorkel tank",                              		/*!< SPWOOB_UTYPE_SNRKL_TNK        */
	"special tank",                              		/*!< SPWOOB_UTYPE_SPEC_TNK         */
	"captured tank",                             		/*!< SPWOOB_UTYPE_CAPT_TNK         */
	"colonial tank",                             		/*!< SPWOOB_UTYPE_COL_TNK          */
	"lend-lease tank",                           		/*!< SPWOOB_UTYPE_LL_TNK           */
	"prototype tank",                            		/*!< SPWOOB_UTYPE_PROTO_TNK        */
	"APC",                                       		/*!< SPWOOB_UTYPE_APC              */
	"wheeled APC",                               		/*!< SPWOOB_UTYPE_WH_APC           */
	"wheeled gun APC",                           		/*!< SPWOOB_UTYPE_WH_APC_GUN       */
	"halftrack",                                 		/*!< SPWOOB_UTYPE_HT               */
	"halftrack gun APC",                         		/*!< SPWOOB_UTYPE_HT_APC_GUN       */
	"armed halftrack",                           		/*!< SPWOOB_UTYPE_AHT              */
	"tracked APC",                               		/*!< SPWOOB_UTYPE_TR_APC           */
	"heavy tracked APC",                         		/*!< SPWOOB_UTYPE_HV_TR_APC        */
	"tracked gun APC",                           		/*!< SPWOOB_UTYPE_TR_APC_GUN       */
	"colonial APC",                              		/*!< SPWOOB_UTYPE_COL_APC          */
	"improvised apc",                            		/*!< SPWOOB_UTYPE_IMP_APC          */
	"mortar",                                    		/*!< SPWOOB_UTYPE_MORT             */
	"SP mortar",                                 		/*!< SPWOOB_UTYPE_SP_MORT          */
	"light mortar",                              		/*!< SPWOOB_UTYPE_LT_MORT          */
	"light sp mortar",                           		/*!< SPWOOB_UTYPE_LT_SP_MORT       */
	"heavy mortar",                              		/*!< SPWOOB_UTYPE_HV_MORT          */
	"mortar(sub type a)",                        		/*!< SPWOOB_UTYPE_SA_MORT          */
	"mortar(sub type b)",                        		/*!< SPWOOB_UTYPE_SB_MORT          */
	"mortar(sub type c)",                        		/*!< SPWOOB_UTYPE_SC_MORT          */
	"mortar(sub type d)",                        		/*!< SPWOOB_UTYPE_SD_MORT          */
	"paratroop mortar",                          		/*!< SPWOOB_UTYPE_PARA_MORT        */
	"field gun",                                 		/*!< SPWOOB_UTYPE_FLD_GUN          */
	"SP gun",                                    		/*!< SPWOOB_UTYPE_SP_GUN           */
	"captured SP gun",                           		/*!< SPWOOB_UTYPE_CAPT_SP_GUN      */
	"infantry gun",                              		/*!< SPWOOB_UTYPE_INF_GUN          */
	"SP infantry gun",                           		/*!< SPWOOB_UTYPE_SP_INF_GUN       */
	"infantry support gun",                      		/*!< SPWOOB_UTYPE_INF_CS_GUN       */
	"colonial field gun",                        		/*!< SPWOOB_UTYPE_COL_FLD_GUN      */
	"howitzer",                                  		/*!< SPWOOB_UTYPE_HWTZR            */
	"infantry howitzer",                         		/*!< SPWOOB_UTYPE_INF_HWTZR        */
	"airborne field howitzer",                   		/*!< SPWOOB_UTYPE_AIR_FLD_HWTZR    */
	"pack howitzer",                             		/*!< SPWOOB_UTYPE_PACK_HWTZR       */
	"SP artillery",                              		/*!< SPWOOB_UTYPE_SP_ART           */
	"rocket launcher",                           		/*!< SPWOOB_UTYPE_RKT              */
	"SP rocket launcher",                        		/*!< SPWOOB_UTYPE_SP_RKT           */
	"offboard artillery",                        		/*!< SPWOOB_UTYPE_OB_ART           */
	"offboard light artillery",                  		/*!< SPWOOB_UTYPE_LT_OB_ART        */
	"heavy offboard artillery",                  		/*!< SPWOOB_UTYPE_HV_OB_ART        */
	"offboard rocket launcher",                  		/*!< SPWOOB_UTYPE_OB_RKT           */
	"AT gun",                                    		/*!< SPWOOB_UTYPE_AT               */
	"light AT gun",                              		/*!< SPWOOB_UTYPE_LT_AT            */
	"heavy AT gun",                              		/*!< SPWOOB_UTYPE_HV_AT            */
	"SP AT gun",                                 		/*!< SPWOOB_UTYPE_SP_AT            */
	"AT gun truck",                              		/*!< SPWOOB_UTYPE_WH_AT            */
	"tank destroyer",                            		/*!< SPWOOB_UTYPE_TD               */
	"turreted tank destroyer",                   		/*!< SPWOOB_UTYPE_TTD              */
	"heavy tank destroyer",                      		/*!< SPWOOB_UTYPE_HV_TD            */
	"AT infantry",                               		/*!< SPWOOB_UTYPE_INF_AT           */
	"commando AT infantry",                      		/*!< SPWOOB_UTYPE_CMD_INF_AT       */
	"marines AT gun",                            		/*!< SPWOOB_UTYPE_MRNS_AT          */
	"paratroop AT infantry",                     		/*!< SPWOOB_UTYPE_PARA_INF_AT      */
	"ski AT infantry",                           		/*!< SPWOOB_UTYPE_SKI_INF_AT       */
	"AA gun",                                    		/*!< SPWOOB_UTYPE_FLAK             */
	"heavy AA gun",                              		/*!< SPWOOB_UTYPE_HV_FLAK          */
	"SP AA gun",                                 		/*!< SPWOOB_UTYPE_SP_FLAK          */
	"light SP AA gun",                           		/*!< SPWOOB_UTYPE_LT_SP_FLAK       */
	"AA gun truck",                              		/*!< SPWOOB_UTYPE_WH_FLAK          */
	"AA MG infantry",                            		/*!< SPWOOB_UTYPE_INF_FLAK         */
	"amphibian unit",                            		/*!< SPWOOB_UTYPE_AMPHIB           */
	"light amphibian unit",                      		/*!< SPWOOB_UTYPE_LT_AMPHIB        */
	"heavy amphibian unit",                      		/*!< SPWOOB_UTYPE_HV_AMPHIB        */
	"amphibian tank",                            		/*!< SPWOOB_UTYPE_AMPHIB_TNK       */
	"marines",                                   		/*!< SPWOOB_UTYPE_MRNS             */
	"marines light infantry",                    		/*!< SPWOOB_UTYPE_MRNS_LT_INF      */
	"marines heavy wpns squad",                  		/*!< SPWOOB_UTYPE_MRNS_HV_INF      */
	"marines support squad",                     		/*!< SPWOOB_UTYPE_MRNS_SPT         */
	"marines engineers",                         		/*!< SPWOOB_UTYPE_MRNS_ENG         */
	"marines sniper",                            		/*!< SPWOOB_UTYPE_MRNS_SNIPER      */
	"forward observer",                          		/*!< SPWOOB_UTYPE_FO               */
	"FO vehicle",                                		/*!< SPWOOB_UTYPE_VEH_FO           */
	"tracked FO",                                		/*!< SPWOOB_UTYPE_TR_FO            */
	"FO aircraft",                               		/*!< SPWOOB_UTYPE_AIR_FO           */
	"FO helicopter",                             		/*!< SPWOOB_UTYPE_HELI_FO          */
	"marines FO",                                		/*!< SPWOOB_UTYPE_MRNS_FO          */
	"mountain FO",                               		/*!< SPWOOB_UTYPE_MTN_FO           */
	"paratroop FO",                              		/*!< SPWOOB_UTYPE_PARA_FO          */
	"Ranger FO",                                 		/*!< SPWOOB_UTYPE_RNGR_FO          */
	"scout",                                     		/*!< SPWOOB_UTYPE_SCOUT            */
	"scout vehicle",                             		/*!< SPWOOB_UTYPE_VEH_SCOUT        */
	"scout halftrack",                           		/*!< SPWOOB_UTYPE_HT_SCOUT         */
	"scout APC",                                 		/*!< SPWOOB_UTYPE_APC_SCOUT        */
	"scout tank",                                		/*!< SPWOOB_UTYPE_TNK_SCOUT        */
	"cavalry scout",                             		/*!< SPWOOB_UTYPE_CAV_SCOUT        */
	"commando scout",                            		/*!< SPWOOB_UTYPE_CMD_SCOUT        */
	"colonial scout",                            		/*!< SPWOOB_UTYPE_COL_SCOUT        */
	"Guards scout",                              		/*!< SPWOOB_UTYPE_GRDS_SCOUT       */
	"militia scout",                             		/*!< SPWOOB_UTYPE_MLT_SCOUT        */
	"marines scout",                             		/*!< SPWOOB_UTYPE_MRNS_SCOUT       */
	"mountain scout",                            		/*!< SPWOOB_UTYPE_MTN_SCOUT        */
	"native scout",                              		/*!< SPWOOB_UTYPE_NAT_SCOUT        */
	"paratroop scout",                           		/*!< SPWOOB_UTYPE_PARA_SCOUT       */
	"partisan scout",                            		/*!< SPWOOB_UTYPE_PRTS_SCOUT       */
	"Ranger scout",                              		/*!< SPWOOB_UTYPE_RNGR_SCOUT       */
	"ski scout",                                 		/*!< SPWOOB_UTYPE_SKI_SCOUT        */
	"bicycle squad",                             		/*!< SPWOOB_UTYPE_BCYCLE           */
	"bicycle heavy wpns squad",                  		/*!< SPWOOB_UTYPE_HV_BCYCLE        */
	"motorcycle",                                		/*!< SPWOOB_UTYPE_MCYCLE           */
	"motorcycle heavy wpns squad",               		/*!< SPWOOB_UTYPE_HV_MCYCLE        */
	"armored car",                               		/*!< SPWOOB_UTYPE_ARM_CAR          */
	"light armored car",                         		/*!< SPWOOB_UTYPE_LT_ARM_CAR       */
	"heavy armored car",                         		/*!< SPWOOB_UTYPE_HV_ARM_CAR       */
	"patrol",                                    		/*!< SPWOOB_UTYPE_PATROL           */
	"force HQ",                                  		/*!< SPWOOB_UTYPE_HQ               */
	"platoon HQ",                                		/*!< SPWOOB_UTYPE_PLT_HQ           */
	"command tank",                              		/*!< SPWOOB_UTYPE_CC_TNK           */
	"prime mover",                               		/*!< SPWOOB_UTYPE_PRMV             */
	"artillery prime mover",                     		/*!< SPWOOB_UTYPE_ART_PRMV         */
	"light truck",                               		/*!< SPWOOB_UTYPE_LT_TRK           */
	"medium truck",                              		/*!< SPWOOB_UTYPE_MD_TRK           */
	"heavy truck",                               		/*!< SPWOOB_UTYPE_HV_TRK           */
	"armored truck",                             		/*!< SPWOOB_UTYPE_ARM_TRK          */
	"heavy transporter",                         		/*!< SPWOOB_UTYPE_HETS             */
	"transport aircraft",                        		/*!< SPWOOB_UTYPE_CRG_AIR          */
	"paratroop transport",                       		/*!< SPWOOB_UTYPE_PARA_AIR         */
	"gun team",                                  		/*!< SPWOOB_UTYPE_GUN_TEAM         */
	"horse team",                                		/*!< SPWOOB_UTYPE_TEAM             */
	"wagon",                                     		/*!< SPWOOB_UTYPE_WAGON            */
	"pack animals",                              		/*!< SPWOOB_UTYPE_PACK             */
	"fighter-bomber",                            		/*!< SPWOOB_UTYPE_AIRF             */
	"level bomber",                              		/*!< SPWOOB_UTYPE_AIRB             */
	"helicopter",                                		/*!< SPWOOB_UTYPE_HELI             */
	"attack helicopter",                         		/*!< SPWOOB_UTYPE_AHELI            */
	"ground attack",                             		/*!< SPWOOB_UTYPE_GA               */
	"glider",                                    		/*!< SPWOOB_UTYPE_GLIDER           */
	"heavy glider",                              		/*!< SPWOOB_UTYPE_HV_GLIDER        */
	"landing barge",                             		/*!< SPWOOB_UTYPE_BARGE            */
	"barge carrier",                             		/*!< SPWOOB_UTYPE_BARGECARR        */
	"landing craft gunboat",                     		/*!< SPWOOB_UTYPE_LCG              */
	"landing craft support",                     		/*!< SPWOOB_UTYPE_LCS              */
	"assault boat",                              		/*!< SPWOOB_UTYPE_ABOAT            */
	"patrol boat",                               		/*!< SPWOOB_UTYPE_PBOAT            */
	"rivercraft",                                		/*!< SPWOOB_UTYPE_RBOAT            */
	"torpedo boat",                              		/*!< SPWOOB_UTYPE_TBOAT            */
	"ship",                                      		/*!< SPWOOB_UTYPE_SHIP             */
	"naval bombardment",                         		/*!< SPWOOB_UTYPE_NAVBOMB          */
	"fort",                                      		/*!< SPWOOB_UTYPE_FORT             */
	"gun emplacement",                           		/*!< SPWOOB_UTYPE_EMP_FORT         */
	"naval fortification",                       		/*!< SPWOOB_UTYPE_NAV_FORT         */
	"bunker",                                    		/*!< SPWOOB_UTYPE_BUNKER           */
	"snow vehicle",                              		/*!< SPWOOB_UTYPE_SNOW             */
	"utility vehicle",                           		/*!< SPWOOB_UTYPE_UTIL             */
	"ammo carrier",                              		/*!< SPWOOB_UTYPE_AMMOCARR         */
	"ammo dump",                                 		/*!< SPWOOB_UTYPE_AMMODUMP         */
	"crew",                                      		/*!< SPWOOB_UTYPE_CREW             */
	"cave",                                      		/*!< SPWOOB_UTYPE_CAVE             */
	"explosive",                                 		/*!< SPWOOB_UTYPE_EXPLOSIVE        */
	"reserved",                                  		/*!< SPWOOB_UTYPE_RESERVED         */
	"unknown unit type",                         		/*!< SPWOOB_UTYPE_UNKNOWN          */
	"*unknown value*"                            		/*!< SPWOOB_UTYPE__NONE            */
};

/*! SPWOOB unit type lookup function
 *
 * \param e	SPWOOB unit type
 * \return	pointer to const string with SPWOOB unit type name
 */
const char *
SPWOOB_UTYPE_lookup (SPWOOB_UTYPE e) {
	return (SPWOOB_UTYPE_names[e]);
}

/*! SPWOOB unit type classification function
 *
 * \param e	SPWOOB unit type value
 * \return	SPWOOB_UCLASS enum
 */
SPWOOB_UCLASS
SPWOOB_UTYPE_classify (SPWOOB_UTYPE e)
{
	SPWOOB_UCLASS	c = SPWOOB_UCLASS_LIMIT;

	switch (e) {
		case SPWOOB_UTYPE_INF:
		case SPWOOB_UTYPE_LT_INF:
		case SPWOOB_UTYPE_MD_INF:
		case SPWOOB_UTYPE_HV_INF:
		case SPWOOB_UTYPE_RFL_INF:
		case SPWOOB_UTYPE_RFL_SEC:
		case SPWOOB_UTYPE_SMG_SEC:
		case SPWOOB_UTYPE_MG:
		case SPWOOB_UTYPE_MG_SEC:
		case SPWOOB_UTYPE_MG_TEAM:
		case SPWOOB_UTYPE_LMG_SEC:
		case SPWOOB_UTYPE_HV_MG:
		case SPWOOB_UTYPE_SPECOP:
		case SPWOOB_UTYPE_ENG:
		case SPWOOB_UTYPE_ENG_SPT:
		case SPWOOB_UTYPE_ELITE:
		case SPWOOB_UTYPE_AIR_INF:
		case SPWOOB_UTYPE_LNCR:
		case SPWOOB_UTYPE_SNIPER:
		case SPWOOB_UTYPE_CONSCR:
		case SPWOOB_UTYPE_SL_INF:
		case SPWOOB_UTYPE_SL_SPT:
		case SPWOOB_UTYPE_IRR_INF:
		case SPWOOB_UTYPE_GUERILLA:
		case SPWOOB_UTYPE_NAT_INF:
		case SPWOOB_UTYPE_POLICE:
		case SPWOOB_UTYPE_ARM_INF:
		case SPWOOB_UTYPE_LT_ARM_INF:
		case SPWOOB_UTYPE_MD_ARM_INF:
		case SPWOOB_UTYPE_HV_ARM_INF:
		case SPWOOB_UTYPE_ARM_SMG:
		case SPWOOB_UTYPE_ARM_SPT:
		case SPWOOB_UTYPE_ARM_ENG:
		case SPWOOB_UTYPE_CMD:
		case SPWOOB_UTYPE_LT_CMD:
		case SPWOOB_UTYPE_MD_CMD:
		case SPWOOB_UTYPE_HV_CMD:
		case SPWOOB_UTYPE_CMD_SPT:
		case SPWOOB_UTYPE_CMD_ENG:
		case SPWOOB_UTYPE_CMD_SNIPER:
		case SPWOOB_UTYPE_COL_INF:
		case SPWOOB_UTYPE_COL_LT_INF:
		case SPWOOB_UTYPE_COL_HV_INF:
		case SPWOOB_UTYPE_COL_SPT:
		case SPWOOB_UTYPE_GHRK:
		case SPWOOB_UTYPE_HV_GHRK:
		case SPWOOB_UTYPE_GRDS_INF:
		case SPWOOB_UTYPE_LT_GRDS:
		case SPWOOB_UTYPE_MD_GRDS:
		case SPWOOB_UTYPE_HV_GRDS:
		case SPWOOB_UTYPE_GRDS_SMG:
		case SPWOOB_UTYPE_GRDS_SPT:
		case SPWOOB_UTYPE_GRDS_ENG:
		case SPWOOB_UTYPE_LGN:
		case SPWOOB_UTYPE_LGN_SMG:
		case SPWOOB_UTYPE_LGN_SPT:
		case SPWOOB_UTYPE_MLT:
		case SPWOOB_UTYPE_LT_MLT:
		case SPWOOB_UTYPE_HV_MLT:
		case SPWOOB_UTYPE_MLT_SPT:
		case SPWOOB_UTYPE_MOT_INF:
		case SPWOOB_UTYPE_MOT_ENG:
		case SPWOOB_UTYPE_MTN_INF:
		case SPWOOB_UTYPE_MTN_LT_INF:
		case SPWOOB_UTYPE_MTN_HV_INF:
		case SPWOOB_UTYPE_MTN_SPT:
		case SPWOOB_UTYPE_MTN_ENG:
		case SPWOOB_UTYPE_MTN_ENG_SPT:
		case SPWOOB_UTYPE_MTN_SNIPER:
		case SPWOOB_UTYPE_PARA:
		case SPWOOB_UTYPE_PARA_LT_INF:
		case SPWOOB_UTYPE_PARA_MD_INF:
		case SPWOOB_UTYPE_PARA_SMG:
		case SPWOOB_UTYPE_PARA_MG:
		case SPWOOB_UTYPE_PARA_SPT:
		case SPWOOB_UTYPE_PARA_ENG:
		case SPWOOB_UTYPE_PARA_SNIPER:
		case SPWOOB_UTYPE_PRTS:
		case SPWOOB_UTYPE_PRTS_INF:
		case SPWOOB_UTYPE_PRTS_SPT:
		case SPWOOB_UTYPE_PRTS_ENG:
		case SPWOOB_UTYPE_PRTS_BAND:
		case SPWOOB_UTYPE_RNGR:
		case SPWOOB_UTYPE_LT_RNGR:
		case SPWOOB_UTYPE_HV_RNGR:
		case SPWOOB_UTYPE_RNGR_SPT:
		case SPWOOB_UTYPE_RNGR_ENG:
		case SPWOOB_UTYPE_RNGR_SNIPER:
		case SPWOOB_UTYPE_SKI:
		case SPWOOB_UTYPE_LT_SKI:
		case SPWOOB_UTYPE_HV_SKI:
		case SPWOOB_UTYPE_SKI_SMG:
		case SPWOOB_UTYPE_SKI_ENG:
		case SPWOOB_UTYPE_SKI_SNIPER:
		case SPWOOB_UTYPE_WSS_INF:
		case SPWOOB_UTYPE_WSS_HV_INF:
		case SPWOOB_UTYPE_WSS_RFL:
		case SPWOOB_UTYPE_WSS_SMG:
		case SPWOOB_UTYPE_WSS_ENG:
			c = SPWOOB_UCLASS_INF;
			break;
		case SPWOOB_UTYPE_CAV:
		case SPWOOB_UTYPE_MD_CAV:
		case SPWOOB_UTYPE_LT_CAV_TNK:
		case SPWOOB_UTYPE_CAV_TNK:
		case SPWOOB_UTYPE_CAV_DIS:
		case SPWOOB_UTYPE_CAV_MNT:
		case SPWOOB_UTYPE_COL_CAV:
		case SPWOOB_UTYPE_COL_LT_CAV:
			c = SPWOOB_UCLASS_CAV;
			break;
		case SPWOOB_UTYPE_TNY_TNK:
		case SPWOOB_UTYPE_CS_TNY_TNK:
		case SPWOOB_UTYPE_TNK:
		case SPWOOB_UTYPE_CS_TNK:
		case SPWOOB_UTYPE_MB_TNK:
		case SPWOOB_UTYPE_LT_TNK:
		case SPWOOB_UTYPE_MD_TNK:
		case SPWOOB_UTYPE_HV_TNK:
		case SPWOOB_UTYPE_VHV_TNK:
		case SPWOOB_UTYPE_CR_TNK:
		case SPWOOB_UTYPE_HV_CR_TNK:
		case SPWOOB_UTYPE_CS_CR_TNK:
		case SPWOOB_UTYPE_INF_TNK:
		case SPWOOB_UTYPE_LT_INF_TNK:
		case SPWOOB_UTYPE_CS_INF_TNK:
		case SPWOOB_UTYPE_ASSAULT:
		case SPWOOB_UTYPE_LT_AIR_TNK:
		case SPWOOB_UTYPE_DD_TNK:
		case SPWOOB_UTYPE_ENG_TNK:
		case SPWOOB_UTYPE_FLM_TNK:
		case SPWOOB_UTYPE_MG_TNK:
		case SPWOOB_UTYPE_MINECLR_TNK:
		case SPWOOB_UTYPE_SNRKL_TNK:
		case SPWOOB_UTYPE_SPEC_TNK:
		case SPWOOB_UTYPE_CAPT_TNK:
		case SPWOOB_UTYPE_COL_TNK:
		case SPWOOB_UTYPE_LL_TNK:
		case SPWOOB_UTYPE_PROTO_TNK:
			c = SPWOOB_UCLASS_ARM;
			break;
		case SPWOOB_UTYPE_APC:
		case SPWOOB_UTYPE_WH_APC:
		case SPWOOB_UTYPE_WH_APC_GUN:
		case SPWOOB_UTYPE_HT:
		case SPWOOB_UTYPE_HT_APC_GUN:
		case SPWOOB_UTYPE_AHT:
		case SPWOOB_UTYPE_TR_APC:
		case SPWOOB_UTYPE_HV_TR_APC:
		case SPWOOB_UTYPE_TR_APC_GUN:
		case SPWOOB_UTYPE_COL_APC:
		case SPWOOB_UTYPE_IMP_APC:
			c = SPWOOB_UCLASS_APC;
			break;
		case SPWOOB_UTYPE_MORT:
		case SPWOOB_UTYPE_SP_MORT:
		case SPWOOB_UTYPE_LT_MORT:
		case SPWOOB_UTYPE_LT_SP_MORT:
		case SPWOOB_UTYPE_HV_MORT:
		case SPWOOB_UTYPE_SA_MORT:
		case SPWOOB_UTYPE_SB_MORT:
		case SPWOOB_UTYPE_SC_MORT:
		case SPWOOB_UTYPE_SD_MORT:
		case SPWOOB_UTYPE_PARA_MORT:
		case SPWOOB_UTYPE_FLD_GUN:
		case SPWOOB_UTYPE_SP_GUN:
		case SPWOOB_UTYPE_CAPT_SP_GUN:
		case SPWOOB_UTYPE_INF_GUN:
		case SPWOOB_UTYPE_SP_INF_GUN:
		case SPWOOB_UTYPE_INF_CS_GUN:
		case SPWOOB_UTYPE_COL_FLD_GUN:
		case SPWOOB_UTYPE_HWTZR:
		case SPWOOB_UTYPE_INF_HWTZR:
		case SPWOOB_UTYPE_AIR_FLD_HWTZR:
		case SPWOOB_UTYPE_PACK_HWTZR:
		case SPWOOB_UTYPE_SP_ART:
		case SPWOOB_UTYPE_RKT:
		case SPWOOB_UTYPE_SP_RKT:
		case SPWOOB_UTYPE_OB_ART:
		case SPWOOB_UTYPE_LT_OB_ART:
		case SPWOOB_UTYPE_HV_OB_ART:
		case SPWOOB_UTYPE_OB_RKT:
			c = SPWOOB_UCLASS_ART;
			break;
		case SPWOOB_UTYPE_AT:
		case SPWOOB_UTYPE_LT_AT:
		case SPWOOB_UTYPE_HV_AT:
		case SPWOOB_UTYPE_SP_AT:
		case SPWOOB_UTYPE_WH_AT:
		case SPWOOB_UTYPE_TD:
		case SPWOOB_UTYPE_TTD:
		case SPWOOB_UTYPE_HV_TD:
		case SPWOOB_UTYPE_INF_AT:
		case SPWOOB_UTYPE_CMD_INF_AT:
		case SPWOOB_UTYPE_MRNS_AT:
		case SPWOOB_UTYPE_PARA_INF_AT:
		case SPWOOB_UTYPE_SKI_INF_AT:
			c = SPWOOB_UCLASS_AT;
			break;
		case SPWOOB_UTYPE_FLAK:
		case SPWOOB_UTYPE_HV_FLAK:
		case SPWOOB_UTYPE_SP_FLAK:
		case SPWOOB_UTYPE_LT_SP_FLAK:
		case SPWOOB_UTYPE_WH_FLAK:
		case SPWOOB_UTYPE_INF_FLAK:
			c = SPWOOB_UCLASS_AA;
			break;
		case SPWOOB_UTYPE_AMPHIB:
		case SPWOOB_UTYPE_LT_AMPHIB:
		case SPWOOB_UTYPE_HV_AMPHIB:
		case SPWOOB_UTYPE_AMPHIB_TNK:
		case SPWOOB_UTYPE_MRNS:
		case SPWOOB_UTYPE_MRNS_LT_INF:
		case SPWOOB_UTYPE_MRNS_HV_INF:
		case SPWOOB_UTYPE_MRNS_SPT:
		case SPWOOB_UTYPE_MRNS_ENG:
		case SPWOOB_UTYPE_MRNS_SNIPER:
			c = SPWOOB_UCLASS_MAR;
			break;
		case SPWOOB_UTYPE_FO:
		case SPWOOB_UTYPE_VEH_FO:
		case SPWOOB_UTYPE_TR_FO:
		case SPWOOB_UTYPE_AIR_FO:
		case SPWOOB_UTYPE_HELI_FO:
		case SPWOOB_UTYPE_MRNS_FO:
		case SPWOOB_UTYPE_MTN_FO:
		case SPWOOB_UTYPE_PARA_FO:
		case SPWOOB_UTYPE_RNGR_FO:
		case SPWOOB_UTYPE_SCOUT:
		case SPWOOB_UTYPE_VEH_SCOUT:
		case SPWOOB_UTYPE_HT_SCOUT:
		case SPWOOB_UTYPE_APC_SCOUT:
		case SPWOOB_UTYPE_TNK_SCOUT:
		case SPWOOB_UTYPE_CAV_SCOUT:
		case SPWOOB_UTYPE_CMD_SCOUT:
		case SPWOOB_UTYPE_COL_SCOUT:
		case SPWOOB_UTYPE_GRDS_SCOUT:
		case SPWOOB_UTYPE_MLT_SCOUT:
		case SPWOOB_UTYPE_MRNS_SCOUT:
		case SPWOOB_UTYPE_MTN_SCOUT:
		case SPWOOB_UTYPE_NAT_SCOUT:
		case SPWOOB_UTYPE_PARA_SCOUT:
		case SPWOOB_UTYPE_PRTS_SCOUT:
		case SPWOOB_UTYPE_RNGR_SCOUT:
		case SPWOOB_UTYPE_SKI_SCOUT:
		case SPWOOB_UTYPE_BCYCLE:
		case SPWOOB_UTYPE_HV_BCYCLE:
		case SPWOOB_UTYPE_MCYCLE:
		case SPWOOB_UTYPE_HV_MCYCLE:
		case SPWOOB_UTYPE_ARM_CAR:
		case SPWOOB_UTYPE_LT_ARM_CAR:
		case SPWOOB_UTYPE_HV_ARM_CAR:
		case SPWOOB_UTYPE_PATROL:
			c = SPWOOB_UCLASS_REC;
			break;
		case SPWOOB_UTYPE_HQ:
		case SPWOOB_UTYPE_PLT_HQ:
		case SPWOOB_UTYPE_CC_TNK:
			c = SPWOOB_UCLASS_CC;
			break;
		case SPWOOB_UTYPE_PRMV:
		case SPWOOB_UTYPE_ART_PRMV:
		case SPWOOB_UTYPE_LT_TRK:
		case SPWOOB_UTYPE_MD_TRK:
		case SPWOOB_UTYPE_HV_TRK:
		case SPWOOB_UTYPE_ARM_TRK:
		case SPWOOB_UTYPE_HETS:
		case SPWOOB_UTYPE_CRG_AIR:
		case SPWOOB_UTYPE_PARA_AIR:
		case SPWOOB_UTYPE_GUN_TEAM:
		case SPWOOB_UTYPE_TEAM:
		case SPWOOB_UTYPE_WAGON:
		case SPWOOB_UTYPE_PACK:
			c = SPWOOB_UCLASS_TRN;
			break;
		case SPWOOB_UTYPE_AIRF:
		case SPWOOB_UTYPE_AIRB:
		case SPWOOB_UTYPE_HELI:
		case SPWOOB_UTYPE_AHELI:
		case SPWOOB_UTYPE_GA:
		case SPWOOB_UTYPE_GLIDER:
		case SPWOOB_UTYPE_HV_GLIDER:
			c = SPWOOB_UCLASS_AIR;
			break;
		case SPWOOB_UTYPE_BARGE:
		case SPWOOB_UTYPE_BARGECARR:
		case SPWOOB_UTYPE_LCG:
		case SPWOOB_UTYPE_LCS:
		case SPWOOB_UTYPE_ABOAT:
		case SPWOOB_UTYPE_PBOAT:
		case SPWOOB_UTYPE_RBOAT:
		case SPWOOB_UTYPE_TBOAT:
		case SPWOOB_UTYPE_SHIP:
		case SPWOOB_UTYPE_NAVBOMB:
			c = SPWOOB_UCLASS_NVY;
			break;
		case SPWOOB_UTYPE_FORT:
		case SPWOOB_UTYPE_EMP_FORT:
		case SPWOOB_UTYPE_NAV_FORT:
		case SPWOOB_UTYPE_BUNKER:
			c = SPWOOB_UCLASS_FRT;
			break;
		case SPWOOB_UTYPE_SNOW:
		case SPWOOB_UTYPE_UTIL:
		case SPWOOB_UTYPE_AMMOCARR:
		case SPWOOB_UTYPE_AMMODUMP:
		case SPWOOB_UTYPE_CREW:
		case SPWOOB_UTYPE_CAVE:
		case SPWOOB_UTYPE_EXPLOSIVE:
		case SPWOOB_UTYPE_RESERVED:
		case SPWOOB_UTYPE_UNKNOWN:
			c = SPWOOB_UCLASS_OTH;
			break;
		default:
			c = SPWOOB_UCLASS_OTH;
			break;
	}
	return (c);
}



/*! raw SPWAW OOB unit type translation function
 *
 * \param r	raw SPWAW OOB unit type value
 * \return	SPWOOB_UTYPE enum
 */
SPWOOB_UTYPE
SPWOOB_SPWAW_UTYPE_xlt (BYTE r)
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
			e = SPWOOB_UTYPE_ARM_CAR;
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
			e = SPWOOB_UTYPE_AIRF;
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
			e = SPWOOB_UTYPE_BARGECARR;
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
			e = SPWOOB_UTYPE_HELI_FO;
			break;
		case 54:
			e = SPWOOB_UTYPE_CREW;
			break;
		case 55:
			e = SPWOOB_UTYPE_HQ;
			break;
		case 56:
			e = SPWOOB_UTYPE_AMMOCARR;
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
			e = SPWOOB_UTYPE_AMMODUMP;
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
			e = SPWOOB_UTYPE_GUN_TEAM;
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
			e = SPWOOB_UTYPE_LT_ARM_CAR;
			break;
		case 89:
			e = SPWOOB_UTYPE_HV_ARM_CAR;
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
			e = SPWOOB_UTYPE_CAPT_SP_GUN;
			break;
		case 95:
			e = SPWOOB_UTYPE_TNK_SCOUT;
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
			e = SPWOOB_UTYPE_HT_SCOUT;
			break;
		case 105:
			e = SPWOOB_UTYPE_LL_TNK;
			break;
		case 106:
			e = SPWOOB_UTYPE_MRNS;
			break;
		case 107:
			e = SPWOOB_UTYPE_SL_INF;
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
			e = SPWOOB_UTYPE_NAVBOMB;
			break;
		case 112:
			e = SPWOOB_UTYPE_AIRB;
			break;
		default:
			e = SPWOOB_UTYPE_UNKNOWN;
			break;
	}
	return (e);
}



/*! raw winSPWW2 OOB unit type translation function
 *
 * \param r	raw winSPWW2 OOB unit type value
 * \return	SPWOOB_UTYPE enum
 */
SPWOOB_UTYPE
SPWOOB_WINSPWW2_UTYPE_xlt (BYTE r)
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
			e = SPWOOB_UTYPE_INF_GUN;
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
			e = SPWOOB_UTYPE_ARM_CAR;
			break;
		case 12:
			e = SPWOOB_UTYPE_LT_TNK;
			break;
		case 13:
			e = SPWOOB_UTYPE_MD_TNK;
			break;
		case 14:
			e = SPWOOB_UTYPE_HV_TNK;
			break;
		case 15:
			e = SPWOOB_UTYPE_CS_TNK;
			break;
		case 16:
			e = SPWOOB_UTYPE_INF_CS_GUN;
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
			e = SPWOOB_UTYPE_INF_FLAK;
			break;
		case 30:
			e = SPWOOB_UTYPE_PROTO_TNK;
			break;
		case 31:
			e = SPWOOB_UTYPE_OB_RKT;
			break;
		case 32:
			e = SPWOOB_UTYPE_VEH_SCOUT;
			break;
		case 33:
			e = SPWOOB_UTYPE_LT_AMPHIB;
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
			e = SPWOOB_UTYPE_AIRF;
			break;
		case 45:
			e = SPWOOB_UTYPE_SNIPER;
			break;
		case 46:
			e = SPWOOB_UTYPE_LCG;
			break;
		case 47:
			e = SPWOOB_UTYPE_GLIDER;
			break;
		case 48:
			e = SPWOOB_UTYPE_BARGECARR;
			break;
		case 49:
			e = SPWOOB_UTYPE_MCYCLE;
			break;
		case 50:
			e = SPWOOB_UTYPE_AIR_FO;
			break;
		case 51:
			e = SPWOOB_UTYPE_LT_INF;
			break;
		case 52:
			e = SPWOOB_UTYPE_WH_AT;
			break;
		case 53:
			e = SPWOOB_UTYPE_HELI;
			break;
		case 54:
			e = SPWOOB_UTYPE_CREW;
			break;
		case 55:
			e = SPWOOB_UTYPE_HQ;
			break;
		case 56:
			e = SPWOOB_UTYPE_AMMOCARR;
			break;
		case 57:
			e = SPWOOB_UTYPE_VEH_FO;
			break;
		case 58:
			e = SPWOOB_UTYPE_SCOUT;
			break;
		case 59:
			e = SPWOOB_UTYPE_MG_TNK;
			break;
		case 60:
			e = SPWOOB_UTYPE_PARA_AIR;
			break;
		case 61:
			e = SPWOOB_UTYPE_CRG_AIR;
			break;
		case 62:
			e = SPWOOB_UTYPE_AIRB;
			break;
		case 63:
			e = SPWOOB_UTYPE_FO;
			break;
		case 64:
			e = SPWOOB_UTYPE_MD_INF;
			break;
		case 65:
			e = SPWOOB_UTYPE_LMG_SEC;
			break;
		case 66:
			e = SPWOOB_UTYPE_SMG_SEC;
			break;
		case 67:
			e = SPWOOB_UTYPE_RFL_SEC;
			break;
		case 68:
			e = SPWOOB_UTYPE_RFL_INF;
			break;
		case 69:
			e = SPWOOB_UTYPE_ARM_INF;
			break;
		case 70:
			e = SPWOOB_UTYPE_HV_ARM_INF;
			break;
		case 71:
			e = SPWOOB_UTYPE_MD_ARM_INF;
			break;
		case 72:
			e = SPWOOB_UTYPE_LT_ARM_INF;
			break;
		case 73:
			e = SPWOOB_UTYPE_ARM_SPT;
			break;
		case 74:
			e = SPWOOB_UTYPE_ARM_SMG;
			break;
		case 75:
			e = SPWOOB_UTYPE_IRR_INF;
			break;
		case 76:
			e = SPWOOB_UTYPE_PRTS;
			break;
		case 77:
			e = SPWOOB_UTYPE_PRTS_SPT;
			break;
		case 78:
			e = SPWOOB_UTYPE_PRTS_BAND;
			break;
		case 79:
			e = SPWOOB_UTYPE_NAT_INF;
			break;
		case 80:
			e = SPWOOB_UTYPE_PRTS_SCOUT;
			break;
		case 81:
			e = SPWOOB_UTYPE_APC_SCOUT;
			break;
		case 82:
			e = SPWOOB_UTYPE_PATROL;
			break;
		case 83:
			e = SPWOOB_UTYPE_CMD;
			break;
		case 84:
			e = SPWOOB_UTYPE_CMD_SPT;
			break;
		case 85:
			e = SPWOOB_UTYPE_LT_CMD;
			break;
		case 86:
			e = SPWOOB_UTYPE_MD_CMD;
			break;
		case 87:
			e = SPWOOB_UTYPE_HV_CMD;
			break;
		case 88:
			e = SPWOOB_UTYPE_CMD_SCOUT;
			break;
		case 89:
			e = SPWOOB_UTYPE_GRDS_INF;
			break;
		case 90:
			e = SPWOOB_UTYPE_GRDS_SMG;
			break;
		case 91:
			e = SPWOOB_UTYPE_LT_GRDS;
			break;
		case 92:
			e = SPWOOB_UTYPE_MD_GRDS;
			break;
		case 93:
			e = SPWOOB_UTYPE_HV_GRDS;
			break;
		case 94:
			e = SPWOOB_UTYPE_GRDS_SCOUT;
			break;
		case 95:
			e = SPWOOB_UTYPE_GRDS_SPT;
			break;
		case 96:
			e = SPWOOB_UTYPE_PARA;
			break;
		case 97:
			e = SPWOOB_UTYPE_PARA_SPT;
			break;
		case 98:
			e = SPWOOB_UTYPE_PARA_LT_INF;
			break;
		case 99:
			e = SPWOOB_UTYPE_PARA_MD_INF;
			break;
		case 100:
			e = SPWOOB_UTYPE_PARA_SMG;
			break;
		case 101:
			e = SPWOOB_UTYPE_PARA_SCOUT;
			break;
		case 102:
			e = SPWOOB_UTYPE_CR_TNK;
			break;
		case 103:
			e = SPWOOB_UTYPE_HV_CR_TNK;
			break;
		case 104:
			e = SPWOOB_UTYPE_CS_CR_TNK;
			break;
		case 105:
			e = SPWOOB_UTYPE_INF_TNK;
			break;
		case 106:
			e = SPWOOB_UTYPE_CS_INF_TNK;
			break;
		case 107:
			e = SPWOOB_UTYPE_VHV_TNK;
			break;
		case 108:
			e = SPWOOB_UTYPE_HV_ARM_CAR;
			break;
		case 109:
			e = SPWOOB_UTYPE_WAGON;
			break;
		case 110:
			e = SPWOOB_UTYPE_MRNS;
			break;
		case 111:
			e = SPWOOB_UTYPE_RNGR;
			break;
		case 112:
			e = SPWOOB_UTYPE_MRNS_LT_INF;
			break;
		case 113:
			e = SPWOOB_UTYPE_MRNS_SPT;
			break;
		case 114:
			e = SPWOOB_UTYPE_RNGR_SPT;
			break;
		case 115:
			e = SPWOOB_UTYPE_WSS_INF;
			break;
		case 116:
			e = SPWOOB_UTYPE_WSS_SMG;
			break;
		case 117:
			e = SPWOOB_UTYPE_LGN;
			break;
		case 118:
			e = SPWOOB_UTYPE_LGN_SMG;
			break;
		case 119:
			e = SPWOOB_UTYPE_LGN_SPT;
			break;
		case 120:
			e = SPWOOB_UTYPE_LL_TNK;
			break;
		case 121:
			e = SPWOOB_UTYPE_TNY_TNK;
			break;
		case 122:
			e = SPWOOB_UTYPE_CS_TNY_TNK;
			break;
		case 123:
			e = SPWOOB_UTYPE_CR_TNK;
			break;
		case 124:
			e = SPWOOB_UTYPE_LT_INF_TNK;
			break;
		case 125:
			e = SPWOOB_UTYPE_HT_APC_GUN;
			break;
		case 126:
			e = SPWOOB_UTYPE_WH_APC_GUN;
			break;
		case 127:
			e = SPWOOB_UTYPE_TR_APC_GUN;
			break;
		case 128:
			e = SPWOOB_UTYPE_LT_SP_FLAK;
			break;
		case 129:
			e = SPWOOB_UTYPE_CS_TNK;
			break;
		case 130:
			e = SPWOOB_UTYPE_SP_INF_GUN;
			break;
		case 131:
			e = SPWOOB_UTYPE_CAV_TNK;
			break;
		case 132:
			e = SPWOOB_UTYPE_LT_CAV_TNK;
			break;
		case 133:
			e = SPWOOB_UTYPE_LT_SP_MORT;
			break;
		case 134:
			e = SPWOOB_UTYPE_DD_TNK;
			break;
		case 135:
			e = SPWOOB_UTYPE_SNRKL_TNK;
			break;
		case 136:
			e = SPWOOB_UTYPE_IMP_APC;
			break;
		case 137:
			e = SPWOOB_UTYPE_SP_RKT;
			break;
		case 138:
			e = SPWOOB_UTYPE_HETS;
			break;
		case 139:
			e = SPWOOB_UTYPE_SP_GUN;
			break;
		case 140:
			e = SPWOOB_UTYPE_ENG;
			break;
		case 141:
			e = SPWOOB_UTYPE_PARA_ENG;
			break;
		case 142:
			e = SPWOOB_UTYPE_PARA_INF_AT;
			break;
		case 143:
			e = SPWOOB_UTYPE_PARA_SNIPER;
			break;
		case 144:
			e = SPWOOB_UTYPE_PARA_MG;
			break;
		case 145:
			e = SPWOOB_UTYPE_CMD_ENG;
			break;
		case 146:
			e = SPWOOB_UTYPE_CMD_INF_AT;
			break;
		case 147:
			e = SPWOOB_UTYPE_CMD_SNIPER;
			break;
		case 148:
			e = SPWOOB_UTYPE_LNCR;
			break;
		case 149:
			e = SPWOOB_UTYPE_CAV_MNT;
			break;
		case 150:
			e = SPWOOB_UTYPE_CAV_SCOUT;
			break;
		case 151:
			e = SPWOOB_UTYPE_RKT;
			break;
		case 152:
			e = SPWOOB_UTYPE_HV_MORT;
			break;
		case 153:
			e = SPWOOB_UTYPE_LT_MORT;
			break;
		case 154:
			e = SPWOOB_UTYPE_INF_HWTZR;
			break;
		case 155:
			e = SPWOOB_UTYPE_HV_OB_ART;
			break;
		case 156:
			e = SPWOOB_UTYPE_LT_OB_ART;
			break;
		case 157:
			e = SPWOOB_UTYPE_LT_AT;
			break;
		case 158:
			e = SPWOOB_UTYPE_HV_AT;
			break;
		case 159:
			e = SPWOOB_UTYPE_CAPT_TNK;
			break;
		case 160:
			e = SPWOOB_UTYPE_SL_INF;
			break;
		case 161:
			e = SPWOOB_UTYPE_SL_SPT;
			break;
		case 162:
			e = SPWOOB_UTYPE_SKI;
			break;
		case 163:
			e = SPWOOB_UTYPE_HV_SKI;
			break;
		case 164:
			e = SPWOOB_UTYPE_SKI_SNIPER;
			break;
		case 165:
			e = SPWOOB_UTYPE_SKI_INF_AT;
			break;
		case 166:
			e = SPWOOB_UTYPE_SKI_ENG;
			break;
		case 167:
			e = SPWOOB_UTYPE_SNOW;
			break;
		case 168:
			e = SPWOOB_UTYPE_SKI_SCOUT;
			break;
		case 169:
			e = SPWOOB_UTYPE_SKI_SMG;
			break;
		case 170:
			e = SPWOOB_UTYPE_GRDS_ENG;
			break;
		case 171:
			e = SPWOOB_UTYPE_MRNS_ENG;
			break;
		case 172:
			e = SPWOOB_UTYPE_BCYCLE;
			break;
		case 173:
			e = SPWOOB_UTYPE_PARA_MORT;
			break;
		case 174:
			e = SPWOOB_UTYPE_WSS_ENG;
			break;
		case 175:
			e = SPWOOB_UTYPE_LT_AIR_TNK;
			break;
		case 176:
			e = SPWOOB_UTYPE_GHRK;
			break;
		case 177:
			e = SPWOOB_UTYPE_HV_GHRK;
			break;
		case 178:
			e = SPWOOB_UTYPE_POLICE;
			break;
		case 179:
			e = SPWOOB_UTYPE_HV_BCYCLE;
			break;
		case 180:
			e = SPWOOB_UTYPE_ART_PRMV;
			break;
		case 181:
			e = SPWOOB_UTYPE_WH_FLAK;
			break;
		case 182:
			e = SPWOOB_UTYPE_PACK;
			break;
		case 183:
			e = SPWOOB_UTYPE_LT_TRK;
			break;
		case 184:
			e = SPWOOB_UTYPE_AIR_FLD_HWTZR;
			break;
		case 185:
			e = SPWOOB_UTYPE_WSS_HV_INF;
			break;
		case 186:
			e = SPWOOB_UTYPE_WSS_RFL;
			break;
		case 187:
			e = SPWOOB_UTYPE_MCYCLE;
			break;
		case 188:
			e = SPWOOB_UTYPE_HV_MCYCLE;
			break;
		case 189:
			e = SPWOOB_UTYPE_MLT;
			break;
		case 190:
			e = SPWOOB_UTYPE_LT_MLT;
			break;
		case 191:
			e = SPWOOB_UTYPE_HV_MLT;
			break;
		case 192:
			e = SPWOOB_UTYPE_MLT_SPT;
			break;
		case 193:
			e = SPWOOB_UTYPE_MG;
			break;
		case 194:
			e = SPWOOB_UTYPE_MG_SEC;
			break;
		case 195:
			e = SPWOOB_UTYPE_ENG_SPT;
			break;
		case 196:
			e = SPWOOB_UTYPE_PACK_HWTZR;
			break;
		case 197:
			e = SPWOOB_UTYPE_RBOAT;
			break;
		case 198:
			e = SPWOOB_UTYPE_TBOAT;
			break;
		case 199:
			e = SPWOOB_UTYPE_PBOAT;
			break;
		case 200:
			e = SPWOOB_UTYPE_SA_MORT;
			break;
		case 201:
			e = SPWOOB_UTYPE_SB_MORT;
			break;
		case 202:
			e = SPWOOB_UTYPE_SC_MORT;
			break;
		case 203:
			e = SPWOOB_UTYPE_MTN_INF;
			break;
		case 204:
			e = SPWOOB_UTYPE_MTN_LT_INF;
			break;
		case 205:
			e = SPWOOB_UTYPE_MTN_HV_INF;
			break;
		case 206:
			e = SPWOOB_UTYPE_MTN_SPT;
			break;
		case 207:
			e = SPWOOB_UTYPE_MTN_ENG;
			break;
		case 208:
			e = SPWOOB_UTYPE_MTN_SCOUT;
			break;
		case 209:
			e = SPWOOB_UTYPE_MTN_SNIPER;
			break;
		case 210:
			e = SPWOOB_UTYPE_MTN_FO;
			break;
		case 211:
			e = SPWOOB_UTYPE_MD_CAV;
			break;
		case 212:
			e = SPWOOB_UTYPE_PARA_FO;
			break;
		case 213:
			e = SPWOOB_UTYPE_MRNS_HV_INF;
			break;
		case 214:
			e = SPWOOB_UTYPE_MRNS_SCOUT;
			break;
		case 215:
			e = SPWOOB_UTYPE_MRNS_SNIPER;
			break;
		case 216:
			e = SPWOOB_UTYPE_MRNS_FO;
			break;
		case 217:
			e = SPWOOB_UTYPE_MRNS_AT;
			break;
		case 218:
			e = SPWOOB_UTYPE_CAV_DIS;
			break;
		case 219:
			e = SPWOOB_UTYPE_RNGR_ENG;
			break;
		case 220:
			e = SPWOOB_UTYPE_COL_INF;
			break;
		case 221:
			e = SPWOOB_UTYPE_COL_LT_INF;
			break;
		case 222:
			e = SPWOOB_UTYPE_COL_HV_INF;
			break;
		case 223:
			e = SPWOOB_UTYPE_COL_SPT;
			break;
		case 224:
			e = SPWOOB_UTYPE_COL_FLD_GUN;
			break;
		case 225:
			e = SPWOOB_UTYPE_COL_SCOUT;
			break;
		case 226:
			e = SPWOOB_UTYPE_SNIPER;
			break;
		case 227:
			e = SPWOOB_UTYPE_COL_CAV;
			break;
		case 228:
			e = SPWOOB_UTYPE_EXPLOSIVE;
			break;
		case 229:
			e = SPWOOB_UTYPE_COL_LT_CAV;
			break;
		case 230:
			e = SPWOOB_UTYPE_COL_APC;
			break;
		case 231:
			e = SPWOOB_UTYPE_COL_TNK;
			break;
		case 232:
			e = SPWOOB_UTYPE_ARM_TRK;
			break;
		case 233:
			e = SPWOOB_UTYPE_MG_SEC;
			break;
		case 234:
			e = SPWOOB_UTYPE_PRTS_INF;
			break;
		case 235:
			e = SPWOOB_UTYPE_PRTS_ENG;
			break;
		case 236:
			e = SPWOOB_UTYPE_MLT_SCOUT;
			break;
		case 237:
			e = SPWOOB_UTYPE_FLD_GUN;
			break;
		case 238:
			e = SPWOOB_UTYPE_AT;
			break;
		case 239:
			e = SPWOOB_UTYPE_LT_SKI;
			break;
		case 240:
			e = SPWOOB_UTYPE_LT_ARM_CAR;
			break;
		case 241:
			e = SPWOOB_UTYPE_MG_TEAM;
			break;
		case 242:
			e = SPWOOB_UTYPE_SD_MORT;
			break;
		case 243:
			e = SPWOOB_UTYPE_GA;
			break;
		case 244:
			e = SPWOOB_UTYPE_RESERVED;
			break;
		case 245:
			e = SPWOOB_UTYPE_LT_RNGR;
			break;
		case 246:
			e = SPWOOB_UTYPE_HV_RNGR;
			break;
		case 247:
			e = SPWOOB_UTYPE_RNGR_SCOUT;
			break;
		case 248:
			e = SPWOOB_UTYPE_RNGR_SNIPER;
			break;
		case 249:
			e = SPWOOB_UTYPE_RNGR_FO;
			break;
		case 250:
			e = SPWOOB_UTYPE_MTN_ENG_SPT;
			break;
		case 251:
			e = SPWOOB_UTYPE_NAT_SCOUT;
			break;
		case 252:
			e = SPWOOB_UTYPE_APC;
			break;
		case 253:
			e = SPWOOB_UTYPE_TNK;
			break;
		case 254:
			e = SPWOOB_UTYPE_BUNKER;
			break;
		case 255:
			e = SPWOOB_UTYPE_HV_GLIDER;
			break;
		default:
			e = SPWOOB_UTYPE_UNKNOWN;
			break;
	}
	return (e);
}



