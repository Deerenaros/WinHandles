#include "WinAPIWrapper.h"

#pragma region ObjTypeMap
ObjTypeMap::ObjTypeMap () {
// i don't know, which number match to every kernel object type
// so, i'd used this copy-paste from one of internet-forum
	typeNames = new string[32];
	typeNames [ 0 ] = "NULL           ",
	typeNames [ 1 ] = "Not identified ",
	typeNames [ 2 ] = "Directory      ",
	typeNames [ 3 ] = "Symbolic Link  ",
	typeNames [ 4 ] = "Token          ",
	typeNames [ 5 ] = "Process        ",
	typeNames [ 6 ] = "Thread         ",
	typeNames [ 7 ] = "Job            ",
	typeNames [ 8 ] = "Not identified ",
	typeNames [ 9 ] = "Event          ",
	typeNames [ 10 ] = "Not identified ",
	typeNames [ 11 ] = "Mutant         ",
	typeNames [ 12 ] = "Not identified ",
	typeNames [ 13 ] = "Semaphore      ",
	typeNames [ 14 ] = "Timer          ",
	typeNames [ 15 ] = "Not identified ",
	typeNames [ 16 ] = "Keyed event    ",
	typeNames [ 17 ] = "Windows station",
	typeNames [ 18 ] = "Desktop        ",
	typeNames [ 19 ] = "Section        ",
	typeNames [ 20 ] = "Registry key   ",
	typeNames [ 21 ] = "Port           ",
	typeNames [ 22 ] = "Waitable       ",
	typeNames [ 23 ] = "Not identified ",
	typeNames [ 24 ] = "Not identified ",
	typeNames [ 25 ] = "Not identified ",
	typeNames [ 26 ] = "Not identified ",
	typeNames [ 27 ] = "IO Completion  ",
	typeNames [ 28 ] = "File           ",
	typeNames [ 29 ] = "WMI GUID       ",
	typeNames [ 30 ] = "Not identified ",
	typeNames [ 31 ] = "Not identified ";
}

ObjTypeMap::~ObjTypeMap () {
	//delete typeNames; //it's strange...
}

const char *ObjTypeMap::getObjectTypeNameFromNumber ( int n ) const {
	if ( n < 32 ) {
		return typeNames [ n ].c_str ();
	}

	return "Not identified ";
}
#pragma endregion

HandleEnumarator::HandleEnumarator () : m_current ( 0 ) {
	HINSTANCE hNtDll = LoadLibraryA ( "ntdll.dll" );
	if ( hNtDll == NULL ) {
		throw Exception ( "LoadLibrary", "Can not load ntdll.dll" );
	}

	PFN_NTQUERYSYSTEMINFORMATION NtQuerySystemInformation =  ( PFN_NTQUERYSYSTEMINFORMATION ) GetProcAddress ( hNtDll, "NtQuerySystemInformation" );
	if ( NtQuerySystemInformation == NULL ) {
		throw Exception ( "NtQuerySystemInformation", "Can not query system information" );
	}

	// Getting handles
	DWORD nSize = 4096, nReturn;
	m_sysHandleInfo = (PSYSTEM_HANDLE_INFORMATION) HeapAlloc ( GetProcessHeap (), 0, nSize );

	while ( NtQuerySystemInformation ( SystemHandleInformation, m_sysHandleInfo, nSize, &nReturn ) == STATUS_INFO_LENGTH_MISMATCH ) {
		HeapFree ( GetProcessHeap (), 0, m_sysHandleInfo );
		nSize *= 2;
		m_sysHandleInfo = ( SYSTEM_HANDLE_INFORMATION* ) HeapAlloc ( GetProcessHeap (), 0, nSize );
	}
}

HandleEnumarator::~HandleEnumarator () {
	HeapFree ( GetProcessHeap(), 0, m_sysHandleInfo );
}

const PSYSTEM_HANDLE HandleEnumarator::getNext () {
	if ( m_current < m_sysHandleInfo->NumberOfHandles ) {
		return &( m_sysHandleInfo->Handles [ m_current++ ] );
	} else {
		return NULL;
	}
}