#pragma once
#include <Engine/Base/Types.h>

//class CListHead;
class CListNode
{
public:
	CListNode* pPrev = nullptr;
	CListNode* pNext = nullptr;
};

class CListHead
{
public:
	CListNode * pHead = nullptr;
	CListNode * pNull = nullptr;
	CListNode * pTail = nullptr;
};