#include "Hack.h"
#include "Options.h"
#include <Engine/Base/Input.cpp> // don't fucking know why it's making me do this... literally same setup as _pLibrary but not working o.O

HANDLE hEngine = nullptr;
HANDLE hEntities = nullptr;

Menu* pMenu = nullptr;

TInjManager* pRapidfireInjs = nullptr;
TInjManager* pLowGravInj = nullptr;

void __fastcall hSwapBuffers( void* pViewport, void* EDX );
void hRenderView( CWorld &woWorld, CEntity &enViewer, CAnyProjection3D &prProjection, CDrawPort &dpDrawport );
void __fastcall hSetPlacement_Internal( void* pECX, void* pEDX, CPlacement3D& pNew, FLOATmatrix3D& mMatrix, BOOL bNear );
void* pOldWndProc = nullptr;
LRESULT CALLBACK hWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void DoFly();
void Triggerbot();

HANDLE Hack::hDLL = nullptr;
bool Hack::Install( HANDLE hDLL )
{
	auto pHooks = HookManager::Get();
	auto hkoSwapBuffers = new HookIAT( HKID_SWAPBUFFERS, hSwapBuffers, "?SwapBuffers@CViewPort@@QAEXXZ" );
	if ( !pHooks->Add( hkoSwapBuffers ) )
		return false;

	Hack::hDLL = hDLL;

	return true;
}

bool Hack::Init()
{
	auto pHooks = HookManager::Get();
	hEngine = GetModuleHandle( "engine.dll" );
	DWORD dwEngine = reca( DWORD, hEngine );
	_pInput = *reca( CInput**, dwEngine + 0x1C09AC );
	_pGfx = *reca( CGfxLibrary**, dwEngine + 0x1D5EC0 );
	_pLibrary = *reca( CNetworkLibrary**, dwEngine + 0x1DC0BC );

	auto renderView = GetProcAddress( (HMODULE) hEngine, "?RenderView@@YAXAAVCWorld@@AAVCEntity@@AAVCAnyProjection3D@@AAVCDrawPort@@@Z" );
	auto hkRenderView = new HookTH( HKID_RENDERVIEW, renderView, hRenderView, HKSZ_RENDERVIEW );
	if ( !pHooks->Add( hkRenderView ) )
		return false;

	auto setPlacementInternal = GetProcAddress( (HMODULE) hEngine, "?SetPlacement_internal@CEntity@@UAEXABVCPlacement3D@@ABV?$Matrix@M$02$02@@H@Z" );
	auto hkSetPlacementInternal = new HookTH( HKID_SETPLACEMENT, setPlacementInternal, hSetPlacement_Internal, HKSZ_SETPLACEMENT );
	if ( !pHooks->Add( hkSetPlacementInternal ) )
		return false;

	auto hwnd = FindWindow( "SeriousSam", nullptr );
	pOldWndProc = (void*) GetWindowLongPtr( hwnd, GWLP_WNDPROC );
	SetWindowLongPtr( hwnd, GWLP_WNDPROC, (LONG) hWndProc );

	auto pMem = TMem::Get();
	hEntities = GetModuleHandle( "entities.dll" );
	uintptr_t entDll = (uintptr_t) hEntities;
	G::pDecAmmo = reca( void*, entDll + G::osDecAmmoOffset );
	G::pDecDblColtAmmo = reca( void*, entDll + G::osDecDblColtOffset );
	G::pDecSingleColtAmmo = reca( void*, entDll + G::osDecSingleColtOffset );

	pMenu = new Menu();

	return true;
}

void Hack::HandleInput()
{
	if ( Input::GetKeyState( VK_TAB ).isUp() )
	{
		pMenu->Toggle();
	}

	pMenu->HandleInput();

	Input::ClearStates();
}

FLOAT3D vCurrentAngles;
CPerspectiveProjection3D currProjection;
void Hack::Run()
{
	HandleInput();
	
	auto pSam = static_cast<CPlayer*>(GetSamPtr());
	if ( pSam )
	{
		if ( O::bGod )
		{
			pSam->en_fHealth = 999;
		}
		if ( O::bFly )
		{
			DoFly();
		}
		if ( O::bTriggerbot )
		{
			Triggerbot();
		}
		if ( O::bSuperJump )
		{
			if ( GetAsyncKeyState( VK_SPACE ) & 1 )
			{
				pSam->en_vCurrentTranslationAbsolute( 2 ) = 25.0f;
			}
		}
		if ( O::bLowGravity )
		{
			// To make a proper lo grav we should do a small code inj at:
			// 601F8087 - 89 4A 04 - mov [edx+04],ecx
			// and do the writing in there.
			////if ( pSam->en_vCurrentTranslationAbsolute( 2 ) < -5.0f )
			////	pSam->en_vCurrentTranslationAbsolute( 2 ) = 5.0f;
		}
		if ( O::bAimbot || O::bESP )
		{
			auto pWorld = pSam->GetWorld();
			auto entList = pWorld->wo_cenEntities;
			auto size = entList.sa_UsedCount;
			auto samPlacement = pSam->GetPlacement();

			for ( int x = 0; x < size; x++ )
			{
				auto pEntity = entList.sa_Array[x];
				if ( pEntity->en_RenderType == CEntity::RT_NONE || !pEntity->en_pecClass || !pEntity->en_RenderType || pEntity->en_RenderType > 11 )
					continue;

				if ( O::bItemESP )
				{
					if ( IsDerivedFromClass( pEntity, "Item" ) )
					{

						bool bRes = pEntity->en_ulFlags & ENF_VALIDSHADINGINFO;
						if ( bRes )
						{
							ESP::W2S( &currProjection, pEntity );
						}
					}
				}

				if ( IsDerivedFromClass( pEntity, "Enemy Base" ) )
				{
					auto pClass = pEntity->en_pecClass;
					auto pdecClass = pClass->ec_pdecDLLClass;

					bool bRes = pEntity->en_ulFlags & ENF_VALIDSHADINGINFO;
					if ( bRes && reca( CLiveEntity*, pEntity )->IsAlive() )
					{
						// process current entity
						bool bVisible = false;
						if ( O::bAimbotVischeck || O::bESPVisCheck )
						{
							samPlacement.pl_PositionVector( 2 ) += 1.0f;
							CCastRay ray( (CEntity*) pSam, samPlacement );
							ray.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
							ray.cr_penIgnore = pSam;
							ray.cr_vTarget = pEntity->GetPlacement().pl_PositionVector;
							ray.cr_vTargetRelative = ray.cr_vOrigin - ray.cr_vTarget;
							ray.Cast( pSam->GetWorld() );
							if ( ray.cr_penHit == pEntity )
								bVisible = true;
						}
						ESP::W2S( &currProjection, pEntity, bVisible );
						Aimbot::UpdateTarget( &currProjection, pEntity, bVisible );
					}

				}
			}

			ESP::Draw();
			Aimbot::DoBot( &currProjection );
		}
	}

	// draw menu
	pMenu->Draw();

	if ( GetAsyncKeyState( VK_END ) )
		Unload();
}

void Hack::Unload()
{
	auto hwnd = FindWindow( "SeriousSam", nullptr );
	SetWindowLongPtr( hwnd, GWLP_WNDPROC, (LONG) pOldWndProc );
	delete pMenu;
	delete pRapidfireInjs;
	delete pLowGravInj;
	TSyscall::Destroy();
	TMem::Get()->Destroy();
	HookManager::Get()->Destroy();
	CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) FreeLibrary, hDLL, 0, 0 );
}

// all the drawing goes here... 
// doesn't work in the RenderView hook for some reason i don't care to find out

void __fastcall hSwapBuffers( void* pViewport, void* EDX )
{
	static bool bOnce = true;
	if ( bOnce )
	{
		bOnce = false;
		Hack::Init();
	}

	auto pDraw = Draw::Get();
	if ( pDraw->PreDraw() )
	{
		Hack::Run();
		pDraw->PostDraw();
	}

	using fnSwapBuffers = void( __thiscall* )(void*);
	static fnSwapBuffers fpSwapBuffers = nullptr;
	if ( !fpSwapBuffers )
		fpSwapBuffers = (fnSwapBuffers) HookManager::Get()->GetHook<HookIAT>( HKID_SWAPBUFFERS )->GetTarget();
	fpSwapBuffers( pViewport );
}

void hRenderView( CWorld &woWorld, CEntity &enViewer, CAnyProjection3D &prProjection, CDrawPort &dpDrawport )
{
	static CProjection3D* pProjection = nullptr;

	if ( prProjection->IsPerspective() )
	{
		currProjection = prProjection.ap_Perspective;
		vCurrentAngles = prProjection->pr_ViewerPlacement.pl_OrientationAngle;
	}

	using fnRenderView = void( __cdecl* )(CWorld &woWorld, CEntity &enViewer, CAnyProjection3D &prProjection, CDrawPort &dpDrawport);
	static fnRenderView fpRenderView = nullptr;
	if ( !fpRenderView )
		fpRenderView = (fnRenderView) HookManager::Get()->GetHook<HookTH>( HKID_RENDERVIEW )->GetTrampLoc();
	fpRenderView( woWorld, enViewer, prProjection, dpDrawport );
}

void __fastcall hSetPlacement_Internal( void* pECX, void* pEDX, CPlacement3D& pNew, FLOATmatrix3D& mMatrix, BOOL bNear )
{
	auto pSam = GetSamPtr();
	if ( pECX == pSam )
	{
		if ( O::bFly )
		{
			pNew.pl_PositionVector = GetSamPtr()->GetPlacement().pl_PositionVector;
		}
	}
	
	using fnSetPlacement = void( __thiscall* )(void* pThis, CPlacement3D& pNew, FLOATmatrix3D& mMatrix, BOOL bNear);
	static fnSetPlacement fpSetPlacement = nullptr;
	if ( !fpSetPlacement )
		fpSetPlacement = (fnSetPlacement) HookManager::Get()->GetHook<HookTH>( HKID_SETPLACEMENT )->GetTrampLoc();
	fpSetPlacement( pECX, pNew, mMatrix, bNear );
}

LRESULT CALLBACK hWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case WM_KEYDOWN:
		{
			Input::SetKeyState( (int) wParam, (int) lParam );
			break;
		}
		case WM_KEYUP:
		{
			Input::SetKeyState( (int) wParam, (int) lParam );
			break;
		}
	}
	return reca( WNDPROC, pOldWndProc )(hWnd, uMsg, wParam, lParam);
}

void DoFly()
{
	auto pSam = (CPlayer*) GetSamPtr();

	auto yaw = vCurrentAngles( 1 );
	auto pitch = vCurrentAngles( 2 );

	auto sinYaw = Sin( yaw );
	auto cosYaw = Cos( yaw );
	auto sinPitch = Sin( pitch );

	auto newX = 0.15f * sinYaw;
	auto newZ = 0.15f * cosYaw;
	auto newY = 0.15f * sinPitch;

	if ( GetAsyncKeyState( 'W' ) )
	{
		pSam->en_plPlacement.pl_PositionVector( 1 ) -= newX;
		pSam->en_plPlacement.pl_PositionVector( 3 ) -= newZ;
		pSam->en_plPlacement.pl_PositionVector( 2 ) += newY;
	}
	if ( GetAsyncKeyState( 'S' ) )
	{
		pSam->en_plPlacement.pl_PositionVector( 1 ) += newX;
		pSam->en_plPlacement.pl_PositionVector( 3 ) += newZ;
		pSam->en_plPlacement.pl_PositionVector( 2 ) -= newY;
	}
	if ( GetAsyncKeyState( 'A' ) )
	{
		pSam->en_plPlacement.pl_PositionVector( 1 ) -= newZ;
		pSam->en_plPlacement.pl_PositionVector( 3 ) += newX;
	}
	if ( GetAsyncKeyState( 'D' ) )
	{
		pSam->en_plPlacement.pl_PositionVector( 1 ) += newZ;
		pSam->en_plPlacement.pl_PositionVector( 3 ) -= newX;
	}
	if ( GetAsyncKeyState( VK_SPACE ) )
		pSam->en_plPlacement.pl_PositionVector( 2 ) += 0.1f;
	if ( GetAsyncKeyState( 'C' ) )
		pSam->en_plPlacement.pl_PositionVector( 2 ) -= 0.1f;
}

void Triggerbot()
{
	auto pPlayer = static_cast<CPlayer*>( GetSamPtr() );
	auto pWeapons = static_cast<CPlayerWeapons*>(pPlayer->m_penWeapons.ep_pen);
	static bool bNeedRelease = false;
	INPUT inDown{ 0 };
	inDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	inDown.mi.time = 0;
	INPUT inUp{ 0 };
	inUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	inUp.mi.time = 10;
	INPUT pInputs[] = { inDown, inUp };
	if ( IsDerivedFromClass( pWeapons->m_penRayHit.ep_pen, "Enemy Base" ) )
	{		
		SendInput( 1, &inDown, sizeof( inDown ) );
		bNeedRelease = true;
	}	
	else
	{
		if ( bNeedRelease )
		{
			SendInput( 1, &inUp, sizeof( inUp ) );
			bNeedRelease = false;
		}
	}
}
	
void FillHealthAndArmor()
{
	auto pPlayer = (CPlayer*) GetSamPtr();
	if ( !pPlayer )
		return;
	pPlayer->en_fHealth = pPlayer->m_fMaxHealth;
	pPlayer->m_fArmor = pPlayer->m_fMaxHealth;
}

void GiveAllWeapons()
{
	auto pPlayer = reca(CPlayer*, GetSamPtr());
	if ( !pPlayer )
		return;

	EWeaponItem epu;
	epu.iAmmo = -1;
	epu.bDropped = FALSE;
	for ( int i = 1; i < WIT_GHOSTBUSTER; i++ )
	{
		epu.iWeapon = i;
		pPlayer->ReceiveItem( epu );
	}
}

void FillAmmo()
{
	auto pPlayer = static_cast<CPlayer*>( GetSamPtr() );
	if ( !pPlayer )
		return;

	auto pWeapons = (CPlayerWeapons*) pPlayer->m_penWeapons.ep_pen;
	pWeapons->m_iBullets = pWeapons->m_iMaxBullets;
	pWeapons->m_iShells = pWeapons->m_iMaxShells;
	pWeapons->m_iRockets = pWeapons->m_iMaxRockets;
	pWeapons->m_iGrenades = pWeapons->m_iMaxGrenades;
	pWeapons->m_iNapalm = pWeapons->m_iMaxNapalm;
	pWeapons->m_iElectricity = pWeapons->m_iMaxElectricity;
	pWeapons->m_iIronBalls = pWeapons->m_iMaxIronBalls;
	pWeapons->m_iSniperBullets = pWeapons->m_iMaxSniperBullets;
}

void ToggleInfAmmo()
{
	auto pMem = TMem::Get();
	static std::vector<BYTE> vDecAmmo;
	static std::vector<BYTE> vDecDColtAmmo;
	static std::vector<BYTE> vDecSColtAmmo;
	if ( O::bInfAmmo )
	{
		if ( !vDecAmmo.size() )
		{
			vDecAmmo.resize( G::szDecAmmoSize );
			vDecDColtAmmo.resize( G::szDecDblColtSize );
			vDecSColtAmmo.resize( G::szDecSingleColtSize );

			pMem->Read( G::pDecAmmo, vDecAmmo.data(), G::szDecAmmoSize );
			pMem->Read( G::pDecDblColtAmmo, vDecSColtAmmo.data(), G::szDecDblColtSize );
			pMem->Read( G::pDecSingleColtAmmo, vDecSColtAmmo.data(), G::szDecSingleColtSize );
		}
		pMem->Nop( G::pDecAmmo, G::szDecAmmoSize );
		pMem->Nop( G::pDecDblColtAmmo, G::szDecDblColtSize );
		pMem->Nop( G::pDecSingleColtAmmo, G::szDecSingleColtSize );
	}
	else
	{
		pMem->Write( G::pDecAmmo, vDecAmmo.data(), G::szDecAmmoSize );
		pMem->Write( G::pDecDblColtAmmo, vDecDColtAmmo.data(), G::szDecDblColtSize );
		pMem->Write( G::pDecSingleColtAmmo, vDecSColtAmmo.data(), G::szDecSingleColtSize );
	}
}

void ToggleLowGravity()
{
	//TCodeBuffer ogCode = TPattern( "89 4A 04 D9 5D F4" ).Get();
	TCodeBuffer code = TPattern( "C7 42 04 0F 00 00 C0 D9 42 04 89 4A 04 D9 42 04 DF F1 D9 5A 04 0F 82 05 00 00 00 E9 05 00 00 00 B9 0F 00 00 C0 89 4A 04 D9 5D F4" ).Get();
	code.AddJumpBack( (uintptr_t) hEngine + 0x13808D );
	if ( !pLowGravInj )
	{
		pLowGravInj = new TInjManager();
		pLowGravInj->AddInjection( (void*) ((uintptr_t) hEngine + 0x138087), 6, code );
	}
	pLowGravInj->ToggleAll();
}

void ToggleJumpWalk()
{
	auto pSam = static_cast<CPlayer*>(GetSamPtr());
	if ( O::bJumpWalk )
	{
		pSam->en_fJumpControlMultiplier = 20.0f;
		pSam->en_tmMaxJumpControl = 20.0f;
	}
	else
	{
		pSam->en_fJumpControlMultiplier = 0.5f;
		pSam->en_tmMaxJumpControl = 0.5f;
	}
}

void ToggleRapidFire()
{
	auto pMem = TMem::Get();
	DWORD dwEntities = (DWORD) hEntities;

	if ( !pRapidfireInjs )
	{
		pRapidfireInjs = new TInjManager();
		TCodeBuffer scb( TPattern( "51 8B CE C7 04 24 00 00 00 00 90 90" ).Get() );
		TCodeBuffer cbCB( TPattern( "6A 14 90" ).Get() );
		TCodeBuffer cbCB1( TPattern( "BA 00 00 00 00 90 90 90" ).Get() );
		TCodeBuffer cbCB2( TPattern( "8B CE 6A 14 90 90" ).Get() );
		TCodeBuffer cbTG( { 0x6A, 0x00 } );
		TCodeBuffer cbNL( { 0xB9, 0x00, 0x00, 0x00, 0x00 } );
		TCodeBuffer cbNL1( { 0xB8, 0x0F, 0x00, 0x00, 0x00 } );
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xBF971 ), scb.Size(), scb ); // single colt
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xBFDE5 ), scb.Size(), scb );	// dbl colt 1
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xBFFB9 ), scb.Size(), scb );	// dbl colt 2
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC078A ), 0x16, scb );	// single sg 1
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC0A42 ), 0x10, scb );	// single sg 2
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC128F ), 0x20, scb );	// dbl sg 1
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC1C0D ), 5, cbTG );	// tommygun
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC322D ), scb.Size(), scb ); // rocket 
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC389B ), 7, cbNL );	// nade lawncher		
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC3756 ), 0x20, cbNL1 );	// nade lawncher
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC4277 ), cbCB1.Size(), cbCB1 );	// cannonball
		pRapidfireInjs->AddInjection( reca( void*, dwEntities + 0xC451C ), cbCB2.Size(), cbCB2 );	// cannonball
	}

	pRapidfireInjs->ToggleAll();

	if ( O::bRapidfire )
	{
		// knife
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF268 ), 0x10 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF3C1 ), 0x10 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF4CD ), 0x10 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF4F0 ), 0x35 );

		pMem->Write( reca( void*, dwEntities + 0xC113F ), 0 ); // dbl barrel

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC324C ), 0x74 ); // rocket lawncher
		pMem->Write( reca( void*, dwEntities + 0xC30FA ), 0x3A000000 ); // rocket lawncher

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC358C ), 0x7A ); // nade lawncher
		pMem->Write( reca( void*, dwEntities + 0xC3ACF ), 0x3A000000 ); // nade lawncher

		pMem->Write( reca( void*, dwEntities + 0xC3DD3 ), 0 ); // laser

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC429C ), 0x8F ); // cannonball
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC459E ), 0x95 ); // cannonball
		//pMem->Write<BYTE>( reca( void*, dwEntities + 0xC48FB ), 0x8B ); // cannonball
	}
	else
	{
		// knife
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF268 ), 0x18 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF3C1 ), 0x18 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF4CD ), 0x18 );
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xBF4F0 ), 0x2E );

		pMem->Write( reca( void*, dwEntities + 0xC113F ), 0x3E800000 ); // dbl barrel

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC324C ), 0x72 ); // rocket lawncher
		pMem->Write( reca( void*, dwEntities + 0xC30FA ), 0x3D4CCCCD ); // rocket lawncher

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC358C ), 0x77 ); // nade lawncher		
		pMem->Write( reca( void*, dwEntities + 0xC3ACF ), 0x3E800000 ); // nade lawncher

		pMem->Write( reca( void*, dwEntities + 0xC3DD3 ), 0x3DCCCCCD ); // laser

		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC459E ), 0x92 ); // cannonball
		pMem->Write<BYTE>( reca( void*, dwEntities + 0xC429C ), 0x8C ); // cannonball
		//pMem->Write<BYTE>( reca( void*, dwEntities + 0xC48FB ), 0xA4 ); // cannonball
	}
}

void ToggleCollisionDmg()
{
	auto pSam = (CPlayer*)GetSamPtr();

	if ( O::bNoCollisionDmg )
	{
		pSam->en_fCollisionDamageFactor = 0;
	}
	else
	{
		pSam->en_fCollisionDamageFactor = 0.5f;
	}
}
