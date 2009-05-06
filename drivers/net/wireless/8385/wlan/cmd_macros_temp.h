#ifndef _CMD_MACROS_TEMP_H
#define _CMD_MACROS_TEMP_H

#define HostCmd_CMD_SET_DTIM_MULTIPLE 0x004f
#define HostCmd_CMD_GET_HW_SPEC 0x0003
#define HostCmd_CMD_802_11_SCAN	0x0006
#define HostCmd_CMD_MAC_CONTROL	0x0028

#ifdef NEW_ASSOCIATION
#define	HostCmd_CMD_802_11_ASSOCIATE		0x0050
#else
#define HostCmd_CMD_802_11_ASSOCIATE		0x0012
#endif

#define HostCmd_CMD_802_11_REASSOCIATE		0x0025
#define HostCmd_CMD_802_11_DEAUTHENTICATE	0x0024
#define HostCmd_CMD_802_11_SET_WEP		0x0013
#define HostCmd_CMD_802_11_AD_HOC_START		0x002b
#define HostCmd_CMD_802_11_RESET		0x0005
#define HostCmd_CMD_802_11_AUTHENTICATE		0x0011
#define HostCmd_CMD_802_11_SNMP_MIB		0x0016
#define HostCmd_CMD_802_11_RF_TX_POWER		0x001e
#define HostCmd_CMD_802_11_DATA_RATE		0x0022
#define HostCmd_CMD_MAC_MULTICAST_ADR		0x0010
#define HostCmd_CMD_802_11_AD_HOC_JOIN		0x002c
#define HostCmd_CMD_802_11_RSSI			0x001f
#define HostCmd_CMD_802_11_DISASSOCIATE		0x0026
#define HostCmd_CMD_802_11_AD_HOC_STOP		0x0040
#define HostCmd_CMD_802_11_ENABLE_RSN		0x002f
#define HostCmd_CMD_802_11_RSN_AUTH_SUITES	0x0032
#define HostCmd_CMD_802_11_PWK_KEY	 	0x0034
#define HostCmd_CMD_802_11_GRP_KEY		0x0035
#define HostCmd_CMD_802_11_RADIO_CONTROL	0x001c
#define HostCmd_CMD_802_11_RF_ANTENNA		0x0020
#define HostCmd_CMD_802_11_RF_CHANNEL		0x001D
#define HostCmd_CMD_802_11_MAC_ADDRESS		0X004D
#define HostCmd_CMD_802_11_EEPROM_ACCESS        0x0059

//command responses

#define HostCmd_RET_HW_SPEC_INFO		0x0380
#define HostCmd_RET_802_11_SCAN			0x0680
#define HostCmd_RET_802_11_ASSOCIATE		0x1280
#define HostCmd_RET_802_11_REASSOCIATE		0x2580
#define HostCmd_RET_802_11_SNMP_MIB		0x1680
#define HostCmd_RET_802_11_RF_TX_POWER		0x1e80
#define HostCmd_RET_802_11_DATA_RATE		0x2280
#define HostCmd_RET_MAC_CONTROL			0x2880
#define HostCmd_RET_802_11_AD_HOC_START		0x2B80
#define HostCmd_RET_802_11_AD_HOC_JOIN		0x2C80
#define HostCmd_RET_802_11_AD_HOC_STOP		0x4080
#define HostCmd_RET_802_11_RF_CHANNEL		0x1D80
#define HostCmd_RET_802_11_SET_WEP		0x1380
#define HostCmd_RET_802_11_AUTHENTICATE		0x1180
#define HostCmd_RET_802_11_DEAUTHENTICATE       0x2480
#define HostCmd_RET_802_11_RADIO_CONTROL	0x1c80
#define HostCmd_CMD_802_11_NEW_ASSOCIATE 	0x5080 //special case
#define HostCmd_RET_MAC_MULTICAST_ADR           0x1080
#define HostCmd_RET_802_11_RF_ANTENNA           0x2080
#define HostCmd_RET_802_11_MAC_ADDRESS 		0x4D80


#endif	//_CMD_MACROS_TEMP_H