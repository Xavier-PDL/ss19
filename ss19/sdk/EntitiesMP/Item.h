/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */
#ifndef _EntitiesMP_Item_INCLUDED
#define _EntitiesMP_Item_INCLUDED 1
#include <Engine/Entities/Entity.h>
#include <Engine/Classes/MovableModelEntity.h>
#include <Engine/Entities/EntityPointer.h>
#include <Engine/Sound/SoundObject.h>
//extern "C" DECL_DLL CDLLEntityClass CItem_DLLClass;
class CItem : public CMovableModelEntity {
public:
	CTString m_strName;
	CTString m_strDescription;
	FLOAT m_fValue;
	FLOAT m_fRespawnTime;
	FLOAT m_fCustomRespawnTime;
	BOOL m_bRespawn;
	CEntityPointer m_penTarget;
	BOOL m_bPickupOnce;
	CSoundObject m_soPick;
	FLOAT m_fPickSoundLen;
	BOOL m_bDropped;
	int iPad;
	INDEX m_ulPickedMask;
	BOOL m_bFloating;
	CEntityPointer m_penPrediction;

	virtual const CTString &GetName( void ) const { return m_strName; };
	virtual const CTString &GetDescription( void ) const { return m_strDescription; };
	virtual BOOL IsTargetable( void ) const { return TRUE; };
	virtual CEntity *GetPredictionPair( void ) { return m_penPrediction.ep_pen; };
	virtual void SetPredictionPair( CEntity *penPair ) { m_penPrediction.ep_pen = penPair; };
	//virtual void SetDefaultProperties( void );

//#line 53 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	virtual void AdjustDifficulty( void );
//
//#line 58 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void AdjustMipFactor( FLOAT & fMipFactor );
//
//#line 97 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	BOOL ShowItemParticles( void );
//
//#line 113 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	BOOL MarkPickedBy( CEntity * pen );
//
//#line 125 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	FLOAT GetPredictionRange( void );
//
//#line 132 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	BOOL AdjustShadingParameters( FLOAT3D & vLightDirection, COLOR & colLight, COLOR & colAmbient );
//
//#line 162 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void Initialize( void );
//
//#line 188 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void AddItem( ULONG ulIDModel, ULONG ulIDTexture,
//#line 189 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//				  ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture );
//
//#line 193 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void AddItemSpecial( INDEX iAttachmentPos, ULONG ulIDModel, ULONG ulIDTexture,
//#line 194 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//						 ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture );
//
//#line 200 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void AddItemAttachment( INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture,
//#line 201 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//							ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture );
//
//#line 207 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void SetItemAttachmentAnim( INDEX iAttachment, INDEX iAnim );
//
//#line 216 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void AddFlare( ULONG ulIDModel, ULONG ulIDTexture,
//#line 217 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//				   const FLOAT3D & vPos, const FLOAT3D & vStretch );
//
//#line 231 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	void StretchItem( const FLOAT3D & vStretch );
//
//#line 240 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//	SLONG GetUsedMemory( void );
//#define  STATE_CItem_ItemCollected 0x03200000
//	BOOL
//#line 258 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//		ItemCollected( const CEntityEvent &__eeInput );
//#define  STATE_CItem_ItemLoop 0x03200001
//	BOOL
//#line 266 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//		ItemLoop( const CEntityEvent &__eeInput );
//	BOOL H0x03200002_ItemLoop_01( const CEntityEvent &__eeInput );
//	BOOL H0x03200003_ItemLoop_02( const CEntityEvent &__eeInput );
//	BOOL H0x03200004_ItemLoop_03( const CEntityEvent &__eeInput );
//	BOOL H0x03200005_ItemLoop_04( const CEntityEvent &__eeInput );
//	BOOL H0x03200006_ItemLoop_05( const CEntityEvent &__eeInput );
//	BOOL H0x03200007_ItemLoop_06( const CEntityEvent &__eeInput );
//#define  STATE_CItem_ItemReceived 0x03200008
//	BOOL
//#line 296 "X:/Git/Repos/Serious-Engine/Sources/EntitiesMP/Item.es"
//		ItemReceived( const CEntityEvent &__eeInput );
//	BOOL H0x03200009_ItemReceived_01( const CEntityEvent &__eeInput );
//	BOOL H0x0320000a_ItemReceived_02( const CEntityEvent &__eeInput );
//	BOOL H0x0320000b_ItemReceived_03( const CEntityEvent &__eeInput );
//	BOOL H0x0320000c_ItemReceived_04( const CEntityEvent &__eeInput );
};
#endif // _EntitiesMP_Item_INCLUDED
