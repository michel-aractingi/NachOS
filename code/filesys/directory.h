#include "copyright.h"

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "openfile.h"

#define INCREASE_FACTOR 2   // factor by which the capacity of the directory will expand by
#define FileNameMaxLen 		9	// for simplicity, we assume
// file names are <= 9 characters long

// The following class defines a "directory entry", representing a file
// in the directory.  Each entry gives the name of the file, and where
// the file's header is to be found on disk.
//
// Internal data structures kept public so that Directory operations can
// access them directly.

class DirectoryEntry {
public:
    bool isDir;
    bool inUse;				// Is this directory entry in use?
    int sector;				// Location on disk to find the
    //   FileHeader for this file
    char name[FileNameMaxLen + 1];	// Text name for file, with +1 for
    // the trailing '\0'
};

// The following class defines a UNIX-like "directory".  Each entry in
// the directory describes a file, and where to find it on disk.
//
// The directory data structure can be stored in memory, or on disk.
// When it is on disk, it is stored as a regular Nachos file.
//
// The constructor initializes a directory structure in memory; the
// FetchFrom/WriteBack operations shuffle the directory information
// from/to disk.

class Directory {
public:
    Directory(int size); 		// Initialize an empty directory
    // with space for "size" files
    ~Directory();			// De-allocate the directory

    void FetchFrom(OpenFile *file);  	// Init directory contents from disk
    void WriteBack(OpenFile *file);	// Write modifications to
    // directory contents back to disk

    int Find(char *name);		// Find the sector number of the
    // FileHeader for file: "name"

    bool isDirectory(char *name);

    bool Add(char *name, int newSector);  // Add a file name into the directory

    bool AddDirectory(char *name, int newSector);

    bool Remove(char *name);		// Remove a file from the directory

    void List(int tabs);			// Print the names of all the files
    //  in the directory
    void Print();			// Verbose print of the contents
    //  of the directory -- all the file
    //  names and their contents.

    void Expand(int size);

private:
    int tableSize;			// Number of directory entries
    DirectoryEntry *table;		// Table of pairs:
    // <file name, file header location>

    int FindIndex(char *name);		// Find the index into the directory
    //  table corresponding to "name"
};

#endif // DIRECTORY_H
