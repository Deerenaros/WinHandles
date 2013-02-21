#include "WinAPIWrapper.h"

void printBinary ( int x );

int main () {
	PSYSTEM_HANDLE h;
	HandleEnumarator hEn;
	ObjTypeMap objTypeMap;
	try {
		int i = 0;
		while ( ( h = hEn.getNext () ) != NULL ) {
			if ( ! ( i++ % 10 ) ) {
				Sleep ( 250 );
				printf ( "\t\t\tObjecSpecAccessRights\tStdAcRghAsRsvAEWR-G\n" ); //http://goo.gl/UC6kl
			}
			printf ( "%s (%d)\t",
				objTypeMap.getObjectTypeNameFromNumber( h->ObjectTypeNumber ),
				h->ObjectTypeNumber);
			printBinary ( h->GrantedAccess );
			printf ( "   0%04x\n", h->Handle );
		}
	} catch ( Exception e ) {
		printf ( "Unexpected error: %s %s\n", e.getName (), e.getDescription () );
		return -1;
	}

	return 0;
}

void printBinary ( int x ) {
	char binary [] = "0000000000000000\t00000000 00000000";
	int k = 1;
	for ( int i = 0; i < 32; i++ ) {
		if ( x & k ) {
			int shift = ( i > 15 ? 1 : 0 ) + ( i > 15+8 ? 1 : 0 );
			binary [ i+shift ] = '1';
		}
		k <<= 1;

	}

	printf ( "%s", binary );
}