#pragma once

namespace Options
{
	extern bool bGod;
	extern bool bInfAmmo;
	extern bool bRapidfire;
	extern bool bFly;
	extern bool bSuperJump;
	extern bool bLowGravity;
	extern bool bTriggerbot;
	extern bool bNoCollisionDmg;
	extern bool bJumpWalk;
	extern bool bESP;
	extern bool bESPVisCheck;
	extern bool bBoxESP;
	extern bool bLineESP;
	extern bool bItemESP;
	extern bool bItemDescESP;
	extern bool bDistanceESP;
	extern bool bNameESP;
	extern unsigned int uColEnemyESP;
	extern unsigned int uColItemESP;
	
	extern bool bAimbot;
	extern bool bAimbotVischeck;
	extern bool bFOV;
}
#define O Options

namespace Game
{
	// inf ammo stuff
	extern void* pDecAmmo;
	constexpr unsigned int osDecAmmoOffset = 0xb76bd;
	constexpr unsigned int szDecAmmoSize = 2;
	
	extern void* pDecDblColtAmmo;
	constexpr unsigned int osDecDblColtOffset = 0xbfd54;
	constexpr int szDecDblColtSize = 6;
	
	extern void* pDecSingleColtAmmo;
	constexpr unsigned int osDecSingleColtOffset = 0xbf8f5;
	constexpr int szDecSingleColtSize = 6;
}

#define G Game