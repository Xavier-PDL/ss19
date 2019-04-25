#include "ESP.h"
#include <vector>
#include "Draw.h"


struct ESPEnt
{
	FLOAT3D scrTop;
	FLOAT3D scrBottom;
	CEntity* pEntity;
	bool bVisible;
};

std::vector<ESPEnt> espEnts;

void ESP::W2S( CProjection3D * pProjection, CEntity * pTarget, bool bVisible )
{
	if ( !O::bESP )
		return;

	FLOAT3D vScreen;
	FLOAT3D vScreenHead;

	FLOAT3D vTarget = pTarget->en_plPlacement.pl_PositionVector;
	auto maxHeight = pTarget->en_pciCollisionInfo->ci_fMaxHeight;
	FLOAT3D vHead = vTarget;
	vHead( 2 ) += maxHeight;


	if ( !pProjection->pr_Prepared )
	{
		pProjection->ObjectPlacementL() = CPlacement3D( FLOAT3D( 0.0f, 0.0f, 0.0f ), ANGLE3D( 0, 0, 0 ) );
		pProjection->Prepare();
	}

	pProjection->ProjectCoordinate( vTarget, vScreen );
	pProjection->ProjectCoordinate( vHead, vScreenHead );

	if ( vScreen( 3 ) < 0.0f )
		espEnts.push_back( ESPEnt{ vScreenHead, vScreen, pTarget, bVisible } );
}

void ESP::Draw()
{
	if ( !O::bESP )
		return;

	auto pDraw = Draw::Get();

	for ( auto ent : espEnts )
	{
		auto scr = ent.scrBottom;
		auto scrHead = ent.scrTop;
		auto pEnt = ent.pEntity;

		auto myPos = GetSamPtr()->en_plPlacement.pl_PositionVector;
		auto dist = (myPos - pEnt->en_plPlacement.pl_PositionVector).Length();
		auto width = 800.0f / dist;
		auto height = (ent.scrTop( 2 ) - ent.scrBottom( 2 ));

		auto pDllEntClass = pEnt->en_pecClass->ec_pdecDLLClass;
		bool bIsEnemy = (!strstr( pDllEntClass->dec_strName, "Item" ));

		if ( bIsEnemy )
		{
			auto pEnemy = (CEnemyBase*) pEnt;
			if ( !pEnemy->m_penWatcher.ep_pen )
				continue;
			auto tmPredict = pEnemy->m_tmPredict;
		}

		if ( O::bESPVisCheck )
		{
			if ( ent.bVisible )
				O::uColEnemyESP = C_YELLOW;
			else
				O::uColEnemyESP = C_RED;
		}

		if ( O::bBoxESP )
		{
			pDraw->Box( (int) (ent.scrBottom( 1 ) - (width / 2)),
				(int) (Draw::Get()->pBaseDP->GetHeight() - ent.scrTop( 2 )),
						(int) width,
						(int) height,
						bIsEnemy ? O::uColEnemyESP : O::uColItemESP, 0xFF );
		}

		if ( O::bLineESP )
		{
			if(bIsEnemy )
				pDraw->Line( int(Draw::pBaseDP->GetWidth() / 2),
							 int(Draw::pBaseDP->GetHeight()),
							 (int)ent.scrBottom(1),
							 (int) (Draw::Get()->pBaseDP->GetHeight() - ent.scrBottom( 2 )),
							 bIsEnemy ? O::uColEnemyESP : O::uColItemESP, 0xFF );
		}

		if ( O::bNameESP )
		{
			if ( bIsEnemy )
			{
				pDraw->Text( pEnt->en_pecClass->ec_pdecDLLClass->dec_strName,
							 (int) (scr( 1 ) + (width / 2)),
							 (int) (Draw::pBaseDP->GetHeight() - (int) scrHead( 2 )),
							 O::uColEnemyESP, 0xFF );
			}
		}
		if ( O::bItemESP )
		{
			if ( !bIsEnemy )
			{
				auto pItemEnt = (CItem*) pEnt;
				if ( pItemEnt->m_ulPickedMask )
				{
					if ( O::bNameESP )
					{
						pDraw->Text( pEnt->en_pecClass->ec_pdecDLLClass->dec_strName,
							(int) (scr( 1 ) + (width / 2)),
									 (int) (Draw::pBaseDP->GetHeight() - (int) (scrHead( 2 ))),
									 O::uColItemESP, 0xFF );
					}

					if ( O::bItemDescESP )
					{
						pDraw->Text( pItemEnt->m_strDescription.pString,
							(int) (scr( 1 ) + (width / 2)),
									 Draw::pBaseDP->GetHeight() - (int) (scrHead( 2 ) - 10),
									 O::uColItemESP, 0xFF );
					}
				}
			}
		}
		if ( O::bDistanceESP )
		{
			char sDist[20]{ 0 };
			sprintf_s( sDist, 20, "%.2f", dist );
			pDraw->Text( sDist, (int) scr( 1 ), (int) (Draw::pBaseDP->GetHeight() - scr( 2 )), bIsEnemy ? O::uColEnemyESP : O::uColItemESP, 0xFF );
		}
	}
	espEnts.clear();
}
