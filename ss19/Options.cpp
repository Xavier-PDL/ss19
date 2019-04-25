#include "Options.h"

namespace Options
{
	bool bGod = false;
	bool bInfAmmo = false;
	bool bRapidfire = false;
	bool bFly = false;
	bool bSuperJump = false;
	bool bLowGravity = false;
	bool bTriggerbot = true;
	bool bNoCollisionDmg = false;
	bool bJumpWalk = false;
	bool bESP = false;
	bool bESPVisCheck = true;
	bool bEnemyESP = true;
	bool bBoxESP = true;
	bool bLineESP = true;
	bool bItemESP = true;
	bool bItemDescESP = true;
	bool bDistanceESP = true;
	bool bNameESP = true;
	unsigned int uColEnemyESP = 0xFF000000;
	unsigned int uColItemESP = 0x00FF0000;

	bool bAimbot = false;
	bool bAimbotVischeck = false;
	bool bFOV = true;
	
}

namespace Game
{
	void* pDecAmmo = nullptr;
	void* pDecDblColtAmmo = nullptr;
	void* pDecSingleColtAmmo = nullptr;
}