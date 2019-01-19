#include <iostream>
#include <Windows.h>

#include "includes.h"
#include "mem.h"
#include "offsets.h"

DWORD processID;
DWORD clientDll;
DWORD engineDLL;

DWORD localPlayer;
int myTeamNum;
DWORD entity;
int entityTeamNum;

int main()
{
    //Splash
	std::cout << "CSGO radar" << std::endl;

    //Attach
	processID = getProcessIdFromWindowName("Counter-Strike: Global Offensive");

	handle = getHandleFromProcessID(processID);
	std::cout << std::endl << "Attached!" << std::endl;

	clientDll = getModule("client_panorama.dll", processID);
	//engineDLL = getModule("engine.dll", processID);

	//std::cout << "enigneDll: " << engineDLL << std::endl;
	//std::cout << "clientDll: " << clientDll << std::endl << std::endl;

    while(handle) {
        //radar only
        for (int i = 0; i < 64; i++) {
            localPlayer = readMem<DWORD>(clientDll, oLocalPlayer);
            myTeamNum = readMem<int>(localPlayer, oTeamNum);
            entity = readMem<DWORD>(clientDll, oEntityList + (i * 0x10));
            entityTeamNum = readMem<int>(entity, oTeamNum);

            if (myTeamNum != entityTeamNum && entityTeamNum != 0) {
                writeMem<bool>(entity, oSpotted, true);
            }
        }

        Sleep(33);
    }

    return 0;
}
