// Group D
// Author: Shengfang Ma
// Email: shengfang.ma@okstate.edu
// Description: this is the client side of tcp


#include "client.h"

#define PORT 8988         //define port
#define BUFFER_SIZE 8096

//main function
int main(){

	ssize_t ret;                  //check if send or recv successful

    int client_socket,connt;
    struct sockaddr_in server_addr;  //TCP address
    int menuOption[6];
    char *recv_msg=malloc(BUFFER_SIZE);    //
    char *send_msg=malloc(BUFFER_SIZE);
//char *recv_msg;
//char *send_msg;   
    client_socket = socket (AF_INET, SOCK_STREAM,0);   //Create TCP socket. 
    if(client_socket <0){
        printf("Error in creating socket.\n");
        exit(1);
    }
    
    // Configure settings of the server address struct
     //Set all bits of the padding field to 0 
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
    
    // Address family = Internet 
    server_addr.sin_family = AF_INET;

    //Set port number, using htons function to use proper byte order 
    server_addr.sin_port = htons(PORT);

    //Set IP address to localhost 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //Connect the socket to the server using the address
    connt=connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connt <0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
	//clients interface
	printf("1. Buyer\n");
	printf("2. Seller\n");
	printf("3. Exit\n");
	printf("Please choose your account type:\n");
	int is_send_recv=1;
	char choice[16];
	fgets(choice, 16, stdin);
	if(*choice == '1')  //buyer section
	{
		int choice_mark[8]={0,0,0,0,0,0,0,0};
		char subCh[5];
		int ch;
		while(1){
			//display buyer window
			Buyer_disply();
			is_send_recv=1;			
			//scanf("%d",&ch);
			fgets(subCh, 16, stdin);
			ch=atoi(subCh);
            //fflush(stdin);
			if(ch==1){          //register new buyer
				
				strcpy(send_msg,"Buyer: 1,");
				//client register
				char *newclient=addNewClient();
				strcat(send_msg,newclient);
				free(newclient);
				//strcat(send_msg,addNewClient());
				choice_mark[1]=1;
			}
			else if(ch==2){       //update buyer information
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Buyer: 2,");
					//update client information
					strcat(send_msg,updateClient());
					choice_mark[2]=1;
				}
			}
			else if(ch==3){        //view products
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Buyer: 3,");
					choice_mark[3]=1;
				}
			}
			else if(ch==4){         //purchase product
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else if(choice_mark[3]!=1){
					printf("Please display oroduct information first!\n\n\n");
					is_send_recv=-1;
				}
				else{               
					//printf("mark_3 %d",choice_mark[3]);
					strcpy(send_msg,"Buyer: 4,");
					strcat(send_msg,purchaseProduct());
					choice_mark[4]=1;
				}
			}
			else if(ch==5){         //return product
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				/*else if(choice_mark[3]!=1){
					printf("Please display oroduct information first!\n\n\n");
					is_send_recv=-1;
				}*/
				else{
					strcpy(send_msg,"Buyer: 5,");
					strcat(send_msg,returnProduct());
					choice_mark[5]=1;
					
				}
				
			}
			else if(ch==6){               //view buyer's order
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				
				else{
				strcpy(send_msg,"Buyer: 6,");
					strcat(send_msg,viewOrderBill());
					choice_mark[6]=1;
				}
			}
			else if(ch==7){             //view buyer's bill
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				
				else{
				strcpy(send_msg,"Buyer: 7,");
					strcat(send_msg,viewOrderBill());
					choice_mark[7]=1;
				}
			}
			else if(ch==0){              //exit 
				strcpy(send_msg,"Buyer: 0");
			    printf("Exit!\n");
				exit(0);
			}
			
			if(is_send_recv>0){          //if need to send message     
			ret =send(client_socket, send_msg , BUFFER_SIZE , 0) ;
			if(ret <0 )
			{
				printf("Send failed\n");
				exit(1);
			}
			 //Read the message from the server into the recv_msg
			 //memset (recv_msg,'\0',sizeof(char)*BUFFER_SIZE);
			 ret=recv(client_socket, recv_msg, BUFFER_SIZE, 0);
			 if(ret < 0)    {
				   printf("Receive failed\n");   //if fail
				   exit(1);
			 }
			 printf("\n%s\n", recv_msg);
			}
		 }
	}
	else if(*choice == '2'){ //seller section
		
		int choice_mark[8]={0,0,0,0,0,0,0,0};			
		int ch;
        char subCh[5];		
		while(1){
			
			Seller_disply();
			is_send_recv=1;
		    //scanf("%d",&ch);
			fgets(subCh, 16, stdin);
			ch=atoi(subCh);
            //fflush(stdin);
        
			if(ch==1){      //register new seller
				
				strcpy(send_msg,"Seller: 1,");
				
				strcat(send_msg, addNewClient());
				choice_mark[1]=1;

			}
			else if(ch==2){        //update seller information
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Seller: 2,");
					
					strcat(send_msg,updateClient());
					choice_mark[2]==1;
				}
			}
			else if(ch==3){           //seller add her product
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Seller: 3,");
					
					strcat(send_msg,addProduct());
					choice_mark[3]==1;
				}
			}
			else if(ch==4){             //seller remove his/her product
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Seller: 4,");
					
					strcat(send_msg,removeProduct());
					choice_mark[3]==1;
				}
			}
			else if(ch==5){           //seller update product information
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{
					strcpy(send_msg,"Seller: 5,");
					strcat(send_msg,updateProduct());
					choice_mark[5]==1;
				}
			}
			else if(ch==6){           //view products of seller
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{//display my products
					strcpy(send_msg,"Seller: 6");
					
					choice_mark[6]==1;
				}
			}
			else if(ch==7){           //view order from seller's product
				if(choice_mark[1]!=1){
					printf("Please register first!\n\n\n");
					is_send_recv=-1;
				}
				else{//view order of my products
					strcpy(send_msg,"Seller: 7");
					//view order of my products
					
					choice_mark[7]==1;
				}
			}
			else if(ch==0){
			    printf("Exit!\n");
				exit(0);
			}
printf("is_send_recv = %d \n", is_send_recv);
		if(is_send_recv>0){
			printf("sending\n");
			ret =send(client_socket , send_msg , BUFFER_SIZE , 0) ;
			if(ret <0 )
			{
				printf("Send failed\n");
				exit(1);
			}
			 //Read the message from the server into the recv_msg
			 memset (recv_msg,'\0',sizeof(char)*BUFFER_SIZE);
			 
			 ret=recv(client_socket, recv_msg, BUFFER_SIZE, 0);
			 printf("receiving\n");
			 if(ret < 0)    {
				   printf("Receive failed\n");   //if fail
				   exit(1);
			 }
			 printf("%s", recv_msg);
		}
		printf("\n%s\n\n", recv_msg);
		}
		 
	}
	else if(*choice == '3'){  //exit
		printf("Exit!\n");
		exit(0);
	}
	else {    //wrong choice
		printf("Wrong choice_mark!\n");
		exit(0);
	}
}
