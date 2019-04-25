#pragma once
#include <sdk.h>
#include "Options.h"
enum AimbotTargetingMode
{
	DISTANCE,
	FOV
};

class Aimbot
{
	static CEntity* pTarget;
	static CEntity* pTarget2;
public:
	static AimbotTargetingMode aimbotTargetingMode;
	static bool bRayCast;
	static bool bPickCloserFOV;

public:
	static FLOAT3D CalcAngle( CEntity* pEntity );
	static void UpdateTarget( CProjection3D* pProjection, CEntity* pEntity, bool bVisible=false );
	static void DoBot(CProjection3D* pProjection);
};

