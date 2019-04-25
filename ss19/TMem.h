#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <tchar.h>
#include <functional>
#include <vector>
#include <map>
#include <algorithm>

//#define TMEM_USE_XORSTR
#ifdef TMEM_USE_XORSTR
#include "xorstr.hpp"
#define _X(s) xorstr_(s)
#else
#define _X(s) (s)
#endif

#define reca(t, p) reinterpret_cast<t>(p)

using wstring = std::wstring;
using string = std::string;
#ifdef _UNICODE
using tstring = wstring;
#else
using tstring = string;
#endif // _UNICODE

#define RVAtoVA(pBase, iRVA) ((uintptr_t)pBase + (uintptr_t)iRVA)

namespace TError
{
	extern tstring sLastError;
	enum LastErrorType
	{
		WINDOWS,
		OTHER
	};
	extern LastErrorType letLastErrorType;

	void SetLastError( LastErrorType let, const tstring & szError );
	tstring GetLastErrorStr();
}
#define SetWindowsError() TError::SetLastError(TError::WINDOWS, _T(""))
#define SetCustomError(s) TError::SetLastError(TError::OTHER, _T(s))

#pragma region NT

#ifdef _WIN64
#define Xip Rip
#else
#define Xip Eip
#endif

#pragma region NT_Types

typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_LDR_DATA {
	BYTE Reserved1[8];
	PVOID Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef
VOID
( NTAPI *PPS_POST_PROCESS_INIT_ROUTINE ) (
	VOID
	);

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID Reserved4[3];
	PVOID AtlThunkSListPtr;
	PVOID Reserved5;
	ULONG Reserved6;
	PVOID Reserved7;
	ULONG Reserved8;
	ULONG AtlThunkSListPtr32;
	PVOID Reserved9[45];
	BYTE Reserved10[96];
	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE Reserved11[128];
	PVOID Reserved12[1];
	ULONG SessionId;
} PEB, *PPEB;

typedef struct _TEB {
	PVOID Reserved1[12];
	PPEB ProcessEnvironmentBlock;
	PVOID Reserved2[399];
	BYTE Reserved3[1952];
	PVOID TlsSlots[64];
	BYTE Reserved4[8];
	PVOID Reserved5[26];
	PVOID ReservedForOle;  // Windows 2000 only
	PVOID Reserved6[4];
	PVOID TlsExpansionSlots;
} TEB, *PTEB;

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27,
	ProcessBreakOnTermination = 29
} PROCESSINFOCLASS;

typedef enum _THREAD_INFO_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} THREAD_INFO_CLASS, *PTHREAD_INFO_CLASS;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PPEB PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef unsigned short RTL_ATOM, *PRTL_ATOM;
typedef struct atom_table *RTL_ATOM_TABLE, **PRTL_ATOM_TABLE;

typedef enum _KEY_INFORMATION_CLASS {
	KeyBasicInformation,
	KeyNodeInformation,
	KeyFullInformation,
	KeyNameInformation,
	KeyCachedInformation,
	KeyFlagsInformation,
	KeyVirtualizationInformation,
	KeyHandleTagsInformation,
	KeyTrustInformation,
	KeyLayerInformation,
	MaxKeyInfoClass
} KEY_INFORMATION_CLASS;

typedef enum _KEY_VALUE_INFORMATION_CLASS {
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
	KeyValueFullInformationAlign64,
	KeyValuePartialInformationAlign64,
	KeyValueLayerInformation,
	MaxKeyValueInfoClass
} KEY_VALUE_INFORMATION_CLASS;

typedef enum _KPROFILE_SOURCE {
	ProfileTime,
	ProfileAlignmentFixup,
	ProfileTotalIssues,
	ProfilePipelineDry,
	ProfileLoadInstructions,
	ProfilePipelineFrozen,
	ProfileBranchInstructions,
	ProfileTotalNonissues,
	ProfileDcacheMisses,
	ProfileIcacheMisses,
	ProfileCacheMisses,
	ProfileBranchMispredictions,
	ProfileStoreInstructions,
	ProfileFpInstructions,
	ProfileIntegerInstructions,
	Profile2Issue,
	Profile3Issue,
	Profile4Issue,
	ProfileSpecialInstructions,
	ProfileTotalCycles,
	ProfileIcacheIssues,
	ProfileDcacheAccesses,
	ProfileMemoryBarrierCycles,
	ProfileLoadLinkedIssues,
	ProfileMaximum
} KPROFILE_SOURCE, *PKPROFILE_SOURCE;

typedef enum _MUTANT_INFORMATION_CLASS
{
	MutantBasicInformation
} MUTANT_INFORMATION_CLASS, *PMUTANT_INFORMATION_CLASS;

typedef struct _INITIAL_TEB {
	PVOID                StackBase;
	PVOID                StackLimit;
	PVOID                StackCommit;
	PVOID                StackCommitMax;
	PVOID                StackReserved;
} INITIAL_TEB, *PINITIAL_TEB;

typedef enum _SECTION_INHERIT {
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT, *PSECTION_INHERIT;

typedef struct _FILE_BASIC_INFORMATION {
	LARGE_INTEGER           CreationTime;
	LARGE_INTEGER           LastAccessTime;
	LARGE_INTEGER           LastWriteTime;
	LARGE_INTEGER           ChangeTime;
	ULONG                   FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _OBJDIR_INFORMATION {
	UNICODE_STRING          ObjectName;
	UNICODE_STRING          ObjectTypeName;
	BYTE                    Data[1];
} OBJDIR_INFORMATION, *POBJDIR_INFORMATION;

typedef enum _EVENT_INFORMATION_CLASS {
	EventBasicInformation
} EVENT_INFORMATION_CLASS, *PEVENT_INFORMATION_CLASS;

typedef enum _ATOM_INFORMATION_CLASS {
	AtomBasicInformation,
	AtomTableInformation
} ATOM_INFORMATION_CLASS, *PATOM_INFORMATION_CLASS;

typedef enum _PORT_INFORMATION_CLASS {
	PortNoInformation
} PORT_INFORMATION_CLASS, *PPORT_INFORMATION_CLASS;

typedef enum _IO_COMPLETION_INFORMATION_CLASS {
	IoCompletionBasicInformation
} IO_COMPLETION_INFORMATION_CLASS, *PIO_COMPLETION_INFORMATION_CLASS;

typedef struct _KEY_MULTIPLE_VALUE_INFORMATION {
	PUNICODE_STRING         ValueName;
	ULONG                   DataLength;
	ULONG                   DataOffset;
	ULONG                   Type;
} KEY_MULTIPLE_VALUE_INFORMATION, *PKEY_MULTIPLE_VALUE_INFORMATION;

typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation,
	SectionImageInformation
} SECTION_INFORMATION_CLASS, *PSECTION_INFORMATION_CLASS;

typedef enum _SEMAPHORE_INFORMATION_CLASS {
	SemaphoreBasicInformation
} SEMAPHORE_INFORMATION_CLASS, *PSEMAPHORE_INFORMATION_CLASS;

typedef enum _FS_INFORMATION_CLASS {
	FileFsVolumeInformation = 1,
	FileFsLabelInformation,
	FileFsSizeInformation,
	FileFsDeviceInformation,
	FileFsAttributeInformation,
	FileFsControlInformation,
	FileFsFullSizeInformation,
	FileFsObjectIdInformation,
	FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef enum _TIMER_INFORMATION_CLASS {
	TimerBasicInformation
} TIMER_INFORMATION_CLASS, *PTIMER_INFORMATION_CLASS;

typedef enum _HARDERROR_RESPONSE_OPTION {
	OptionAbortRetryIgnore,
	OptionOk,
	OptionOkCancel,
	OptionRetryCancel,
	OptionYesNo,
	OptionYesNoCancel,
	OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
	ResponseReturnToCaller,
	ResponseNotHandled,
	ResponseAbort,
	ResponseCancel,
	ResponseIgnore,
	ResponseNo,
	ResponseOk,
	ResponseRetry,
	ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

typedef void( *PTIMER_APC_ROUTINE )(IN PVOID TimerContext, IN ULONG TimerLowValue, IN LONG TimerHighValue);

typedef enum _SHUTDOWN_ACTION {
	ShutdownNoReboot,
	ShutdownReboot,
	ShutdownPowerOff
} SHUTDOWN_ACTION, *PSHUTDOWN_ACTION;

typedef enum _SYSDBG_COMMAND {
	SysDbgQueryModuleInformation = 1,
	SysDbgQueryTraceInformation,
	SysDbgSetTracepoint,
	SysDbgSetSpecialCall,
	SysDbgClearSpecialCalls,
	SysDbgQuerySpecialCalls
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;

typedef enum _OBJECT_WAIT_TYPE {
	WaitAllObject,
	WaitAnyObject
} OBJECT_WAIT_TYPE, *POBJECT_WAIT_TYPE;

typedef enum _TIMER_TYPE {
	NotificationTimer,
	SynchronizationTimer
} TIMER_TYPE;

typedef enum _FILE_INFORMATION_CLASS {
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileCopyOnWriteInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileObjectIdInformation,
	FileTrackingInformation,
	FileOleDirectoryInformation,
	FileContentIndexInformation,
	FileInheritContentIndexInformation,
	FileOleInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllInformation,
	ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemNextEventIdInformation,
	SystemEventIdsInformation,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemPlugPlayBusInformation,
	SystemDockInformation,
	SystemPowerInfo,
	SystemProcessorSpeedInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

typedef enum _KEY_SET_INFORMATION_CLASS {
	KeyWriteTimeInformation,
	KeyWow64FlagsInformation,
	KeyControlFlagsInformation,
	KeySetVirtualizationInformation,
	KeySetDebugInformation,
	KeySetHandleTagsInformation,
	KeySetLayerInformation,
	MaxKeySetInfoClass  // MaxKeySetInfoClass should always be the last enum
} KEY_SET_INFORMATION_CLASS;

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID* PCLIENT_ID;

typedef DWORD EVENT_TYPE;
typedef void* PLPC_MESSAGE;
typedef void* PLPC_SECTION_MEMORY;
typedef void* PLPC_SECTION_OWNER_MEMORY;
typedef void* POBJECT_ATTRIBUTES;
typedef void* PIO_STATUS_BLOCK;
typedef void* PIO_APC_ROUTINE;

#ifdef NTAPI
#undef NTAPI
#endif

#ifdef _WIN64
#define NTAPI __fastcall
#else
#define NTAPI __stdcall
#endif

#pragma endregion

#pragma region NtFuncTypes

using fnNtAcceptConnectPort = NTSTATUS( NTAPI* )(OUT PHANDLE ServerPortHandle, IN HANDLE AlternativeReceivePortHandle, IN PLPC_MESSAGE ConnectionReply, IN BOOLEAN AcceptConnection, IN OUT PLPC_SECTION_OWNER_MEMORY ServerSharedMemory, OUT PLPC_SECTION_MEMORY ClientSharedMemory);
using fnNtAccessCheck = NTSTATUS( NTAPI* )(IN PSECURITY_DESCRIPTOR SecurityDescriptor, IN HANDLE ClientToken, IN ACCESS_MASK DesiredAccess, IN PGENERIC_MAPPING GenericMapping, OUT PPRIVILEGE_SET RequiredPrivilegesBuffer, IN OUT PULONG BufferLength, OUT PACCESS_MASK GrantedAccess, OUT PNTSTATUS AccessStatus);
using fnNtAccessCheckAndAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN HANDLE ObjectHandle, IN PUNICODE_STRING ObjectTypeName, IN PUNICODE_STRING ObjectName, IN PSECURITY_DESCRIPTOR SecurityDescriptor, IN ACCESS_MASK DesiredAccess, IN PGENERIC_MAPPING GenericMapping, IN BOOLEAN ObjectCreation, OUT PULONG GrantedAccess, OUT PULONG AccessStatus, OUT PBOOLEAN GenerateOnClose);
using fnNtAddAtom = NTSTATUS( NTAPI* )(IN PWCHAR AtomName, OUT PRTL_ATOM Atom);
using fnNtAdjustGroupsToken = NTSTATUS( NTAPI* )(IN HANDLE TokenHandle, IN BOOLEAN ResetToDefault, IN PTOKEN_GROUPS TokenGroups, IN ULONG PreviousGroupsLength, OUT PTOKEN_GROUPS PreviousGroups, OUT PULONG RequiredLength);
using fnNtAdjustPrivilegesToken = NTSTATUS( NTAPI* )(IN HANDLE TokenHandle, IN BOOLEAN DisableAllPrivileges, IN PTOKEN_PRIVILEGES TokenPrivileges, IN ULONG PreviousPrivilegesLength, OUT PTOKEN_PRIVILEGES PreviousPrivileges, OUT PULONG RequiredLength);
using fnNtAlertResumeThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, OUT PULONG SuspendCount);
using fnNtAlertThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle);
using fnNtAllocateLocallyUniqueId = NTSTATUS( NTAPI* )(OUT PLUID LocallyUniqueId);
using fnNtAllocateUuids = NTSTATUS( NTAPI* )(OUT PLARGE_INTEGER Time, OUT PULONG Range, OUT PULONG Sequence);
using fnNtAllocateVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress, IN PULONG ZeroBits, IN OUT PSIZE_T RegionSize, IN ULONG AllocationType, IN ULONG Protect);
using fnNtCallbackReturn = NTSTATUS( NTAPI* )(IN PVOID Result, IN ULONG ResultLength, IN NTSTATUS Status);
using fnNtCancelIoFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock);
using fnNtCancelTimer = NTSTATUS( NTAPI* )(IN HANDLE TimerHandle, OUT PBOOLEAN CurrentState);
using fnNtClearEvent = NTSTATUS( NTAPI* )(IN HANDLE EventHandle);
using fnNtClose = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle);
using fnNtCloseObjectAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN HANDLE ObjectHandle, IN BOOLEAN GenerateOnClose);
using fnNtCompactKeys = NTSTATUS( NTAPI* )(IN ULONG NrOfKeys, IN HANDLE KeysArray[]);
using fnNtCompleteConnectPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle);
using fnNtCompressKey = NTSTATUS( NTAPI* )(IN HANDLE Key);
using fnNtConnectPort = NTSTATUS( NTAPI* )(OUT PHANDLE ClientPortHandle, IN PUNICODE_STRING ServerPortName, IN PSECURITY_QUALITY_OF_SERVICE SecurityQos, IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory, OUT PLPC_SECTION_MEMORY ServerSharedMemory, OUT PULONG MaximumMessageLength, IN void* ConnectionInfo, IN PULONG ConnectionInfoLength);
using fnNtContinue = NTSTATUS( NTAPI* )(IN PCONTEXT ThreadContext, IN BOOLEAN RaiseAlert);
using fnNtCreateDebugObject = NTSTATUS( NTAPI* )(OUT PHANDLE DebugObjectHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN BOOLEAN KillProcessOnExit);
using fnNtCreateDirectoryObject = NTSTATUS( NTAPI* )(OUT PHANDLE DirectoryHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtCreateEvent = NTSTATUS( NTAPI* )(OUT PHANDLE EventHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN EVENT_TYPE EventType, IN BOOLEAN InitialState);
using fnNtCreateEventPair = NTSTATUS( NTAPI* )(OUT PHANDLE EventPairHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtCreateFile = NTSTATUS( NTAPI* )(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PLARGE_INTEGER AllocationSize, IN ULONG FileAttributes, IN ULONG ShareAccess, IN ULONG CreateDisposition, IN ULONG CreateOptions, IN PVOID EaBuffer, IN ULONG EaLength);
using fnNtCreateIoCompletion = NTSTATUS( NTAPI* )(OUT PHANDLE IoCompletionHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG NumberOfConcurrentThreads);
using fnNtCreateKey = NTSTATUS( NTAPI* )(OUT PHANDLE pKeyHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG TitleIndex, IN PUNICODE_STRING Class, IN ULONG CreateOptions, OUT PULONG Disposition);
using fnNtCreateKeyedEvent = NTSTATUS( NTAPI* )(OUT PHANDLE KeyedEventHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG Reserved);
using fnNtCreateMailslotFile = NTSTATUS( NTAPI* )(OUT PHANDLE MailslotFileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG CreateOptions, IN ULONG MailslotQuota, IN ULONG MaxMessageSize, IN PLARGE_INTEGER ReadTimeOut);
using fnNtCreateMutant = NTSTATUS( NTAPI* )(OUT PHANDLE MutantHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN BOOLEAN InitialOwner);
using fnNtCreateNamedPipeFile = NTSTATUS( NTAPI* )(OUT PHANDLE NamedPipeFileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG ShareAccess, IN ULONG CreateDisposition, IN ULONG CreateOptions, IN BOOLEAN WriteModeMessage, IN BOOLEAN ReadModeMessage, IN BOOLEAN NonBlocking, IN ULONG MaxInstances, IN ULONG InBufferSize, IN ULONG OutBufferSize, IN PLARGE_INTEGER DefaultTimeOut);
using fnNtCreatePagingFile = NTSTATUS( NTAPI* )(IN PUNICODE_STRING PageFileName, IN PLARGE_INTEGER MiniumSize, IN PLARGE_INTEGER MaxiumSize, OUT PLARGE_INTEGER ActualSize);
using fnNtCreatePort = NTSTATUS( NTAPI* )(OUT PHANDLE PortHandle, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG MaxConnectInfoLength, IN ULONG MaxDataLength, IN OUT PULONG Reserved);
using fnNtCreateProcess = NTSTATUS( NTAPI* )(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ParentProcess, IN BOOLEAN InheritObjectTable, IN HANDLE SectionHandle, IN HANDLE DebugPort, IN HANDLE ExceptionPort);
using fnNtCreateProfile = NTSTATUS( NTAPI* )(OUT PHANDLE ProfileHandle, IN HANDLE Process, IN PVOID ImageBase, IN ULONG ImageSize, IN ULONG BucketSize, IN PVOID Buffer, IN ULONG BufferSize, IN KPROFILE_SOURCE ProfileSource, IN KAFFINITY Affinity);
using fnNtCreateSection = NTSTATUS( NTAPI* )(OUT PHANDLE SectionHandle, IN ULONG DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PLARGE_INTEGER MaximumSize, IN ULONG PageAttributess, IN ULONG SectionAttributes, IN HANDLE FileHandle);
using fnNtCreateSemaphore = NTSTATUS( NTAPI* )(OUT PHANDLE SemaphoreHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG InitialCount, IN ULONG MaximumCount);
using fnNtCreateSymbolicLinkObject = NTSTATUS( NTAPI* )(OUT PHANDLE pHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PUNICODE_STRING DestinationName);
using fnNtCreateThread = NTSTATUS( NTAPI* )(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle, OUT PCLIENT_ID ClientId, IN PCONTEXT ThreadContext, IN PINITIAL_TEB InitialTeb, IN BOOLEAN CreateSuspended);
using fnNtCreateThreadEx = NTSTATUS( NTAPI* )(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN HANDLE ProcessHandle, IN PTHREAD_START_ROUTINE ThreadStartRoutine, IN PVOID ThreadArguments, IN ULONG Flags, IN SIZE_T ZeroBits, IN SIZE_T StackSize, IN SIZE_T MaxStackSize, OUT PVOID pAttrListOut);
using fnNtCreateTimer = NTSTATUS( NTAPI* )(OUT PHANDLE TimerHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN TIMER_TYPE TimerType);
using fnNtCreateToken = NTSTATUS( NTAPI* )(OUT PHANDLE TokenHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN TOKEN_TYPE TokenType, IN PLUID AuthenticationId, IN PLARGE_INTEGER ExpirationTime, IN PTOKEN_USER TokenUser, IN PTOKEN_GROUPS TokenGroups, IN PTOKEN_PRIVILEGES TokenPrivileges, IN PTOKEN_OWNER TokenOwner, IN PTOKEN_PRIMARY_GROUP TokenPrimaryGroup, IN PTOKEN_DEFAULT_DACL TokenDefaultDacl, IN PTOKEN_SOURCE TokenSource);
using fnNtDebugActiveProcess = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN HANDLE DebugObjectHandle);
using fnNtDelayExecution = NTSTATUS( NTAPI* )(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval);
using fnNtDeleteAtom = NTSTATUS( NTAPI* )(IN RTL_ATOM Atom);
using fnNtDeleteFile = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtDeleteKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle);
using fnNtDeleteObjectAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN HANDLE ObjectHandle, IN BOOLEAN GenerateOnClose);
using fnNtDeleteValueKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName);
using fnNtDeviceIoControlFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG IoControlCode, IN PVOID InputBuffer, IN ULONG InputBufferLength, OUT PVOID OutputBuffer, IN ULONG OutputBufferLength);
using fnNtDisplayString = NTSTATUS( NTAPI* )(IN PUNICODE_STRING String);
using fnNtDuplicateObject = NTSTATUS( NTAPI* )(IN HANDLE SourceProcessHandle, IN PHANDLE SourceHandle, IN HANDLE TargetProcessHandle, OUT PHANDLE TargetHandle, IN ACCESS_MASK DesiredAccess, IN BOOLEAN InheritHandle, IN ULONG Options);
using fnNtDuplicateToken = NTSTATUS( NTAPI* )(IN HANDLE ExistingToken, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN SECURITY_IMPERSONATION_LEVEL ImpersonationLevel, IN TOKEN_TYPE TokenType, OUT PHANDLE NewToken);
using fnNtEnumerateKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN ULONG Index, IN KEY_INFORMATION_CLASS KeyInformationClass, OUT PVOID KeyInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtEnumerateValueKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN ULONG Index, IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, OUT PVOID KeyValueInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtExtendSection = NTSTATUS( NTAPI* )(IN HANDLE SectionHandle, IN PLARGE_INTEGER NewSectionSize);
using fnNtFindAtom = NTSTATUS( NTAPI* )(IN PWCHAR AtomName, OUT PRTL_ATOM Atom);
using fnNtFlushBuffersFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock);
using fnNtFlushInstructionCache = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID BaseAddress, IN ULONG NumberOfBytesToFlush);
using fnNtFlushKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle);
using fnNtFlushVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress, IN OUT PULONG NumberOfBytesToFlush, OUT PIO_STATUS_BLOCK IoStatusBlock);
using fnNtFlushWriteBuffer = NTSTATUS( NTAPI* )(VOID);
using fnNtFreeVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID *BaseAddress, IN OUT PSIZE_T RegionSize, IN ULONG FreeType);
using fnNtFsControlFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG FsControlCode, IN PVOID InputBuffer, IN ULONG InputBufferLength, OUT PVOID OutputBuffer, IN ULONG OutputBufferLength);
using fnNtGetContextThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, OUT PCONTEXT pContext);
using fnNtImpersonateClientOfPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Request);
using fnNtImpersonateThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN HANDLE ThreadToImpersonate, IN PSECURITY_QUALITY_OF_SERVICE SecurityQualityOfService);
using fnNtListenPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, OUT PLPC_MESSAGE ConnectionRequest);
using fnNtLoadDriver = NTSTATUS( NTAPI* )(IN PUNICODE_STRING DriverServiceName);
using fnNtLoadKey = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES DestinationKeyName, IN POBJECT_ATTRIBUTES HiveFileName);
using fnNtLoadKey2 = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES DestinationKeyName, IN POBJECT_ATTRIBUTES HiveFileName, IN ULONG Flags);
using fnNtLockFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE LockGrantedEvent, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PLARGE_INTEGER ByteOffset, IN PLARGE_INTEGER Length, IN PULONG Key, IN BOOLEAN ReturnImmediately, IN BOOLEAN ExclusiveLock);
using fnNtLockVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID *BaseAddress, IN OUT PULONG NumberOfBytesToLock, IN ULONG LockOption);
using fnNtMakeTemporaryObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle);
using fnNtMapViewOfSection = NTSTATUS( NTAPI* )(IN HANDLE SectionHandle, IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress, IN ULONG ZeroBits, IN ULONG CommitSize, IN OUT PLARGE_INTEGER SectionOffset, IN OUT PULONG ViewSize, IN SECTION_INHERIT InheritDisposition, IN ULONG AllocationType, IN ULONG Protect);
using fnNtNotifyChangeDirectoryFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID Buffer, IN ULONG BufferSize, IN ULONG CompletionFilter, IN BOOLEAN WatchTree);
using fnNtNotifyChangeKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN HANDLE EventHandle, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcRoutineContext, IN PIO_STATUS_BLOCK IoStatusBlock, IN ULONG NotifyFilter, IN BOOLEAN WatchSubtree, OUT PVOID RegChangesDataBuffer, IN ULONG RegChangesDataBufferLength, IN BOOLEAN Asynchronous);
using fnNtOpenDirectoryObject = NTSTATUS( NTAPI* )(OUT PHANDLE DirectoryObjectHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenEvent = NTSTATUS( NTAPI* )(OUT PHANDLE EventHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenEventPair = NTSTATUS( NTAPI* )(OUT PHANDLE EventPairHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenFile = NTSTATUS( NTAPI* )(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG ShareAccess, IN ULONG OpenOptions);
using fnNtOpenIoCompletion = NTSTATUS( NTAPI* )(OUT PHANDLE IoCompletionHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenKey = NTSTATUS( NTAPI* )(OUT PHANDLE pKeyHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenKeyedEvent = NTSTATUS( NTAPI* )(OUT PHANDLE KeyedEventHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenMutant = NTSTATUS( NTAPI* )(OUT PHANDLE MutantHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenObjectAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN PHANDLE ObjectHandle, IN PUNICODE_STRING ObjectTypeName, IN PUNICODE_STRING ObjectName, IN PSECURITY_DESCRIPTOR SecurityDescriptor, IN HANDLE ClientToken, IN ACCESS_MASK DesiredAccess, IN ACCESS_MASK GrantedAccess, IN PPRIVILEGE_SET Privileges, IN BOOLEAN ObjectCreation, IN BOOLEAN AccessGranted, OUT PBOOLEAN GenerateOnClose);
using fnNtOpenProcess = NTSTATUS( NTAPI* )(OUT PHANDLE ProcessHandle, IN ACCESS_MASK AccessMask, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId);
using fnNtOpenProcessToken = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, OUT PHANDLE TokenHandle);
using fnNtOpenSection = NTSTATUS( NTAPI* )(OUT PHANDLE SectionHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenSemaphore = NTSTATUS( NTAPI* )(OUT PHANDLE SemaphoreHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenSymbolicLinkObject = NTSTATUS( NTAPI* )(OUT PHANDLE pHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtOpenThread = NTSTATUS( NTAPI* )(OUT PHANDLE ThreadHandle, IN ACCESS_MASK AccessMask, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId);
using fnNtOpenThreadToken = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess, IN BOOLEAN OpenAsSelf, OUT PHANDLE TokenHandle);
using fnNtOpenTimer = NTSTATUS( NTAPI* )(OUT PHANDLE TimerHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using fnNtPrivilegeCheck = NTSTATUS( NTAPI* )(IN HANDLE TokenHandle, IN PPRIVILEGE_SET RequiredPrivileges, IN PBOOLEAN Result);
using fnNtPrivilegeObjectAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN HANDLE ObjectHandle, IN HANDLE ClientToken, IN ULONG DesiredAccess, IN PPRIVILEGE_SET ClientPrivileges, IN BOOLEAN AccessGranted);
using fnNtPrivilegedServiceAuditAlarm = NTSTATUS( NTAPI* )(IN PUNICODE_STRING SubsystemName, IN PUNICODE_STRING ServiceName, IN HANDLE ClientToken, IN PPRIVILEGE_SET ClientPrivileges, IN BOOLEAN AccessGranted);
using fnNtProtectVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN OUT PVOID *BaseAddress, IN OUT PSIZE_T NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);
using fnNtPulseEvent = NTSTATUS( NTAPI* )(IN HANDLE EventHandle, OUT PLONG PreviousState);
using fnNtQueryAttributesFile = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PFILE_BASIC_INFORMATION FileAttributes);
using fnNtQueryDefaultLocale = NTSTATUS( NTAPI* )(IN BOOLEAN UserProfile, OUT PLCID DefaultLocaleId);
using fnNtQueryDirectoryFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FileInformation, IN ULONG Length, IN FILE_INFORMATION_CLASS FileInformationClass, IN BOOLEAN ReturnSingleEntry, IN PUNICODE_STRING FileMask, IN BOOLEAN RestartScan);
using fnNtQueryDirectoryObject = NTSTATUS( NTAPI* )(IN HANDLE DirectoryObjectHandle, OUT POBJDIR_INFORMATION DirObjInformation, IN ULONG BufferLength, IN BOOLEAN GetNextIndex, IN BOOLEAN IgnoreInputIndex, IN OUT PULONG ObjectIndex, OUT PULONG DataWritten);
using fnNtQueryEaFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID Buffer, IN ULONG Length, IN BOOLEAN ReturnSingleEntry, IN PVOID EaList, IN ULONG EaListLength, IN PULONG EaIndex, IN BOOLEAN RestartScan);
using fnNtQueryEvent = NTSTATUS( NTAPI* )(IN HANDLE EventHandle, IN EVENT_INFORMATION_CLASS EventInformationClass, OUT PVOID EventInformation, IN ULONG EventInformationLength, OUT PULONG ReturnLength);
using fnNtQueryFullAttributesFile = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PVOID Attributes);
using fnNtQueryInformationAtom = NTSTATUS( NTAPI* )(IN RTL_ATOM Atom, IN ATOM_INFORMATION_CLASS AtomInformationClass, OUT PVOID AtomInformation, IN ULONG AtomInformationLength, OUT PULONG ReturnLength);
using fnNtQueryInformationFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FileInformation, IN ULONG Length, IN FILE_INFORMATION_CLASS FileInformationClass);
using fnNtQueryInformationPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PORT_INFORMATION_CLASS PortInformationClass, OUT PVOID PortInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtQueryInformationProcess = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PROCESS_INFORMATION_CLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength);
using fnNtQueryInformationThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN THREAD_INFO_CLASS ThreadInformationClass, OUT PVOID ThreadInformation, IN ULONG ThreadInformationLength, OUT PULONG ReturnLength);
using fnNtQueryInformationToken = NTSTATUS( NTAPI* )(IN HANDLE TokenHandle, IN TOKEN_INFORMATION_CLASS TokenInformationClass, OUT PVOID TokenInformation, IN ULONG TokenInformationLength, OUT PULONG ReturnLength);
using fnNtQueryIntervalProfile = NTSTATUS( NTAPI* )(IN KPROFILE_SOURCE ProfileSource, OUT PULONG Interval);
using fnNtQueryIoCompletion = NTSTATUS( NTAPI* )(IN HANDLE IoCompletionHandle, IN IO_COMPLETION_INFORMATION_CLASS InformationClass, OUT PVOID IoCompletionInformation, IN ULONG InformationBufferLength, OUT PULONG RequiredLength);
using fnNtQueryKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN KEY_INFORMATION_CLASS KeyInformationClass, OUT PVOID KeyInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtQueryMultipleValueKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN OUT PKEY_MULTIPLE_VALUE_INFORMATION ValuesList, IN ULONG NumberOfValues, OUT PVOID DataBuffer, IN OUT ULONG BufferLength, OUT PULONG RequiredLength);
using fnNtQueryMutant = NTSTATUS( NTAPI* )(IN HANDLE MutantHandle, IN MUTANT_INFORMATION_CLASS MutantInformationClass, OUT PVOID MutantInformation, IN ULONG MutantInformationLength, OUT PULONG ResultLength);
using fnNtQueryObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle, IN OBJECT_INFORMATION_CLASS ObjectInformationClass, OUT PVOID ObjectInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtQueryPerformanceCounter = NTSTATUS( NTAPI* )(OUT PLARGE_INTEGER PerformanceCounter, OUT PLARGE_INTEGER PerformanceFrequency);
using fnNtQuerySection = NTSTATUS( NTAPI* )(IN HANDLE SectionHandle, IN SECTION_INFORMATION_CLASS InformationClass, OUT PVOID InformationBuffer, IN ULONG InformationBufferSize, OUT PULONG ResultLength);
using fnNtQuerySecurityObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle, IN SECURITY_INFORMATION SecurityInformationClass, OUT PSECURITY_DESCRIPTOR DescriptorBuffer, IN ULONG DescriptorBufferLength, OUT PULONG RequiredLength);
using fnNtQuerySemaphore = NTSTATUS( NTAPI* )(IN HANDLE SemaphoreHandle, IN SEMAPHORE_INFORMATION_CLASS SemaphoreInformationClass, OUT PVOID SemaphoreInformation, IN ULONG SemaphoreInformationLength, OUT PULONG ReturnLength);
using fnNtQuerySymbolicLinkObject = NTSTATUS( NTAPI* )(IN HANDLE SymbolicLinkHandle, OUT PUNICODE_STRING pLinkName, OUT PULONG pDataWritten);
using fnNtQuerySystemEnvironmentValue = NTSTATUS( NTAPI* )(IN PUNICODE_STRING VariableName, OUT PWCHAR Value, IN ULONG ValueBufferLength, OUT PULONG RequiredLength);
using fnNtQuerySystemInformation = NTSTATUS( NTAPI* )(IN SYSTEM_INFORMATION_CLASS SystemInformationClass, OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength);
using fnNtQueryTimer = NTSTATUS( NTAPI* )(IN HANDLE TimerHandle, IN TIMER_INFORMATION_CLASS TimerInformationClass, OUT PVOID TimerInformation, IN ULONG TimerInformationLength, OUT PULONG ReturnLength);
using fnNtQueryTimerResolution = NTSTATUS( NTAPI* )(OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);
using fnNtQueryValueKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName, IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, OUT PVOID KeyValueInformation, IN ULONG Length, OUT PULONG ResultLength);
using fnNtQueryVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID BaseAddress, IN MEMORY_INFORMATION_CLASS MemoryInformationClass, OUT PVOID Buffer, IN SIZE_T Length, OUT PSIZE_T ResultLength);
using fnNtQueryVolumeInformationFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID FileSystemInformation, IN ULONG Length, IN FS_INFORMATION_CLASS FileSystemInformationClass);
using fnNtQueueApcThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcRoutineContext, IN PIO_STATUS_BLOCK ApcStatusBlock, IN ULONG ApcReserved);
using fnNtRaiseException = NTSTATUS( NTAPI* )(IN PEXCEPTION_RECORD ExceptionRecord, IN PCONTEXT ThreadContext, IN BOOLEAN HandleException);
using fnNtRaiseHardError = NTSTATUS( NTAPI* )(IN NTSTATUS ErrorStatus, IN ULONG NumberOfParameters, IN PUNICODE_STRING UnicodeStringParameterMask, IN PVOID *Parameters, IN HARDERROR_RESPONSE_OPTION ResponseOption, OUT PHARDERROR_RESPONSE Response);
using fnNtReadFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, OUT PVOID Buffer, IN ULONG Length, IN PLARGE_INTEGER ByteOffset, IN PULONG Key);
using fnNtReadFileScatter = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN FILE_SEGMENT_ELEMENT SegmentArray, IN ULONG Length, IN PLARGE_INTEGER ByteOffset, IN PULONG Key);
using fnNtReadRequestData = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Request, IN ULONG DataIndex, OUT PVOID Buffer, IN ULONG Length, OUT PULONG ResultLength);
using fnNtReadVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID BaseAddress, OUT PVOID Buffer, IN ULONG NumberOfBytesToRead, OUT PULONG NumberOfBytesReaded);
using fnNtRegisterThreadTerminatePort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle);
using fnNtReleaseKeyedEvent = NTSTATUS( NTAPI* )(IN HANDLE KeyedEventHandle, IN PVOID Key, IN BOOLEAN Alertable, IN PLARGE_INTEGER Timeout);
using fnNtReleaseMutant = NTSTATUS( NTAPI* )(IN HANDLE MutantHandle, OUT PLONG PreviousCount);
using fnNtReleaseSemaphore = NTSTATUS( NTAPI* )(IN HANDLE SemaphoreHandle, IN ULONG ReleaseCount, OUT PULONG PreviousCount);
using fnNtRemoveIoCompletion = NTSTATUS( NTAPI* )(IN HANDLE IoCompletionHandle, OUT PULONG CompletionKey, OUT PULONG CompletionValue, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PLARGE_INTEGER Timeout);
using fnNtRemoveProcessDebug = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN HANDLE DebugObjectHandle);
using fnNtReplaceKey = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES NewHiveFileName, IN HANDLE KeyHandle, IN POBJECT_ATTRIBUTES BackupHiveFileName);
using fnNtReplyPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Reply);
using fnNtReplyWaitReceivePort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, OUT PHANDLE ReceivePortHandle, IN PLPC_MESSAGE Reply, OUT PLPC_MESSAGE IncomingRequest);
using fnNtReplyWaitReplyPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN OUT PLPC_MESSAGE Reply);
using fnNtRequestPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Request);
using fnNtRequestWaitReplyPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Request, OUT PLPC_MESSAGE IncomingReply);
using fnNtResetEvent = NTSTATUS( NTAPI* )(IN HANDLE EventHandle, OUT PLONG PreviousState);
using fnNtRestoreKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN HANDLE FileHandle, IN ULONG RestoreOption);
using fnNtResumeThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, OUT PULONG SuspendCount);
using fnNtSaveKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN HANDLE FileHandle);
using fnNtSetContextThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN PCONTEXT Context);
using fnNtSetDefaultHardErrorPort = NTSTATUS( NTAPI* )(IN HANDLE PortHandle);
using fnNtSetDefaultLocale = NTSTATUS( NTAPI* )(IN BOOLEAN UserProfile, IN LCID DefaultLocaleId);
using fnNtSetEaFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID EaBuffer, IN ULONG EaBufferSize);
using fnNtSetEvent = NTSTATUS( NTAPI* )(IN HANDLE EventHandle, OUT PLONG PreviousState);
using fnNtSetEventBoostPriority = NTSTATUS( NTAPI* )(IN HANDLE EventHandle);
using fnNtSetHighEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtSetHighWaitLowEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtSetInformationFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID FileInformation, IN ULONG Length, IN FILE_INFORMATION_CLASS FileInformationClass);
using fnNtSetInformationKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN KEY_SET_INFORMATION_CLASS InformationClass, IN PVOID KeyInformationData, IN ULONG DataLength);
using fnNtSetInformationObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle, IN OBJECT_INFORMATION_CLASS ObjectInformationClass, IN PVOID ObjectInformation, IN ULONG Length);
using fnNtSetInformationProcess = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PROCESS_INFORMATION_CLASS ProcessInformationClass, IN PVOID ProcessInformation, IN ULONG ProcessInformationLength);
using fnNtSetInformationThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN THREAD_INFORMATION_CLASS ThreadInformationClass, IN PVOID ThreadInformation, IN ULONG ThreadInformationLength);
using fnNtSetInformationToken = NTSTATUS( NTAPI* )(IN HANDLE TokenHandle, IN TOKEN_INFORMATION_CLASS TokenInformationClass, OUT PVOID TokenInformation, IN ULONG TokenInformationLength);
using fnNtSetIntervalProfile = NTSTATUS( NTAPI* )(IN ULONG Interval, IN KPROFILE_SOURCE Source);
using fnNtSetIoCompletion = NTSTATUS( NTAPI* )(IN HANDLE IoCompletionHandle, IN ULONG CompletionKey, OUT PIO_STATUS_BLOCK IoStatusBlock, IN NTSTATUS CompletionStatus, IN ULONG NumberOfBytesTransfered);
using fnNtSetLowEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtSetLowWaitHighEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtSetSecurityObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle, IN SECURITY_INFORMATION SecurityInformationClass, IN PSECURITY_DESCRIPTOR DescriptorBuffer);
using fnNtSetSystemEnvironmentValue = NTSTATUS( NTAPI* )(IN PUNICODE_STRING VariableName, IN PUNICODE_STRING Value);
using fnNtSetSystemInformation = NTSTATUS( NTAPI* )(IN SYSTEM_INFORMATION_CLASS SystemInformationClass, IN PVOID SystemInformation, IN ULONG SystemInformationLength);
using fnNtSetSystemTime = NTSTATUS( NTAPI* )(IN PLARGE_INTEGER SystemTime, OUT PLARGE_INTEGER PreviousTime);
using fnNtSetTimer = NTSTATUS( NTAPI* )(IN HANDLE TimerHandle, IN PLARGE_INTEGER DueTime, IN PTIMER_APC_ROUTINE TimerApcRoutine, IN PVOID TimerContext, IN BOOLEAN ResumeTimer, IN LONG Period, OUT PBOOLEAN PreviousState);
using fnNtSetTimerResolution = NTSTATUS( NTAPI* )(IN ULONG DesiredResolution, IN BOOLEAN SetResolution, OUT PULONG CurrentResolution);
using fnNtSetValueKey = NTSTATUS( NTAPI* )(IN HANDLE KeyHandle, IN PUNICODE_STRING ValueName, IN ULONG TitleIndex, IN ULONG Type, IN PVOID Data, IN ULONG DataSize);
using fnNtSetVolumeInformationFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID FileSystemInformation, IN ULONG Length, IN FS_INFORMATION_CLASS FileSystemInformationClass);
using fnNtShutdownSystem = NTSTATUS( NTAPI* )(IN SHUTDOWN_ACTION Action);
using fnNtSignalAndWaitForSingleObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectToSignal, IN HANDLE WaitableObject, IN BOOLEAN Alertable, IN PLARGE_INTEGER Time);
using fnNtStartProfile = NTSTATUS( NTAPI* )(IN HANDLE ProfileHandle);
using fnNtStopProfile = NTSTATUS( NTAPI* )(IN HANDLE ProfileHandle);
using fnNtSuspendThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, OUT PULONG PreviousSuspendCount);
using fnNtSystemDebugControl = NTSTATUS( NTAPI* )(IN SYSDBG_COMMAND Command, IN PVOID InputBuffer, IN ULONG InputBufferLength, OUT PVOID OutputBuffer, IN ULONG OutputBufferLength, OUT PULONG ReturnLength);
using fnNtTerminateProcess = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus);
using fnNtTerminateThread = NTSTATUS( NTAPI* )(IN HANDLE ThreadHandle, IN NTSTATUS ExitStatus);
using fnNtTestAlert = NTSTATUS( NTAPI* )(VOID);
using fnNtUnloadDriver = NTSTATUS( NTAPI* )(IN PUNICODE_STRING DriverServiceName);
using fnNtUnloadKey = NTSTATUS( NTAPI* )(IN POBJECT_ATTRIBUTES DestinationKeyName);
using fnNtUnlockFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PLARGE_INTEGER ByteOffset, IN PLARGE_INTEGER Length, IN PULONG Key);
using fnNtUnlockVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID *BaseAddress, IN OUT PULONG NumberOfBytesToUnlock, IN ULONG LockType);
using fnNtUnmapViewOfSection = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID BaseAddress);
using fnNtWaitForKeyedEvent = NTSTATUS( NTAPI* )(IN HANDLE KeyedEventHandle, IN PVOID Key, IN BOOLEAN Alertable, IN PLARGE_INTEGER Timeout);
using fnNtWaitForMultipleObjects = NTSTATUS( NTAPI* )(IN ULONG ObjectCount, IN PHANDLE ObjectsArray, IN OBJECT_WAIT_TYPE WaitType, IN BOOLEAN Alertable, IN PLARGE_INTEGER TimeOut);
using fnNtWaitForSingleObject = NTSTATUS( NTAPI* )(IN HANDLE ObjectHandle, IN BOOLEAN Alertable, IN PLARGE_INTEGER TimeOut);
using fnNtWaitHighEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtWaitLowEventPair = NTSTATUS( NTAPI* )(IN HANDLE EventPairHandle);
using fnNtWriteFile = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN PVOID Buffer, IN ULONG Length, IN PLARGE_INTEGER ByteOffset, IN PULONG Key);
using fnNtWriteFileGather = NTSTATUS( NTAPI* )(IN HANDLE FileHandle, IN HANDLE Event, IN PIO_APC_ROUTINE ApcRoutine, IN PVOID ApcContext, OUT PIO_STATUS_BLOCK IoStatusBlock, IN FILE_SEGMENT_ELEMENT SegmentArray, IN ULONG Length, IN PLARGE_INTEGER ByteOffset, IN PULONG Key);
using fnNtWriteRequestData = NTSTATUS( NTAPI* )(IN HANDLE PortHandle, IN PLPC_MESSAGE Request, IN ULONG DataIndex, IN PVOID Buffer, IN ULONG Length, OUT PULONG ResultLength);
using fnNtWriteVirtualMemory = NTSTATUS( NTAPI* )(IN HANDLE ProcessHandle, IN PVOID BaseAddress, IN PVOID Buffer, IN ULONG NumberOfBytesToWrite, OUT PULONG NumberOfBytesWritten);
using fnNtYieldExecution = NTSTATUS( NTAPI* )(VOID);

#pragma endregion

// these are custom/re'd structs
struct _VEH_LIST_NODE;
typedef struct _VEH_LIST_NODE {
	_VEH_LIST_NODE* fLink;
	_VEH_LIST_NODE* bLink;
	DWORD dwOne;
#ifdef _WIN64
	DWORD dwReserved;
#endif
	PVOID pEncodedFuncPtr;
}VEH_LIST_NODE;
typedef VEH_LIST_NODE *PVEH_LIST_NODE;

typedef struct _VEH_HANDLER_LIST
{
	PVOID pLock;
	PVEH_LIST_NODE pHead;
	PVEH_LIST_NODE pTail;
}VEH_HANDLER_LIST;
typedef VEH_HANDLER_LIST *PVEH_HANDLER_LIST;

typedef struct _VEH_INFO
{
	PVOID hHandler;
	PVOID pFuncPtr;
}VEH_INFO;
typedef VEH_INFO *PVEH_INFO;

#define VehEncodePtr64(p, k) _rotr64((uintptr_t)p ^ k, k & 0x3F)
#define VehDecodePtr64(p, k) _rotr64((uintptr_t)p, 0x40 - (k & 0x3F)) ^ k
#define VehEncodePtr32(p, k) _rotr((uintptr_t)p ^ k, k & 0x1F)
#define VehDecodePtr32(p, k) _rotr((uintptr_t)p, 0x20 - (k & 0x1F)) ^ k
#ifdef _WIN64
#define VehEncodePtr VehEncodePtr64
#define VehDecodePtr VehDecodePtr64
#else
#define VehEncodePtr VehEncodePtr32
#define VehDecodePtr VehDecodePtr32
#endif


#pragma region Syscall_Defines
class TSyscall;
#define _sc(t, s) TSyscall::GetInvoke(s, (t)nullptr)
#define ScAcceptConnectPort _sc(fnNtAcceptConnectPort, "NtAcceptConnectPort")
#define ScAccessCheck _sc(fnNtAccessCheck, "NtAccessCheck")
#define ScAccessCheckAndAuditAlarm _sc(fnNtAccessCheckAndAuditAlarm, "NtAccessCheckAndAuditAlarm")
#define ScAddAtom _sc(fnNtAddAtom, "NtAddAtom")
#define ScAdjustGroupsToken _sc(fnNtAdjustGroupsToken, "NtAdjustGroupsToken")
#define ScAdjustPrivilegesToken _sc(fnNtAdjustPrivilegesToken, "NtAdjustPrivilegesToken")
#define ScAlertResumeThread _sc(fnNtAlertResumeThread, "NtAlertResumeThread")
#define ScAlertThread _sc(fnNtAlertThread, "NtAlertThread")
#define ScAllocateLocallyUniqueId _sc(fnNtAllocateLocallyUniqueId, "NtAllocateLocallyUniqueId")
#define ScAllocateUuids _sc(fnNtAllocateUuids, "NtAllocateUuids")
#define ScAllocateVirtualMemory _sc(fnNtAllocateVirtualMemory, "NtAllocateVirtualMemory")
#define ScCallbackReturn _sc(fnNtCallbackReturn, "NtCallbackReturn")
#define ScCancelIoFile _sc(fnNtCancelIoFile, "NtCancelIoFile")
#define ScCancelTimer _sc(fnNtCancelTimer, "NtCancelTimer")
#define ScClearEvent _sc(fnNtClearEvent, "NtClearEvent")
#define ScClose _sc(fnNtClose, "NtClose")
#define ScCloseObjectAuditAlarm _sc(fnNtCloseObjectAuditAlarm, "NtCloseObjectAuditAlarm")
#define ScCompactKeys _sc(fnNtCompactKeys, "NtCompactKeys")
#define ScCompleteConnectPort _sc(fnNtCompleteConnectPort, "NtCompleteConnectPort")
#define ScCompressKey _sc(fnNtCompressKey, "NtCompressKey")
#define ScConnectPort _sc(fnNtConnectPort, "NtConnectPort")
#define ScContinue _sc(fnNtContinue, "NtContinue")
#define ScCreateDebugObject _sc(fnNtCreateDebugObject, "NtCreateDebugObject")
#define ScCreateDirectoryObject _sc(fnNtCreateDirectoryObject, "NtCreateDirectoryObject")
#define ScCreateEvent _sc(fnNtCreateEvent, "NtCreateEvent")
#define ScCreateEventPair _sc(fnNtCreateEventPair, "NtCreateEventPair")
#define ScCreateFile _sc(fnNtCreateFile, "NtCreateFile")
#define ScCreateIoCompletion _sc(fnNtCreateIoCompletion, "NtCreateIoCompletion")
#define ScCreateKey _sc(fnNtCreateKey, "NtCreateKey")
#define ScCreateKeyedEvent _sc(fnNtCreateKeyedEvent, "NtCreateKeyedEvent")
#define ScCreateMailslotFile _sc(fnNtCreateMailslotFile, "NtCreateMailslotFile")
#define ScCreateMutant _sc(fnNtCreateMutant, "NtCreateMutant")
#define ScCreateNamedPipeFile _sc(fnNtCreateNamedPipeFile, "NtCreateNamedPipeFile")
#define ScCreatePagingFile _sc(fnNtCreatePagingFile, "NtCreatePagingFile")
#define ScCreatePort _sc(fnNtCreatePort, "NtCreatePort")
#define ScCreateProcess _sc(fnNtCreateProcess, "NtCreateProcess")
#define ScCreateProfile _sc(fnNtCreateProfile, "NtCreateProfile")
#define ScCreateSection _sc(fnNtCreateSection, "NtCreateSection")
#define ScCreateSemaphore _sc(fnNtCreateSemaphore, "NtCreateSemaphore")
#define ScCreateSymbolicLinkObject  _sc(fnNtCreateSymbolicLinkObject , "NtCreateSymbolicLinkObject ")
#define ScCreateThread _sc(fnNtCreateThread, "NtCreateThread")
#define ScCreateThreadEx _sc(fnNtCreateThreadEx, "NtCreateThreadEx")
#define ScCreateTimer _sc(fnNtCreateTimer, "NtCreateTimer")
#define ScCreateToken _sc(fnNtCreateToken, "NtCreateToken")
#define ScDebugActiveProcess _sc(fnNtDebugActiveProcess, "NtDebugActiveProcess")
#define ScDelayExecution _sc(fnNtDelayExecution, "NtDelayExecution")
#define ScDeleteAtom _sc(fnNtDeleteAtom, "NtDeleteAtom")
#define ScDeleteFile _sc(fnNtDeleteFile, "NtDeleteFile")
#define ScDeleteKey _sc(fnNtDeleteKey, "NtDeleteKey")
#define ScDeleteObjectAuditAlarm _sc(fnNtDeleteObjectAuditAlarm, "NtDeleteObjectAuditAlarm")
#define ScDeleteValueKey _sc(fnNtDeleteValueKey, "NtDeleteValueKey")
#define ScDeviceIoControlFile _sc(fnNtDeviceIoControlFile, "NtDeviceIoControlFile")
#define ScDisplayString _sc(fnNtDisplayString, "NtDisplayString")
#define ScDuplicateObject _sc(fnNtDuplicateObject, "NtDuplicateObject")
#define ScDuplicateToken _sc(fnNtDuplicateToken, "NtDuplicateToken")
#define ScEnumerateKey _sc(fnNtEnumerateKey, "NtEnumerateKey")
#define ScEnumerateValueKey _sc(fnNtEnumerateValueKey, "NtEnumerateValueKey")
#define ScExtendSection _sc(fnNtExtendSection, "NtExtendSection")
#define ScFindAtom _sc(fnNtFindAtom, "NtFindAtom")
#define ScFlushBuffersFile _sc(fnNtFlushBuffersFile, "NtFlushBuffersFile")
#define ScFlushInstructionCache _sc(fnNtFlushInstructionCache, "NtFlushInstructionCache")
#define ScFlushKey _sc(fnNtFlushKey, "NtFlushKey")
#define ScFlushVirtualMemory _sc(fnNtFlushVirtualMemory, "NtFlushVirtualMemory")
#define ScFlushWriteBuffer _sc(fnNtFlushWriteBuffer, "NtFlushWriteBuffer")
#define ScFreeVirtualMemory _sc(fnNtFreeVirtualMemory, "NtFreeVirtualMemory")
#define ScFsControlFile _sc(fnNtFsControlFile, "NtFsControlFile")
#define ScGetContextThread _sc(fnNtGetContextThread, "NtGetContextThread")
#define ScImpersonateClientOfPort _sc(fnNtImpersonateClientOfPort, "NtImpersonateClientOfPort")
#define ScImpersonateThread _sc(fnNtImpersonateThread, "NtImpersonateThread")
#define ScListenPort _sc(fnNtListenPort, "NtListenPort")
#define ScLoadDriver _sc(fnNtLoadDriver, "NtLoadDriver")
#define ScLoadKey _sc(fnNtLoadKey, "NtLoadKey")
#define ScLoadKey2 _sc(fnNtLoadKey2, "NtLoadKey2")
#define ScLockFile _sc(fnNtLockFile, "NtLockFile")
#define ScLockVirtualMemory _sc(fnNtLockVirtualMemory, "NtLockVirtualMemory")
#define ScMakeTemporaryObject _sc(fnNtMakeTemporaryObject, "NtMakeTemporaryObject")
#define ScMapViewOfSection _sc(fnNtMapViewOfSection, "NtMapViewOfSection")
#define ScNotifyChangeDirectoryFile _sc(fnNtNotifyChangeDirectoryFile, "NtNotifyChangeDirectoryFile")
#define ScNotifyChangeKey _sc(fnNtNotifyChangeKey, "NtNotifyChangeKey")
#define ScOpenDirectoryObject _sc(fnNtOpenDirectoryObject, "NtOpenDirectoryObject")
#define ScOpenEvent _sc(fnNtOpenEvent, "NtOpenEvent")
#define ScOpenEventPair _sc(fnNtOpenEventPair, "NtOpenEventPair")
#define ScOpenFile _sc(fnNtOpenFile, "NtOpenFile")
#define ScOpenIoCompletion _sc(fnNtOpenIoCompletion, "NtOpenIoCompletion")
#define ScOpenKey _sc(fnNtOpenKey, "NtOpenKey")
#define ScOpenKeyedEvent _sc(fnNtOpenKeyedEvent, "NtOpenKeyedEvent")
#define ScOpenMutant _sc(fnNtOpenMutant, "NtOpenMutant")
#define ScOpenObjectAuditAlarm _sc(fnNtOpenObjectAuditAlarm, "NtOpenObjectAuditAlarm")
#define ScOpenProcess _sc(fnNtOpenProcess, "NtOpenProcess")
#define ScOpenProcessToken _sc(fnNtOpenProcessToken, "NtOpenProcessToken")
#define ScOpenSection _sc(fnNtOpenSection, "NtOpenSection")
#define ScOpenSemaphore _sc(fnNtOpenSemaphore, "NtOpenSemaphore")
#define ScOpenSymbolicLinkObject _sc(fnNtOpenSymbolicLinkObject, "NtOpenSymbolicLinkObject")
#define ScOpenThread _sc(fnNtOpenThread, "NtOpenThread")
#define ScOpenThreadToken _sc(fnNtOpenThreadToken, "NtOpenThreadToken")
#define ScOpenTimer _sc(fnNtOpenTimer, "NtOpenTimer")
#define ScPrivilegeCheck _sc(fnNtPrivilegeCheck, "NtPrivilegeCheck")
#define ScPrivilegeObjectAuditAlarm _sc(fnNtPrivilegeObjectAuditAlarm, "NtPrivilegeObjectAuditAlarm")
#define ScPrivilegedServiceAuditAlarm _sc(fnNtPrivilegedServiceAuditAlarm, "NtPrivilegedServiceAuditAlarm")
#define ScProtectVirtualMemory _sc(fnNtProtectVirtualMemory, "NtProtectVirtualMemory")
#define ScPulseEvent _sc(fnNtPulseEvent, "NtPulseEvent")
#define ScQueryAttributesFile _sc(fnNtQueryAttributesFile, "NtQueryAttributesFile")
#define ScQueryDefaultLocale _sc(fnNtQueryDefaultLocale, "NtQueryDefaultLocale")
#define ScQueryDirectoryFile _sc(fnNtQueryDirectoryFile, "NtQueryDirectoryFile")
#define ScQueryDirectoryObject _sc(fnNtQueryDirectoryObject, "NtQueryDirectoryObject")
#define ScQueryEaFile _sc(fnNtQueryEaFile, "NtQueryEaFile")
#define ScQueryEvent _sc(fnNtQueryEvent, "NtQueryEvent")
#define ScQueryFullAttributesFile _sc(fnNtQueryFullAttributesFile, "NtQueryFullAttributesFile")
#define ScQueryInformationAtom _sc(fnNtQueryInformationAtom, "NtQueryInformationAtom")
#define ScQueryInformationFile _sc(fnNtQueryInformationFile, "NtQueryInformationFile")
#define ScQueryInformationPort _sc(fnNtQueryInformationPort, "NtQueryInformationPort")
#define ScQueryInformationProcess _sc(fnNtQueryInformationProcess, "NtQueryInformationProcess")
#define ScQueryInformationThread _sc(fnNtQueryInformationThread, "NtQueryInformationThread")
#define ScQueryInformationToken _sc(fnNtQueryInformationToken, "NtQueryInformationToken")
#define ScQueryIntervalProfile _sc(fnNtQueryIntervalProfile, "NtQueryIntervalProfile")
#define ScQueryIoCompletion _sc(fnNtQueryIoCompletion, "NtQueryIoCompletion")
#define ScQueryKey _sc(fnNtQueryKey, "NtQueryKey")
#define ScQueryMultipleValueKey _sc(fnNtQueryMultipleValueKey, "NtQueryMultipleValueKey")
#define ScQueryMutant _sc(fnNtQueryMutant, "NtQueryMutant")
#define ScQueryObject _sc(fnNtQueryObject, "NtQueryObject")
#define ScQueryPerformanceCounter _sc(fnNtQueryPerformanceCounter, "NtQueryPerformanceCounter")
#define ScQuerySection _sc(fnNtQuerySection, "NtQuerySection")
#define ScQuerySecurityObject _sc(fnNtQuerySecurityObject, "NtQuerySecurityObject")
#define ScQuerySemaphore _sc(fnNtQuerySemaphore, "NtQuerySemaphore")
#define ScQuerySymbolicLinkObject _sc(fnNtQuerySymbolicLinkObject, "NtQuerySymbolicLinkObject")
#define ScQuerySystemEnvironmentValue _sc(fnNtQuerySystemEnvironmentValue, "NtQuerySystemEnvironmentValue")
#define ScQuerySystemInformation _sc(fnNtQuerySystemInformation, "NtQuerySystemInformation")
#define ScQueryTimer _sc(fnNtQueryTimer, "NtQueryTimer")
#define ScQueryTimerResolution _sc(fnNtQueryTimerResolution, "NtQueryTimerResolution")
#define ScQueryValueKey _sc(fnNtQueryValueKey, "NtQueryValueKey")
#define ScQueryVirtualMemory _sc(fnNtQueryVirtualMemory, "NtQueryVirtualMemory")
#define ScQueryVolumeInformationFile _sc(fnNtQueryVolumeInformationFile, "NtQueryVolumeInformationFile")
#define ScQueueApcThread _sc(fnNtQueueApcThread, "NtQueueApcThread")
#define ScRaiseException _sc(fnNtRaiseException, "NtRaiseException")
#define ScRaiseHardError _sc(fnNtRaiseHardError, "NtRaiseHardError")
#define ScReadFile _sc(fnNtReadFile, "NtReadFile")
#define ScReadFileScatter _sc(fnNtReadFileScatter, "NtReadFileScatter")
#define ScReadRequestData _sc(fnNtReadRequestData, "NtReadRequestData")
#define ScReadVirtualMemory _sc(fnNtReadVirtualMemory, "NtReadVirtualMemory")
#define ScRegisterThreadTerminatePort _sc(fnNtRegisterThreadTerminatePort, "NtRegisterThreadTerminatePort")
#define ScReleaseKeyedEvent _sc(fnNtReleaseKeyedEvent, "NtReleaseKeyedEvent")
#define ScReleaseMutant _sc(fnNtReleaseMutant, "NtReleaseMutant")
#define ScReleaseSemaphore _sc(fnNtReleaseSemaphore, "NtReleaseSemaphore")
#define ScRemoveIoCompletion _sc(fnNtRemoveIoCompletion, "NtRemoveIoCompletion")
#define ScRemoveProcessDebug _sc(fnNtRemoveProcessDebug, "NtRemoveProcessDebug")
#define ScReplaceKey _sc(fnNtReplaceKey, "NtReplaceKey")
#define ScReplyPort _sc(fnNtReplyPort, "NtReplyPort")
#define ScReplyWaitReceivePort _sc(fnNtReplyWaitReceivePort, "NtReplyWaitReceivePort")
#define ScReplyWaitReplyPort _sc(fnNtReplyWaitReplyPort, "NtReplyWaitReplyPort")
#define ScRequestPort _sc(fnNtRequestPort, "NtRequestPort")
#define ScRequestWaitReplyPort _sc(fnNtRequestWaitReplyPort, "NtRequestWaitReplyPort")
#define ScResetEvent _sc(fnNtResetEvent, "NtResetEvent")
#define ScRestoreKey _sc(fnNtRestoreKey, "NtRestoreKey")
#define ScResumeThread _sc(fnNtResumeThread, "NtResumeThread")
#define ScSaveKey _sc(fnNtSaveKey, "NtSaveKey")
#define ScSetContextThread _sc(fnNtSetContextThread, "NtSetContextThread")
#define ScSetDefaultHardErrorPort _sc(fnNtSetDefaultHardErrorPort, "NtSetDefaultHardErrorPort")
#define ScSetDefaultLocale _sc(fnNtSetDefaultLocale, "NtSetDefaultLocale")
#define ScSetEaFile _sc(fnNtSetEaFile, "NtSetEaFile")
#define ScSetEvent _sc(fnNtSetEvent, "NtSetEvent")
#define ScSetEventBoostPriority _sc(fnNtSetEventBoostPriority, "NtSetEventBoostPriority")
#define ScSetHighEventPair _sc(fnNtSetHighEventPair, "NtSetHighEventPair")
#define ScSetHighWaitLowEventPair _sc(fnNtSetHighWaitLowEventPair, "NtSetHighWaitLowEventPair")
#define ScSetInformationFile _sc(fnNtSetInformationFile, "NtSetInformationFile")
#define ScSetInformationKey _sc(fnNtSetInformationKey, "NtSetInformationKey")
#define ScSetInformationObject _sc(fnNtSetInformationObject, "NtSetInformationObject")
#define ScSetInformationProcess _sc(fnNtSetInformationProcess, "NtSetInformationProcess")
#define ScSetInformationThread _sc(fnNtSetInformationThread, "NtSetInformationThread")
#define ScSetInformationToken _sc(fnNtSetInformationToken, "NtSetInformationToken")
#define ScSetIntervalProfile _sc(fnNtSetIntervalProfile, "NtSetIntervalProfile")
#define ScSetIoCompletion _sc(fnNtSetIoCompletion, "NtSetIoCompletion")
#define ScSetLowEventPair _sc(fnNtSetLowEventPair, "NtSetLowEventPair")
#define ScSetLowWaitHighEventPair _sc(fnNtSetLowWaitHighEventPair, "NtSetLowWaitHighEventPair")
#define ScSetSecurityObject _sc(fnNtSetSecurityObject, "NtSetSecurityObject")
#define ScSetSystemEnvironmentValue _sc(fnNtSetSystemEnvironmentValue, "NtSetSystemEnvironmentValue")
#define ScSetSystemInformation _sc(fnNtSetSystemInformation, "NtSetSystemInformation")
#define ScSetSystemTime _sc(fnNtSetSystemTime, "NtSetSystemTime")
#define ScSetTimer _sc(fnNtSetTimer, "NtSetTimer")
#define ScSetTimerResolution _sc(fnNtSetTimerResolution, "NtSetTimerResolution")
#define ScSetValueKey _sc(fnNtSetValueKey, "NtSetValueKey")
#define ScSetVolumeInformationFile _sc(fnNtSetVolumeInformationFile, "NtSetVolumeInformationFile")
#define ScShutdownSystem _sc(fnNtShutdownSystem, "NtShutdownSystem")
#define ScSignalAndWaitForSingleObject _sc(fnNtSignalAndWaitForSingleObject, "NtSignalAndWaitForSingleObject")
#define ScStartProfile _sc(fnNtStartProfile, "NtStartProfile")
#define ScStopProfile _sc(fnNtStopProfile, "NtStopProfile")
#define ScSuspendThread _sc(fnNtSuspendThread, "NtSuspendThread")
#define ScSystemDebugControl _sc(fnNtSystemDebugControl, "NtSystemDebugControl")
#define ScTerminateProcess _sc(fnNtTerminateProcess, "NtTerminateProcess")
#define ScTerminateThread _sc(fnNtTerminateThread, "NtTerminateThread")
#define ScTestAlert _sc(fnNtTestAlert, "NtTestAlert")
#define ScUnloadDriver _sc(fnNtUnloadDriver, "NtUnloadDriver")
#define ScUnloadKey _sc(fnNtUnloadKey, "NtUnloadKey")
#define ScUnlockFile _sc(fnNtUnlockFile, "NtUnlockFile")
#define ScUnlockVirtualMemory _sc(fnNtUnlockVirtualMemory, "NtUnlockVirtualMemory")
#define ScUnmapViewOfSection _sc(fnNtUnmapViewOfSection, "NtUnmapViewOfSection")
#define ScWaitForKeyedEvent _sc(fnNtWaitForKeyedEvent, "NtWaitForKeyedEvent")
#define ScWaitForMultipleObjects _sc(fnNtWaitForMultipleObjects, "NtWaitForMultipleObjects")
#define ScWaitForSingleObject _sc(fnNtWaitForSingleObject, "NtWaitForSingleObject")
#define ScWaitHighEventPair _sc(fnNtWaitHighEventPair, "NtWaitHighEventPair")
#define ScWaitLowEventPair _sc(fnNtWaitLowEventPair, "NtWaitLowEventPair")
#define ScWriteFile _sc(fnNtWriteFile, "NtWriteFile")
#define ScWriteFileGather _sc(fnNtWriteFileGather, "NtWriteFileGather")
#define ScWriteRequestData _sc(fnNtWriteRequestData, "NtWriteRequestData")
#define ScWriteVirtualMemory _sc(fnNtWriteVirtualMemory, "NtWriteVirtualMemory")
#define ScYieldExecution _sc(fnNtYieldExecution, "NtYieldExecution")

#pragma endregion

//using fnRtlAddVectoredExceptionHandler = PVOID( NTAPI* )(ULONG ulFirst, PVECTORED_EXCEPTION_HANDLER pHandler);
//using fnRtlRemoveVectoredExceptionHandler = NTSTATUS( NTAPI* )(PVOID pHandlerHandle);
using fnRtlAllocateHeap = PVOID( NTAPI* )(PVOID hHeap, ULONG ulFlags, SIZE_T szSize);

namespace NT
{
	//extern fnNtQueryInformationProcess pNtQueryInformationProcess;
	//extern fnNtQueryInformationThread pNtQueryInformationThread;
	//extern fnNtQueryVirtualMemory pNtQueryVirtualMemory;
	//extern fnNtProtectVirtualMemory pNtProtectVirtualMemory;
	//extern fnRtlAddVectoredExceptionHandler pRtlAddVectoredExceptionHandler;
	//extern fnRtlRemoveVectoredExceptionHandler pRtlRemoveVectoredExceptionHandler;
	extern fnRtlAllocateHeap pRtlAllocateHeap;

	extern PVEH_HANDLER_LIST pVehHandlerList;
	extern DWORD dwEncoderKey;
	// Initialize all the NT functions.
	bool InitNT();

	// Get pointer to current process' PEB
	_PEB* GetPEB();

	// Get start address of specific thread
	void* GetThreadStartAddress( HANDLE hThread );

	// PE Header parsing method (export table)
	void* GetProcedureAddress( const char* szModule, const char* szFunction );

	// walk the current proc's IAT and get the entry address (IAT hooker helper)
	void* GetProcAddressIAT( const char* szFunction );

	// Get info on the page pAddress is on
	bool QueryVirtualMemory( void* pAddress, MEMORY_BASIC_INFORMATION* pMbi );

	// Change protection on the page pAddress is on
	bool ProtectVirtualMemory( void* pAddress, DWORD dwProtection, DWORD* pOldProtection );

	void* AddVEH( DWORD uFirst, PVECTORED_EXCEPTION_HANDLER pVEH );

	void GetVectoredExceptionHandlers( std::vector<VEH_INFO> & vHandlers );

	void RemoveVEH( void* hHandler );
}

#pragma endregion

#pragma region TPattern

class TPattern
{
	std::string sMask;
	std::string sPattern;
	std::vector<uint8_t> bytes;
	bool bIdaStyle = false;
public:
	// 90 90 90 ? ? ? 90 90
	// 90 90 90 ?? ?? ?? 90 90
	TPattern( const char* sPattern );
	// \x90\x90\x90\x90\x00\x00\x90\x90 xxxx??xx
	TPattern( const char* sPattern, const char* sMask );
	~TPattern() = default;

	std::vector<uint8_t>& Get();
	const std::string& GetMask() const;
	size_t Size();

private:
	void Parse();
	void ParsePattern();
	void ParsePatternIDA();
};

#pragma endregion

#pragma region TCodeBuffer

class TCodeBuffer
{
	std::vector<uint8_t> buffer;
	int iJmpBackOffset = 0;
	uintptr_t pJmpBack = 0;
public:
	TCodeBuffer() = default;
	TCodeBuffer( char * pBuffer, uint32_t size );
	TCodeBuffer( const std::vector < uint8_t> & buf );
	TCodeBuffer( std::initializer_list<uint8_t> bytes );
	~TCodeBuffer() = default;

	//! Update buffer at specified offset
	bool Update( size_t offset, std::initializer_list<uint8_t> bytes );
	//! Update buffer at specified offset
	bool Update( size_t offset, void* ptr );

	template<class T>
	bool UpdateIntegral( size_t offset, T n );

	void AddJumpBack( uintptr_t pJmpBack );

	size_t Size();
	std::vector<uint8_t> Get();

	// pAddress Contains the address this buffer is being written to.
	// This is so, for example in the install trampoline hook (InstallTH) function
	// we can resolve the jmpback address at runtime. 
	std::vector<uint8_t> Get( uintptr_t pAddress );

	uint8_t* GetData() { return buffer.data(); }

	void operator=( std::vector<uint8_t> bytes );
	void operator=( std::initializer_list<uint8_t> bytes );
	void operator+=( std::vector<uint8_t> bytes );
	void operator+=( std::initializer_list<uint8_t> bytes );
	void operator+=( void* ptr );
	template <class T>
	void operator+=( T rhs );

	//Use these with a bit of caution...
	void operator=( const char* byteString );
	void operator+=( const char* byteString );

private:
	template<class T>
	void PushIntegral( T n );

};

template<class T>
inline void TCodeBuffer::PushIntegral( T n )
{
	char* pByte = (char*) &n;
	for ( size_t x = 0; x < sizeof( n ); x++ )
	{
		buffer.push_back( *pByte );
		pByte++;
	}
}

template<class T>
inline bool TCodeBuffer::UpdateIntegral( size_t offset, T n )
{
	if ( offset + sizeof( T ) > buffer.size() )
	{
		auto diff = (offset + sizeof( T )) - buffer.size();
		buffer.resize( buffer.size() + diff );
	}
	if ( !memcpy( &buffer[offset], &n, sizeof( n ) ) )
		return false;
	return true;
}

template<class T>
inline void TCodeBuffer::operator+=( T rhs )
{
	PushIntegral( rhs );
}

#pragma endregion

#pragma region Hooks

class TMem;
class HookManager;
class Hook
{
	friend HookManager;

public:
	enum HookType
	{
		MF, // Mid-Function
		VMT,// Virtual Method Table
		TH, // Trampoline
		RS, // Register steal
		PG, // Page-guard(VEH) hook
		IAT // Pretty self explanatory...
	};
protected:
	HookType hookType;
	void* pHookTarget = nullptr; // This is the location the hook is being placed
	void* pHookLoc = nullptr; // This is where the hook will reroute code to
	int iSize = 0; // number of bytes that need to be overwritten/copied
	std::vector<byte> vOgBytes; // Original bytes that are overwritten
	int id = 0;
	bool status = false; // true == installed

	Hook() = default;
	Hook( int id, void* pTarget, void* pLoc ) { this->id = id; SetTarget( pTarget ); SetLoc( pLoc ); }
public:
	HookType GetType() { return hookType; }
	void SetTarget( void* pTarget ) { this->pHookTarget = pTarget; }
	void* GetTarget() { return pHookTarget; }
	void SetLoc( void* pLoc ) { this->pHookLoc = pLoc; }
	void* GetLoc() { return pHookLoc; }
	int GetID() { return id; }
	size_t GetSize() { return iSize; }
	std::vector<byte>* GetOgBytes() { return &vOgBytes; }
};

// Midfunction hooks
class HookMF : public Hook
{
	TCodeBuffer cb;
public:
	HookMF( int id, void* pTarget, void* pHook, int size )
		:
		Hook( id, pTarget, pHook )
	{
		hookType = MF;
		this->iSize = size;
	}

};

// VMT hooks
class HookVMT : public Hook
{
private:
	int iFuncIndex = 0;
	void* pOgFunc = nullptr;
public:
	HookVMT( int id, void* pObject, void* pHook, int iFuncIndex )
		:
		Hook( id, pObject, pHook )
	{
		hookType = VMT;
		this->iFuncIndex = iFuncIndex;
	}
	int GetFunctionIndex() { return iFuncIndex; }
	void* GetOriginalFunc() { return pOgFunc; }
	void* SetOriginalFunc( void* pFunc ) { return pOgFunc = pFunc; }
};

// Trampoline hooks
class HookTH : public Hook
{
	TCodeBuffer cbTrampBuffer;
	void* pTrampLoc = nullptr;
public:
	HookTH( int id, void* pTarget, void* pHook, int szSize ) :
		Hook( id, pTarget, pHook )
	{
		this->hookType = TH;
		this->iSize = szSize;
	}

	TCodeBuffer& GetTrampolineBuffer() { return cbTrampBuffer; }
	void SetTrampLoc( void* pLoc ) { this->pTrampLoc = pLoc; }
	void* GetTrampLoc() { return pTrampLoc; }
};

// Page-Guard (VEH) hooks
class HookPG : public Hook
{
	friend HookManager;
	void* hHandler = nullptr;
	PVECTORED_EXCEPTION_HANDLER pVEH;
	DWORD dwProtect = 0;
public:
	HookPG( int id, void* pTarget, void* pHook, PVECTORED_EXCEPTION_HANDLER pVEH )
		:
		Hook( id, pTarget, pHook )
	{
		hookType = PG;
		this->pVEH = pVEH;
	}

	DWORD GetPageProtection() { return dwProtect; }
	PVECTORED_EXCEPTION_HANDLER GetHandler() { return pVEH; }
};

class HookIAT : public Hook
{
	void* pIatEntry = nullptr;
	char* pFuncName = nullptr;
public:
	HookIAT( int id, void* pHook, const char* szFunction )
		:
		Hook( id, nullptr, pHook )
	{
		hookType = IAT;
		auto strLen = lstrlenA( szFunction ) + 1;
		this->pFuncName = new char[strLen];
		strcpy_s( pFuncName, strLen, szFunction );
	}

	char* GetFuncName() { return pFuncName; }
	void SetIatEntry( void* pEntry ) { this->pIatEntry = pEntry; }
	void* GetIatEntry() { return pIatEntry; }
};

class HookManager
{
	static HookManager* pManager;
	TMem* pMem = nullptr;
	std::map<int, Hook*> hooks;
	HookManager();
public:
	static HookManager* Get();
	static void Destroy();
	~HookManager();

	bool Add( Hook* pHook, bool bInstall = true );
	bool Add( int id, Hook* pHook, bool bInstall = true );
	void AddMF( int id, void* pTarget, void* pHook, int iBytes, bool bInstall = true );

	bool Install( int id );
	bool Install( Hook* pHook );
	void Uninstall( int id );
	void Uninstall( Hook* pHook );

	template<class HkType>
	HkType* GetHook( int id );

	Hook* Get( int id );
	bool GetHookStatus( int id );

private:
	bool InstallMF( HookMF* pHook );
	void UninstallMF( HookMF* pHook );
	bool InstallVMT( HookVMT* pHook );
	void UninstallVMT( HookVMT* pHook );
	bool InstallTH( HookTH* pHook );
	void UninstallTH( HookTH* pHook );
	bool InstallPG( HookPG* pHook );
	void UninstallPG( HookPG* pHook );
	bool InstallIAT( HookIAT* pHook );
	void UninstallIAT( HookIAT* pHook );
};

template<class HkType>
inline HkType * HookManager::GetHook( int id )
{
	return (HkType*) hooks[id];
}


#pragma endregion

#pragma region TCodeInj

class TCodeInj
{
	static std::vector<TCodeInj*> vInjections;
	void* pAddress = nullptr;
	void* pCode = nullptr;
	size_t szTarget = 0; // how many bytes should be overwritten
	TCodeBuffer cbCode;
	std::vector<uint8_t> vOgBytes;
	bool bEnabled = false;
public:
	TCodeInj( void* pAddress, size_t size, TCodeBuffer& code );
	~TCodeInj();

	void Toggle();
	
};

class TInjManager
{
	std::vector<TCodeInj*> vInjs;
public:
	void AddInjection( void* pAddress, size_t size, TCodeBuffer& code );
	void ToggleAll();
	void Destroy();
};

#pragma endregion

#pragma region TMem

class TMem
{
	static TMem* pMem;
private:
	TMem();
public:
	~TMem();
	static TMem* Get();
	static void Destroy();

	template<class T>
	bool Read( void* pAddress, T* pBuffer );

	template<class T>
	bool Read( void* pAddress, T* pBuffer, size_t uSize );

	template<class T>
	bool Write( void* pAddress, T value );

	template<class T>
	bool Write( void* pAddress, T* buffer, size_t size );

	bool Nop( void* pAddress, size_t size );

public: // Scan
	// Scan for first instance of pattern
	void* ScanPattern( const char* sPattern );
	void* ScanPattern( const char* sPattern, const char* sMask );
	void* ScanPattern( HMODULE hModule, const char* sPattern );
	void* ScanPattern( HMODULE hModule, const char* sPattern, const char* sMask );
	void* ScanPattern( TPattern & pattern );
	void* ScanPattern( HMODULE hModule, TPattern & pattern );

	// Scan for multiples of a pattern?
	int ScanPatternMulti( const char* sPattern, std::vector<void*>& vResults );
	int ScanPatternMulti( const char* sPattern, const char* sMask, std::vector<void*>& vResults );
	int ScanPatternMulti( HMODULE hModule, const char* sPattern, std::vector<void*>& vResults );
	int ScanPatternMulti( HMODULE hModule, const char* sPattern, const char* sMask, std::vector<void*>& vResults );
	int ScanPatternMulti( TPattern & pattern, std::vector<void*> & vResults );
	int ScanPatternMulti( HMODULE hModule, TPattern & pattern, std::vector<void*> & vResults );

private:
	void* Scan( void* pAddress, size_t size, TPattern & pattern );
};

template<class T>
inline bool TMem::Read( void * pAddress, T * pBuffer )
{
	return Read( pAddress, pBuffer, sizeof( T ) );
}

template<class T>
inline bool TMem::Read( void * pAddress, T * pBuffer, size_t uSize )
{
	MEMORY_BASIC_INFORMATION mbi{ 0 };
	if ( !NT::QueryVirtualMemory( pAddress, &mbi ) )
		return false;

	if ( mbi.State == MEM_FREE )
		return false;

	DWORD dwOld = 0;
	void* pBaseAddress = mbi.BaseAddress;
	if ( !NT::ProtectVirtualMemory( pAddress, PAGE_EXECUTE_READWRITE, &dwOld ) )
		return false;

	memcpy( pBuffer, pAddress, uSize );

	NT::ProtectVirtualMemory( pAddress, dwOld, &dwOld );
	return true;
}

template<class T>
inline bool TMem::Write( void * pAddress, T value )
{
	return Write( pAddress, &value, sizeof( value ) );
}

template<class T>
inline bool TMem::Write( void * pAddress, T * buffer, size_t size )
{
	DWORD dwOld = 0;
	if ( !NT::ProtectVirtualMemory( pAddress, PAGE_EXECUTE_READWRITE, &dwOld ) )
		return false;

	memcpy( pAddress, buffer, size );

	NT::ProtectVirtualMemory( pAddress, dwOld, &dwOld );
	return true;
}

#pragma endregion

#pragma region TSyscall

class TSyscall
{
	static void* pCodeLoc;
public:

	template<class T>
	static std::function<T> GetInvoke( const char* sFunction, T* pAddress = nullptr );
	
	static void Destroy();
};

template<class T>
std::function<T> TSyscall::GetInvoke( const char* sFunction, T * pAddress )
{
	auto pMem = TMem::Get();
	if ( !pCodeLoc )
	{
		//pCodeLoc = VirtualAlloc( nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
		auto ntAllocateVirtualMemory = (fnNtAllocateVirtualMemory) NT::GetProcedureAddress( _X( "ntdll.dll" ), _X( "NtAllocateVirtualMemory" ) );
		SIZE_T dwSize = 0x1000;
		NTSTATUS ntStatus = ntAllocateVirtualMemory( HANDLE( -1 ), &pCodeLoc, (PULONG)0, &dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

#ifdef _WIN64
		BYTE cb[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x05, 0xC3 };
		memcpy( pCodeLoc, cb, ARRAYSIZE( cb ) );
#endif
	}

	auto pStub = (BYTE*) NT::GetProcedureAddress( _X( "ntdll.dll" ), sFunction );
#ifdef _WIN64
	*((DWORD*) pCodeLoc + 1) = *((DWORD*) pStub + 1);
#else
	memcpy( pCodeLoc, pStub, 15 );
#endif
	return std::function<T>( (T*) pCodeLoc );
}

#pragma endregion
