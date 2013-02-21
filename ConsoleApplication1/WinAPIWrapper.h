#ifndef WinAPIWrapper
#define WinAPIWrapper
#pragma region Includes
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <psapi.h>
#include <strsafe.h>
#include <assert.h>
#include <winternl.h>
#pragma endregion

#define BUFFER_SIZE						512
#define STATUS_SUCCESS					((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH		((NTSTATUS)0xc0000004L)

using std::string;

const SYSTEM_INFORMATION_CLASS SystemHandleInformation = (SYSTEM_INFORMATION_CLASS)16;

typedef NTSTATUS ( WINAPI * PFN_NTQUERYSYSTEMINFORMATION ) (
	IN	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT	PVOID                    SystemInformation,
	IN	ULONG                    SystemInformationLength,
	OUT	PULONG                   ReturnLength OPTIONAL
);

typedef struct _SYSTEM_HANDLE {
	ULONG ProcessId;
	UCHAR ObjectTypeNumber;
	UCHAR Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG NumberOfHandles;
	SYSTEM_HANDLE Handles [ 1 ];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef NTSTATUS ( WINAPI * PFN_NTQUERYINFORMATIONFILE ) (
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
	);

typedef struct _FILE_NAME_INFORMATION {
	ULONG FileNameLength;
	WCHAR FileName [ 1 ];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;


BOOL GetFileNameFromHandle ( HANDLE hFile );

class ObjTypeMap {
	string *typeNames;
public:
	ObjTypeMap ();
	~ObjTypeMap ();
	const char *getObjectTypeNameFromNumber ( int n ) const;
};

class HandleEnumarator {
	PSYSTEM_HANDLE_INFORMATION m_sysHandleInfo;
	unsigned m_current;
public:
	HandleEnumarator ();
	~HandleEnumarator ();
	const PSYSTEM_HANDLE getNext ();
};

class Exception {
	string m_name;
	string m_descript;
public:
	Exception ( char *name, char *description = "" ) {
		m_name = name; m_descript = description;
	}
	const char *getName () const {
		return m_name.c_str ();
	}

	const char *getDescription () const {
		return m_descript.c_str ();
	}
};
#endif