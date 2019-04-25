#pragma once
#include <Engine/Network/Network.h>
#include <Engine/Base/Input.h>
#include <Engine/Classes/PlayerEntity.h>
#include <Engine/Graphics/Font.h>
#include <Engine/Graphics/DrawPort.h>
#include <EntitiesMP/Camera.h>
#include <Engine/Math/Projection.h>
#include <Engine/World/WorldRayCasting.h>
#include <EntitiesMP/PlayerWeapons.h>
#include <Engine/Entities/EntityClass.h>
#include <EntitiesMP/WeaponItem.h>
#include <EntitiesMP/EnemyBase.h>

#define GetSamPtr() CEntity::GetPlayerEntity(0)