#pragma once

class CTString
{
public:
	char* pString = nullptr;
public:
	CTString() { Create( "" ); };
	CTString(const char* szText) { Create( szText ); };
//	~CTString() { if ( pString ) delete pString; }
	~CTString();
	CTString* Create( const char* str );
};