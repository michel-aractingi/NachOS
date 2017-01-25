#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"
#include <new>

#define TransferSize 	10 	// make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void
Copy(char *from, char *to)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {
        printf("Copy: couldn't open input file %s\n", from);
        return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

// Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    if (!fileSystem->Create(to, fileLength, 1)) {	 // Create Nachos file
        printf("Copy: couldn't create output file %s\n", to);
        fclose(fp);
        return;
    }

    openFile = fileSystem->Open(to, 1);
    ASSERT(openFile != NULL);

    printf("file length: %d\n", fileLength);

// Copy the data in TransferSize chunks
    int total = 0;
    buffer = new(std::nothrow) char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0) {
        // printf("amount read: %d\n", amountRead);
        // for(int i = 0; i < amountRead; ++i)
        //     putchar(buffer[i]);
        total += openFile->Write(buffer, amountRead);
        // printf("total amount written: %d\n", total);
    }
    delete [] buffer;

    printf("finsihed copying file\n");

// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void
Print(char *name)
{
    OpenFile *openFile;
    int i, amountRead;
    char *buffer;

    if ((openFile = fileSystem->Open(name, 1)) == NULL) {
        printf("Print: unable to open file %s\n", name);
        return;
    }

    buffer = new(std::nothrow) char[TransferSize];
    while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
        for (i = 0; i < amountRead; i++) {
            printf("%c", buffer[i]);
            fflush(stdout);
        }
    delete [] buffer;

    delete openFile;		// close the Nachos file
    return;
}

void
MakeDirectory(char *name) {
    if(!fileSystem->MakeDir(name, 0, 1))
        printf("error: could not create directory\n");
    else
        printf("created directory\n");
    return ;
}

void Lists() {
    fileSystem->List(1);
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName 	"TestFile"
#define Contents 	"1234567890"
#define ContentSize 	strlen(Contents)
#define FileSize 	((int)(ContentSize * 50))

static void
FileWrite()
{
    OpenFile *openFile;
    int i, numBytes;

    printf("Sequential write of %d byte file, in %d byte chunks\n",
           FileSize, ContentSize);
    if (!fileSystem->Create((char *)FileName, FileSize, 1)) {
        printf("Perf test: can't create %s\n", FileName);
        return;
    }
    openFile = fileSystem->Open((char *)FileName, 1);
    if (openFile == NULL) {
        printf("Perf test: unable to open %s\n", FileName);
        return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Write((char *)Contents, ContentSize);
        if (numBytes < 10) {
            printf("Perf test: unable to write %s\n", FileName);
            delete openFile;
            return;
        }
    }
    delete openFile;	// close file
}

static void
FileRead()
{
    OpenFile *openFile;
    char *buffer = new(std::nothrow) char[ContentSize];
    int i, numBytes;

    printf("Sequential read of %d byte file, in %d byte chunks\n",
           FileSize, ContentSize);

    if ((openFile = fileSystem->Open((char *)FileName, 1)) == NULL) {
        printf("Perf test: unable to open file %s\n", FileName);
        delete [] buffer;
        return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Read(buffer, ContentSize);
        if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
            printf("Perf test: unable to read %s\n", FileName);
            delete openFile;
            delete [] buffer;
            return;
        }
    }
    delete [] buffer;
    delete openFile;	// close file
}

void
PerformanceTest()
{
    printf("Starting file system performance test:\n");
    stats->Print();
    FileWrite();
    FileRead();
    if (!fileSystem->Remove((char *)FileName, 1)) {
        printf("Perf test: unable to remove %s\n", FileName);
        return;
    }
    stats->Print();
}

void ParseArguments(char *buffer, const char s[], int *number_of_arguments, char **tokens){

    char *token;
    token = strtok(buffer, s);
    tokens[0] = token;
    int counter = 1;
    while( tokens != NULL )
    {
        token = strtok(NULL, s);
        if(token != NULL) {
            tokens[counter++] = token;
        }
        else{
            break;
        }
    }
    *number_of_arguments = counter;
}

void PrintArguments(char **arguments, int number_of_arguments){
    for(int i=0;i< number_of_arguments;i++){
        printf("%d : %s\n",i,arguments[i]);
    }
}

void FileSystemShell(){

    char buffer[160];
    const char s[5] = " \n";
    char *arguments[80];
    int number_of_arguments = 0;
    int sector = 1;
    while(1){
        printf("Shell > ");
        if (fgets(buffer, 80 , stdin) == NULL) break;
        ParseArguments(buffer, s, &number_of_arguments,arguments);
        if(!strcmp(arguments[0], "ls" )){
            Lists();
        }
        else if(!strcmp(arguments[0], "cd")){

            fileSystem->ChangeDir(arguments[1],sector);
        }
        else if(!strcmp(arguments[0], "cp")){
            //Copy(arguments[1],arguments[2]);
        }
        else if(!strcmp(arguments[0], "rm")){
            fileSystem->Remove(arguments[1],1);
        }
        else if(!strcmp(arguments[0], "mkdir")){
            //fileSystem->MakeDirectory(arguments[1],0,1);
        }
        else if(!strcmp(arguments[0], "p")){
            fileSystem->Print();
        }
        else if(!strcmp(arguments[0], "pwd")){
            //fileSystem->WorkingDirectory();
        }
        else if(!strcmp(arguments[0], "gs")){
            //int data = (int) arguments[1][0] - 48;
            //fileSystem->GoToSector(data);
        }
        else if(!strcmp(arguments[0], "pd")){
            //int data = (int) arguments[1][0] - 48;

            //fileSystem->List(data);
        }
        else if(!strcmp(arguments[0], "ud")){
            //int data = (int) arguments[1][0] - 48;
            //fileSystem->UpdateDirectory(data);
        }
        else{
            printf("Invalid Command%s1",arguments[0]);
        }
    }
}
