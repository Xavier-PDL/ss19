#include "Aimbot.h"
#include <cmath>
#include "TMem.h"
CEntity* Aimbot::pTarget = nullptr;
CEntity* Aimbot::pTarget2 = nullptr;
AimbotTargetingMode Aimbot::aimbotTargetingMode = FOV;
bool Aimbot::bRayCast = false;
bool Aimbot::bPickCloserFOV = false;

FLOAT3D vSmallestAngles;
float fShortestDist = 0;
float fSmallestFOV = -1.0f;
FLOAT3D Aimbot::CalcAngle( CEntity * pEntity )
{
	FLOAT3D vAngles;
	auto pSam = (CPlayer*) GetSamPtr();
	auto vDst = pSam->GetPlacement().pl_PositionVector;
	
	// need to do it like this for some rather short statured entities
	auto boxCenter = ((CMovableEntity*) pEntity)->en_boxMovingEstimate.Center();
	auto vSrc = pEntity->GetPlacement().pl_PositionVector;
	vSrc( 2 ) -= ((boxCenter( 2 ) - vSrc( 2 )) / 4);
	auto vDiff = vDst - vSrc;
	vAngles( 1 ) = atan2( vDiff( 1 ) , vDiff( 3 ) ) * 180.0f / PI;
	vAngles( 2 ) = asin( vDiff( 2 ) / vDiff.Length() ) * 180 / PI * -1;
	vAngles( 3 ) = 0;
	return vAngles;
}

void Aimbot::UpdateTarget( CProjection3D* pProjection, CEntity * pEntity, bool bVisible )
{
	if ( !O::bAimbot )
		return;

	auto pSam = (CPlayerEntity*)GetSamPtr();
	auto samPlacement = pSam->GetPlacement();
	bool bValid = true;

	if ( O::bAimbotVischeck )
	{
		samPlacement.pl_PositionVector( 2 ) += 2.0f;
		CCastRay ray( (CEntity*) pSam, samPlacement );
		ray.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
		ray.cr_penIgnore = pSam;
		ray.cr_vTarget = pEntity->GetPlacement().pl_PositionVector;
		ray.cr_vTargetRelative = ray.cr_vOrigin - ray.cr_vTarget;
		ray.Cast( pSam->GetWorld() );
		if ( ray.cr_penHit != pEntity )
			bValid = false;
	}

	if ( bValid )
	{
		if ( O::bFOV )
		{	
			auto myYaw = samPlacement.pl_OrientationAngle( 1 );
			auto newYaw = CalcAngle( pEntity )(1);
			float yawDiff = 0;// newYaw - myYaw;
			yawDiff = abs( myYaw - newYaw );
			if ( fSmallestFOV < 0 || yawDiff < fSmallestFOV )
			{
				fSmallestFOV = yawDiff;
				pTarget = pEntity;
			}		
		}
		else
		{
			// Get Target by distance
			auto dist = (pEntity->GetPlacement().pl_PositionVector - GetSamPtr()->GetPlacement().pl_PositionVector).Length();
			if ( fShortestDist < 0.0f || dist < fShortestDist )
			{
				fShortestDist = dist;
				pTarget = pEntity;
			}
		}
	}
}

void Aimbot::DoBot( CProjection3D* pProjection )
{
	if ( !pTarget || !O::bAimbot)
		return;
	if ( GetAsyncKeyState( VK_RBUTTON ) )
	{
		auto pSam = (CPlayer*) GetSamPtr();
		auto pOrientation = &pSam->en_plViewpoint.pl_OrientationAngle;
		auto currAngles = pProjection->pr_ViewerPlacement.pl_OrientationAngle;
		auto newAngles = CalcAngle( pTarget );
		auto vDiff = newAngles - currAngles;
		//pOrientation->operator()( 2 ) += newAngles(1) - currAngles(1); // yaw
		pOrientation->operator()( 2 ) += vDiff( 1 ); // yaw
		pOrientation->operator()( 3 ) = newAngles( 2 ); // pitch
	}
	vSmallestAngles = { 0,0,0 };
	fShortestDist = -1.0f;
	fSmallestFOV = -1.0f;
	pTarget = nullptr;
}
