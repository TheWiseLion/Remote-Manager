#include <winsock2.h>
#include <process.h>
#include <time.h>
#include <iostream>
#include <io.h>
#define WIN32_MEAN_AND_LEAN
#include <WinSock2.h>
#include <Psapi.h>
#include <windows.h>
#include "zlib.h"
#include "zconf.h"
#include <string>
#include <sstream>

#include <string.h>
#include <math.h>
#pragma once
/* NETWORKING LIBRARY 100% BASED ON objects */


static int UniqueId=0;
static int MAXCONNECTIONSOCKETS=1000; // no more than 1000 connection assocated with 1 connection

///Functions? Find socket? to change remote socket data?

WSADATA StartUp(){ //easy to change when conversion is needed.
	
	WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        throw("Network.c Error: WinSock failed to launch.\n");
    }
	return wsaData;
}


enum class SocketType{UDP,TCP};
enum class InnerTask{In,Out,Both};
enum class AddrType{Slocal,Blocal,IPV4, IPV6};
enum class SocketStatus{Connected,Connecting,Disconnected=-1,Completed};
enum class ConnectionType{Outbound,Inbound};
enum class ConnectionMethod{Direct,Search,Remote,Inbounded};//Applied to Outbound, if inbound use Inbounded
enum class RemoteAccess{On,Off};
enum class SocketPurpose{Reg,VNC,UserDefined};  // what does userdefined mean?
namespace compile{
	enum class Status{Disconnected,Connected,Finished};
};
// general task (VNC/Client/Server)then inner task (sending data out/Data In/ Data both)
using namespace compile;
struct SocketID{
	int uniqueID; // this gets filled by uniquedata
	AddrType AdressType;
	SocketType SockType;
	ConnectionType MyConnType;
	ConnectionMethod MyConnMethod;
	SocketPurpose MyPurpose;
	compile::Status MyStatus;
	RemoteAccess MyAccess;
	int TimeOut; // time in seconds till connection will timeout (send, rec)
//	struct AllocatedTask SocketTask;
	ConnectionMethod ThisDirect;
	ConnectionMethod ThisSearch;
	ConnectionMethod ThisRemote;
	ConnectionMethod ThisLoginRemote;

};


struct ConnectionInfo{
	// client, server, VNC out, VNC In
	string IP; // if connection being tunnled this info will be IP of the connection not the socket
	string IPofFinal;
	string Name;
	string ConnectedToName;
	string PcName;
	string MyPcName;
	int port;
	int ConnectionID;
	struct SocketID SocketInfo;


};

//Keeps track of unique data and helps assign new unique data (primarily socket ID's)
class UniqueData{
	// self creating  
	friend class SmartSocks;
	friend class ConnectionManager;
	friend class SmartSock;
	friend class Connection;

	




};



		//////all connections get unique id's
//holds all socket data based on some ideas of vectors (each socket is not an object that has:
// location (where it connects to), ecryption, task (can be arbitrary)	

// holds just socket data relevent to socketID structure


// winsock structures are started here.
class SmartSocket{
	friend class SmartSockets;
	public:
		struct SocketID ThisSocketInfo;
		
	



	private:
	SOCKET smartsocket;
	
	void AttachSocket(SocketID ConnectingInfo){
		//if (ThisSocket==NULL)
		//throw error



	}






};


class SmartSockets{
	public:
		




	private:
	int SockNum; //number of sockets
	SmartSocket * MySmartSockets;
	

};

class Connection{
	public:
	
	
	struct ConnectionInfo;

	Connection(){




	}

	SmartSockets Connect(string IP,int Port){



	}



	Connection & operator=(const Connection &Right){ // = sign operator overloading to change what connection references 
		//copy constructor aka point to guy next to me
		*this=Right; // is this right?

	}

	private:
	SmartSockets SockMgr;
	int SocketCounter; 
	

};

class ConnectionManager{

private:
	UniqueData UniqueSocketData;
	WSADATA MyWsa;
	Connection * Connections;
	static int ConnectionCounter;
public:
	ConnectionManager(){
		try{
			MyWsa=StartUp();
		}catch(string Msg){
			// do something with this msg
			cout<<Msg<<endl;
		}
	}
	Connection CreateConnection(){
		Connections=new Connection;
		ConnectionCounter++;
		return Connections[ConnectionCounter-1];

	}
};


/// things id like to beable to type
	// SmartSock.Socket3.task() == VNCSOCKET
	// SmartSock.Socket3.sock; ---< returns socket.