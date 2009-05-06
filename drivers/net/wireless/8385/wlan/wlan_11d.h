/*
File: wlan_11d.h
*/

#ifndef _WLAN_11D_
#define _WLAN_11D_

#define MAX_CHAN_NUM		255

#define UNIVERSAL_REGION_CODE	0xff

/* (Beaconsize(256)-5(IEId,len,contrystr(3))/3(FirstChan,NoOfChan,MaxPwr) */ 
#define MRVDRV_MAX_SUBBAND_802_11D		83 

#define COUNTRY_CODE_LEN	3
#define MAX_NO_OF_CHAN 		40

typedef struct _REGION_CHANNEL *PREGION_CHANNEL;

typedef enum {
	DISABLE_11D = 0,
	ENABLE_11D  = 1,
} state_11d_t;

/*Data structure for Country IE*/
typedef struct _IEEEtypes_SubbandSet {
	u8	FirstChan;
	u8	NoOfChan;
	u8	MaxTxPwr;	
} __ATTRIB_PACK__ IEEEtypes_SubbandSet_t;

typedef struct _IEEEtypes_CountryInfoSet {
	u8	ElementId;
	u8	Len;
	u8	CountryCode[COUNTRY_CODE_LEN];
	IEEEtypes_SubbandSet_t	Subband[1];
} __ATTRIB_PACK__ IEEEtypes_CountryInfoSet_t;

typedef struct _IEEEtypes_CountryInfoFullSet {
	u8	ElementId;
	u8	Len;
	u8	CountryCode[COUNTRY_CODE_LEN];
	IEEEtypes_SubbandSet_t	Subband[MRVDRV_MAX_SUBBAND_802_11D];
} __ATTRIB_PACK__ IEEEtypes_CountryInfoFullSet_t;


typedef struct _MrvlIEtypes_DomainParamSet {
	MrvlIEtypesHeader_t	Header;
	u8			CountryCode[COUNTRY_CODE_LEN];
 	IEEEtypes_SubbandSet_t	Subband[1];
} __ATTRIB_PACK__ MrvlIEtypes_DomainParamSet_t;



/* Define data structure for HostCmd_CMD_802_11D_DOMAIN_INFO */
typedef struct _HostCmd_DS_802_11D_DOMAIN_INFO {
	u16	Action;
	MrvlIEtypes_DomainParamSet_t Domain;
} __ATTRIB_PACK__ HostCmd_DS_802_11D_DOMAIN_INFO, *PHostCmd_DS_802_11D_DOMAIN_INFO;

/* Define data structure for HostCmd_RET_802_11D_DOMAIN_INFO */
typedef struct _HostCmd_DS_802_11D_DOMAIN_INFO_RSP {
	u16	Action;
	MrvlIEtypes_DomainParamSet_t Domain;
} __ATTRIB_PACK__ HostCmd_DS_802_11D_DOMAIN_INFO_RSP, *PHostCmd_DS_802_11D_DOMIAN_INFO_RSP;

/* domain regulatory information */
typedef struct _wlan_802_11d_domain_reg
{
	u8			CountryCode[COUNTRY_CODE_LEN];	/*country Code*/
	u8			NoOfSubband;	/*No. of subband*/
	IEEEtypes_SubbandSet_t	Subband[MRVDRV_MAX_SUBBAND_802_11D]; 
} wlan_802_11d_domain_reg_t;

	
typedef struct  _chan_power_11d {
	u8 chan;
	u8 pwr;
} __ATTRIB_PACK__ chan_power_11d_t;

typedef struct  _parsed_region_chan_11d {
	u8 			band;
	u8 			region;
	char 			CountryCode[COUNTRY_CODE_LEN];
	chan_power_11d_t 	chanPwr[MAX_NO_OF_CHAN];
	u8 			NoOfChan;
} __ATTRIB_PACK__ parsed_region_chan_11d_t;

/* Data for state machine, refer to . ... */
typedef struct _wlan_802_11d_state {
	BOOLEAN		Enable11D; /* True for Enabling  11D*/
} wlan_802_11d_state_t;


typedef struct _region_code_mapping{
	char	region[COUNTRY_CODE_LEN];
	u8	code;
} region_code_mapping_t;

/* function prototypes*/
int wlan_generate_domain_info_11d( 
		parsed_region_chan_11d_t * parsed_region_chan, 
		wlan_802_11d_domain_reg_t *domaininfo);

int wlan_parse_domain_info_11d(
		IEEEtypes_CountryInfoFullSet_t *CountryInfo, u8 band,
		parsed_region_chan_11d_t * parsed_region_chan );
 
u8 wlan_get_scan_type_11d(  u8 chan,  
			parsed_region_chan_11d_t *parsed_region_chan);

u8 wlan_region_2_code( char *region );

char *wlan_code_2_region( u8 code );

u32 chan_2_freq( u16 chan, u8 band );

int wlan_set_domain_info_11d( wlan_private * priv );

state_11d_t wlan_get_state_11d( wlan_private *priv );

void wlan_init_11d( wlan_private *priv );

int wlan_enable_11d( wlan_private * priv, state_11d_t flag );

int wlan_set_universaltable(wlan_private *priv, u8 band);

void  wlan_generate_parsed_region_chan_11d(
		PREGION_CHANNEL region_chan,
		parsed_region_chan_11d_t *parsed_region_chan );


int wlan_cmd_802_11d_domain_info(wlan_private * priv,
	   HostCmd_DS_COMMAND * cmd, u16 cmdno, u16 CmdOption);

int wlan_cmd_enable_11d( wlan_private *priv, struct iwreq *wrq );

int wlan_ret_802_11d_domain_info(wlan_private * priv, 
				HostCmd_DS_COMMAND * resp);

int wlan_parse_dnld_countryinfo_11d( wlan_private *priv );

int wlan_create_dnld_countryinfo_11d( wlan_private *priv );

#endif /* _WLAN_11D_ */

