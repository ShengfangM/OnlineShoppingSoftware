// Author: Shengfang Ma
// Email: shengfang.ma@okstate.edu
// Description:headfiles used by server and server

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     // for string operation
#include <unistd.h>     // for fork sleep

#include <sys/types.h>   // basic derived types like pid_t
#include <sys/socket.h>  // for socket
#include <arpa/inet.h>    //for internet
#include <netinet/in.h>
#include <fcntl.h>		// for file control options

#include <pthread.h>      //for thread
#include <ctype.h>
#include <semaphore.h>

//-----------------------------------------------------------------
struct customerInformation
{
int CustomerID;
char CustomerName[20];
char ContactNumber[14];
char CustomerAddress[20];

}buyer={};

//--------------------------------------------------------------------
struct sellerInformation {
    int SellerID;
    char SellerName[20];
    char SContactNumber[10];
    char SellerAddress[20];
}seller={};

//--------------------------------------------------------------
struct productInformation {
    int productID;
    char productDescription[20];
    int sellerID;
    int productQuantity;
    int productPrice;
}product={};

//--------------------------------------------------------------
struct customerOrder {
    int OrderID;
    int ProductID;
    int QuantityPurchased;
    char CustomerAddress[20];
    int OrderTotal;
}order={};
//----------------------------------------------------------------
struct billingInformation {
    int OrderID;
    int CustomerID;
    char CustomerAddress[20];
    int totalBill;
}bill={};

//---------------------------------------------------------------


//node in the client queue
typedef struct node{
	int *client_socket;  //current client
	struct node *next_node; //next client
}node;

//client queue
typedef struct client_thread_queue{

	node *head_node; //first client in the queue 
	node *tail_node; //the last client in the queue
	int num;
    
}client_queue;

void send_recv(int );                  //server's comunication  with client
void * thread_function(void * );
void create_server(unsigned int );



#endif

