/** \file
 * The SPWaW Library - SPWOOB type codes.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
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



/*! SPWOOB unit class */
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

/*! SPWOOB unit class lookup function
 *
 * \param e	SPWOOB unit class
 * \return	Pointer to const string with SPWOOB unit class name
 */
extern const char *SPWOOB_UCLASS_lookup (SPWOOB_UCLASS e);
/*! SPWOOB unit class code lookup function
 *
 * \param e	SPWOOB unit class
 * \return	Pointer to const string with SPWOOB unit class code
 */
extern const char *SPWOOB_UCLASS_lookup_code (SPWOOB_UCLASS e);



/*! SPWOOB weapon class */
typedef enum e_SPWOOB_WCLASS {
	SPWOOB_WCLASS_START = 0,                     		/*!< \internal start code         */
	SPWOOB_WCLASS_UNKNOWN = 0,                   		/*!< unknown weapon               */
	SPWOOB_WCLASS_INF_PRM,                       		/*!< primary infantry weapon      */
	SPWOOB_WCLASS_INF_SEC,                       		/*!< secondary infantry weapon    */
	SPWOOB_WCLASS_TEAM,                          		/*!< team weapon                  */
	SPWOOB_WCLASS_AA,                            		/*!< anti-air weapon              */
	SPWOOB_WCLASS_SML_GUN,                       		/*!< light gun                    */
	SPWOOB_WCLASS_MDM_GUN,                       		/*!< medium gun                   */
	SPWOOB_WCLASS_LRG_GUN,                       		/*!< large gun                    */
	SPWOOB_WCLASS_FLAME_INF,                     		/*!< infantry flamethrower        */
	SPWOOB_WCLASS_FLAME_VHC,                     		/*!< vehicle flamethrower         */
	SPWOOB_WCLASS_NAVAL,                         		/*!< naval artillery              */
	SPWOOB_WCLASS_AIRCRAFT,                      		/*!< aircraft weapon              */
	SPWOOB_WCLASS_SAM,                           		/*!< surface-to-air missile       */
	SPWOOB_WCLASS_AT,                            		/*!< anti-tank missile            */
	SPWOOB_WCLASS_LRG_CLSTR,                     		/*!< large cluster bomb           */
	SPWOOB_WCLASS_SML_CLSTR,                     		/*!< small cluster bomb           */
	SPWOOB_WCLASS_ARM,                           		/*!< ARM                          */
	SPWOOB_WCLASS_ASM,                           		/*!< ASM                          */
	SPWOOB_WCLASS_NAPALM,                        		/*!< napalm                       */
	SPWOOB_WCLASS_AUTO_CANNON,                   		/*!< auto cannon                  */
	SPWOOB_WCLASS__NONE,                         		/*!< \internal default code       */
	SPWOOB_WCLASS_LIMIT = SPWOOB_WCLASS__NONE    		/*!< \internal final code         */
} SPWOOB_WCLASS;
#define	SPWOOB_WCLASS_CNT	(SPWOOB_WCLASS_LIMIT - SPWOOB_WCLASS_START + 1)

/*! SPWOOB weapon class lookup function
 *
 * \param e	SPWOOB weapon class
 * \return	pointer to const string with SPWOOB weapon class name
 */
extern const char *SPWOOB_WCLASS_lookup (SPWOOB_WCLASS e);

/*! SPWOOB weapon class translation function
 *
 * \param r	raw SPWOOB weapon class value
 * \return	SPWOOB weapon class
 */
extern SPWOOB_WCLASS SPWOOB_WCLASS_xlt (BYTE r);



/*! SPWOOB formation status */
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

/*! SPWOOB formation status lookup function
 *
 * \param e	SPWOOB formation status
 * \return	pointer to const string with SPWOOB formation status name
 */
extern const char *SPWOOB_FSTAT_lookup (SPWOOB_FSTAT e);

/*! SPWOOB formation status code lookup function
 *
 * \param e	SPWOOB formation status
 * \return	pointer to const string with SPWOOB formation status code
 */
extern const char *SPWOOB_FSTAT_lookup_code (SPWOOB_FSTAT e);

/*! SPWOOB formation status translation function
 *
 * \param r	raw SPWOOB formation status value
 * \return	SPWOOB formation status
 */
extern SPWOOB_FSTAT SPWOOB_FSTAT_xlt (BYTE r);



/*! SPWOOB formation type */
typedef enum e_SPWOOB_FTYPE {
	SPWOOB_FTYPE_START = 0,                      		/*!< \internal start code */
	SPWOOB_FTYPE_SEC = 0,                        		/*!< section              */
	SPWOOB_FTYPE_PLT,                            		/*!< platoon              */
	SPWOOB_FTYPE_COM,                            		/*!< company              */
	SPWOOB_FTYPE_GRP,                            		/*!< combat group         */
	SPWOOB_FTYPE_NAV,                            		/*!< naval                */
	SPWOOB_FTYPE_FHQ,                            		/*!< force HQ             */
	SPWOOB_FTYPE__NONE,                          		/*!< \internal default code */
	SPWOOB_FTYPE_LIMIT = SPWOOB_FTYPE__NONE      		/*!< \internal final code */
} SPWOOB_FTYPE;
#define	SPWOOB_FTYPE_CNT	(SPWOOB_FTYPE_LIMIT - SPWOOB_FTYPE_START + 1)

/*! SPWOOB formation type lookup function
 *
 * \param e	SPWOOB formation type
 * \return	pointer to const string with SPWOOB formation type name
 */
extern const char *SPWOOB_FTYPE_lookup (SPWOOB_FTYPE e);

/*! SPWOOB formation type translation function
 *
 * \param r	raw SPWOOB formation type value
 * \return	SPWOOB formation type
 */
extern SPWOOB_FTYPE SPWOOB_FTYPE_xlt (BYTE r);



/*! SPWOOB formation purchase screen type */
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

/*! SPWOOB formation purchase screen type lookup function
 *
 * \param e	SPWOOB formation purchase screen type
 * \return	pointer to const string with SPWOOB formation purchase screen type name
 */
extern const char *SPWOOB_FPSCR_lookup (SPWOOB_FPSCR e);

/*! SPWOOB formation purchase screen type translation function
 *
 * \param r	raw SPWOOB formation purchase screen type value
 * \return	SPWOOB formation purchase screen type
 */
extern SPWOOB_FPSCR SPWOOB_FPSCR_xlt (BYTE r);



/*! SPWOOB movement class */
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

/*! SPWOOB movement class lookup function
 *
 * \param e	SPWOOB movement class
 * \return	pointer to const string with SPWOOB movement class name
 */
extern const char *SPWOOB_MOVCL_lookup (SPWOOB_MOVCL e);

/*! SPWOOB movement class translation function
 *
 * \param r	raw SPWOOB movement class value
 * \return	SPWOOB movement class
 */
extern SPWOOB_MOVCL SPWOOB_MOVCL_xlt (BYTE r);



/*! SPWOOB unit type */
typedef enum e_SPWOOB_UTYPE {
	SPWOOB_UTYPE_START = 0,                      		/*!< \internal start code                */
	SPWOOB_UTYPE_INF = 0,                        		/*!< infantry                            */
	SPWOOB_UTYPE_LT_INF,                         		/*!< light infantry                      */
	SPWOOB_UTYPE_MD_INF,                         		/*!< medium infantry                     */
	SPWOOB_UTYPE_HV_INF,                         		/*!< heavy wpns squad                    */
	SPWOOB_UTYPE_RFL_INF,                        		/*!< rifle squad                         */
	SPWOOB_UTYPE_RFL_SEC,                        		/*!< rifle section                       */
	SPWOOB_UTYPE_SMG_SEC,                        		/*!< SMG section                         */
	SPWOOB_UTYPE_MG,                             		/*!< machine gun                         */
	SPWOOB_UTYPE_MG_SEC,                         		/*!< MG section                          */
	SPWOOB_UTYPE_MG_TEAM,                        		/*!< MG team                             */
	SPWOOB_UTYPE_LMG_SEC,                        		/*!< LMG section                         */
	SPWOOB_UTYPE_HV_MG,                          		/*!< heavy MG squad                      */
	SPWOOB_UTYPE_SPECOP,                         		/*!< special forces                      */
	SPWOOB_UTYPE_ENG,                            		/*!< engineers                           */
	SPWOOB_UTYPE_ENG_SPT,                        		/*!< engineer support squad              */
	SPWOOB_UTYPE_ELITE,                          		/*!< elite infantry                      */
	SPWOOB_UTYPE_AIR_INF,                        		/*!< airborne infantry                   */
	SPWOOB_UTYPE_LNCR,                           		/*!< lancers                             */
	SPWOOB_UTYPE_SNIPER,                         		/*!< sniper                              */
	SPWOOB_UTYPE_CONSCR,                         		/*!< conscripts                          */
	SPWOOB_UTYPE_SL_INF,                         		/*!< second line infantry                */
	SPWOOB_UTYPE_SL_SPT,                         		/*!< second line support squad           */
	SPWOOB_UTYPE_IRR_INF,                        		/*!< irregulars                          */
	SPWOOB_UTYPE_GUERILLA,                       		/*!< guerilla forces                     */
	SPWOOB_UTYPE_NAT_INF,                        		/*!< native troops                       */
	SPWOOB_UTYPE_POLICE,                         		/*!< police                              */
	SPWOOB_UTYPE_ARM_INF,                        		/*!< armored infantry                    */
	SPWOOB_UTYPE_LT_ARM_INF,                     		/*!< light armored infantry              */
	SPWOOB_UTYPE_MD_ARM_INF,                     		/*!< medium armored infantry             */
	SPWOOB_UTYPE_HV_ARM_INF,                     		/*!< armored heavy wpns squad            */
	SPWOOB_UTYPE_ARM_SMG,                        		/*!< armored SMG squad                   */
	SPWOOB_UTYPE_ARM_SPT,                        		/*!< armored support squad               */
	SPWOOB_UTYPE_ARM_ENG,                        		/*!< armored engineers                   */
	SPWOOB_UTYPE_CMD,                            		/*!< commandos                           */
	SPWOOB_UTYPE_LT_CMD,                         		/*!< light commandos                     */
	SPWOOB_UTYPE_MD_CMD,                         		/*!< medium commandos                    */
	SPWOOB_UTYPE_HV_CMD,                         		/*!< commandos heavy wpns squad          */
	SPWOOB_UTYPE_CMD_SPT,                        		/*!< commando support squad              */
	SPWOOB_UTYPE_CMD_ENG,                        		/*!< commando engineers                  */
	SPWOOB_UTYPE_CMD_SNIPER,                     		/*!< commando sniper                     */
	SPWOOB_UTYPE_COL_INF,                        		/*!< colonial infantry                   */
	SPWOOB_UTYPE_COL_LT_INF,                     		/*!< colonial light infantry             */
	SPWOOB_UTYPE_COL_HV_INF,                     		/*!< colonial heavy wpns squad           */
	SPWOOB_UTYPE_COL_SPT,                        		/*!< colonial support squad              */
	SPWOOB_UTYPE_GHRK,                           		/*!< Ghurkas                             */
	SPWOOB_UTYPE_HV_GHRK,                        		/*!< Ghurkas heavy wpns squad            */
	SPWOOB_UTYPE_GRDS_INF,                       		/*!< Guards infantry                     */
	SPWOOB_UTYPE_LT_GRDS,                        		/*!< light Guards squad                  */
	SPWOOB_UTYPE_MD_GRDS,                        		/*!< medium Guards squad                 */
	SPWOOB_UTYPE_HV_GRDS,                        		/*!< Guards heavy wpns squad             */
	SPWOOB_UTYPE_GRDS_SMG,                       		/*!< Guards SMG squad                    */
	SPWOOB_UTYPE_GRDS_SPT,                       		/*!< Guards support squad                */
	SPWOOB_UTYPE_GRDS_ENG,                       		/*!< Guards engineers                    */
	SPWOOB_UTYPE_LGN,                            		/*!< Legionnaires                        */
	SPWOOB_UTYPE_LGN_SMG,                        		/*!< Legionnaires SMG squad              */
	SPWOOB_UTYPE_LGN_SPT,                        		/*!< Legionnaires support squad          */
	SPWOOB_UTYPE_MLT,                            		/*!< militia                             */
	SPWOOB_UTYPE_LT_MLT,                         		/*!< light militia                       */
	SPWOOB_UTYPE_HV_MLT,                         		/*!< militia heavy wpns squad            */
	SPWOOB_UTYPE_MLT_SPT,                        		/*!< militia support squad               */
	SPWOOB_UTYPE_MOT_INF,                        		/*!< motorized infantry                  */
	SPWOOB_UTYPE_MOT_ENG,                        		/*!< motorized engineers                 */
	SPWOOB_UTYPE_MTN_INF,                        		/*!< mountain infantry                   */
	SPWOOB_UTYPE_MTN_LT_INF,                     		/*!< mountain light infantry             */
	SPWOOB_UTYPE_MTN_HV_INF,                     		/*!< mountain heavy wpns sqd             */
	SPWOOB_UTYPE_MTN_SPT,                        		/*!< mountain support squad              */
	SPWOOB_UTYPE_MTN_ENG,                        		/*!< mountain engineers                  */
	SPWOOB_UTYPE_MTN_ENG_SPT,                    		/*!< mountain engineers support squad    */
	SPWOOB_UTYPE_MTN_SNIPER,                     		/*!< mountain sniper                     */
	SPWOOB_UTYPE_PARA,                           		/*!< paratroop                           */
	SPWOOB_UTYPE_PARA_LT_INF,                    		/*!< paratroop light infantry            */
	SPWOOB_UTYPE_PARA_MD_INF,                    		/*!< paratroop medium infantry           */
	SPWOOB_UTYPE_PARA_SMG,                       		/*!< paratroop SMG squad                 */
	SPWOOB_UTYPE_PARA_MG,                        		/*!< paratroop MG squad                  */
	SPWOOB_UTYPE_PARA_SPT,                       		/*!< paratroop support squad             */
	SPWOOB_UTYPE_PARA_ENG,                       		/*!< paratroop engineers                 */
	SPWOOB_UTYPE_PARA_SNIPER,                    		/*!< paratroop sniper                    */
	SPWOOB_UTYPE_PRTS,                           		/*!< partisan                            */
	SPWOOB_UTYPE_PRTS_INF,                       		/*!< partisan squad                      */
	SPWOOB_UTYPE_PRTS_SPT,                       		/*!< partisan support squad              */
	SPWOOB_UTYPE_PRTS_ENG,                       		/*!< partisan pioneers                   */
	SPWOOB_UTYPE_PRTS_BAND,                      		/*!< partisan band                       */
	SPWOOB_UTYPE_RNGR,                           		/*!< Ranger                              */
	SPWOOB_UTYPE_LT_RNGR,                        		/*!< light Rangers squad                 */
	SPWOOB_UTYPE_HV_RNGR,                        		/*!< Rangers heavy wpns squad            */
	SPWOOB_UTYPE_RNGR_SPT,                       		/*!< Ranger support squad                */
	SPWOOB_UTYPE_RNGR_ENG,                       		/*!< Ranger engineers                    */
	SPWOOB_UTYPE_RNGR_SNIPER,                    		/*!< Ranger sniper                       */
	SPWOOB_UTYPE_SKI,                            		/*!< ski troops                          */
	SPWOOB_UTYPE_LT_SKI,                         		/*!< light ski infantry                  */
	SPWOOB_UTYPE_HV_SKI,                         		/*!< ski heavy wpns squad                */
	SPWOOB_UTYPE_SKI_SMG,                        		/*!< ski SMG squad                       */
	SPWOOB_UTYPE_SKI_ENG,                        		/*!< ski engineers                       */
	SPWOOB_UTYPE_SKI_SNIPER,                     		/*!< ski sniper                          */
	SPWOOB_UTYPE_WSS_INF,                        		/*!< Waffen SS                           */
	SPWOOB_UTYPE_WSS_HV_INF,                     		/*!< Waffen SS heavy wpns squad          */
	SPWOOB_UTYPE_WSS_RFL,                        		/*!< Waffen SS rifle squad               */
	SPWOOB_UTYPE_WSS_SMG,                        		/*!< Waffen SS SMG squad                 */
	SPWOOB_UTYPE_WSS_ENG,                        		/*!< Waffen SS engineers                 */
	SPWOOB_UTYPE_CAV,                            		/*!< cavalry                             */
	SPWOOB_UTYPE_MD_CAV,                         		/*!< medium cavalry                      */
	SPWOOB_UTYPE_LT_CAV_TNK,                     		/*!< light cavalry tank                  */
	SPWOOB_UTYPE_CAV_TNK,                        		/*!< cavalry tank                        */
	SPWOOB_UTYPE_CAV_DIS,                        		/*!< dismounted cavalry                  */
	SPWOOB_UTYPE_CAV_MNT,                        		/*!< cavalry mounts                      */
	SPWOOB_UTYPE_COL_CAV,                        		/*!< colonial cavalry                    */
	SPWOOB_UTYPE_COL_LT_CAV,                     		/*!< colonial light cavalry              */
	SPWOOB_UTYPE_TNY_TNK,                        		/*!< tankette                            */
	SPWOOB_UTYPE_CS_TNY_TNK,                     		/*!< CS tankette                         */
	SPWOOB_UTYPE_TNK,                            		/*!< tank                                */
	SPWOOB_UTYPE_CS_TNK,                         		/*!< CS tank                             */
	SPWOOB_UTYPE_MB_TNK,                         		/*!< main battle tank                    */
	SPWOOB_UTYPE_LT_TNK,                         		/*!< light tank                          */
	SPWOOB_UTYPE_MD_TNK,                         		/*!< medium tank                         */
	SPWOOB_UTYPE_HV_TNK,                         		/*!< heavy tank                          */
	SPWOOB_UTYPE_VHV_TNK,                        		/*!< very heavy tank                     */
	SPWOOB_UTYPE_CR_TNK,                         		/*!< cruiser tank                        */
	SPWOOB_UTYPE_HV_CR_TNK,                      		/*!< heavy cruiser tank                  */
	SPWOOB_UTYPE_CS_CR_TNK,                      		/*!< CS cruiser tank                     */
	SPWOOB_UTYPE_INF_TNK,                        		/*!< infantry tank                       */
	SPWOOB_UTYPE_LT_INF_TNK,                     		/*!< light infantry tank                 */
	SPWOOB_UTYPE_CS_INF_TNK,                     		/*!< CS infantry tank                    */
	SPWOOB_UTYPE_ASSAULT,                        		/*!< assault gun                         */
	SPWOOB_UTYPE_LT_AIR_TNK,                     		/*!< airborne lt tank                    */
	SPWOOB_UTYPE_DD_TNK,                         		/*!< DD tank                             */
	SPWOOB_UTYPE_ENG_TNK,                        		/*!< engineer tank                       */
	SPWOOB_UTYPE_FLM_TNK,                        		/*!< flame tank                          */
	SPWOOB_UTYPE_MG_TNK,                         		/*!< guntank                             */
	SPWOOB_UTYPE_MINECLR_TNK,                    		/*!< mine clearing tank                  */
	SPWOOB_UTYPE_SNRKL_TNK,                      		/*!< snorkel tank                        */
	SPWOOB_UTYPE_SPEC_TNK,                       		/*!< special tank                        */
	SPWOOB_UTYPE_CAPT_TNK,                       		/*!< captured tank                       */
	SPWOOB_UTYPE_COL_TNK,                        		/*!< colonial tank                       */
	SPWOOB_UTYPE_LL_TNK,                         		/*!< lend-lease tank                     */
	SPWOOB_UTYPE_PROTO_TNK,                      		/*!< prototype tank                      */
	SPWOOB_UTYPE_APC,                            		/*!< APC                                 */
	SPWOOB_UTYPE_WH_APC,                         		/*!< wheeled APC                         */
	SPWOOB_UTYPE_WH_APC_GUN,                     		/*!< wheeled gun APC                     */
	SPWOOB_UTYPE_HT,                             		/*!< halftrack                           */
	SPWOOB_UTYPE_HT_APC_GUN,                     		/*!< halftrack gun APC                   */
	SPWOOB_UTYPE_AHT,                            		/*!< armed halftrack                     */
	SPWOOB_UTYPE_TR_APC,                         		/*!< tracked APC                         */
	SPWOOB_UTYPE_HV_TR_APC,                      		/*!< heavy tracked APC                   */
	SPWOOB_UTYPE_TR_APC_GUN,                     		/*!< tracked gun APC                     */
	SPWOOB_UTYPE_COL_APC,                        		/*!< colonial APC                        */
	SPWOOB_UTYPE_IMP_APC,                        		/*!< improvised apc                      */
	SPWOOB_UTYPE_MORT,                           		/*!< mortar                              */
	SPWOOB_UTYPE_SP_MORT,                        		/*!< SP mortar                           */
	SPWOOB_UTYPE_LT_MORT,                        		/*!< light mortar                        */
	SPWOOB_UTYPE_LT_SP_MORT,                     		/*!< light sp mortar                     */
	SPWOOB_UTYPE_HV_MORT,                        		/*!< heavy mortar                        */
	SPWOOB_UTYPE_SA_MORT,                        		/*!< mortar(sub type a)                  */
	SPWOOB_UTYPE_SB_MORT,                        		/*!< mortar(sub type b)                  */
	SPWOOB_UTYPE_SC_MORT,                        		/*!< mortar(sub type c)                  */
	SPWOOB_UTYPE_SD_MORT,                        		/*!< mortar(sub type d)                  */
	SPWOOB_UTYPE_PARA_MORT,                      		/*!< paratroop mortar                    */
	SPWOOB_UTYPE_FLD_GUN,                        		/*!< field gun                           */
	SPWOOB_UTYPE_SP_GUN,                         		/*!< SP gun                              */
	SPWOOB_UTYPE_CAPT_SP_GUN,                    		/*!< captured SP gun                     */
	SPWOOB_UTYPE_INF_GUN,                        		/*!< infantry gun                        */
	SPWOOB_UTYPE_SP_INF_GUN,                     		/*!< SP infantry gun                     */
	SPWOOB_UTYPE_INF_CS_GUN,                     		/*!< infantry support gun                */
	SPWOOB_UTYPE_COL_FLD_GUN,                    		/*!< colonial field gun                  */
	SPWOOB_UTYPE_HWTZR,                          		/*!< howitzer                            */
	SPWOOB_UTYPE_INF_HWTZR,                      		/*!< infantry howitzer                   */
	SPWOOB_UTYPE_AIR_FLD_HWTZR,                  		/*!< airborne field howitzer             */
	SPWOOB_UTYPE_PACK_HWTZR,                     		/*!< pack howitzer                       */
	SPWOOB_UTYPE_SP_ART,                         		/*!< SP artillery                        */
	SPWOOB_UTYPE_RKT,                            		/*!< rocket launcher                     */
	SPWOOB_UTYPE_SP_RKT,                         		/*!< SP rocket launcher                  */
	SPWOOB_UTYPE_OB_ART,                         		/*!< offboard artillery                  */
	SPWOOB_UTYPE_LT_OB_ART,                      		/*!< offboard light artillery            */
	SPWOOB_UTYPE_HV_OB_ART,                      		/*!< heavy offboard artillery            */
	SPWOOB_UTYPE_OB_RKT,                         		/*!< offboard rocket launcher            */
	SPWOOB_UTYPE_AT,                             		/*!< AT gun                              */
	SPWOOB_UTYPE_LT_AT,                          		/*!< light AT gun                        */
	SPWOOB_UTYPE_HV_AT,                          		/*!< heavy AT gun                        */
	SPWOOB_UTYPE_SP_AT,                          		/*!< SP AT gun                           */
	SPWOOB_UTYPE_WH_AT,                          		/*!< AT gun truck                        */
	SPWOOB_UTYPE_TD,                             		/*!< tank destroyer                      */
	SPWOOB_UTYPE_TTD,                            		/*!< turreted tank destroyer             */
	SPWOOB_UTYPE_HV_TD,                          		/*!< heavy tank destroyer                */
	SPWOOB_UTYPE_INF_AT,                         		/*!< AT infantry                         */
	SPWOOB_UTYPE_CMD_INF_AT,                     		/*!< commando AT infantry                */
	SPWOOB_UTYPE_MRNS_AT,                        		/*!< marines AT gun                      */
	SPWOOB_UTYPE_PARA_INF_AT,                    		/*!< paratroop AT infantry               */
	SPWOOB_UTYPE_SKI_INF_AT,                     		/*!< ski AT infantry                     */
	SPWOOB_UTYPE_FLAK,                           		/*!< AA gun                              */
	SPWOOB_UTYPE_HV_FLAK,                        		/*!< heavy AA gun                        */
	SPWOOB_UTYPE_SP_FLAK,                        		/*!< SP AA gun                           */
	SPWOOB_UTYPE_LT_SP_FLAK,                     		/*!< light SP AA gun                     */
	SPWOOB_UTYPE_WH_FLAK,                        		/*!< AA gun truck                        */
	SPWOOB_UTYPE_INF_FLAK,                       		/*!< AA MG infantry                      */
	SPWOOB_UTYPE_AMPHIB,                         		/*!< amphibian unit                      */
	SPWOOB_UTYPE_LT_AMPHIB,                      		/*!< light amphibian unit                */
	SPWOOB_UTYPE_HV_AMPHIB,                      		/*!< heavy amphibian unit                */
	SPWOOB_UTYPE_AMPHIB_TNK,                     		/*!< amphibian tank                      */
	SPWOOB_UTYPE_MRNS,                           		/*!< marines                             */
	SPWOOB_UTYPE_MRNS_LT_INF,                    		/*!< marines light infantry              */
	SPWOOB_UTYPE_MRNS_HV_INF,                    		/*!< marines heavy wpns squad            */
	SPWOOB_UTYPE_MRNS_SPT,                       		/*!< marines support squad               */
	SPWOOB_UTYPE_MRNS_ENG,                       		/*!< marines engineers                   */
	SPWOOB_UTYPE_MRNS_SNIPER,                    		/*!< marines sniper                      */
	SPWOOB_UTYPE_FO,                             		/*!< forward observer                    */
	SPWOOB_UTYPE_VEH_FO,                         		/*!< FO vehicle                          */
	SPWOOB_UTYPE_TR_FO,                          		/*!< tracked FO                          */
	SPWOOB_UTYPE_AIR_FO,                         		/*!< FO aircraft                         */
	SPWOOB_UTYPE_HELI_FO,                        		/*!< FO helicopter                       */
	SPWOOB_UTYPE_MRNS_FO,                        		/*!< marines FO                          */
	SPWOOB_UTYPE_MTN_FO,                         		/*!< mountain FO                         */
	SPWOOB_UTYPE_PARA_FO,                        		/*!< paratroop FO                        */
	SPWOOB_UTYPE_RNGR_FO,                        		/*!< Ranger FO                           */
	SPWOOB_UTYPE_SCOUT,                          		/*!< scout                               */
	SPWOOB_UTYPE_VEH_SCOUT,                      		/*!< scout vehicle                       */
	SPWOOB_UTYPE_HT_SCOUT,                       		/*!< scout halftrack                     */
	SPWOOB_UTYPE_APC_SCOUT,                      		/*!< scout APC                           */
	SPWOOB_UTYPE_TNK_SCOUT,                      		/*!< scout tank                          */
	SPWOOB_UTYPE_CAV_SCOUT,                      		/*!< cavalry scout                       */
	SPWOOB_UTYPE_CMD_SCOUT,                      		/*!< commando scout                      */
	SPWOOB_UTYPE_COL_SCOUT,                      		/*!< colonial scout                      */
	SPWOOB_UTYPE_GRDS_SCOUT,                     		/*!< Guards scout                        */
	SPWOOB_UTYPE_MLT_SCOUT,                      		/*!< militia scout                       */
	SPWOOB_UTYPE_MRNS_SCOUT,                     		/*!< marines scout                       */
	SPWOOB_UTYPE_MTN_SCOUT,                      		/*!< mountain scout                      */
	SPWOOB_UTYPE_NAT_SCOUT,                      		/*!< native scout                        */
	SPWOOB_UTYPE_PARA_SCOUT,                     		/*!< paratroop scout                     */
	SPWOOB_UTYPE_PRTS_SCOUT,                     		/*!< partisan scout                      */
	SPWOOB_UTYPE_RNGR_SCOUT,                     		/*!< Ranger scout                        */
	SPWOOB_UTYPE_SKI_SCOUT,                      		/*!< ski scout                           */
	SPWOOB_UTYPE_BCYCLE,                         		/*!< bicycle squad                       */
	SPWOOB_UTYPE_HV_BCYCLE,                      		/*!< bicycle heavy wpns squad            */
	SPWOOB_UTYPE_MCYCLE,                         		/*!< motorcycle                          */
	SPWOOB_UTYPE_HV_MCYCLE,                      		/*!< motorcycle heavy wpns squad         */
	SPWOOB_UTYPE_ARM_CAR,                        		/*!< armored car                         */
	SPWOOB_UTYPE_LT_ARM_CAR,                     		/*!< light armored car                   */
	SPWOOB_UTYPE_HV_ARM_CAR,                     		/*!< heavy armored car                   */
	SPWOOB_UTYPE_PATROL,                         		/*!< patrol                              */
	SPWOOB_UTYPE_HQ,                             		/*!< force HQ                            */
	SPWOOB_UTYPE_PLT_HQ,                         		/*!< platoon HQ                          */
	SPWOOB_UTYPE_CC_TNK,                         		/*!< command tank                        */
	SPWOOB_UTYPE_PRMV,                           		/*!< prime mover                         */
	SPWOOB_UTYPE_ART_PRMV,                       		/*!< artillery prime mover               */
	SPWOOB_UTYPE_LT_TRK,                         		/*!< light truck                         */
	SPWOOB_UTYPE_MD_TRK,                         		/*!< medium truck                        */
	SPWOOB_UTYPE_HV_TRK,                         		/*!< heavy truck                         */
	SPWOOB_UTYPE_ARM_TRK,                        		/*!< armored truck                       */
	SPWOOB_UTYPE_HETS,                           		/*!< heavy transporter                   */
	SPWOOB_UTYPE_CRG_AIR,                        		/*!< transport aircraft                  */
	SPWOOB_UTYPE_PARA_AIR,                       		/*!< paratroop transport                 */
	SPWOOB_UTYPE_GUN_TEAM,                       		/*!< gun team                            */
	SPWOOB_UTYPE_TEAM,                           		/*!< horse team                          */
	SPWOOB_UTYPE_WAGON,                          		/*!< wagon                               */
	SPWOOB_UTYPE_PACK,                           		/*!< pack animals                        */
	SPWOOB_UTYPE_AIRF,                           		/*!< fighter-bomber                      */
	SPWOOB_UTYPE_AIRB,                           		/*!< level bomber                        */
	SPWOOB_UTYPE_HELI,                           		/*!< helicopter                          */
	SPWOOB_UTYPE_AHELI,                          		/*!< attack helicopter                   */
	SPWOOB_UTYPE_GA,                             		/*!< ground attack                       */
	SPWOOB_UTYPE_GLIDER,                         		/*!< glider                              */
	SPWOOB_UTYPE_HV_GLIDER,                      		/*!< heavy glider                        */
	SPWOOB_UTYPE_BARGE,                          		/*!< landing barge                       */
	SPWOOB_UTYPE_BARGECARR,                      		/*!< barge carrier                       */
	SPWOOB_UTYPE_LCG,                            		/*!< landing craft gunboat               */
	SPWOOB_UTYPE_LCS,                            		/*!< landing craft support               */
	SPWOOB_UTYPE_ABOAT,                          		/*!< assault boat                        */
	SPWOOB_UTYPE_PBOAT,                          		/*!< patrol boat                         */
	SPWOOB_UTYPE_RBOAT,                          		/*!< rivercraft                          */
	SPWOOB_UTYPE_TBOAT,                          		/*!< torpedo boat                        */
	SPWOOB_UTYPE_SHIP,                           		/*!< ship                                */
	SPWOOB_UTYPE_NAVBOMB,                        		/*!< naval bombardment                   */
	SPWOOB_UTYPE_FORT,                           		/*!< fort                                */
	SPWOOB_UTYPE_EMP_FORT,                       		/*!< gun emplacement                     */
	SPWOOB_UTYPE_NAV_FORT,                       		/*!< naval fortification                 */
	SPWOOB_UTYPE_BUNKER,                         		/*!< bunker                              */
	SPWOOB_UTYPE_SNOW,                           		/*!< snow vehicle                        */
	SPWOOB_UTYPE_UTIL,                           		/*!< utility vehicle                     */
	SPWOOB_UTYPE_AMMOCARR,                       		/*!< ammo carrier                        */
	SPWOOB_UTYPE_AMMODUMP,                       		/*!< ammo dump                           */
	SPWOOB_UTYPE_CREW,                           		/*!< crew                                */
	SPWOOB_UTYPE_CAVE,                           		/*!< cave                                */
	SPWOOB_UTYPE_EXPLOSIVE,                      		/*!< explosive                           */
	SPWOOB_UTYPE_RESERVED,                       		/*!< reserved                            */
	SPWOOB_UTYPE_UNKNOWN,                        		/*!< unknown unit type                   */
	SPWOOB_UTYPE__NONE,                          		/*!< \internal default code              */
	SPWOOB_UTYPE_LIMIT = SPWOOB_UTYPE__NONE      		/*!< \internal final code                */
} SPWOOB_UTYPE;
#define	SPWOOB_UTYPE_CNT	(SPWOOB_UTYPE_LIMIT - SPWOOB_UTYPE_START + 1)

/*! SPWOOB unit type lookup function
 *
 * \param e	SPWOOB unit type
 * \return	pointer to const string with SPWOOB unit type name
 */
extern const char *SPWOOB_UTYPE_lookup (SPWOOB_UTYPE e);

/*! SPWOOB unit type classification function
 *
 * \param e	SPWOOB unit type value
 * \return	SPWOOB_UCLASS enum
 */
extern SPWOOB_UCLASS SPWOOB_UTYPE_classify (SPWOOB_UTYPE e);



/*! raw SPWAW OOB unit type translation function
 *
 * \param r	raw SPWAW OOB unit type value
 * \return	SPWOOB_UTYPE enum
 */
extern SPWOOB_UTYPE SPWOOB_SPWAW_UTYPE_xlt (BYTE r);



/*! raw winSPWW2 OOB unit type translation function
 *
 * \param r	raw winSPWW2 OOB unit type value
 * \return	SPWOOB_UTYPE enum
 */
extern SPWOOB_UTYPE SPWOOB_WINSPWW2_UTYPE_xlt (BYTE r);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* protection__________INCLUDE_SPWAWLIB_SPWOOB_TYPES_H */

/** @} */

