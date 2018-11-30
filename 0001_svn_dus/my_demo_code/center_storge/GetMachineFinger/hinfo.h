#ifndef __HINFO_H__
#define __HINFO_H__

char* GetDMIString(int type,char* str,int len);

#define DMI_TYPE_BIOS 0
#define DMI_TYPE_SYSTEM 1
#define DMI_TYPE_BASEBOARD 2
#define DMI_TYPE_CHASSIS 3
#define DMI_TYPE_PROCESSOR 4
#define DMI_TYPE_SLOT 9
#define DMI_TYPE_ONBOARD_DEVICE 10
#define DMI_TYPE_OEM_STRING 11
#define DMI_TYPE_BIOS_LANGUAGE 13


#endif
