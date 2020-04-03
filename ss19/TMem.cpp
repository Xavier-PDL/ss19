#include "TMem.h"

void* TSyscall::pCodeLoc = nullptr;

namespace TError
{
	tstring sLastError;
	LastErrorType letLastErrorType;

	void SetLastError( LastErrorType let, const tstring & szError )
	{
		letLastErrorType = let;
		if ( let != WINDOWS && !szError.empty() )
			sLastError = szError;
	}

	tstring GetLastErrorStr()
	{
		if ( letLastErrorType == WINDOWS )
		{
			tstring buf( size_t( 256 ), wchar_t( 0 ) );
			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				GetLastError(),
				LANG_NEUTRAL,
				&buf[0],
				(DWORD) buf.size(),
				NULL
			);
			return buf;
		}
		else
			return sLastError;
	}
}

namespace NT
{
	//fnNtQueryInformationProcess pNtQueryInformationProcess = nullptr;
	//fnNtQueryInformationThread pNtQueryInformationThread = nullptr;
	//fnNtQueryVirtualMemory pNtQueryVirtualMemory = nullptr;
	//fnNtProtectVirtualMemory pNtProtectVirtualMemory = nullptr;
	//fnRtlAddVectoredExceptionHandler pRtlAddVectoredExceptionHandler = nullptr;
	//fnRtlRemoveVectoredExceptionHandler pRtlRemoveVectoredExceptionHandler = nullptr;
	fnRtlAllocateHeap pRtlAllocateHeap = nullptr;

	PVEH_HANDLER_LIST pVehHandlerList = nullptr;
	DWORD dwEncoderKey = 0;
	bool InitNT()
	{
		//pNtQueryInformationProcess = (fnNtQueryInformationProcess) GetProcedureAddress( _X( "ntdll.dll" ), _X( "NtQueryInformationProcess" ) );
		//if ( !pNtQueryInformationProcess )
		//	return false;

		//pNtQueryInformationThread = (fnNtQueryInformationThread) GetProcedureAddress( _X( "ntdll.dll" ), _X( "NtQueryInformationThread" ) );
		//if ( !pNtQueryInformationThread )
		//	return false;

		//pNtQueryVirtualMemory = (fnNtQueryVirtualMemory) GetProcedureAddress( _X( "ntdll.dll" ), _X( "NtQueryVirtualMemory" ) );
		//if ( !pNtQueryVirtualMemory )
		//	return false;

		//pNtProtectVirtualMemory = (fnNtProtectVirtualMemory) GetProcedureAddress( _X( "ntdll.dll" ), _X( "NtProtectVirtualMemory" ) );
		//if ( !pNtProtectVirtualMemory )
		//	return false;

		//pRtlAddVectoredExceptionHandler = (fnRtlAddVectoredExceptionHandler) GetProcedureAddress( _X( "ntdll.dll" ), _X( "RtlAddVectoredExceptionHandler" ) );
		//if ( !pRtlAddVectoredExceptionHandler )
		//	return false;

		//pRtlRemoveVectoredExceptionHandler = (fnRtlRemoveVectoredExceptionHandler) GetProcedureAddress( _X( "ntdll.dll" ), _X( "RtlRemoveVectoredExceptionHandler" ) );
		//if ( !pRtlRemoveVectoredExceptionHandler )
		//	return false;

		pRtlAllocateHeap = (fnRtlAllocateHeap) GetProcedureAddress( _X( "ntdll.dll" ), _X( "RtlAllocateHeap" ) );
		if ( !pRtlAllocateHeap )
			return false;

#ifdef _WIN64
		auto pLoc = (BYTE*) TMem::Get()->ScanPattern( GetModuleHandle( _X( "ntdll.dll" ) ), "48 8D 0D ? ? ? ? 48 8B 0C F1" );
		if(pLoc)
			pVehHandlerList = (PVEH_HANDLER_LIST) ((pLoc + 7 + *(DWORD*) (pLoc + 3)));
		pLoc = (BYTE*) TMem::Get()->ScanPattern( GetModuleHandle( _X( "ntdll.dll" ) ), "8B 0D ? ? ? ? C7 40 10 01 00 00 00 85 C9" );
		if(pLoc)
			dwEncoderKey = *(DWORD*) (pLoc + 6 + *(DWORD*) (pLoc + 2));
#else
		auto pLoc = (DWORD) TMem::Get()->ScanPattern( GetModuleHandle( _X( "ntdll.dll" ) ), "81 C3 ? ? ? ? 8D 7B 04" );
		if(pLoc)
			pVehHandlerList = *(PVEH_HANDLER_LIST*) (pLoc + 2);
		pLoc = ((DWORD) TMem::Get()->ScanPattern( GetModuleHandle( _X( "ntdll.dll" ) ), "A1 ? ? ? ? C7 46 ? 01 00 00 00 85 C0" ));
		if(pLoc)
			dwEncoderKey = **(DWORD**) (pLoc + 1);
#endif
		if ( !pVehHandlerList )
			return false;

		return true;
	}

	_PEB* GetPEB()
	{
		//PROCESS_BASIC_INFORMATION pbi{ 0 };
		//if ( !pNtQueryInformationProcess )
		//	return nullptr;

		//ULONG ulSize = 0;
		//pNtQueryInformationProcess( GetCurrentProcess(), ProcessBasicInformation, &pbi, sizeof( PROCESS_BASIC_INFORMATION ), &ulSize );
		//return pbi.PebBaseAddress;

#ifdef _WIN64
		return (_PEB*) __readgsqword( 0x60 );
#else
		return (_PEB*) __readfsdword( 0x30 );
#endif
	}

	void * GetThreadStartAddress( HANDLE hThread )
	{
		void* pStartAddress = nullptr;
		ULONG retLen = 0;
		ScQueryInformationThread( hThread, ThreadQuerySetWin32StartAddress, (PVOID) &pStartAddress, (ULONG)sizeof( pStartAddress ), (PULONG) &retLen );
		return pStartAddress;
	}

	void * GetProcedureAddress( const char * szModule, const char * szFunction )
	{
		auto hModule = GetModuleHandle( szModule );
		if ( !hModule )
			return nullptr;

		IMAGE_DOS_HEADER* pDos = (IMAGE_DOS_HEADER*) hModule;
		IMAGE_NT_HEADERS* pNT = (IMAGE_NT_HEADERS*) (pDos->e_lfanew + (uintptr_t) hModule);

		auto pDataDir = &pNT->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		if ( !pDataDir->VirtualAddress )
			return nullptr;

		IMAGE_EXPORT_DIRECTORY* pExportDir = (IMAGE_EXPORT_DIRECTORY*) RVAtoVA( pDos, pDataDir->VirtualAddress );
		for ( DWORD x = 0; x < pExportDir->NumberOfNames; x++ )
		{
			DWORD nameRVA = reinterpret_cast<DWORD*>(RVAtoVA( pDos, pExportDir->AddressOfNames ))[x];
			uintptr_t pName = RVAtoVA( pDos, nameRVA );

			if ( !lstrcmpA( (char*) (pName), szFunction ) )
			{
				WORD funcOrd = reinterpret_cast<WORD*>(RVAtoVA( pDos, pExportDir->AddressOfNameOrdinals ))[x];
				auto pAddresses = RVAtoVA( pDos, pExportDir->AddressOfFunctions );
				auto funcRVA = reinterpret_cast<DWORD*>(pAddresses)[funcOrd];
				return (void*) RVAtoVA( hModule, funcRVA );
			}
		}
		return nullptr;
	}

	void * GetProcAddressIAT( const char * szFunction )
	{
		auto hModule = GetModuleHandle( 0 );
		IMAGE_DOS_HEADER* pDos = (IMAGE_DOS_HEADER*) hModule;
		IMAGE_NT_HEADERS* pNT = (IMAGE_NT_HEADERS*) (pDos->e_lfanew + (uintptr_t) hModule);

		auto pDataDir = &pNT->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

		IMAGE_IMPORT_DESCRIPTOR* pImportDir = (IMAGE_IMPORT_DESCRIPTOR*) RVAtoVA( pDos, pDataDir->VirtualAddress );

		for ( unsigned int x = 0; x < pDataDir->Size / sizeof( IMAGE_IMPORT_DESCRIPTOR ); x++ )
		{
			auto nameRVA = pImportDir->Name;
			char* pName = (char*) RVAtoVA( pDos, nameRVA );

			auto pImportsLookup = (uintptr_t**) RVAtoVA( pDos, pImportDir->OriginalFirstThunk );
			auto pImportFunc = (void**) RVAtoVA( pDos, pImportDir->FirstThunk );


			auto ImportLookupRVA = *pImportsLookup;
			auto pImportLookup = (IMAGE_IMPORT_BY_NAME*) RVAtoVA( pDos, ImportLookupRVA );
			do
			{
				ImportLookupRVA = *pImportsLookup;
				if ( !ImportLookupRVA )
					break;
				pImportLookup = (IMAGE_IMPORT_BY_NAME*) RVAtoVA( pDos, ImportLookupRVA );

				char* name = pImportLookup->Name;
				if ((int)name & 0x80000000)
					break;
				if (!lstrcmpA(name, szFunction))
				{
					return (void*)pImportFunc;
				}
				
				pImportsLookup++;
				pImportFunc++;
			} while ( pImportLookup );

			pImportDir++;
		}

		return nullptr;
	}

	bool QueryVirtualMemory( void * pAddress, MEMORY_BASIC_INFORMATION * pMbi )
	{
		SIZE_T retSize = 0;
		NTSTATUS ntStatus = ScQueryVirtualMemory( GetCurrentProcess(), pAddress, MemoryBasicInformation, pMbi, sizeof( MEMORY_BASIC_INFORMATION ), &retSize );

		if ( ntStatus )
			return false;

		return true;
	}

	bool ProtectVirtualMemory( void * pAddress, DWORD dwProtection, DWORD * pOldProtection )
	{
		SIZE_T size = 0x1;

		NTSTATUS ntStatus = ScProtectVirtualMemory( HANDLE( -1 ), &pAddress, &size, dwProtection, pOldProtection );

		if ( ntStatus )
			return false;

		return true;
	}

	void * AddVEH( DWORD uFirst, PVECTORED_EXCEPTION_HANDLER pVEH )
	{
		uintptr_t pFunc = (uintptr_t) pVEH;

		// Get heap handle from peb
		auto pPeb = GetPEB();
		auto hHeap = pPeb->Reserved4[1];
		pPeb->Reserved6 = 5; // some sort of flag that i'm assuming lets ntdll know "we're using veh!"

		// allocate space on heap for a VEH list node
		PVEH_LIST_NODE pListNode = nullptr;
		pListNode = (PVEH_LIST_NODE) pRtlAllocateHeap( hHeap, 0, sizeof( VEH_LIST_NODE ) );

		uintptr_t encodedPtr = VehEncodePtr( pFunc, dwEncoderKey );

		// write that 1 to the heap handler thingy
		pListNode->dwOne = 1;

		// write encoded ptr to heap handler thingy
		pListNode->pEncodedFuncPtr = (void*) encodedPtr;

		auto listHead = (PVEH_LIST_NODE) pVehHandlerList->pHead;
		auto lastNode = listHead;
		PVEH_LIST_NODE pNode = nullptr;

		DWORD dwOld = 0;
		ProtectVirtualMemory( pVehHandlerList, PAGE_READWRITE, &dwOld );
		if ( uFirst || listHead->fLink == listHead )
		{
			pNode = listHead->fLink;

			if ( pNode == listHead )
			{
				pListNode->fLink = pNode;
				pListNode->bLink = listHead;
				pNode->fLink = pListNode;
				pNode->bLink = pListNode;

				ProtectVirtualMemory( pVehHandlerList, dwOld, &dwOld );
				return (void*) pListNode;
			}
			else
			{
				lastNode->bLink = pListNode;
				pListNode->fLink = lastNode;
				pListNode->bLink = pNode;
				pNode->fLink = pListNode;
			}
		}
		else
		{
			auto pTail = pVehHandlerList->pTail;
			pListNode->fLink = pTail->fLink;
			pTail->fLink = pListNode;
			pListNode->bLink = pTail;
			pVehHandlerList->pTail = pListNode;
		}
		ProtectVirtualMemory( pVehHandlerList, dwOld, &dwOld );
		return (void*) pListNode;
	}

	void GetVectoredExceptionHandlers( std::vector<VEH_INFO> & vHandlers )
	{
		auto listHead = pVehHandlerList->pHead->bLink;

		if ( listHead == listHead->fLink )
			return;

		auto pNode = pVehHandlerList->pHead;
		do
		{
			if ( pNode->fLink == pNode->bLink )
			{
				vHandlers.push_back( VEH_INFO( { pNode, (void*) (VehDecodePtr( pNode->pEncodedFuncPtr, dwEncoderKey )) } ) );
				break;
			}
			vHandlers.push_back( VEH_INFO( { pNode, (void*) (VehDecodePtr( pNode->pEncodedFuncPtr, dwEncoderKey )) } ) );
			if ( pNode->fLink == listHead )
				break;
			pNode = pNode->fLink;
		} while ( true );
	}

	void RemoveVEH( void * hHandler )
	{
		auto listHead = pVehHandlerList->pHead->bLink;

		if ( listHead == listHead->fLink )
			return;

		auto pNode = listHead->fLink;
		do
		{
			if ( pNode == listHead )
				break;
			if ( pNode == hHandler )
			{
				auto bLink = pNode->bLink;
				auto fLink = pNode->fLink;
				DWORD dwOld = 0;

				// could VirtualQuery both of these to check if it's either the head or tail
				// or just VirtualProtect both of em and keep it moving...

				ProtectVirtualMemory( (void*) bLink, PAGE_READWRITE, &dwOld );
				bLink->fLink = pNode->fLink;
				ProtectVirtualMemory( (void*) bLink, dwOld, &dwOld );

				ProtectVirtualMemory( (void*) fLink, PAGE_READWRITE, &dwOld );
				fLink->bLink = bLink;
				ProtectVirtualMemory( (void*) fLink, dwOld, &dwOld );
				break;
			}
			pNode = (PVEH_LIST_NODE) pNode->fLink;
		} while ( true );
	}
}

#pragma region TPattern

TPattern::TPattern( const char* sPattern )
{
	bIdaStyle = true;
	this->sPattern = sPattern;
}

TPattern::TPattern( const char* sPattern, const char* sMask )
{
	this->sMask = sMask;
	bytes.resize( this->sMask.length() );
	memcpy( &bytes[0], sPattern, bytes.size() );
}

std::vector<uint8_t>& TPattern::Get()
{
	if ( bytes.empty() )
		Parse();
	return bytes;
}

const std::string & TPattern::GetMask() const
{
	return sMask;
}

size_t TPattern::Size()
{
	if ( bytes.empty() )
		Parse();
	return bytes.size();
}

void TPattern::Parse()
{
	if ( bIdaStyle )
		ParsePatternIDA();
	else
		ParsePattern();
}

void TPattern::ParsePattern()
{
	auto uLen = sMask.length();
	for ( uint32_t i = 0; i < uLen; i++ )
	{
		if ( i < uLen && sPattern[i] == 0 )
			sPattern[i] = (char) 0xFF;

		if ( sMask[i] == '?' )
			bytes.push_back( 0 );
		else
		{
			auto sub = sPattern.substr( i, 1 );
			auto byte = uint8_t( sub[0] );
			bytes.push_back( byte );
		}
	}
}

void TPattern::ParsePatternIDA()
{
	auto uLen = sPattern.length();
	sMask.clear();
	int b = 0;
	char x, byteBuf = 0;
	for ( uint32_t i = 0; i < uLen; i++ )
	{
		char c = (char) sPattern[i];
		if ( c >= '0' && c <= '9' )
		{
			x = (0x0F & c);
			byteBuf += (b) ? x : x << 4;
			b++;
		}
		else if ( c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f' )
		{
			if ( c > 0x46 )
				c -= 0x20;
			x = c - 0x37;
			byteBuf += (b) ? x : x << 4;
			b++;
		}
		else if ( c == '?' )
		{
			if ( sPattern[i + 1] == '?' )
				i++;
			bytes.push_back( 0 );
			sMask += '?';
		}
		if ( b > 1 )
		{
			bytes.push_back( byteBuf );
			b = 0;
			byteBuf = 0;
			sMask += 'x';
		}
	}
}

#pragma endregion

#pragma region CodeBuffer

TCodeBuffer::TCodeBuffer( char * pBuffer, uint32_t size )
{
	this->buffer.resize( (size_t) size );
	memcpy_s( buffer.data(), buffer.size(), pBuffer, size );
}

TCodeBuffer::TCodeBuffer( const std::vector<uint8_t>& buf )
{
	this->buffer.resize( buf.size() );
	std::memmove( buffer.data(), buf.data(), buf.size() );
}

TCodeBuffer::TCodeBuffer( std::initializer_list<uint8_t> bytes )
{
	this->buffer = bytes;
}

bool TCodeBuffer::Update( size_t offset, std::initializer_list<uint8_t> bytes )
{
	if ( offset + bytes.size() > buffer.size() )
	{
		auto diff = (offset + bytes.size()) - buffer.size();
		buffer.resize( buffer.size() + diff );
	}
	if ( !memmove( &buffer[offset], bytes.begin(), bytes.size() ) )
		return false;
	return true;
}

bool TCodeBuffer::Update( size_t offset, void * ptr )
{
	return UpdateIntegral( offset, (uintptr_t) ptr );
}

void TCodeBuffer::AddJumpBack( uintptr_t pJmpBack )
{
	this->pJmpBack = pJmpBack;
	this->iJmpBackOffset = (int) buffer.size();
}

size_t TCodeBuffer::Size()
{
	return buffer.size();
}

std::vector<uint8_t> TCodeBuffer::Get()
{
	return buffer;
}

std::vector<uint8_t> TCodeBuffer::Get( uintptr_t pAddress )
{
	buffer.push_back( 0xe9 );
	auto src = pAddress + this->iJmpBackOffset + 5;
	this->operator+=( (DWORD) (pJmpBack - src) );
	return buffer;
}

void TCodeBuffer::operator=( std::vector<uint8_t> bytes )
{
	this->buffer = bytes;
}

void TCodeBuffer::operator=( std::initializer_list<uint8_t> bytes )
{
	this->buffer = bytes;
}

void TCodeBuffer::operator+=( std::vector<uint8_t> bytes )
{
	for ( auto b : bytes )
		this->operator+=( b );
}

void TCodeBuffer::operator+=( void * ptr )
{
	this->operator+=( (uintptr_t) ptr );
}

void TCodeBuffer::operator+=( std::initializer_list<uint8_t> bytes )
{
	for ( auto b : bytes )
		buffer.push_back( b );
}

void TCodeBuffer::operator=( const char * byteString )
{
	buffer.clear();
	this->operator+=( byteString );
}

void TCodeBuffer::operator+=( const char * byteString )
{
	auto len = strlen( byteString );
	for ( size_t x = 0; x < len; x++ )
	{
		buffer.push_back( byteString[x] );
	}
}

#pragma endregion

#pragma region HookManager

HookManager* HookManager::pManager = nullptr;

HookManager * HookManager::Get()
{
	if ( !pManager )
		pManager = new HookManager();
	return pManager;
}

void HookManager::Destroy()
{
	delete HookManager::pManager;
}

HookManager::HookManager()
{
	pManager = this;
	pMem = TMem::Get();
}

HookManager::~HookManager()
{
	for ( auto pair : hooks )
	{
		Uninstall( pair.second );
	}
}

bool HookManager::Add( Hook * pHook, bool bInstall )
{
	hooks[pHook->GetID()] = pHook;
	
	if ( bInstall )
		return Install( pHook );
	return false;
}

bool HookManager::Add( int id, Hook * pHook, bool bInstall )
{
	return Add( pHook, bInstall );
}

void HookManager::AddMF( int id, void * pTarget, void * pHook, int iBytes, bool bInstall )
{
	HookMF* hook = new HookMF( id, pTarget, pHook, iBytes );
	Add( hook, bInstall );
}

bool HookManager::Install( int id )
{
	return Install( Get( id ) );
}

bool HookManager::Install( Hook * pHook )
{
	pHook->status = true;
	switch ( pHook->GetType() )
	{
		case Hook::MF:
			return InstallMF( reinterpret_cast<HookMF*>(pHook) );
		case Hook::VMT:
			return InstallVMT( reinterpret_cast<HookVMT*>(pHook) );
		case Hook::TH:
			return InstallTH( reinterpret_cast<HookTH*>(pHook) );
		case Hook::PG:
			return InstallPG( reinterpret_cast<HookPG*>(pHook) );
		case Hook::IAT:
			return InstallIAT( reinterpret_cast<HookIAT*>(pHook) );
		default:
			break;
	}
	return true;
}

void HookManager::Uninstall( int id )
{
	Uninstall( Get( id ) );
}

void HookManager::Uninstall( Hook * pHook )
{
	pHook->status = false;
	switch ( pHook->GetType() )
	{
		case Hook::MF:
			UninstallMF( reinterpret_cast<HookMF*>(pHook) );
			break;
		case Hook::VMT:
			UninstallVMT( reinterpret_cast<HookVMT*>(pHook) );
		case Hook::TH:
			UninstallTH( reinterpret_cast<HookTH*>(pHook) );
			break;
		case Hook::PG:
			UninstallPG( reinterpret_cast<HookPG*>(pHook) );
			break;
		case Hook::IAT:
			UninstallIAT( reinterpret_cast<HookIAT*>(pHook) );
			break;
		default:
			break;
	}
}

Hook * HookManager::Get( int id )
{
	return hooks[id];
}

bool HookManager::GetHookStatus( int id )
{
	return Get( id )->status;
}

bool HookManager::InstallMF( HookMF * pHook )
{
	void* pTarget = pHook->GetTarget();

	// copy the original bytes from the target
	std::vector<uint8_t>* pBytes = pHook->GetOgBytes();
	pBytes->resize( pHook->GetSize() );
	pMem->Read( pTarget, pBytes->data(), pHook->GetSize() );

	// write jmp @ target to reroute to our hook
	TCodeBuffer cbHook;
	cbHook += '\xE9';
	cbHook += (uintptr_t) pHook->GetLoc() - (uintptr_t) pTarget - 5;

	// fill with nops if need to
	while ( cbHook.Size() < pHook->GetSize() )
		cbHook += '\x90';

	return pMem->Write( pTarget, cbHook.GetData(), cbHook.Size() );
}

void HookManager::UninstallMF( HookMF * pHook )
{
	pMem->Write( pHook->GetTarget(), *pHook->GetOgBytes() );
}

bool HookManager::InstallVMT( HookVMT * pHook )
{
	auto pObject = pHook->GetTarget();
	void* pDest = pHook->GetLoc();
	auto index = pHook->GetFunctionIndex();
	auto pVMT = *(void***) pObject;
	//TMem::Get()->hooks.hooks[&pVMT[index]] = pHook;

	this->Add( pHook->GetID(), pHook );

	void* pFunc = nullptr;
	if ( !TMem::Get()->Read( &pVMT[index], &pFunc ) )
		return false;

	pHook->SetOriginalFunc( pFunc );
	return TMem::Get()->Write( &pVMT[index], &pDest );
}

void HookManager::UninstallVMT( HookVMT * pHook )
{
	auto pObject = pHook->GetTarget();
	auto pDest = pHook->GetOriginalFunc();
	auto index = pHook->GetFunctionIndex();
	auto pVMT = *(void***) pObject;
	TMem::Get()->Write( &pVMT[index], &pDest );
	this->hooks[pHook->id] = nullptr;
}

bool HookManager::InstallTH( HookTH * pHook )
{
	void* pTarget = pHook->GetTarget();
	void* pTrampLoc = nullptr;
	void* pLoc = reinterpret_cast<void*>((uintptr_t) pTarget - 0x2500);

	while ( !pTrampLoc )
	{
		pTrampLoc = VirtualAlloc( pLoc, 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		pLoc = reinterpret_cast<void*>((uintptr_t) pLoc + 0x200);
	}
	pHook->SetTrampLoc( pTrampLoc );

	// copy original bytes
	auto pMem = TMem::Get();
	auto pOgBytes = pHook->GetOgBytes();
	pOgBytes->resize( pHook->GetSize() );
	if ( !pMem->Read<uint8_t>( pTarget, pOgBytes->data(), pHook->GetSize() ) )
		return false;

	// write trampoline
	// Let's just get the code buffer from the user so they can take care of any specifics
	TCodeBuffer& cb = pHook->GetTrampolineBuffer();

	if ( !cb.Size() )
	{
		cb += *pOgBytes;
		cb += '\xE9';
		cb += (uintptr_t) pTarget + pOgBytes->size() - ((uintptr_t) pTrampLoc + pOgBytes->size() + 5);
		cb += '\xE9';
		cb += (uintptr_t) pHook->GetLoc() - ((uintptr_t) pTrampLoc + pOgBytes->size() + 5) - 5;
	}

	if ( !pMem->Write( pTrampLoc, cb.GetData(), cb.Size() ) )
		return false;

	// write nops to target
	pMem->Nop( pTarget, pHook->GetSize() );

	// write hook @ target to reroute to trampoline
	TCodeBuffer cbHook;
	cbHook += '\xE9';
	cbHook += (uintptr_t) pHook->GetLoc() - (uintptr_t) pTarget - 5;

	return pMem->Write( pTarget, cbHook.GetData(), cbHook.Size() );
}

void HookManager::UninstallTH( HookTH * pHook )
{
	TMem::Get()->Write( pHook->GetTarget(), pHook->GetOgBytes()->data(), pHook->GetOgBytes()->size() );
	VirtualFree( pHook->GetTrampLoc(), 1, MEM_FREE );
}

bool HookManager::InstallPG( HookPG * pHook )
{
	//pHook->hHandler = NT::pRtlAddVectoredExceptionHandler( 0, pHook->GetHandler() );
	pHook->hHandler = NT::AddVEH( 1, pHook->GetHandler() );
	DWORD dwOld = 0;
	MEMORY_BASIC_INFORMATION mbi;
	NT::QueryVirtualMemory( pHook->GetTarget(), &mbi );
	pHook->dwProtect = mbi.Protect;

	if ( !NT::ProtectVirtualMemory( pHook->GetTarget(), mbi.Protect | PAGE_GUARD, &dwOld ) )
		return false;

	return true;
}

void HookManager::UninstallPG( HookPG * pHook )
{
	NT::RemoveVEH( pHook->hHandler );
}

bool HookManager::InstallIAT( HookIAT * pHook )
{
	auto pFuncAddress = NT::GetProcAddressIAT( pHook->GetFuncName() );
	pHook->SetIatEntry( pFuncAddress );

	auto vBytes = pHook->GetOgBytes();
	pHook->SetTarget( *(void**) pFuncAddress );
	vBytes->resize( sizeof( uintptr_t ) );
	if ( !pMem->Read( pFuncAddress, vBytes->data(), sizeof( uintptr_t ) ) )
		return false;

	if ( !pMem->Write( pFuncAddress, pHook->GetLoc() ) )
		return false;

	return true;
}

void HookManager::UninstallIAT( HookIAT * pHook )
{
	auto pIatEntry = pHook->GetIatEntry();
	auto pBytes = pHook->GetOgBytes();
	pMem->Write( pIatEntry, pBytes->data(), pBytes->size() );
}

#pragma endregion

TMem* TMem::pMem = nullptr;
TMem::TMem()
{
}

TMem::~TMem()
{
}

TMem * TMem::Get()
{
	if ( !pMem )
	{
		pMem = new TMem;
		NT::InitNT();
	}
	return pMem;
}

void TMem::Destroy()
{
	delete pMem;
}

bool TMem::Nop( void * pAddress, size_t size )
{
	std::vector<byte> nops( size, 0x90 );
	bool bStatus = Write( pAddress, nops.data(), size );
	return bStatus;
}

void * TMem::ScanPattern( const char * sPattern )
{
	TPattern pattern( sPattern );
	return ScanPattern( pattern );
}

void * TMem::ScanPattern( const char * sPattern, const char * sMask )
{
	TPattern pattern( sPattern, sMask );
	return ScanPattern( pattern );
}

void * TMem::ScanPattern( HMODULE hModule, const char * sPattern )
{
	TPattern pattern( sPattern );
	return ScanPattern( hModule, pattern );
}

void * TMem::ScanPattern( HMODULE hModule, const char * sPattern, const char * sMask )
{
	TPattern pattern( sPattern, sMask );
	return ScanPattern( hModule, pattern );
}

void * TMem::ScanPattern( TPattern & pattern )
{
	HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId() );
	HMODULE hMod = 0;
	MODULEENTRY32 me32{ 0 };
	me32.dwSize = sizeof( me32 );
	void* pResult = nullptr;
	if ( Module32First( hSnap, &me32 ) )
	{
		hMod = me32.hModule;
		pResult = ScanPattern( hMod, pattern );
		if ( pResult )
		{
			CloseHandle( hSnap );
			return pResult;
		}
		while ( Module32Next( hSnap, &me32 ) )
		{
			hMod = me32.hModule;
			pResult = ScanPattern( hMod, pattern );
			if ( pResult )
			{
				CloseHandle( hSnap );
				return pResult;
			}
		}
	}

	CloseHandle( hSnap );
	return nullptr;
}

void * TMem::ScanPattern( HMODULE hModule, TPattern & pattern )
{
	void* pResult = nullptr;

	MODULEINFO mi{ 0 };
	if ( !GetModuleInformation( GetCurrentProcess(), hModule, &mi, sizeof( mi ) ) )
	{
		SetWindowsError();
		return nullptr;
	}

	void* pAddress = hModule;
	MEMORY_BASIC_INFORMATION mbi{ 0 };
	NT::QueryVirtualMemory( pAddress, &mbi );

	while ( (uintptr_t) pAddress < (uintptr_t) hModule + mi.SizeOfImage )
	{

		if ( mbi.State == MEM_COMMIT )
		{
			bool res = !(mbi.Protect & PAGE_GUARD);
			if ( res )
			{
				pResult = Scan( pAddress, mbi.RegionSize, pattern );
			}
		}
		if ( pResult )
			return pResult;
		pAddress = (void*) ((uintptr_t) pAddress + mbi.RegionSize);
		NT::QueryVirtualMemory( pAddress, &mbi );
	}

	return nullptr;
}

int TMem::ScanPatternMulti( const char * sPattern, std::vector<void*>& vResults )
{
	TPattern pattern( sPattern );
	return ScanPatternMulti( pattern, vResults );
}

int TMem::ScanPatternMulti( const char * sPattern, const char * sMask, std::vector<void*>& vResults )
{
	TPattern pattern( sPattern, sMask );
	return ScanPatternMulti( pattern, vResults );
}

int TMem::ScanPatternMulti( HMODULE hModule, const char * sPattern, std::vector<void*>& vResults )
{
	TPattern pattern( sPattern );
	return ScanPatternMulti( hModule, pattern, vResults );
}

int TMem::ScanPatternMulti( HMODULE hModule, const char * sPattern, const char * sMask, std::vector<void*>& vResults )
{
	TPattern pattern( sPattern, sMask );
	return ScanPatternMulti( hModule, pattern, vResults );
}

int TMem::ScanPatternMulti( TPattern & pattern, std::vector<void*> & vResults )
{
	HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId() );
	HMODULE hMod = 0;
	MODULEENTRY32 me32{ 0 };
	me32.dwSize = sizeof( me32 );

	if ( Module32First( hSnap, &me32 ) )
	{
		hMod = me32.hModule;
		ScanPatternMulti( hMod, pattern, vResults );

		while ( Module32Next( hSnap, &me32 ) )
		{
			hMod = me32.hModule;
			ScanPatternMulti( hMod, pattern, vResults );
		}
	}

	CloseHandle( hSnap );

	return (int) vResults.size();
}

int TMem::ScanPatternMulti( HMODULE hModule, TPattern & pattern, std::vector<void*>& vResults )
{
	void* pResult = nullptr;
	auto patSize = pattern.Size();
	MODULEINFO mi{ 0 };
	if ( !GetModuleInformation( GetCurrentProcess(), hModule, &mi, sizeof( mi ) ) )
	{
		SetWindowsError();
		return 0;
	}

	void* pAddress = hModule;
	MEMORY_BASIC_INFORMATION mbi{ 0 };
	NT::QueryVirtualMemory( pAddress, &mbi );
	while ( (uintptr_t) pAddress < (uintptr_t) hModule + mi.SizeOfImage )
	{
		size_t size = mbi.RegionSize;
		if ( mbi.State == MEM_COMMIT )
		{
			bool res = !(mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS));
			if ( res )
			{
				do
				{
					pResult = Scan( pAddress, size, pattern );

					if ( pResult )
					{
						vResults.push_back( pResult );
						size -= ((size_t) pResult - (size_t) pAddress + patSize);
						pAddress = (void*) ((uintptr_t) pResult + patSize);
					}

				} while ( pResult );
			}
		}

		pAddress = (void*) ((uintptr_t) pAddress + size);
		NT::QueryVirtualMemory( pAddress, &mbi );
	}
	return (int) vResults.size();
}

void * TMem::Scan( void * pAddress, size_t size, TPattern & pattern )
{
	auto pat = pattern.Get();
	auto mask = pattern.GetMask();
	uint8_t* pByte = (uint8_t*) pAddress;
	size_t j = 0;
	for ( size_t i = 0; i < size; i++ )
	{
		if ( pByte[i] == pat[j] || mask[j] == '?' )
		{
			j++;
			if ( j == pat.size() - 1 )
				return (void*) ((uintptr_t) pByte + i - pat.size() + 2);
		}
		else
			j = 0;
	}

	return nullptr;
}

void TSyscall::Destroy()
{
	SIZE_T szRegion = 0x1000;
	ScFreeVirtualMemory( HANDLE( -1 ), &pCodeLoc, &szRegion, MEM_FREE );
}

TCodeInj::TCodeInj( void * pAddress, size_t size, TCodeBuffer & code )
{
	this->pAddress = pAddress;
	this->szTarget = size;
	this->cbCode = code;
}

TCodeInj::~TCodeInj()
{
	if ( bEnabled )
		Toggle();
}

void TCodeInj::Toggle()
{
	auto pMem = TMem::Get();
	if ( !pCode )
	{
		SIZE_T szRegion = 0x1000;
		ScAllocateVirtualMemory( HANDLE( -1 ), &pCode, 0, &szRegion, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		auto code = cbCode.Get( (uintptr_t) pCode );
		pMem->Write( pCode, code.data(), code.size() );

		vOgBytes.resize( szTarget );
		pMem->Read( pAddress, vOgBytes.data(), szTarget );
	}
	if ( !bEnabled )
	{
		bEnabled = true;
		TCodeBuffer cbJmp = { (BYTE) 0xE9 };
		cbJmp += (uintptr_t) pCode - (uintptr_t) pAddress - 5;
		pMem->Write( pAddress, cbJmp.GetData(), cbJmp.Size() );
	}
	else
	{
		bEnabled = false;
		pMem->Write( pAddress, vOgBytes.data(), vOgBytes.size() );
	}
}

void TInjManager::AddInjection( void * pAddress, size_t size, TCodeBuffer & code )
{
	this->vInjs.push_back( new TCodeInj( pAddress, size, code ) );
}

void TInjManager::ToggleAll()
{
	for ( auto p : vInjs )
		p->Toggle();
}

void TInjManager::Destroy()
{
	for ( auto p : vInjs )
		delete p;
}
