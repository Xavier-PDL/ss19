#pragma once
#include "Types.h"
#include <Engine/Templates/DynamicStackArray.h>
/*
 * CroTeam stream class -- abstract base class
 */
class CTStream {
public:
	CListNode strm_lnListNode;  // for linking into main library's list of opened streams
public:
	CTString strm_strStreamDescription; // descriptive string

	enum DictionaryMode {
		DM_NONE,        // no dictionary on this file (yet)
		DM_ENABLED,     // dictionary is enabled, reading/writing rest of file
		DM_PROCESSING,  // reading/writing the dictionary itself
	} strm_dmDictionaryMode;    // dictionary mode of operation on this file
	SLONG strm_slDictionaryPos; // dictionary position in file (0 for no dictionary)
	INDEX strm_ctDictionaryImported;  // how many filenames were imported
	class CNameTable_CTFileName &strm_ntDictionary;  // name table for the dictionary
	CDynamicStackArray<CTFileName> strm_afnmDictionary; // dictionary is stored here

	///* Throw an exception of formatted string. */
	//void Throw_t( char *strFormat, ... ); // throw char *
	//// read the dictionary from given offset in file (internal function)
	//void ReadDictionary_intenal_t( SLONG slOffset );
	//// copy filename dictionary from another stream
	//void CopyDictionary( CTStream &strmOther );
public:
  // modes for opening streams
	enum OpenMode {
		OM_READ = 1,
		OM_WRITE = 2,
		OM_READTEXT = OM_READ,
		OM_WRITETEXT = OM_WRITE,
		OM_READBINARY = OM_READ,
		OM_WRITEBINARY = OM_WRITE,
	};
	enum CreateMode { // OBSOLETE!
		CM_TEXT = 1,
		CM_BINARY = 2,
	};
	// direction for seeking
	enum SeekDir {
		SD_BEG = SEEK_SET,
		SD_END = SEEK_END,
		SD_CUR = SEEK_CUR,
	};

//	/* Static function enable stream handling. */
//	static void EnableStreamHandling( void );
//	/* Static function disable stream handling. */
//	static void DisableStreamHandling( void );
//
//#ifdef PLATFORM_WIN32 /* rcg10042001 !!! FIXME */
//  /* Static function to filter exceptions and intercept access violation */
//	static int ExceptionFilter( DWORD dwCode, _EXCEPTION_POINTERS *pExceptionInfoPtrs );
//#endif
//
//  /* Static function to report fatal exception error. */
//	static void ExceptionFatalError( void );
//
	///* Default constructor. */
	//CTStream( void ) = default;
	///* Destruction. */
	//virtual ~CTStream( void ) = 0;

	///* Check if the stream can be read. -- used mainly for assertions */
	//virtual BOOL IsReadable( void ) = 0;
	///* Check if the stream can be written. -- used mainly for assertions */
	//virtual BOOL IsWriteable( void ) = 0;
	///* Check if the stream can be seeked. -- used mainly for assertions */
	//virtual BOOL IsSeekable( void ) = 0;

	///* Read a block of data from stream. */
	//virtual void Read_t( void *pvBuffer, SLONG slSize ) = 0; // throw char *
	///* Write a block of data to stream. */
	//virtual void Write_t( const void *pvBuffer, SLONG slSize ) = 0; // throw char *

	///* Seek in stream. */
	//virtual void Seek_t( SLONG slOffset, enum SeekDir sd ) = 0; // throw char *
	///* Set absolute position in stream. */
	//virtual void SetPos_t( SLONG slPosition ) = 0; // throw char *
	///* Get absolute position in stream. */
	//virtual SLONG GetPos_t( void ) = 0; // throw char *
	///* Get size of stream */
	//virtual SLONG GetStreamSize( void ) = 0;
	///* Get CRC32 of stream */
	//virtual ULONG GetStreamCRC32_t( void ) = 0;
	///* Check if file position points to the EOF */
	//virtual BOOL AtEOF( void ) = 0;

	///* Get description of this stream (e.g. filename for a CFileStream). */
	//inline CTString &GetDescription( void ) { return strm_strStreamDescription; };

	///* Read an object from stream. */
	//inline CTStream &operator>>( float  &f ) { Read_t( &f, sizeof( f ) ); return *this; } // throw char *
	//inline CTStream &operator>>( double &d ) { Read_t( &d, sizeof( d ) ); return *this; } // throw char *
	//inline CTStream &operator>>( ULONG &ul ) { Read_t( &ul, sizeof( ul ) ); return *this; } // throw char *
	//inline CTStream &operator>>( UWORD &uw ) { Read_t( &uw, sizeof( uw ) ); return *this; } // throw char *
	//inline CTStream &operator>>( UBYTE &ub ) { Read_t( &ub, sizeof( ub ) ); return *this; } // throw char *
	//inline CTStream &operator>>( SLONG &sl ) { Read_t( &sl, sizeof( sl ) ); return *this; } // throw char *
	//inline CTStream &operator>>( SWORD &sw ) { Read_t( &sw, sizeof( sw ) ); return *this; } // throw char *
	//inline CTStream &operator>>( SBYTE &sb ) { Read_t( &sb, sizeof( sb ) ); return *this; } // throw char *
	//inline CTStream &operator>>( BOOL   &b ) { Read_t( &b, sizeof( b ) ); return *this; } // throw char *
	///* Write an object into stream. */
	//inline CTStream &operator<<( const float  &f ) { Write_t( &f, sizeof( f ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const double &d ) { Write_t( &d, sizeof( d ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const ULONG &ul ) { Write_t( &ul, sizeof( ul ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const UWORD &uw ) { Write_t( &uw, sizeof( uw ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const UBYTE &ub ) { Write_t( &ub, sizeof( ub ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const SLONG &sl ) { Write_t( &sl, sizeof( sl ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const SWORD &sw ) { Write_t( &sw, sizeof( sw ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const SBYTE &sb ) { Write_t( &sb, sizeof( sb ) ); return *this; } // throw char *
	//inline CTStream &operator<<( const BOOL   &b ) { Write_t( &b, sizeof( b ) ); return *this; } // throw char *

	//// CTFileName reading/writing
	//ENGINE_API friend CTStream &operator>>( CTStream &strmStream, CTFileName &fnmFileName );
	//ENGINE_API friend CTStream &operator<<( CTStream &strmStream, const CTFileName &fnmFileName );

	///* Put a line of text into stream. */
	//virtual void PutLine_t( const char *strBuffer ); // throw char *
	//virtual void PutString_t( const char *strString ); // throw char *
	//virtual void FPrintF_t( const char *strFormat, ... ); // throw char *
	///* Get a line of text from stream. */
	//virtual void GetLine_t( char *strBuffer, SLONG slBufferSize, char cDelimiter = '\n' ); // throw char *
	//virtual void GetLine_t( CTString &strLine, char cDelimiter = '\n' ); // throw char *

	//virtual CChunkID GetID_t( void ); // throw char *
	//virtual CChunkID PeekID_t( void ); // throw char *
	//virtual void ExpectID_t( const CChunkID &cidExpected ); // throw char *
	//virtual void ExpectKeyword_t( const CTString &strKeyword ); // throw char *
	//virtual SLONG GetSize_t( void ); // throw char *
	//virtual void ReadRawChunk_t( void *pvBuffer, SLONG slSize ); // throw char *
	//virtual void ReadChunk_t( void *pvBuffer, SLONG slExpectedSize ); // throw char *
	//virtual void ReadFullChunk_t( const CChunkID &cidExpected, void *pvBuffer, SLONG slExpectedSize ); // throw char *
	//virtual void *ReadChunkAlloc_t( SLONG slSize = 0 ); // throw char *
	//virtual void ReadStream_t( CTStream &strmOther ); // throw char *

	//virtual void WriteID_t( const CChunkID &cidSave ); // throw char *
	//virtual void WriteSize_t( SLONG slSize ); // throw char *
	//virtual void WriteRawChunk_t( void *pvBuffer, SLONG slSize ); // throw char *  // doesn't write length
	//virtual void WriteChunk_t( void *pvBuffer, SLONG slSize ); // throw char *
	//virtual void WriteFullChunk_t( const CChunkID &cidSave, void *pvBuffer, SLONG slSize ); // throw char *
	//virtual void WriteStream_t( CTStream &strmOther ); // throw char *

	//// whether or not the given pointer is coming from this stream (mainly used for exception handling)
	//virtual BOOL PointerInStream( void* pPointer );

	//// filename dictionary operations

	//// start writing a with a dictionary
	//void DictionaryWriteBegin_t( const CTFileName &fnmImportFrom, SLONG slImportOffset );
	//// stop writing a with a dictionary
	//void DictionaryWriteEnd_t( void );
	//// start reading a with a dictionary
	//SLONG DictionaryReadBegin_t( void );  // returns offset of dictionary for cross-file importing
	//// stop reading a with a dictionary
	//void DictionaryReadEnd_t( void );
	//// preload all files mentioned in the dictionary
	//void DictionaryPreload_t( void );
};

/*
 * CroTeam file stream class
 */
class CTFileStream : public CTStream {
//private:
public:
	FILE *fstrm_pFile;    // ptr to opened file

	INDEX fstrm_iZipHandle; // handle of zip-file entry
	INDEX fstrm_iZipLocation; // location in zip-file entry
	UBYTE* fstrm_pubZipBuffer; // buffer for zip-file entry
	SLONG fstrm_slZipSize; // size of the zip-file entry

	BOOL fstrm_bReadOnly;  // set if file is opened in read-only mode
//public:
//  /* Default constructor. */
//	CTFileStream( void );
//	/* Destructor. */
//	virtual ~CTFileStream( void );
//
//	/* Open an existing file. */
//	void Open_t( const CTFileName &fnFileName, enum CTStream::OpenMode om = CTStream::OM_READ ); // throw char *
//	/* Create a new file or overwrite existing. */
//	void Create_t( const CTFileName &fnFileName, enum CTStream::CreateMode cm = CTStream::CM_BINARY ); // throw char *
//	/* Close an open file. */
//	void Close( void );
//	/* Get CRC32 of stream */
//	ULONG GetStreamCRC32_t( void );
//
//	/* Read a block of data from stream. */
//	void Read_t( void *pvBuffer, SLONG slSize ); // throw char *
//	/* Write a block of data to stream. */
//	void Write_t( const void *pvBuffer, SLONG slSize ); // throw char *
//
//	/* Seek in stream. */
//	void Seek_t( SLONG slOffset, enum SeekDir sd ); // throw char *
//	/* Set absolute position in stream. */
//	void SetPos_t( SLONG slPosition ); // throw char *
//	/* Get absolute position in stream. */
//	SLONG GetPos_t( void ); // throw char *
//	/* Get size of stream */
//	SLONG GetStreamSize( void );
//	/* Check if file position points to the EOF */
//	BOOL AtEOF( void );
//
//	// whether or not the given pointer is coming from this stream (mainly used for exception handling)
//	virtual BOOL PointerInStream( void* pPointer );
//
//	// from CTStream
//	inline virtual BOOL IsWriteable( void ) { return !fstrm_bReadOnly; };
//	inline virtual BOOL IsReadable( void ) { return TRUE; };
//	inline virtual BOOL IsSeekable( void ) { return TRUE; };
};