// Group D
// Author: Shengfang Ma
// Email: shengfang.ma@okstate.edu
// Description:headfiles used by server and server

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     // for string operation
#include <unistd.h>     // for fork sleep

#include <sys/types.h>   // basic derived types like pid_t
#include <sys/socket.h>  // for socket
#include <arpa/inet.h>    //for internet
#include <netinet/in.h>
#include <fcntl.h>		// for file control options
/*
#include <pthread.h>      //for thread
#include <ctype.h>
#include <semaphore.h>
*/

void Buyer_disply();
void Seller_disply();

char * addNewClient();
char * updateClient();
char * purchaseProduct();
char * returnProduct();
char * viewOrderBill();
char * addProduct();
char * removeProduct();
char * updateProduct();


//Display buyer's window
void Buyer_disply()
{
	printf("\n\n Dear Buyer:\n");
	printf("1. Register new account\n");
	printf("2. Update your information\n");
	printf("3. View Products\n");
	printf("4. Purchase Products\n");
	printf("5. Return Products\n");
	printf("6. View Order information\n");
	printf("7. View Billing information\n");
	printf("0. Exit\n\n");
	printf("\nPlease enter your Choice:\n");

}
//Display seller's window
void Seller_disply()
{
	printf("\n\n Dear Seller:\n");
	printf("1. Register new account\n");
	printf("2. Update your information\n");
	printf("3. Add new products\n");
	printf("4. Remove product\n");
	printf("5. Change products information\n");
	printf("6. Display My Products\n");
	printf("7. View Order of my products\n\n");
	printf("0. Exit\n\n");
	printf("\nPlease enter your Choice:\n");
}
//register new client(buyer or seller)
char * addNewClient()
{
	char * newclient=malloc(256);
		
	printf("\n Please enter your name,contact number, address (name,***-***-****,address\n");
	fflush(stdin);
      
	 if(fgets (newclient,256,stdin) == NULL)
    {
       	printf("new client info failed");
	}

    return newclient;
}
//update information of client(buyer or seller)
char * updateClient()
{
	char *  newclient=malloc(256);
    
	printf("\n Please enter your name,contact number, address\n");
	fflush(stdin);
	fgets (newclient,256,stdin);  

    return newclient;
}
//Purchase product
char * purchaseProduct()
{
	char *  products=malloc(256);;
    
	printf("\n Please enter product ID and number to purchase(1 2, 5 3) \n");
	fflush(stdin);
	fgets (products,256,stdin);  


    return products;
}
//return product from previous order
char * returnProduct()
{
	char *  products=malloc(256);;
    
	printf("\n Please enter the order ID, product ID  and number in the order to return(3,2,1)\n");
	fflush(stdin);
	fgets (products,256,stdin);  


    return products;
}
//
char * viewOrderBill()
{
	char *  BUYERID=malloc(256);;
    
	printf("\n Please enter your ID\n");
	fflush(stdin);
	fgets (BUYERID,256,stdin);  
	

    return BUYERID;
}
//add product from previous order
char * addProduct()
{
	char *  products=malloc(256);;
    
	printf("\n Please enter your product ID, product name, quantity , price(pencil,3,2)\n");
	fflush(stdin);	
	fgets (products,256,stdin);  


    return products;
}
//add product from previous order
char * removeProduct()
{
	char *  products=malloc(256);;
    
	printf("\n Please enter your product ID to remove \n");
	fflush(stdin);
	fgets (products,256,stdin);  
	
    return products;
}
//add product from previous order
char * updateProduct()
{
	char *  products=malloc(256);;
    
	printf("\n Please enter your product ID and new name, quantity, price( 3,pecil,2,4)\n");
	fflush(stdin);
	fgets (products,256,stdin);  

    return products;
}

#endif

