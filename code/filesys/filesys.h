#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "openfile.h"
#include "filetable.h"
#include <new>

// these variables are no longer apart of the FileSystem class because of
// needed changes to make extendable files work
extern OpenFile* freeMapFile;   // Bit map of free disk blocks,
// represented as a file
extern OpenFile* directoryFile;   // "Root" directory -- list of
// file names, represented as a file

OpenFile *GetFreeMapFile();   // getter method
OpenFile *GetDirectoryFile(); // getter method
#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {
  public:
    FileSystem(bool format) {}

    bool Create(const char *name, int initialSize) { 
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

    OpenFile* Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  return new OpenFile(fileDescriptor);
      }

    bool Remove(char *name) { return Unlink(name) == 0; }

};

#else // FILESYS


class FileSystem {
public:
    FileSystem(bool format);		// Initialize the file system.
    // Must be called *after* "synchDisk"
    // has been initialized.
    // If "format", there is nothing on
    // the disk, so initialize the directory
    // and the bitmap of free blocks.

    bool Create(char *name, int initialSize, int wdSector);
    // Create a file (UNIX creat)

    bool MakeDir(char *name, int initialSize, int wdSector);

    int ChangeDir(char *name, int wdSector);

    OpenFile* Open(char *name, int wdSector); 	// Open a file (UNIX open)

    bool Remove(char *name, int wdSector);  		// Delete a file (UNIX unlink)

    void List(int dirSector);			// List all the files in the file system

    void Print();			// List all the files and their contents
    void Close(char *name,int wdSector);

    OpenFileTable *globalFileTable;

};
#endif
#endif // FS_H
