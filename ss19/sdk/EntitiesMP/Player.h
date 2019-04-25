#ifndef _EntitiesMP_Player_INCLUDED
#define _EntitiesMP_Player_INCLUDED 1

#include <Engine/Base/Types.h>
#include <Engine/Base/Anim.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundListener.h>
#include <Engine/Light/LightSource.h>
#include "Common/Common.h"
#include "Common/GameInterface.h"
#include <Engine/Models/ModelObject.h>
#include <Engine/Classes/PlayerEntity.h>
#include <Engine/Templates/DynamicStackArray.h>
#include "PowerUpItem.h"
//extern DECL_DLL CEntityPropertyEnumType PlayerViewType_enum;

enum PlayerViewType {
	PVT_PLAYEREYES = 0,
	PVT_PLAYERAUTOVIEW = 1,
	PVT_SCENECAMERA = 2,
	PVT_3RDPERSONVIEW = 3,
};

//DECL_DLL inline void ClearToDefault( PlayerViewType &e ) { e = (PlayerViewType) 0; };
//extern DECL_DLL CEntityPropertyEnumType PlayerState_enum;

enum PlayerState {
	PST_STAND = 0,
	PST_CROUCH = 1,
	PST_SWIM = 2,
	PST_DIVE = 3,
	PST_FALL = 4,
};

//DECL_DLL inline void ClearToDefault( PlayerState &e ) { e = (PlayerState) 0; };
//#define EVENTCODE_ECameraStart 0x01910000
//class DECL_DLL ECameraStart : public CEntityEvent {
//public:
//	ECameraStart();
//	CEntityEvent *MakeCopy( void );
//	CEntityPointer penCamera;
//};
//DECL_DLL inline void ClearToDefault( ECameraStart &e ) { e = ECameraStart(); };
//#define EVENTCODE_ECameraStop 0x01910001
//class DECL_DLL ECameraStop : public CEntityEvent {
//public:
//	ECameraStop();
//	CEntityEvent *MakeCopy( void );
//	CEntityPointer penCamera;
//};
//DECL_DLL inline void ClearToDefault( ECameraStop &e ) { e = ECameraStop(); };
//#define EVENTCODE_ERebirth 0x01910002
//class DECL_DLL ERebirth : public CEntityEvent {
//public:
//	ERebirth();
//	CEntityEvent *MakeCopy( void );
//};
//DECL_DLL inline void ClearToDefault( ERebirth &e ) { e = ERebirth(); };
//#define EVENTCODE_EDisconnected 0x01910003
//class DECL_DLL EDisconnected : public CEntityEvent {
//public:
//	EDisconnected();
//	CEntityEvent *MakeCopy( void );
//};
//DECL_DLL inline void ClearToDefault( EDisconnected &e ) { e = EDisconnected(); };
//#define EVENTCODE_EAutoAction 0x01910004
//class DECL_DLL EAutoAction : public CEntityEvent {
//public:
//	EAutoAction();
//	CEntityEvent *MakeCopy( void );
//	CEntityPointer penFirstMarker;
//};
//DECL_DLL inline void ClearToDefault( EAutoAction &e ) { e = EAutoAction(); };
//extern "C" DECL_DLL CDLLEntityClass CPlayer_DLLClass;

class CPlayer : public CPlayerEntity {
public:
	int pad;
	//virtual CEntity *GetPredictionPair( void ) { return m_penPrediction; };
	//virtual void SetPredictionPair( CEntity *penPair ) { m_penPrediction = penPair; };
	//virtual void SetDefaultProperties( void );
	CTString m_strName;
	COLOR m_ulLastButtons;
	FLOAT m_fArmor;
	CTString m_strGroup;
	INDEX m_ulKeys;
	FLOAT m_fMaxHealth;
	INDEX m_ulFlags;
	CEntityPointer m_penWeapons;
	CEntityPointer m_penAnimator;
	CEntityPointer m_penView;
	CEntityPointer m_pen3rdPersonView;
	INDEX m_iViewState;
	INDEX m_iLastViewState;
	CAnimObject m_aoLightAnimation;
	FLOAT m_fDamageAmmount;
	FLOAT m_tmWoundedTime;
	FLOAT m_tmScreamTime;
	INDEX m_iGender;
	enum PlayerState m_pstState;
	FLOAT m_fFallTime;
	FLOAT m_fSwimTime;
	FLOAT m_tmOutOfWater;
	FLOAT m_tmMoveSound;
	BOOL m_bMoveSoundLeft;
	FLOAT m_tmNextAmbientOnce;
	FLOAT m_tmMouthSoundLast;
	CEntityPointer m_penCamera;
	CTString m_strCenterMessage;
	FLOAT m_tmCenterMessageEnd;
	BOOL m_bPendingMessage;
	FLOAT m_tmMessagePlay;
	FLOAT m_tmAnalyseEnd;
	BOOL m_bComputerInvoked;
	FLOAT m_tmAnimateInbox;
	CEntityPointer m_penMainMusicHolder;
	FLOAT m_tmLastDamage;
	FLOAT m_fMaxDamageAmmount;
	FLOAT3D m_vDamage;
	FLOAT m_tmSpraySpawned;
	FLOAT m_fSprayDamage;
	CEntityPointer m_penSpray;
	CSoundObject m_soWeapon0;
	CSoundObject m_soWeapon1;
	CSoundObject m_soWeapon2;
	CSoundObject m_soWeapon3;
	CSoundObject m_soWeaponAmbient;
	CSoundObject m_soPowerUpBeep;
	CSoundObject m_soMouth;
	CSoundObject m_soFootL;
	CSoundObject m_soFootR;
	CSoundObject m_soBody;
	CSoundObject m_soLocalAmbientLoop;
	CSoundObject m_soLocalAmbientOnce;
	CSoundObject m_soMessage;
	CSoundObject m_soHighScore;
	CSoundObject m_soSpeech;
	CSoundObject m_soSniperZoom;
	INDEX m_iMana;
	FLOAT m_fManaFraction;
	INDEX m_iHighScore;
	INDEX m_iBeatenHighScore;
	FLOAT m_tmLatency;
	FLOAT m_tmLatencyLastAvg;
	FLOAT m_tmLatencyAvgSum;
	INDEX m_ctLatencyAvg;
	BOOL m_bEndOfLevel;
	BOOL m_bEndOfGame;
	INDEX m_iMayRespawn;
	FLOAT m_tmSpawned;
	FLOAT3D m_vDied;
	FLOAT3D m_aDied;
	FLOAT m_tmEstTime;
	INDEX m_iTimeScore;
	INDEX m_iStartTime;
	INDEX m_iEndTime;
	FLOAT m_tmLevelStarted;
	CTString m_strLevelStats;
	CEntityPointer m_penActionMarker;
	FLOAT m_fAutoSpeed;
	INDEX m_iAutoOrgWeapon;
	FLOAT3D m_vAutoSpeed;
	FLOAT m_tmSpiritStart;
	FLOAT m_tmFadeStart;
	FLOAT m_tmLastPicked;
	CTString m_strPickedName;
	FLOAT m_fPickedAmmount;
	FLOAT m_fPickedMana;
	INDEX m_iLastHealth;
	INDEX m_iLastArmor;
	INDEX m_iLastAmmo;
	FLOAT m_tmHealthChanged;
	FLOAT m_tmArmorChanged;
	FLOAT m_tmAmmoChanged;
	FLOAT m_tmMinigunAutoFireStart;
	FLOAT3D m_vLastStain;
	ANGLE3D m_aLastRotation;
	ANGLE3D m_aLastViewRotation;
	FLOAT3D m_vLastTranslation;
	ANGLE3D m_aLocalRotation;
	ANGLE3D m_aLocalViewRotation;
	FLOAT3D m_vLocalTranslation;
	FLOAT m_tmInvisibility;
	FLOAT m_tmInvulnerability;
	FLOAT m_tmSeriousDamage;
	FLOAT m_tmSeriousSpeed;
	FLOAT m_tmInvisibilityMax;
	FLOAT m_tmInvulnerabilityMax;
	FLOAT m_tmSeriousDamageMax;
	FLOAT m_tmSeriousSpeedMax;
	FLOAT m_tmChainShakeEnd;
	FLOAT m_fChainShakeStrength;
	FLOAT m_fChainShakeFreqMod;
	FLOAT m_fChainsawShakeDX;
	FLOAT m_fChainsawShakeDY;
	INDEX m_iSeriousBombCount;
	INDEX m_iLastSeriousBombCount;
	FLOAT m_tmSeriousBombFired;
	CEntityPointer m_penPrediction;
	ShellLaunchData ShellLaunchData_array;
	INDEX m_iFirstEmptySLD;
	BulletSprayLaunchData BulletSprayLaunchData_array;
	INDEX m_iFirstEmptyBSLD;
	GoreSprayLaunchData GoreSprayLaunchData_array;
	INDEX m_iFirstEmptyGSLD;
	ULONG ulButtonsNow;
	ULONG ulButtonsBefore;
	ULONG ulNewButtons;
	ULONG ulReleasedButtons;
	BOOL bUseButtonHeld;
	CSoundListener sliSound;
	CLightSource m_lsLightSource;
	TIME m_tmPredict;
	CDynamicStackArray < CCompMessageID > m_acmiMessages;
	INDEX m_ctUnreadMessages;
	PlayerStats m_psLevelStats;
	PlayerStats m_psLevelTotal;
	PlayerStats m_psGameStats;
	PlayerStats m_psGameTotal;
	CModelObject m_moRender;

//#line 1297 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	INDEX GenderSound( INDEX iSound );
//
//#line 1302 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AddBouble( FLOAT3D vPos, FLOAT3D vSpeedRelative );
//
//#line 1316 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ClearShellLaunchData( void );
//
//#line 1326 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AddBulletSpray( FLOAT3D vPos, EffectParticlesType eptType, FLOAT3D vStretch );
//
//#line 1339 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ClearBulletSprayLaunchData( void );
//
//#line 1348 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AddGoreSpray( FLOAT3D vPos, FLOAT3D v3rdPos, SprayParticlesType sptType, FLOAT3D vSpilDirection,
//#line 1349 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//					   FLOATaabbox3D boxHitted, FLOAT fDamagePower, COLOR colParticles );
//
//#line 1366 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ClearGoreSprayLaunchData( void );
//	CPlayer( void );
//
//#line 1390 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	class CPlayerWeapons * GetPlayerWeapons( void );
//
//#line 1395 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	class CPlayerAnimator * GetPlayerAnimator( void );
//
//#line 1401 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CPlayerSettings * GetSettings( void );
//	void Copy( CEntity & enOther, ULONG ulFlags );
//
//#line 1437 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void UpdateLatency( FLOAT tmLatencyNow );
//
//#line 1464 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ValidateCharacter( void );
//
//#line 1474 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ParseGender( CTString & strName );
//
//#line 1485 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CheckHighScore( void );
//
//#line 1508 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CTString GetPredictName( void )const;
//
//#line 1521 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void Write_t( CTStream * ostr );
//
//#line 1537 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void Read_t( CTStream * istr );
//
//#line 1576 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CLightSource * GetLightSource( void );
//
//#line 1586 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetPredictionTime( TIME tmAdvance );
//
//#line 1592 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	TIME GetPredictionTime( void );
//
//#line 1598 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	FLOAT GetPredictionRange( void );
//
//#line 1604 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AddDependentsToPrediction( void );
//
//#line 1613 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	TIME GetStatsInGameTimeLevel( void );
//
//#line 1621 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	TIME GetStatsInGameTimeGame( void );
//
//#line 1630 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	FLOAT GetStatsRealWorldTime( void );
//
//#line 1641 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CTString GetStatsRealWorldStarted( void );
//	void GetStats( CTString & strStats, const CompStatType csType, INDEX ctCharsPerRow );
//
//#line 1681 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetShortStats( CTString & strStats );
//
//#line 1689 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetDetailStatsDM( CTString & strStats );
//
//#line 1764 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetDetailStatsCoop( CTString & strStats );
//
//#line 1842 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetDetailStatsSP( CTString & strStats, INDEX iCoopType );
//
//#line 1930 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetGameAgentPlayerInfo( INDEX iPlayer, CTString & strOut );
//
//#line 1961 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetMSLegacyPlayerInf( INDEX iPlayer, CTString & strOut );
//
//#line 1978 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL HasMessage( const CTFileName & fnmMessage );
//
//#line 1992 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ReceiveComputerMessage( const CTFileName & fnmMessage, ULONG ulFlags );
//
//#line 2014 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SayVoiceMessage( const CTFileName & fnmMessage );
//
//#line 2024 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CheatAllMessagesDir( const CTString & strDir, ULONG ulFlags );
//
//#line 2039 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CheatAllMessages( void );
//
//#line 2054 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ItemPicked( const CTString & strName, FLOAT fAmmount );
//
//#line 2076 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetupLightSource( void );
//
//#line 2093 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void PlayLightAnim( INDEX iAnim, ULONG ulFlags );
//
//#line 2100 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL AdjustShadingParameters( FLOAT3D & vLightDirection, COLOR & colLight, COLOR & colAmbient );
//
//#line 2128 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CModelObject * GetModelForRendering( void );
//
//#line 2198 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	class CPlayerActionMarker * GetActionMarker( void );
//
//#line 2203 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void FindMusicHolder( void );
//
//#line 2211 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void UpdateLevelStats( void );
//
//#line 2234 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL IsFuss( void );
//
//#line 2245 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetDefaultMouthPitch( void );
//
//#line 2249 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetRandomMouthPitch( FLOAT fMin, FLOAT fMax );
//
//#line 2253 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetSpeakMouthPitch( void );
//
//#line 2259 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ApplyShaking( CPlacement3D & plViewer );
//
//#line 2312 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	COLOR GetWorldGlaring( void );
//
//#line 2325 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderScroll( CDrawPort * pdp );
//
//#line 2335 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderCredits( CDrawPort * pdp );
//
//#line 2345 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderTextFX( CDrawPort * pdp );
//
//#line 2355 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderHudPicFX( CDrawPort * pdp );
//
//#line 2370 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetupView( CDrawPort * pdp, CAnyProjection3D & apr, CEntity * & penViewer,
//#line 2371 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//					CPlacement3D & plViewer, COLOR & colBlend, BOOL bCamera );
//
//#line 2444 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ListenFromEntity( CEntity * penListener, const CPlacement3D & plSound );
//
//#line 2475 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderDummyView( CDrawPort * pdp );
//
//#line 2496 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderPlayerView( CDrawPort * pdp, BOOL bShowExtras );
//
//#line 2581 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderCameraView( CDrawPort * pdp, BOOL bListen );
//
//#line 2662 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderGameView( CDrawPort * pdp, void * pvUserData );
//
//#line 2738 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void PreMoving( void );
//
//#line 2750 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void DoMoving( void );
//
//#line 2764 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void PostMoving( void );
//
//#line 2815 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetUnconnected( void );
//
//#line 2835 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetConnected( void );
//
//#line 2847 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL IsConnected( void )const;
//
//#line 2853 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ChecksumForSync( ULONG & ulCRC, INDEX iExtensiveSyncCheck );
//
//#line 2866 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void DumpSync_t( CTStream & strm, INDEX iExtensiveSyncCheck );
//
//#line 2881 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	virtual void LeaveStain( BOOL bGrow );
//
//#line 2917 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void DamageImpact( enum DamageType dmtType,
//#line 2918 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//					   FLOAT fDamageAmmount, const FLOAT3D & vHitPoint, const FLOAT3D & vDirection );
//
//#line 3060 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ReceiveDamage( CEntity * penInflictor, enum DamageType dmtType,
//#line 3061 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//						FLOAT fDamageAmmount, const FLOAT3D & vHitPoint, const FLOAT3D & vDirection );
//
//#line 3222 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL ShouldBlowUp( void );
//
//#line 3237 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void BlowUp( void );
//
//#line 3291 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void * GetEntityInfo( void );
//
//#line 3309 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
	BOOL ReceiveItem( const CEntityEvent & ee );
//
//#line 3447 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ChangePlayerView();
//
//#line 3478 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ComputerPressed( void );
//
//#line 3495 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void UsePressed( BOOL bOrComputer );
//
//#line 3570 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SetGameEnd( void );
//
//#line 3584 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CheckGameEnd( void );
//
//#line 3609 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void PreapplyAction( const CPlayerAction & paAction );
//
//#line 3614 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ApplyAction( const CPlayerAction & paOriginal, FLOAT tmLatency );
//
//#line 3780 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void Disconnect( void );
//
//#line 3791 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CharacterChanged( const CPlayerCharacter & pcNew );
//
//#line 3847 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AliveActions( const CPlayerAction & pa );
//
//#line 3881 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void AutoActions( const CPlayerAction & pa );
//
//#line 3967 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetLerpedWeaponPosition( FLOAT3D vRel, CPlacement3D & pl );
//
//#line 3976 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SpawnBubbles( INDEX ctBubbles );
//
//#line 3991 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void PlayPowerUpSound( void );
//
//#line 3996 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ActiveActions( const CPlayerAction & paAction );
//
//#line 4416 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RoundViewAngle( ANGLE & aViewAngle, ANGLE aRound );
//
//#line 4426 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void DeathActions( const CPlayerAction & paAction );
//
//#line 4491 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ButtonsActions( CPlayerAction & paAction );
//
//#line 4602 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void ApplySniperZoom( BOOL bZoomIn );
//
//#line 4625 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	BOOL CheatsEnabled( void );
//
//#line 4631 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void Cheats( void );
//
//#line 4694 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void GetLerpedAbsoluteViewPlacement( CPlacement3D & plView );
//
//#line 4769 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CEntity * GetViewEntity( void );
//
//#line 4797 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderChainsawParticles( BOOL bThird );
//
//#line 4833 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderHUD( CPerspectiveProjection3D & prProjection, CDrawPort * pdp,
//#line 4834 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//					FLOAT3D vViewerLightDirection, COLOR colViewerLight, COLOR colViewerAmbient,
//#line 4835 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//					BOOL bRenderWeapon, INDEX iEye );
//
//#line 4942 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	CEntity * GetDeathmatchStartMarker( void );
//
//#line 5011 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void InitializePlayer();
//
//#line 5047 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	FLOAT3D GetTeleportingOffset( void );
//
//#line 5065 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RemapLevelNames( INDEX & iLevel );
//
//#line 5114 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void TeleportPlayer( enum WorldLinkType EwltType );
//
//#line 5359 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RecordEndOfLevelData( void );
//
//#line 5392 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void SpawnTeleport( void );
//
//#line 5414 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void RenderParticles( void );
//
//#line 5461 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void TeleportToAutoMarker( CPlayerActionMarker * ppam );
//
//#line 5491 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//	void CheckDeathForRespawnInPlace( EDeath eDeath );
//#define  STATE_CPlayer_Wounded 0x01910005
//	BOOL
//#line 5512 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		Wounded( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_WorldChange 0x01910006
//	BOOL
//#line 5520 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		WorldChange( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_WorldChangeDead 0x01910007
//	BOOL
//#line 5551 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		WorldChangeDead( const CEntityEvent &__eeInput );
//	BOOL H0x01910008_WorldChangeDead_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910009_WorldChangeDead_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_Death 0x0191000a
//	BOOL
//#line 5582 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		Death( const CEntityEvent &__eeInput );
//	BOOL H0x0191000b_Death_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191000c_Death_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_TheEnd 0x0191000d
//	BOOL
//#line 5842 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		TheEnd( const CEntityEvent &__eeInput );
//	BOOL H0x0191000e_TheEnd_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191000f_TheEnd_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_FirstInit 0x01910010
//	BOOL
//#line 5889 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		FirstInit( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_Rebirth 0x01910011
//	BOOL
//#line 5920 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		Rebirth( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoGoToMarker 0x01910012
//	BOOL
//#line 5962 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoGoToMarker( const CEntityEvent &__eeInput );
//	BOOL H0x01910013_AutoGoToMarker_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910014_AutoGoToMarker_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910015_AutoGoToMarker_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910016_AutoGoToMarker_04( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoGoToMarkerAndStop 0x01910017
//	BOOL
//#line 5994 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoGoToMarkerAndStop( const CEntityEvent &__eeInput );
//	BOOL H0x01910018_AutoGoToMarkerAndStop_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910019_AutoGoToMarkerAndStop_02( const CEntityEvent &__eeInput );
//	BOOL H0x0191001a_AutoGoToMarkerAndStop_03( const CEntityEvent &__eeInput );
//	BOOL H0x0191001b_AutoGoToMarkerAndStop_04( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoUseItem 0x0191001c
//	BOOL
//#line 6034 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoUseItem( const CEntityEvent &__eeInput );
//	BOOL H0x0191001d_AutoUseItem_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191001e_AutoUseItem_02( const CEntityEvent &__eeInput );
//	BOOL H0x0191001f_AutoUseItem_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910020_AutoUseItem_04( const CEntityEvent &__eeInput );
//	BOOL H0x01910021_AutoUseItem_05( const CEntityEvent &__eeInput );
//	BOOL H0x01910022_AutoUseItem_06( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoPickItem 0x01910023
//	BOOL
//#line 6079 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoPickItem( const CEntityEvent &__eeInput );
//	BOOL H0x01910024_AutoPickItem_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910025_AutoPickItem_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910026_AutoPickItem_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910027_AutoPickItem_04( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoFallDown 0x01910028
//	BOOL
//#line 6114 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoFallDown( const CEntityEvent &__eeInput );
//	BOOL H0x01910029_AutoFallDown_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191002a_AutoFallDown_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoFallToAbys 0x0191002b
//	BOOL
//#line 6126 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoFallToAbys( const CEntityEvent &__eeInput );
//	BOOL H0x0191002c_AutoFallToAbys_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191002d_AutoFallToAbys_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoLookAround 0x0191002e
//	BOOL
//#line 6139 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoLookAround( const CEntityEvent &__eeInput );
//	BOOL H0x0191002f_AutoLookAround_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910030_AutoLookAround_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910031_AutoLookAround_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910032_AutoLookAround_04( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoTeleport 0x01910033
//	BOOL
//#line 6163 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoTeleport( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoAppear 0x01910034
//	BOOL
//#line 6172 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoAppear( const CEntityEvent &__eeInput );
//	BOOL H0x01910035_AutoAppear_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910036_AutoAppear_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910037_AutoAppear_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910038_AutoAppear_04( const CEntityEvent &__eeInput );
//	BOOL H0x01910039_AutoAppear_05( const CEntityEvent &__eeInput );
//	BOOL H0x0191003a_AutoAppear_06( const CEntityEvent &__eeInput );
//	BOOL H0x0191003b_AutoAppear_07( const CEntityEvent &__eeInput );
//	BOOL H0x0191003c_AutoAppear_08( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_TravellingInBeam 0x0191003d
//	BOOL
//#line 6224 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		TravellingInBeam( const CEntityEvent &__eeInput );
//	BOOL H0x0191003e_TravellingInBeam_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191003f_TravellingInBeam_02( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_LogoFireMinigun 0x01910040
//	BOOL
//#line 6244 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		LogoFireMinigun( const CEntityEvent &__eeInput );
//	BOOL H0x01910041_LogoFireMinigun_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910042_LogoFireMinigun_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910043_LogoFireMinigun_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910044_LogoFireMinigun_04( const CEntityEvent &__eeInput );
//	BOOL H0x01910045_LogoFireMinigun_05( const CEntityEvent &__eeInput );
//	BOOL H0x01910046_LogoFireMinigun_06( const CEntityEvent &__eeInput );
//	BOOL H0x01910047_LogoFireMinigun_07( const CEntityEvent &__eeInput );
//	BOOL H0x01910048_LogoFireMinigun_08( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_AutoStoreWeapon 0x01910049
//	BOOL
//#line 6305 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		AutoStoreWeapon( const CEntityEvent &__eeInput );
//	BOOL H0x0191004a_AutoStoreWeapon_01( const CEntityEvent &__eeInput );
//	BOOL H0x0191004b_AutoStoreWeapon_02( const CEntityEvent &__eeInput );
//	BOOL H0x0191004c_AutoStoreWeapon_03( const CEntityEvent &__eeInput );
//	BOOL H0x0191004d_AutoStoreWeapon_04( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_DoAutoActions 0x0191004e
//	BOOL
//#line 6339 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		DoAutoActions( const CEntityEvent &__eeInput );
//	BOOL H0x0191004f_DoAutoActions_01( const CEntityEvent &__eeInput );
//	BOOL H0x01910050_DoAutoActions_02( const CEntityEvent &__eeInput );
//	BOOL H0x01910051_DoAutoActions_03( const CEntityEvent &__eeInput );
//	BOOL H0x01910052_DoAutoActions_04( const CEntityEvent &__eeInput );
//	BOOL H0x01910053_DoAutoActions_05( const CEntityEvent &__eeInput );
//	BOOL H0x01910054_DoAutoActions_06( const CEntityEvent &__eeInput );
//	BOOL H0x01910055_DoAutoActions_07( const CEntityEvent &__eeInput );
//	BOOL H0x01910056_DoAutoActions_08( const CEntityEvent &__eeInput );
//	BOOL H0x01910057_DoAutoActions_09( const CEntityEvent &__eeInput );
//	BOOL H0x01910058_DoAutoActions_10( const CEntityEvent &__eeInput );
//	BOOL H0x01910059_DoAutoActions_11( const CEntityEvent &__eeInput );
//	BOOL H0x0191005a_DoAutoActions_12( const CEntityEvent &__eeInput );
//	BOOL H0x0191005b_DoAutoActions_13( const CEntityEvent &__eeInput );
//	BOOL H0x0191005c_DoAutoActions_14( const CEntityEvent &__eeInput );
//	BOOL H0x0191005d_DoAutoActions_15( const CEntityEvent &__eeInput );
//	BOOL H0x0191005e_DoAutoActions_16( const CEntityEvent &__eeInput );
//	BOOL H0x0191005f_DoAutoActions_17( const CEntityEvent &__eeInput );
//	BOOL H0x01910060_DoAutoActions_18( const CEntityEvent &__eeInput );
//	BOOL H0x01910061_DoAutoActions_19( const CEntityEvent &__eeInput );
//	BOOL H0x01910062_DoAutoActions_20( const CEntityEvent &__eeInput );
//	BOOL H0x01910063_DoAutoActions_21( const CEntityEvent &__eeInput );
//	BOOL H0x01910064_DoAutoActions_22( const CEntityEvent &__eeInput );
//	BOOL H0x01910065_DoAutoActions_23( const CEntityEvent &__eeInput );
//	BOOL H0x01910066_DoAutoActions_24( const CEntityEvent &__eeInput );
//	BOOL H0x01910067_DoAutoActions_25( const CEntityEvent &__eeInput );
//	BOOL H0x01910068_DoAutoActions_26( const CEntityEvent &__eeInput );
//	BOOL H0x01910069_DoAutoActions_27( const CEntityEvent &__eeInput );
//	BOOL H0x0191006a_DoAutoActions_28( const CEntityEvent &__eeInput );
//	BOOL H0x0191006b_DoAutoActions_29( const CEntityEvent &__eeInput );
//	BOOL H0x0191006c_DoAutoActions_30( const CEntityEvent &__eeInput );
//	BOOL H0x0191006d_DoAutoActions_31( const CEntityEvent &__eeInput );
//	BOOL H0x0191006e_DoAutoActions_32( const CEntityEvent &__eeInput );
//	BOOL H0x0191006f_DoAutoActions_33( const CEntityEvent &__eeInput );
//	BOOL H0x01910070_DoAutoActions_34( const CEntityEvent &__eeInput );
//	BOOL H0x01910071_DoAutoActions_35( const CEntityEvent &__eeInput );
//	BOOL H0x01910072_DoAutoActions_36( const CEntityEvent &__eeInput );
//	BOOL H0x01910073_DoAutoActions_37( const CEntityEvent &__eeInput );
//	BOOL H0x01910074_DoAutoActions_38( const CEntityEvent &__eeInput );
//	BOOL H0x01910075_DoAutoActions_39( const CEntityEvent &__eeInput );
//	BOOL H0x01910076_DoAutoActions_40( const CEntityEvent &__eeInput );
//	BOOL H0x01910077_DoAutoActions_41( const CEntityEvent &__eeInput );
//	BOOL H0x01910078_DoAutoActions_42( const CEntityEvent &__eeInput );
//	BOOL H0x01910079_DoAutoActions_43( const CEntityEvent &__eeInput );
//	BOOL H0x0191007a_DoAutoActions_44( const CEntityEvent &__eeInput );
//	BOOL H0x0191007b_DoAutoActions_45( const CEntityEvent &__eeInput );
//	BOOL H0x0191007c_DoAutoActions_46( const CEntityEvent &__eeInput );
//	BOOL H0x0191007d_DoAutoActions_47( const CEntityEvent &__eeInput );
//	BOOL H0x0191007e_DoAutoActions_48( const CEntityEvent &__eeInput );
//	BOOL H0x0191007f_DoAutoActions_49( const CEntityEvent &__eeInput );
//	BOOL H0x01910080_DoAutoActions_50( const CEntityEvent &__eeInput );
//	BOOL H0x01910081_DoAutoActions_51( const CEntityEvent &__eeInput );
//	BOOL H0x01910082_DoAutoActions_52( const CEntityEvent &__eeInput );
//	BOOL H0x01910083_DoAutoActions_53( const CEntityEvent &__eeInput );
//	BOOL H0x01910084_DoAutoActions_54( const CEntityEvent &__eeInput );
//	BOOL H0x01910085_DoAutoActions_55( const CEntityEvent &__eeInput );
//	BOOL H0x01910086_DoAutoActions_56( const CEntityEvent &__eeInput );
//	BOOL H0x01910087_DoAutoActions_57( const CEntityEvent &__eeInput );
//	BOOL H0x01910088_DoAutoActions_58( const CEntityEvent &__eeInput );
//	BOOL H0x01910089_DoAutoActions_59( const CEntityEvent &__eeInput );
//	BOOL H0x0191008a_DoAutoActions_60( const CEntityEvent &__eeInput );
//	BOOL H0x0191008b_DoAutoActions_61( const CEntityEvent &__eeInput );
//	BOOL H0x0191008c_DoAutoActions_62( const CEntityEvent &__eeInput );
//	BOOL H0x0191008d_DoAutoActions_63( const CEntityEvent &__eeInput );
//	BOOL H0x0191008e_DoAutoActions_64( const CEntityEvent &__eeInput );
//	BOOL H0x0191008f_DoAutoActions_65( const CEntityEvent &__eeInput );
//	BOOL H0x01910090_DoAutoActions_66( const CEntityEvent &__eeInput );
//	BOOL H0x01910091_DoAutoActions_67( const CEntityEvent &__eeInput );
//	BOOL H0x01910092_DoAutoActions_68( const CEntityEvent &__eeInput );
//	BOOL H0x01910093_DoAutoActions_69( const CEntityEvent &__eeInput );
//	BOOL H0x01910094_DoAutoActions_70( const CEntityEvent &__eeInput );
//	BOOL H0x01910095_DoAutoActions_71( const CEntityEvent &__eeInput );
//	BOOL H0x01910096_DoAutoActions_72( const CEntityEvent &__eeInput );
//	BOOL H0x01910097_DoAutoActions_73( const CEntityEvent &__eeInput );
//	BOOL H0x01910098_DoAutoActions_74( const CEntityEvent &__eeInput );
//	BOOL H0x01910099_DoAutoActions_75( const CEntityEvent &__eeInput );
//	BOOL H0x0191009a_DoAutoActions_76( const CEntityEvent &__eeInput );
//	BOOL H0x0191009b_DoAutoActions_77( const CEntityEvent &__eeInput );
//	BOOL H0x0191009c_DoAutoActions_78( const CEntityEvent &__eeInput );
//	BOOL H0x0191009d_DoAutoActions_79( const CEntityEvent &__eeInput );
//	BOOL H0x0191009e_DoAutoActions_80( const CEntityEvent &__eeInput );
//	BOOL H0x0191009f_DoAutoActions_81( const CEntityEvent &__eeInput );
//	BOOL H0x019100a0_DoAutoActions_82( const CEntityEvent &__eeInput );
//	BOOL H0x019100a1_DoAutoActions_83( const CEntityEvent &__eeInput );
//	BOOL H0x019100a2_DoAutoActions_84( const CEntityEvent &__eeInput );
//	BOOL H0x019100a3_DoAutoActions_85( const CEntityEvent &__eeInput );
//#define  STATE_CPlayer_Main 1
//	BOOL
//#line 6550 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Player.es"
//		Main( const CEntityEvent &__eeInput );
//	BOOL H0x019100a4_Main_01( const CEntityEvent &__eeInput );
//	BOOL H0x019100a5_Main_02( const CEntityEvent &__eeInput );
//	BOOL H0x019100a6_Main_03( const CEntityEvent &__eeInput );
//	BOOL H0x019100a7_Main_04( const CEntityEvent &__eeInput );
};

#endif // _EntitiesMP_Player_INCLUDED
