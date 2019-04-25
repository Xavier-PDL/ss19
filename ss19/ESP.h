#pragma once
#include <sdk.h>
#include "Options.h"

class ESP
{
public:
	static void W2S( CProjection3D * pProjection, CEntity* pTarget, bool bVisible=false );
	static void Draw();
};

