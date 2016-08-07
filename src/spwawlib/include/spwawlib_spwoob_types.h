/** \file
 * The SPWaW Library - SPWaw OOB type codes.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	protection__________INCLUDE_SPWAWLIB_SPWOOB_TYPES_H
#define	protection__________INCLUDE_SPWAWLIB_SPWOOB_TYPES_H	1

/** \addtogroup API
 * @{
 */

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>




#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/*! SPWAW OOB unit class */
typedef enum e_SPWOOB_UCLASS {
	SPWOOB_UCLASS_START = 0,                     		/*!< \internal start code */
	SPWOOB_UCLASS_INF = 0,                       		/*!< infantry             */
	SPWOOB_UCLASS_CAV,                           		/*!< cavalry              */
	SPWOOB_UCLASS_ARM,                           		/*!< armor                */
	SPWOOB_UCLASS_APC,                           		/*!< APC                  */
	SPWOOB_UCLASS_ART,                           		/*!< artillery            */
	SPWOOB_UCLASS_AT,                            		/*!< anti-tank            */
	SPWOOB_UCLASS_AA,                            		/*!< anti-air             */
	SPWOOB_UCLASS_MAR,                           		/*!< marine unit          */
	SPWOOB_UCLASS_REC,                           		/*!< recon                */
	SPWOOB_UCLASS_CC,                            		/*!< C&C                  */
	SPWOOB_UCLASS_TRN,                           		/*!< transport            */
	SPWOOB_UCLASS_AIR,                           		/*!< air force            */
	SPWOOB_UCLASS_NVY,                           		/*!< navy                 */
	SPWOOB_UCLASS_FRT,                           		/*!< fortification        */
	SPWOOB_UCLASS_OTH,                           		/*!< other                */
	SPWOOB_UCLASS_LIMIT = SPWOOB_UCLASS_OTH      		/*!< \internal final code */
} SPWOOB_UCLASS;
#define	SPWOOB_UCLASS_CNT	(SPWOOB_UCLASS_LIMIT - SPWOOB_UCLASS_START + 1)

/*! SPWAW OOB unit class lookup function
 *
 * \param e	SPWAW OOB unit class
 * \return	Pointer to const string with SPWAW OOB unit class name
 */
extern const char *SPWOOB_UCLASS_lookup (SPWOOB_UCLASS e);
/*! SPWAW OOB unit class code lookup function
 *
 * \param e	SPWAW OOB unit class
 * \return	Pointer to const string with SPWAW OOB unit class code
 */
extern const char *SPWOOB_UCLASS_lookup_code (SPWOOB_UCLASS e);



/*! SPWAW OOB weapon class */
typedef enum e_SPWOOB_WCLASS {
	SPWOOB_WCLASS_START = 0,                     		/*!< \internal start code */
	SPWOOB_WCLASS_UNKNOWN = 0,                   		/*!<                      */
	SPWOOB_WCLASS_INF_PRM,                       		/*!<                      */
	SPWOOB_WCLASS_INF_SEC,                       		/*!<                      */
	SPWOOB_WCLASS_TEAM,                          		/*!<                      */
	SPWOOB_WCLASS_AA,                            		/*!<                      */
	SPWOOB_WCLASS_SML_GUN,                       		/*!<                      */
	SPWOOB_WCLASS_MDM_GUN,                       		/*!<                      */
	SPWOOB_WCLASS_LRG_GUN,                       		/*!<                      */
	SPWOOB_WCLASS_FLAME_INF,                     		/*!<                      */
	SPWOOB_WCLASS_FLAME_VHC,                     		/*!<                      */
	SPWOOB_WCLASS_NAVAL,                         		/*!<                      */
	SPWOOB_WCLASS_AIRCRAFT,                      		/*!<                      */
	SPWOOB_WCLASS_SAM,                           		/*!<                      */
	SPWOOB_WCLASS_AT,                            		/*!<                      */
	SPWOOB_WCLASS_LRG_CLSTR,                     		/*!<                      */
	SPWOOB_WCLASS_SML_CLSTR,                     		/*!<                      */
	SPWOOB_WCLASS_ARM,                           		/*!<                      */
	SPWOOB_WCLASS_ASM,                           		/*!<                      */
	SPWOOB_WCLASS_NAPALM,                        		/*!<                      */
	SPWOOB_WCLASS__NONE,                         		/*!< \internal default code */
	SPWOOB_WCLASS_LIMIT = SPWOOB_WCLASS__NONE    		/*!< \internal final code */
} SPWOOB_WCLASS;
#define	SPWOOB_WCLASS_CNT	(SPWOOB_WCLASS_LIMIT - SPWOOB_WCLASS_START + 1)

/*! SPWAW OOB weapon class lookup function
 *
 * \param e	SPWAW OOB weapon class
 * \return	pointer to const string with SPWAW OOB weapon class name
 */
extern const char *SPWOOB_WCLASS_lookup (SPWOOB_WCLASS e);

/*! SPWAW OOB weapon class translation function
 *
 * \param r	raw SPWAW OOB weapon class value
 * \return	SPWAW OOB weapon class
 */
extern SPWOOB_WCLASS SPWOOB_WCLASS_xlt (BYTE r);



/*! SPWAW OOB unit type */
typedef enum e_SPWOOB_UTYPE {
	SPWOOB_UTYPE_START = 0,                      		/*!< \internal start code        */
	SPWOOB_UTYPE_LT_INF = 0,                     		/*!< light infantry              */
	SPWOOB_UTYPE_INF,                            		/*!< infantry                    */
	SPWOOB_UTYPE_MD_INF,                         		/*!< medium infantry             */
	SPWOOB_UTYPE_HV_INF,                         		/*!< heavy infantry              */
	SPWOOB_UTYPE_MOT_INF,                        		/*!< motorized infantry          */
	SPWOOB_UTYPE_ARM_INF,                        		/*!< armored infantry            */
	SPWOOB_UTYPE_ENG,                            		/*!< engineers                   */
	SPWOOB_UTYPE_MOT_ENG,                        		/*!< motorized engineers         */
	SPWOOB_UTYPE_ARM_ENG,                        		/*!< armored engineers           */
	SPWOOB_UTYPE_MG,                             		/*!< machine gun                 */
	SPWOOB_UTYPE_HV_MG,                          		/*!< heavy machine gun           */
	SPWOOB_UTYPE_SKI,                            		/*!< ski troops                  */
	SPWOOB_UTYPE_AIR_INF,                        		/*!< airborne infantry           */
	SPWOOB_UTYPE_BCYCLE,                         		/*!< bicycle infantry            */
	SPWOOB_UTYPE_ELITE,                          		/*!< elite infantry              */
	SPWOOB_UTYPE_SNIPER,                         		/*!< sniper                      */
	SPWOOB_UTYPE_SPECOP,                         		/*!< special forces              */
	SPWOOB_UTYPE_GUERILLA,                       		/*!< guerilla forces             */
	SPWOOB_UTYPE_CONSCR,                         		/*!< conscripts                  */
	SPWOOB_UTYPE_SLINF,                          		/*!< second line infantry        */
	SPWOOB_UTYPE_CAV,                            		/*!< cavalry                     */
	SPWOOB_UTYPE_AHT,                            		/*!< armed halftrack             */
	SPWOOB_UTYPE_LT_TNK,                         		/*!< light tank                  */
	SPWOOB_UTYPE_MD_TNK,                         		/*!< medium tank                 */
	SPWOOB_UTYPE_HV_TNK,                         		/*!< heavy tank                  */
	SPWOOB_UTYPE_VHV_TNK,                        		/*!< very heavy tank             */
	SPWOOB_UTYPE_MB_TNK,                         		/*!< main battle tank            */
	SPWOOB_UTYPE_CR_TNK,                         		/*!< cruiser tank                */
	SPWOOB_UTYPE_CS_TNK,                         		/*!< close support tank          */
	SPWOOB_UTYPE_INF_TNK,                        		/*!< infantry tank               */
	SPWOOB_UTYPE_MINECLR_TNK,                    		/*!< mineclear tank              */
	SPWOOB_UTYPE_ENG_TNK,                        		/*!< engineer tank               */
	SPWOOB_UTYPE_FLM_TNK,                        		/*!< flame tank                  */
	SPWOOB_UTYPE_SPEC_TNK,                       		/*!< special tank                */
	SPWOOB_UTYPE_LL_TNK,                         		/*!< lend lease tank             */
	SPWOOB_UTYPE_CAPT_TNK,                       		/*!< captured tank               */
	SPWOOB_UTYPE_ASSAULT,                        		/*!< assault gun                 */
	SPWOOB_UTYPE_HT,                             		/*!< halftrack                   */
	SPWOOB_UTYPE_WH_APC,                         		/*!< wheeled APC                 */
	SPWOOB_UTYPE_TR_APC,                         		/*!< tracked APC                 */
	SPWOOB_UTYPE_HV_TR_APC,                      		/*!< heavy tracked APC           */
	SPWOOB_UTYPE_LT_MORT,                        		/*!< light mortar                */
	SPWOOB_UTYPE_MORT,                           		/*!< mortar                      */
	SPWOOB_UTYPE_HV_MORT,                        		/*!< heavy mortar                */
	SPWOOB_UTYPE_SP_MORT,                        		/*!< SP mortar                   */
	SPWOOB_UTYPE_SP_GUN,                         		/*!< SP gun                      */
	SPWOOB_UTYPE_INF_GUN,                        		/*!< infantry gun                */
	SPWOOB_UTYPE_HWTZR,                          		/*!< howitzer                    */
	SPWOOB_UTYPE_SP_ART,                         		/*!< SP artillery                */
	SPWOOB_UTYPE_OB_ART,                         		/*!< offboard artillery          */
	SPWOOB_UTYPE_RKT,                            		/*!< rocket launcher             */
	SPWOOB_UTYPE_SP_RKT,                         		/*!< SP rocket launcher          */
	SPWOOB_UTYPE_OB_RKT,                         		/*!< offboard rocket launcher    */
	SPWOOB_UTYPE_CAPT_SPG,                       		/*!< captured SP gun             */
	SPWOOB_UTYPE_INF_AT,                         		/*!< AT infantry                 */
	SPWOOB_UTYPE_AT,                             		/*!< AT gun                      */
	SPWOOB_UTYPE_WH_AT,                          		/*!< wheeled AT gun              */
	SPWOOB_UTYPE_SP_AT,                          		/*!< SP AT gun                   */
	SPWOOB_UTYPE_TD,                             		/*!< tank destroyer              */
	SPWOOB_UTYPE_HV_TD,                          		/*!< heavy tank destroyer        */
	SPWOOB_UTYPE_TTD,                            		/*!< turreted tank destroyer     */
	SPWOOB_UTYPE_FLAK,                           		/*!< flak                        */
	SPWOOB_UTYPE_HV_FLAK,                        		/*!< heavy flak                  */
	SPWOOB_UTYPE_WH_FLAK,                        		/*!< wheeled flak                */
	SPWOOB_UTYPE_SP_FLAK,                        		/*!< SP flak                     */
	SPWOOB_UTYPE_AMPHIB,                         		/*!< amphibian unit              */
	SPWOOB_UTYPE_HV_AMPHIB,                      		/*!< heavy amphibian unit        */
	SPWOOB_UTYPE_AMPHIB_TNK,                     		/*!< amphibian tank              */
	SPWOOB_UTYPE_MRNS,                           		/*!< marines                     */
	SPWOOB_UTYPE_SCOUT,                          		/*!< scout                       */
	SPWOOB_UTYPE_VEH_SCOUT,                      		/*!< scout vehicle               */
	SPWOOB_UTYPE_APC_SCOUT,                      		/*!< scout APC                   */
	SPWOOB_UTYPE_FO,                             		/*!< forward observer            */
	SPWOOB_UTYPE_VEH_FO,                         		/*!< FO vehicle                  */
	SPWOOB_UTYPE_TR_FO,                          		/*!< tracked FO                  */
	SPWOOB_UTYPE_MCYCLE,                         		/*!< motorcycle                  */
	SPWOOB_UTYPE_RHT,                            		/*!< recon halftrack             */
	SPWOOB_UTYPE_LT_ARMCAR,                      		/*!< light armored car           */
	SPWOOB_UTYPE_ARMCAR,                         		/*!< armored car                 */
	SPWOOB_UTYPE_HV_ARMCAR,                      		/*!< heavy armored car           */
	SPWOOB_UTYPE_RE_TNK,                         		/*!< recon tank                  */
	SPWOOB_UTYPE_HQ,                             		/*!< force HQ                    */
	SPWOOB_UTYPE_PLT_HQ,                         		/*!< platoon HQ                  */
	SPWOOB_UTYPE_CC_TNK,                         		/*!< command tank                */
	SPWOOB_UTYPE_PRMV,                           		/*!< prime mover                 */
	SPWOOB_UTYPE_MD_TRK,                         		/*!< medium truck                */
	SPWOOB_UTYPE_HV_TRK,                         		/*!< heavy truck                 */
	SPWOOB_UTYPE_BCARR,                          		/*!< barge carrier               */
	SPWOOB_UTYPE_WAGON,                          		/*!< wagon                       */
	SPWOOB_UTYPE_TEAM,                           		/*!< team                        */
	SPWOOB_UTYPE_GTEAM,                          		/*!< gun team                    */
	SPWOOB_UTYPE_PACK,                           		/*!< pack animal                 */
	SPWOOB_UTYPE_AIR,                            		/*!< aircraft                    */
	SPWOOB_UTYPE_AIRB,                           		/*!< level bomber                */
	SPWOOB_UTYPE_CRG_AIR,                        		/*!< cargo aircraft              */
	SPWOOB_UTYPE_GLIDER,                         		/*!< glider                      */
	SPWOOB_UTYPE_HELI,                           		/*!< helicopter                  */
	SPWOOB_UTYPE_AHELI,                          		/*!< attack helicopter           */
	SPWOOB_UTYPE_OHELI,                          		/*!< observer helicopter         */
	SPWOOB_UTYPE_BARGE,                          		/*!< landing barge               */
	SPWOOB_UTYPE_ABOAT,                          		/*!< assault boat                */
	SPWOOB_UTYPE_LCS,                            		/*!< landing craft support       */
	SPWOOB_UTYPE_LCG,                            		/*!< landing craft gunboat       */
	SPWOOB_UTYPE_SHIP,                           		/*!< ship                        */
	SPWOOB_UTYPE_NAVB,                           		/*!< naval bombardment           */
	SPWOOB_UTYPE_FORT,                           		/*!< fort                        */
	SPWOOB_UTYPE_EMP_FORT,                       		/*!< gun emplacement             */
	SPWOOB_UTYPE_NAV_FORT,                       		/*!< naval fortification         */
	SPWOOB_UTYPE_UTIL,                           		/*!< utility vehicle             */
	SPWOOB_UTYPE_CREW,                           		/*!< crew                        */
	SPWOOB_UTYPE_AMMOC,                          		/*!< ammo carrier                */
	SPWOOB_UTYPE_AMMOD,                          		/*!< ammo dump                   */
	SPWOOB_UTYPE_CAVE,                           		/*!< cave                        */
	SPWOOB_UTYPE__NONE,                          		/*!< \internal default code      */
	SPWOOB_UTYPE_LIMIT = SPWOOB_UTYPE__NONE      		/*!< \internal final code        */
} SPWOOB_UTYPE;
#define	SPWOOB_UTYPE_CNT	(SPWOOB_UTYPE_LIMIT - SPWOOB_UTYPE_START + 1)

/*! SPWAW OOB unit type lookup function
 *
 * \param e	SPWAW OOB unit type
 * \return	pointer to const string with SPWAW OOB unit type name
 */
extern const char *SPWOOB_UTYPE_lookup (SPWOOB_UTYPE e);

/*! SPWAW OOB unit type translation function
 *
 * \param r	raw SPWAW OOB unit type value
 * \return	SPWAW OOB unit type
 */
extern SPWOOB_UTYPE SPWOOB_UTYPE_xlt (BYTE r);



/*! SPWAW OOB formation status */
typedef enum e_SPWOOB_FSTAT {
	SPWOOB_FSTAT_START = 0,                      		/*!< \internal start code               */
	SPWOOB_FSTAT_D = 0,                          		/*!< average                            */
	SPWOOB_FSTAT_R,                              		/*!< recon                              */
	SPWOOB_FSTAT_A,                              		/*!< dedicated artillery                */
	SPWOOB_FSTAT_E,                              		/*!< elite                              */
	SPWOOB_FSTAT_ER,                             		/*!< elite recon                        */
	SPWOOB_FSTAT_EA,                             		/*!< elite dedicated artillery          */
	SPWOOB_FSTAT_S,                              		/*!< second line                        */
	SPWOOB_FSTAT_SR,                             		/*!< second line recon                  */
	SPWOOB_FSTAT_SA,                             		/*!< second line dedicated artillery    */
	SPWOOB_FSTAT__NONE,                          		/*!< \internal default code             */
	SPWOOB_FSTAT_LIMIT = SPWOOB_FSTAT__NONE      		/*!< \internal final code               */
} SPWOOB_FSTAT;
#define	SPWOOB_FSTAT_CNT	(SPWOOB_FSTAT_LIMIT - SPWOOB_FSTAT_START + 1)

/*! SPWAW OOB formation status lookup function
 *
 * \param e	SPWAW OOB formation status
 * \return	pointer to const string with SPWAW OOB formation status name
 */
extern const char *SPWOOB_FSTAT_lookup (SPWOOB_FSTAT e);

/*! SPWAW OOB formation status code lookup function
 *
 * \param e	SPWAW OOB formation status
 * \return	pointer to const string with SPWAW OOB formation status code
 */
extern const char *SPWOOB_FSTAT_lookup_code (SPWOOB_FSTAT e);

/*! SPWAW OOB formation status translation function
 *
 * \param r	raw SPWAW OOB formation status value
 * \return	SPWAW OOB formation status
 */
extern SPWOOB_FSTAT SPWOOB_FSTAT_xlt (BYTE r);



/*! SPWAW OOB formation type */
typedef enum e_SPWOOB_FTYPE {
	SPWOOB_FTYPE_START = 0,                      		/*!< \internal start code */
	SPWOOB_FTYPE_UNKNOWN = 0,                    		/*!<                      */
	SPWOOB_FTYPE_PLT,                            		/*!< platoon              */
	SPWOOB_FTYPE_COM,                            		/*!< company              */
	SPWOOB_FTYPE_NAV,                            		/*!< naval                */
	SPWOOB_FTYPE_FHQ,                            		/*!< force HQ             */
	SPWOOB_FTYPE__NONE,                          		/*!< \internal default code */
	SPWOOB_FTYPE_LIMIT = SPWOOB_FTYPE__NONE      		/*!< \internal final code */
} SPWOOB_FTYPE;
#define	SPWOOB_FTYPE_CNT	(SPWOOB_FTYPE_LIMIT - SPWOOB_FTYPE_START + 1)

/*! SPWAW OOB formation type lookup function
 *
 * \param e	SPWAW OOB formation type
 * \return	pointer to const string with SPWAW OOB formation type name
 */
extern const char *SPWOOB_FTYPE_lookup (SPWOOB_FTYPE e);

/*! SPWAW OOB formation type translation function
 *
 * \param r	raw SPWAW OOB formation type value
 * \return	SPWAW OOB formation type
 */
extern SPWOOB_FTYPE SPWOOB_FTYPE_xlt (BYTE r);



/*! SPWAW OOB formation purchase screen type */
typedef enum e_SPWOOB_FPSCR {
	SPWOOB_FPSCR_START = 0,                      		/*!< \internal start code */
	SPWOOB_FPSCR_DEF = 0,                        		/*!< default              */
	SPWOOB_FPSCR_ARM,                            		/*!< armor                */
	SPWOOB_FPSCR_ART,                            		/*!< artillery            */
	SPWOOB_FPSCR_INF,                            		/*!< infantry             */
	SPWOOB_FPSCR_MISC,                           		/*!< miscellaneous        */
	SPWOOB_FPSCR_AI,                             		/*!< *AI*                 */
	SPWOOB_FPSCR_FHQ,                            		/*!< force HQ             */
	SPWOOB_FPSCR__NONE,                          		/*!< \internal default code */
	SPWOOB_FPSCR_LIMIT = SPWOOB_FPSCR__NONE      		/*!< \internal final code */
} SPWOOB_FPSCR;
#define	SPWOOB_FPSCR_CNT	(SPWOOB_FPSCR_LIMIT - SPWOOB_FPSCR_START + 1)

/*! SPWAW OOB formation purchase screen type lookup function
 *
 * \param e	SPWAW OOB formation purchase screen type
 * \return	pointer to const string with SPWAW OOB formation purchase screen type name
 */
extern const char *SPWOOB_FPSCR_lookup (SPWOOB_FPSCR e);

/*! SPWAW OOB formation purchase screen type translation function
 *
 * \param r	raw SPWAW OOB formation purchase screen type value
 * \return	SPWAW OOB formation purchase screen type
 */
extern SPWOOB_FPSCR SPWOOB_FPSCR_xlt (BYTE r);



/*! SPWAW OOB movement class */
typedef enum e_SPWOOB_MOVCL {
	SPWOOB_MOVCL_START = 0,                      		/*!< \internal start code */
	SPWOOB_MOVCL_DEF = 0,                        		/*!< default              */
	SPWOOB_MOVCL_WHL,                            		/*!< wheel                */
	SPWOOB_MOVCL_WAT,                            		/*!< wheel (AT)           */
	SPWOOB_MOVCL_TRK,                            		/*!< track                */
	SPWOOB_MOVCL_NAV,                            		/*!< ship                 */
	SPWOOB_MOVCL__NONE,                          		/*!< \internal default code */
	SPWOOB_MOVCL_LIMIT = SPWOOB_MOVCL__NONE      		/*!< \internal final code */
} SPWOOB_MOVCL;
#define	SPWOOB_MOVCL_CNT	(SPWOOB_MOVCL_LIMIT - SPWOOB_MOVCL_START + 1)

/*! SPWAW OOB movement class lookup function
 *
 * \param e	SPWAW OOB movement class
 * \return	pointer to const string with SPWAW OOB movement class name
 */
extern const char *SPWOOB_MOVCL_lookup (SPWOOB_MOVCL e);

/*! SPWAW OOB movement class translation function
 *
 * \param r	raw SPWAW OOB movement class value
 * \return	SPWAW OOB movement class
 */
extern SPWOOB_MOVCL SPWOOB_MOVCL_xlt (BYTE r);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* protection__________INCLUDE_SPWAWLIB_SPWOOB_TYPES_H */

/** @} */

