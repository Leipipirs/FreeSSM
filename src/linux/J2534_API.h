/*
 * J2534_API.h - API for accessing SAE-J2534 compliant interfaces
 *
 * Copyright (C) 2009-2010 Comer352l
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef J2534_API_H
#define J2534_API_H


//#define __J2534_API_DEBUG__


#ifdef __J2534_API_DEBUG__
    #include <iostream>
#endif
#include <string>
#include <vector>
#include "../J2534.h"
#include <cstring>	// memset(), strcpy(), ...
extern "C"
{
    #include <errno.h>
    #include <dlfcn.h>
}


#define     J2534API_ERROR_FCN_NOT_SUPPORTED     -1
#define     J2534API_ERROR_INVALID_LIBRARY       -2


enum J2534_API_version {J2534_API_v0202, J2534_API_v0404};


enum J2534_protocol_flags
{
	PROTOCOL_FLAG_J1850VPW     = 0x01,
	PROTOCOL_FLAG_J1850PWM     = 0x02,
	PROTOCOL_FLAG_ISO9141      = 0x04,
	PROTOCOL_FLAG_ISO14230     = 0x08,
	PROTOCOL_FLAG_CAN          = 0x10,
	PROTOCOL_FLAG_ISO15765     = 0x20,
	PROTOCOL_FLAG_SCI_A_ENGINE = 0x40,
	PROTOCOL_FLAG_SCI_A_TRANS  = 0x80,
	PROTOCOL_FLAG_SCI_B_ENGINE = 0x100,
	PROTOCOL_FLAG_SCI_B_TRANS  = 0x200,
};


class J2534Library
{
public:
	std::string path;
	std::string name;
	unsigned long protocols;
	J2534_API_version api;

	J2534Library() { protocols = 0; api = J2534_API_v0404; };
};


class J2534_API
{

public:
	J2534_API();
	~J2534_API();
	static std::vector<J2534Library> getAvailableJ2534Libs();
	bool selectLibrary(std::string libPath);
	std::string library();
	J2534_API_version libraryAPIversion();

	long PassThruOpen(void* pName, unsigned long *pDeviceID);	// 0404-API
	long PassThruClose(unsigned long DeviceID);			// 0404-API
	long PassThruConnect(unsigned long ProtocolID, unsigned long Flags, unsigned long *pChannelID);							// 0202-API
	long PassThruConnect(unsigned long DeviceID, unsigned long ProtocolID, unsigned long Flags, unsigned long BaudRate, unsigned long *pChannelID);	// 0404-API
	long PassThruDisconnect(unsigned long ChannelID);
	long PassThruReadVersion(char *pFirmwareVersion, char *pDllVersion, char *pApiVersion);								// 0202-API
	long PassThruReadVersion(unsigned long DeviceID, char *pFirmwareVersion, char * pDllVersion, char *pApiVersion);				// 0404-API
	long PassThruGetLastError(char *pErrorDescription);
	long PassThruReadMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
	long PassThruStartMsgFilter(unsigned long ChannelID, unsigned long FilterType, PASSTHRU_MSG *pMaskMsg, PASSTHRU_MSG *pPatternMsg, PASSTHRU_MSG *pFlowControlMsg, unsigned long *pMsgID);
	long PassThruStopMsgFilter(unsigned long ChannelID, unsigned long MsgID);
	long PassThruWriteMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
	long PassThruStartPeriodicMsg(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pMsgID, unsigned long TimeInterval);
	long PassThruStopPeriodicMsg(unsigned long ChannelID, unsigned long MsgID);
	long PassThruIoctl(unsigned long HandleID, unsigned long IoctlID, void *pInput, void *pOutput);
	long PassThruSetProgrammingVoltage(unsigned long PinNumber, unsigned long Voltage);								// 0202-API
	long PassThruSetProgrammingVoltage(unsigned long DeviceID, unsigned long PinNumber, unsigned long Voltage);					// 0404-API

private:
	void *_J2534LIB;
	std::string _lib_path;
	J2534_API_version _api_version;

	J2534_PassThruOpen _PassThruOpen;
	J2534_PassThruClose _PassThruClose;
	J2534_PassThruConnect_0202 _PassThruConnect_0202;
	J2534_PassThruConnect_0404 _PassThruConnect_0404;
	J2534_PassThruDisconnect _PassThruDisconnect;
	J2534_PassThruReadVersion_0202 _PassThruReadVersion_0202;
	J2534_PassThruReadVersion_0404 _PassThruReadVersion_0404;
	J2534_PassThruGetLastError _PassThruGetLastError;
	J2534_PassThruReadMsgs _PassThruReadMsgs;
	J2534_PassThruStartMsgFilter _PassThruStartMsgFilter;
	J2534_PassThruStopMsgFilter _PassThruStopMsgFilter;
	J2534_PassThruWriteMsgs _PassThruWriteMsgs;
	J2534_PassThruStartPeriodicMsg _PassThruStartPeriodicMsg;
	J2534_PassThruStopPeriodicMsg _PassThruStopPeriodicMsg;
	J2534_PassThruIoctl _PassThruIoctl;
	J2534_PassThruSetProgrammingVoltage_0202 _PassThruSetProgrammingVoltage_0202;
	J2534_PassThruSetProgrammingVoltage_0404 _PassThruSetProgrammingVoltage_0404;

	void assignJ2534fcns();
#ifdef __J2534_API_DEBUG__
	static void printLibraryInfo(std::vector<J2534Library> PTlibraries);
#endif

};


#endif
