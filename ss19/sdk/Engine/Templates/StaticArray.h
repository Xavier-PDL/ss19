#pragma once

/*
 * Template class for array with static allocation of objects.
 */
template<class Type>
class CStaticArray {
public:
	INDEX sa_Count;      // number of objects in array
	Type *sa_Array;      // objects
};