#pragma once
#include <Engine/Templates/DynamicArray.h>


/*
 * Template class for stack-like array with dynamic allocation of objects.
 */
template<class Type>
class CDynamicStackArray : public CDynamicArray<Type> {
public:
	INDEX da_ctUsed;            // number of used objects in array
	INDEX da_ctAllocationStep;  // how many elements to allocate when stack overflows
};