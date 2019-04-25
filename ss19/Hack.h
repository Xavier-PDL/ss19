#pragma once
#include "TMem.h"
#include <sdk.h>
#include "Draw.h"
#include "ESP.h"
#include "Aimbot.h"
#include "MyInput.h"
#include "Menu.h"

#define HKID_SWAPBUFFERS		0
#define HKSZ_SWAPBUFFERS		6
#define HKID_RENDERVIEW			1
#define HKSZ_RENDERVIEW			6
#define HKID_SETPLACEMENT		2
#define HKSZ_SETPLACEMENT		5

extern HANDLE hEngine;
extern HANDLE hEntities;

class Hack
{
	static HANDLE hDLL;
public:
	static bool Install(HANDLE hDLL);
	static bool Init();
	static void HandleInput();
	static void Run();
	static void Unload();
};

void FillHealthAndArmor();
void GiveAllWeapons();
void FillAmmo();
void ToggleInfAmmo();
void ToggleLowGravity();
void ToggleJumpWalk();
void ToggleRapidFire();
void ToggleCollisionDmg();