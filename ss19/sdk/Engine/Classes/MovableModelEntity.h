#pragma once
#include <Engine/Classes/MovableEntity.h>
#ifndef _Engine_Classes_MovableModelEntity_INCLUDED
#define _Engine_Classes_MovableModelEntity_INCLUDED 1
//extern "C" DECL_DLL CDLLEntityClass CMovableModelEntity_DLLClass;
class  CMovableModelEntity : public CMovableEntity {
public:
   //virtual void SetDefaultProperties(void);
  INDEX en_iCollisionBox;
  INDEX en_iWantedCollisionBox;
//   void ChecksumForSync(ULONG & ulCRC,INDEX iExtensiveSyncCheck);
//   void DumpSync_t(CTStream & strm,INDEX iExtensiveSyncCheck);
//   void PreMoving(void);
//   void DoMoving(void);
//   INDEX GetCollisionBoxIndex(void);
//   BOOL CheckForCollisionNow(INDEX iNewCollisionBox,CEntity * * ppenObstacle);
//   BOOL ChangeCollisionBoxIndexNow(INDEX iNewCollisionBox,CEntity * * ppenObstacle);
//   BOOL ChangeCollisionBoxIndexNow(INDEX iNewCollisionBox);
//   void ForceCollisionBoxIndexChange(INDEX iNewCollisionBox);
//   void ChangeCollisionBoxIndexWhenPossible(INDEX iNewCollisionBox);
//   void Read_t(CTStream * istr);
//   void Write_t(CTStream * ostr);
//   
//#line 179 "X:/Git/Repos/Serious-Engine/Sources/Engine/Classes/MovableModelEntity.es"
//SLONG GetUsedMemory(void);
//#define  STATE_CMovableModelEntity_Dummy 0x00020000
//  BOOL 
//#line 189 "X:/Git/Repos/Serious-Engine/Sources/Engine/Classes/MovableModelEntity.es"
//Dummy(const CEntityEvent &__eeInput);
//#define  STATE_CMovableModelEntity_WaitUntilScheduledAnimStarts 0x00020001
//  BOOL 
//#line 192 "X:/Git/Repos/Serious-Engine/Sources/Engine/Classes/MovableModelEntity.es"
//WaitUntilScheduledAnimStarts(const CEntityEvent &__eeInput);
//  BOOL H0x00020002_WaitUntilScheduledAnimStarts_01(const CEntityEvent &__eeInput);
//  BOOL H0x00020003_WaitUntilScheduledAnimStarts_02(const CEntityEvent &__eeInput);
//  BOOL H0x00020004_WaitUntilScheduledAnimStarts_03(const CEntityEvent &__eeInput);
};
#endif // _Engine_Classes_MovableModelEntity_INCLUDED
