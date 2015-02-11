//Network.c Version 2.0
//By Theodore Maselko
//Bigger, better, badder, faster, stronger, but certainly not f*cking functional...

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
#define DEBUG 0

//Encryption Specifications:
//ENC_PACKET_LEN must be EXACTLY ENC_PARSE_LEN + ENC_HEADER_LEN
//All of the POS macros must be 0 < X < ENC_HEADER_LEN - 1
//The TRANS flags can be w/e, just make 'em literal chars.

#define ENC_PACKET_LEN 24
#define ENC_PARSE_LEN 12
#define ENC_HEADER_LEN 12
#define ENC_TRANS_END 'X'
#define ENC_TRANS_CONT 'Y'
#define ENC_TRANSFLAG_POS 5
#define ENC_LENFLAG_POSA 4
#define ENC_LENFLAG_POSB 8

#define ENC_FPACKET_LEN 65536
#define ENC_FPARSE_LEN 65528
#define ENC_FHEADER_LEN 8
#define ENC_FTRANSFLAG_POS 0
#define ENC_FLENFLAG_POSA 2
#define ENC_FLENFLAG_POSB 6
#define ENC_FLENFLAG_POSC 5
#define ENC_FLENFLAG_POSD 7

//DON'T FORGET: RUN Network Update.bat! AFTER EVERY SAVE!









int TestCompress( unsigned char* pDest, long* pnDestLen, const unsigned char* pSrc, long nSrcLen )
{
    z_stream stream;
    int err;

	int nExtraChunks;
	uInt destlen;

    stream.next_in = (Bytef*)pSrc;
    stream.avail_in = (uInt)nSrcLen;
#ifdef MAXSEG_64K
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != nSrcLen) return Z_BUF_ERROR;
#endif
	destlen = (uInt)*pnDestLen;
	if ((uLong)destlen != (uLong)*pnDestLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit(&stream,9);
	
    if (err != Z_OK) return -5;

	nExtraChunks = 0;
    do {
		stream.next_out = pDest;
		stream.avail_out = destlen;
        err = deflate(&stream, Z_FINISH);
		if (err == Z_STREAM_END )
			break;
		if (err != Z_OK) {
			deflateEnd(&stream);
			return -4;
		}
		nExtraChunks += 1;
    } while (stream.avail_out == 0);

    *pnDestLen = stream.total_out;

    err = deflateEnd(&stream);
    if (err != Z_OK) return -1;

    return nExtraChunks ? Z_BUF_ERROR : Z_OK;
}


int TestUncompress( unsigned char* pDest, long* pnDestLen, const unsigned char* pSrc, long nSrcLen )
{
    z_stream stream;
    int err;

	int nExtraChunks;
	uInt destlen;

    stream.next_in = (Bytef*)pSrc;
    stream.avail_in = (uInt)nSrcLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != (uLong)nSrcLen) return Z_BUF_ERROR;

	destlen = (uInt)*pnDestLen;
	if ((uLong)destlen != (uLong)*pnDestLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit(&stream);
    if (err != Z_OK) return err;

	nExtraChunks = 0;
    do {
		stream.next_out = pDest;
		stream.avail_out = destlen;
        err = inflate(&stream, Z_FINISH);
		if (err == Z_STREAM_END )
			break;
		if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
			err = Z_DATA_ERROR;
		if (err != Z_BUF_ERROR) {
			inflateEnd(&stream);
			return err;
		}
		nExtraChunks += 1;
    } while (stream.avail_out == 0);

    *pnDestLen = stream.total_out;

    err = inflateEnd(&stream);
    if (err != Z_OK) return err;

    return nExtraChunks ? Z_BUF_ERROR : Z_OK;
}

void test(void);
int sendData(SOCKET, char *, int);
char *recvData(SOCKET);
int sendFile(SOCKET, FILE *);
int recvFile(SOCKET, FILE *);

void test (void){
 //   printf("Network.c Version 2.0\nBy Theodore Maselko\n");
    return;
}

//socket: Socket to use
//data: pointer to array of bytes to send
//dataLen: length of array of bytes
//Returns news of success, failure, or errors involved: 0 success, anything else, ehhhh, fawk?
int sendData(SOCKET socket, char *data, int dataLen){

    srand((unsigned)time(NULL));

	//compress packet
	long DefaultLen=dataLen;
	unsigned char* CompressedData=new unsigned char[DefaultLen];
	int err= TestCompress(CompressedData, &DefaultLen, (unsigned char*)data, dataLen );
	//printf("SIZE:%d",DefaultLen);
	// now we can catch lots of errors!
	if (err==-5){ // not enough space so allocate some more....
	delete [] CompressedData;
	CompressedData=new unsigned char[DefaultLen];
	err= TestCompress(CompressedData, &DefaultLen, (unsigned char*)data, dataLen );
	

	}else if(err==0){

		//break?


	}else{



	}
	//printf("SEND LEN:%d\n",DefaultLen);
	data=(char*)CompressedData;

    int packets = (DefaultLen / ENC_PARSE_LEN);
    int totalPackets = packets;

    unsigned char packet[ENC_PACKET_LEN];

    for(; packets >= 0; packets--){
        int k;
        for(k = 0; k < ENC_PACKET_LEN; k++){
            packet[k] = (unsigned char)(rand() % 256);
        }

        if(packets == 0){
            packet[ENC_TRANSFLAG_POS] = ENC_TRANS_END;
        }else{
            packet[ENC_TRANSFLAG_POS] = ENC_TRANS_CONT;
        }

        short int len = (packets == totalPackets ? (DefaultLen - (packets * ENC_PARSE_LEN) + 1) : (ENC_PARSE_LEN));

        packet[ENC_LENFLAG_POSA] = (char)(len >> 8);
        packet[ENC_LENFLAG_POSB] = (char)(len & 0x00FF);



        memcpy((char *)&(packet[ENC_HEADER_LEN]), &(data[(packets) * ENC_PARSE_LEN]), len);
		/*
        #if DEBUG
            printf("Fluff: %d %d\n", ((short int)packet[ENC_LENFLAG_POSA]), len);
            printf("Decrypted Packet:\n%s\n", (char *)&(packet[ENC_HEADER_LEN]));
        #endif
		*/

        //insert further packety obfustication here
        for(k = 1; k < ENC_PACKET_LEN; k++){
            packet[k] ^= packet[k - 1];
        }

        send(socket, (char *)packet, ENC_PACKET_LEN, 0);
			
		
			

    }
	delete[] data;


    return 0;
}

//socket: Socket to listen to
//Returns byte array containing all that wonderful data that was just recieved.
char *recvData(SOCKET socket){
    unsigned char packet[ENC_PACKET_LEN];
    int dc;

    dc = recv(socket, (char *)packet, ENC_PACKET_LEN, 0);

    if(dc == -1){
        printf("Network.c: ERROR: Socket Disconnected!\n");
        return NULL;
    }

    int k;
    for(k = ENC_PACKET_LEN - 1; k > 0; k--){
        packet[k] ^= packet[k - 1];
    }

    int transFlag = (packet[ENC_TRANSFLAG_POS] == ENC_TRANS_CONT);

    int totalLen = (packet[ENC_LENFLAG_POSA] << 8) + (packet[ENC_LENFLAG_POSB]);

	/*
    #if DEBUG
        printf("\nIN (DECRYPTED):\n\"%s\"\n", (char *)&(packet[ENC_HEADER_LEN]));
        printf("Specs: Flag: %c Size: %d\n", packet[ENC_TRANSFLAG_POS], (packet[ENC_LENFLAG_POSA] << 8) + (packet[ENC_LENFLAG_POSB]));
    #endif
	*/
    char *total = new char [totalLen];

	try{
    memcpy(total, (char *)&(packet[ENC_HEADER_LEN]), totalLen);
	}catch(...){
		// do nothing


	}
	/*
    #if DEBUG
        printf("PRE LOOP\n");
    #endif
	*/
	long TotalPkg=0;
    while(transFlag){
        dc = recv(socket, (char *)packet, ENC_PACKET_LEN, 0);

        if(dc == -1){
          //  printf("Network.c: ERROR: Socket Disconnected!\n");
            delete [] total; //--
            return NULL;
        }

        for(k = ENC_PACKET_LEN - 1; k > 0; k--){
            packet[k] ^= packet[k - 1];
        }

        transFlag = (packet[ENC_TRANSFLAG_POS] == ENC_TRANS_CONT);
        short int len = (packet[ENC_LENFLAG_POSA] << 8) + (packet[ENC_LENFLAG_POSB]);
        totalLen += len;

		int tmpcount=totalLen-len;
        char *temp=new char[tmpcount]; 
		
		
        memcpy(temp, total, totalLen-len);
        delete [] total;
        total = new char [totalLen];

		TotalPkg+=totalLen-len;
        memcpy(total, (char *)&(packet[ENC_HEADER_LEN]), ENC_PARSE_LEN);
        memcpy(&(total[len]), temp, totalLen - (len));
		delete [] temp;
		
    }
	/*
    #if DEBUG
        printf("RETURN!\n");
    #endif
	*/
	//plenty of delete statements all around!
	//delete [] 
	
	//total;totalLen;
	//printf("TOTAL LEN:%d\n",totalLen);
	//printf("TOTAL LEN:%d\n",TotalPkg-ENC_PARSE_LEN);
//	printf("Real text Len:%d\n",TotalPkg);
	long DefaultLen=totalLen;
	unsigned char* CompressedData=new unsigned char[DefaultLen];
	int err= TestUncompress(CompressedData, &DefaultLen, (unsigned char*)total,totalLen );
	//printf("SIZE:%d",DefaultLen);
	// now we can catch lots of errors!
	if (err==-5){ // not enough space so allocate some more....
	delete [] CompressedData;
	CompressedData=new unsigned char[DefaultLen];
	int err= TestUncompress(CompressedData, &DefaultLen, (unsigned char*)total, totalLen );
	//printf("SIZE:%d",DefaultLen);

	}else if(err==0){

	}else if(err==-4){
		//break?
		printf("Catch this exception!\n");
		//CompressedData=(unsigned char*)"UnkownError";
	}else if(err==-3){
		printf("Catch this exception1!\n");
	}else if(err==-1){
		printf("Catch this exception2!\n");
	}else{
		
	printf("Error\n");
//	strcpy((char*)CompressedData,"SOCKET ERROR");

	}
	if (CompressedData==NULL){
		CompressedData=(unsigned char*)"UnkownError";

	}	
	CompressedData[DefaultLen]='\0';

	//printf("Recv LEN:%d\n",DefaultLen);
	delete []  total;
	


	//printf((char*) CompressedData);
////	delete [] CompressedData;
//	delete [] ;
    return (char*)CompressedData;
}

int sendFile(SOCKET socket, FILE *file){
    if(file == NULL){
        printf("File is null!\n");
        return 0;
    }
    long fileLen = filelength(fileno(file)); //oh yeahhhhh thats right bitch! btw i think this should break... sorry bout that ;-P

    char headerMsg[64];
    sprintf(headerMsg, "%ld", fileLen);

    send(socket, headerMsg, strlen(headerMsg), 0);

    srand((unsigned)time(NULL));
	/*
    #if DEBUG
        printf("\n\nattempting to send file: \n");
    #endif
	*/

    unsigned char packet[ENC_FPACKET_LEN];

    int bytesRead;

//    printf("|");

    int k;
    for(k = 0; k < ENC_FPACKET_LEN; k++){
        packet[k] = (unsigned char)(rand() % 256);
    }
//    int barSection = fileLen / 10;

    int totalRead = 0;
    while((bytesRead = fread(&(packet[ENC_FHEADER_LEN]), sizeof(unsigned char), ENC_FPARSE_LEN, file))){

        totalRead += bytesRead;

        if(bytesRead < ENC_FPARSE_LEN){
            packet[ENC_FTRANSFLAG_POS] = ENC_TRANS_END;
        }else{
            packet[ENC_FTRANSFLAG_POS] = ENC_TRANS_CONT;
        }

        packet[ENC_FLENFLAG_POSA] = (char)(bytesRead >> 24);
        packet[ENC_FLENFLAG_POSB] = (char)((bytesRead >> 16) & 0xFF);
        packet[ENC_FLENFLAG_POSC] = (char)((bytesRead >> 8) & 0xFF);
        packet[ENC_FLENFLAG_POSD] = (char)(bytesRead & 0xFF);

		/*
        #if DEBUG
            printf("Fluff: %d\n", bytesRead);
//            printf("Unencrypted Packet:\n%s\n", packet);
        #endif
		*/
        //insert further packety obfustication here
        for(k = 1; k < ENC_FPACKET_LEN; k++){
            packet[k] ^= packet[k - 1];
        }

        send(socket, (char *)packet, ENC_FPACKET_LEN, 0);
//        printf("Packet sent. (%d)\n", totalRead);
        recv(socket, (char *) packet, 10, 0);

//        if(totalRead >= barSection){
//            printf(":");
//            totalRead = 0;
//        }
    }
//    printf("|\n");

    return 0;

}

int recvFile(SOCKET socket, FILE *file){
    //receive packets, put together in the rough shape of a file.
    unsigned char packet[ENC_FPACKET_LEN];

    packet[recv(socket, (char *) packet, 10, 0)] = '\0';
    long barSection = atol((char *)packet) / 10;

//    printf("|");

    int bytesIn = 0;
    while((bytesIn += recv(socket, (char *)&packet[bytesIn], ENC_FPACKET_LEN - bytesIn, 0)) < ENC_FPACKET_LEN);
    send(socket, (char *)&packet[55], 2, 0);

    int k;
    for(k = ENC_FPACKET_LEN - 1; k > 0; k--){
        packet[k] ^= packet[k - 1];
    }

    int transFlag = (packet[ENC_FTRANSFLAG_POS] == ENC_TRANS_CONT);
	/*
    #if DEBUG
        printf("Specs: Flag: %c Size: %d\n", packet[ENC_FTRANSFLAG_POS], (packet[ENC_FLENFLAG_POSA] << 24) |
           (packet[ENC_FLENFLAG_POSB] << 16) | (packet[ENC_FLENFLAG_POSC] << 8) | (packet[ENC_FLENFLAG_POSD]));
    #endif
	*/
    fwrite(&(packet[ENC_FHEADER_LEN]), sizeof(unsigned char), (packet[ENC_FLENFLAG_POSA] << 24) |
           (packet[ENC_FLENFLAG_POSB] << 16) | (packet[ENC_FLENFLAG_POSC] << 8) |
           (packet[ENC_FLENFLAG_POSD]), file);

    int totalBytes = bytesIn;
    while(transFlag){
        bytesIn = 0;
        while((bytesIn += recv(socket, (char *)&packet[bytesIn], ENC_FPACKET_LEN - bytesIn, 0)) < ENC_FPACKET_LEN);
        send(socket, (char *)&packet[55], 2, 0);//check for complete transmission of packet, if need be.
        totalBytes += bytesIn;


        for(k = ENC_FPACKET_LEN - 1; k > 0; k--){
            packet[k] ^= packet[k - 1];
        }

        transFlag = (packet[ENC_FTRANSFLAG_POS] == ENC_TRANS_CONT);

        int len = (packet[ENC_FLENFLAG_POSA] << 24) | (packet[ENC_FLENFLAG_POSB] << 16) |
        (packet[ENC_FLENFLAG_POSC] << 8) | (packet[ENC_FLENFLAG_POSD]);
	/*
        #if DEBUG
            printf("Specs: Flag: %c Size: %d\n", packet[ENC_FTRANSFLAG_POS], len);
        #endif
		*/
        fwrite(&(packet[ENC_FHEADER_LEN]), sizeof(unsigned char), len, file);

//        if(totalBytes >= barSection){
//            printf(":");
//            totalBytes = 0;
//        }
//        printf("Packet received. (%d)\n", totalBytes);

    }
//    printf("|\n");


    return 0;
}
