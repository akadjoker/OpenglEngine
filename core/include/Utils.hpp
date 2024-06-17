#pragma once

#include "Config.hpp"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
    

#define GLSL(src) "#version 400 core\n" #src


const char *strprbrk(const char *s, const char *charset);

bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);
char* LoadFileText(const char *fileName);

unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);
 bool SaveFileText(const char *fileName, char *text);

void LogError( const char *msg, ... );
void LogInfo( const char *msg, ... );
void LogWarning( const char *msg, ... );


class  CORE_PUBLIC Logger 
{
    public:
        Logger() ;
        virtual ~Logger() ;

        void Error( const char *msg, ... );
        void Warning( const char *msg, ... );
        void Info( const char *msg, ... );
     
         static Logger& Instance();
         static Logger* InstancePtr();

    private:
        static Logger* m_singleton;

    
};

class  CORE_PUBLIC  System 
{
    public:
        System();
        virtual ~System();

        static System& Instance();
        static System* InstancePtr();


     void Sleep(unsigned int ms);
     void DebugBreak();
     void Abort();
     void Exit(int code);
     void SetEnvironment(const char* name, const char* value);
     const char* GetEnvironment(const char* name);
   
     void* LoadLibrary(const char* path);
     void  FreeLibrary(void* handle);
     void* GetProcAddress(void* handle, const char* name);
 
    bool FileExists(const char *fileName);
    bool DirectoryExists(const char *dirPath);              
    bool IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
    int GetFileLength(const char *fileName);                    
     const char *TextFormat(const char *text, ...);
    const char *GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
    const char *GetFileName(const char *filePath);              // Get pointer to filename for a path string
    const char *GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
    const char *GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)
    const char *GetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses static string)
    
    char *GetWorkingDirectory(void);                      // Get current working directory (uses static string)
    char *GetApplicationDirectory(void);                  // Get the directory if the running application (uses static string)


    bool ChangeDirectory(const char *dir);                      // Change working directory, return true on success
    bool IsPathFile(const char *path);                          // Check if a given path is a file or a directory

    
    unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);      
    bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);  
    char *LoadFileText(const char *fileName);                   
    bool SaveFileText(const char *fileName, char *text);        

    bool LoadDirectoryFiles(const char *dirPath,  std::vector<std::string> &files);
    bool LoadDirectoryFiles(const char *dirPath, const char *filter, std::vector<std::string> &files);
    bool LoadDirectoryFiles(const char *dirPath, const char *filter, bool scanSubdirs,  std::vector<std::string> &files);    

    u64 GetFileModTime(const char *fileName);       

 

private:
    bool ScanDirectoryFilesRecursively(const char *basePath,   std::vector<std::string> &files, const char *filter);
    bool ScanDirectoryFiles(const char *basePath,   std::vector<std::string> &files, const char *filter);

    static System* m_singleton;
 


};
