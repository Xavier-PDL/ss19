#pragma once
#include <Engine/Base/Lists.h>

/*
 * Template class for array with dynamic allocation of objects.
 */
template<class Type>
class CDynamicArray {
public:
	CListHead da_BlocksList;  // list of allocated memory blocks
	Type **da_Pointers;       // array of object pointers
	INDEX da_Count;           // number of objects
#if CHECKARRAYLOCKING
	INDEX da_LockCt;          // lock counter for getting indices
#endif
};