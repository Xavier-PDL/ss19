#pragma once
#include "MovableModelEntity.h"
#include <Engine/Entities/PlayerCharacter.h>

#ifndef _Engine_Classes_PlayerEntity_INCLUDED
#define _Engine_Classes_PlayerEntity_INCLUDED 1
//extern "C" DECL_DLL CDLLEntityClass CPlayerEntity_DLLClass;
class CPlayerEntity : public CMovableModelEntity {
public:
//   virtual void SetDefaultProperties(void);
	FLOAT en_tmPing;
	CPlayerCharacter en_pcCharacter;
	CPlacement3D en_plViewpoint;
	CPlacement3D en_plLastViewpoint;
//   CTString GetPlayerName(void);
//   const CTString & GetName(void)const;
//   INDEX GetMyPlayerIndex(void);
//   void DoMoving(void);
//   void Copy(CEntity & enOther,ULONG ulFlags);
//   void Read_t(CTStream * istr);
//   void Write_t(CTStream * ostr);
//  virtual void ApplyAction(const CPlayerAction & pa,FLOAT tmLatency);
//  virtual void Disconnect(void);
//  virtual void CharacterChanged(const CPlayerCharacter & pcNew);
//  virtual void GetGameAgentPlayerInfo(INDEX iPlayer,CTString & strOut);
//  virtual void GetMSLegacyPlayerInf(INDEX iPlayer,CTString & strOut);
//   void ChecksumForSync(ULONG & ulCRC,INDEX iExtensiveSyncCheck);
//   void DumpSync_t(CTStream & strm,INDEX iExtensiveSyncCheck);
//#define  STATE_CPlayerEntity_Dummy 0x00040000
//  BOOL 
//#line 143 "X:/Git/Repos/Serious-Engine/Sources/Engine/Classes/PlayerEntity.es"
//Dummy(const CEntityEvent &__eeInput);
};
#endif // _Engine_Classes_PlayerEntity_INCLUDED