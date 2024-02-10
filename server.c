// Author: Shengfang Ma
// Email: shengfang.ma@okstate.edu
// Description: this is the server which call function to handle communication with clients
#include <stdio.h>  
#include "server.h"

#define PORT 8988         //define port
#define BUFFER_SIZE 8096   //message size for TCP communication


#define THREAD_NUM 3       //3 threads in the pool
#define QUEUE_NUM 3       //3 clients in the clients queue


#define InfoNo 30
#define InfoSIZE 50
int dataserver;


//dellocate pointer 
void freeInfo(char ** Info){
	
	for(int i = 0; i < InfoNo; i++)
	{
		free(Info[i]);
	}

    free(Info);
}
//get product information for return
int * GetReturnProduct(char** ProductInfo){
	int * product=(int*)malloc(sizeof(int) * 3);
	product[0]=atoi(ProductInfo[1]);
	product[1]=atoi(ProductInfo[2]);
	product[2]=atoi(ProductInfo[3]);
	
	return product;
}
//get product information for purchasing
int * GetPurchaseProduct(char** product_info){
	
	
	int n=atoi(product_info[0]);
	n=n-1;  
	int *productlist=(int*)malloc(sizeof(int) * n * 2);

	char *token;
	for(int i =0 ; i < n; i++)
    {
		//get the first token
		token=strtok(product_info[i+1]," ");
		productlist[i*2+0]=atoi(token);
		//get the first token
		token=strtok(NULL," ");
		productlist[i*2+1]=atoi(token);
	}
	return productlist;
}

//extract infromation from received message
char** getInfo(char * inputmsg){
	
	//allocate 
	char** Info = (char**)malloc(sizeof(char) * InfoNo * InfoSIZE);

    for(int i = 0; i < InfoNo; i++)
    {
        Info[i] = calloc(InfoSIZE, sizeof(char));
    }
    	
	 char *token;
    
    //get the first token
    token=strtok(inputmsg,",");
    
	//walk through other tokend  
		
	int n=0;
    while( token != NULL )
    {/* While there are tokens in "string" */
       
	        /* Get next token: */
			
			strcpy(Info[n],token);
			printf("receive Info %d : %s\n",n,Info[n]);
            n++;
			token=strtok( NULL, "," );
       
    }
    free(token);
	char strNum[5];
	sprintf(strNum,"%d",n); 
	strcpy(Info[0],strNum);
	
	return Info;
	
}
	
//function send and receive message or file from and to client, called by thread function
void send_recv(int new_socket){
	ssize_t rct;                  //check if send or recv successful
	char recv_msg[BUFFER_SIZE];       // receive message 
    char send_msg[BUFFER_SIZE];      // message to send 
	char recv_server_msg[BUFFER_SIZE];
	int clientID=0;
	
  dataserver=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(dataserver==-1)
  {
    perror("Socket failed");
    exit(0);
  }
  struct sockaddr_in server, client;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(5000);
  server.sin_family=AF_INET;

  int c=connect(dataserver,(struct sockaddr*)&server,sizeof(server));
   
  if(c<0){
    perror("connectionto data server failure\n");
    exit(1);
  }
  printf("connected to data server\n");
  
	while(1) {    
	//receive message from client	
	    rct=recv(new_socket, recv_msg, BUFFER_SIZE, 0);
        if(rct<0){
        	printf("recieve failed from:  %d", new_socket);
    	    return;
        }
       recv_msg[strcspn(recv_msg, "\n")] = 0;        
       printf("receive data from client %d\n", new_socket);
	   printf("%s \n",recv_msg);
        
		//get send message according to the received message

		//tell if receive message from seller or buyer
        char *ret_buyer;
        ret_buyer= strstr(recv_msg,"Buyer:");
	    //if receive message, then save message to file and send filename back
        if (ret_buyer){  //
			if(strstr(recv_msg,"Buyer: 1,")){
				
				if(strstr(recv_msg,"old buyer")){
					
				}
				else{
					
					//save clients input to players information
					
					char** clientInfo=getInfo(recv_msg);
					//int buyerID=addCustomer(clientInfo[1],clientInfo[2],clientInfo[3]);
					buyer.CustomerID=0;
					strcpy(buyer.CustomerName,clientInfo[1]);
					strcpy(buyer.ContactNumber,clientInfo[2]);
					//buyer.CustomerName=atoi(clientInfo[2]);
					
					strcpy(buyer.CustomerAddress,clientInfo[3]);
					int buyerID=0;
//race condition
int ch=1;
send(dataserver,&ch,4,0);
send(dataserver,&buyer,sizeof(struct customerInformation),0);

recv(dataserver,&buyerID,4,0);  
					
					clientID=buyerID;
					buyer.CustomerID=buyerID;
					char strID[5];
					sprintf(strID,"%d",buyerID);  //
					strcpy(send_msg,"Your ID is ");				
					strcat(send_msg,strID);
					
				    freeInfo(clientInfo);

				}
			}
			else if(strstr(recv_msg,"Buyer: 2,")){
				//update clients  information
				
			    char** clientInfo=getInfo(recv_msg);
				//updateCustomer(clientID,clientInfo[1],clientInfo[2],clientInfo[3]);
				strcpy(buyer.CustomerName,clientInfo[1]);
				strcpy(buyer.ContactNumber,clientInfo[2]);
					//buyer.CustomerName=atoi(clientInfo[2]);
				strcpy(buyer.CustomerAddress,clientInfo[3]);
					
//race condition
//LOCK when update buyer information
//pthread_mutex_lock(&mutex_lock);
int ch=2;
send(dataserver,&ch,4,0);
send(dataserver,&buyer,sizeof(struct customerInformation),0);
//UNLOCK	
//pthread_mutex_unlock(&mutex_lock);
//
				strcpy(send_msg,"Your new information is saved !\n ");
				freeInfo(clientInfo);
			}
			else if(strstr(recv_msg,"Buyer: 3")){
//view all products				
				strcpy(send_msg,"\n");
//
int ch=3;
send(dataserver,&ch,4,0);
recv(dataserver,recv_server_msg,BUFFER_SIZE,0); 
printf("products list:\n %s \n", recv_server_msg);	
				strcat(send_msg,recv_server_msg);				
				
			}
			else if(strstr(recv_msg,"Buyer: 4,")){
				
				//purchase product
			    char** ProductInfo=getInfo(recv_msg);
				int * Product_list=GetPurchaseProduct(ProductInfo);
				int n=atoi(ProductInfo[0])-1;
				printf("purchase number %d\n", n);
int ch=4; //purchase request to dataserver
send(dataserver,&ch,4,0);
send(dataserver,&buyer,sizeof(struct customerInformation),0);
send(dataserver,&n,4,0);

printf("receive data : %d,%d,%d,%d \n",Product_list[0], Product_list[1],Product_list[2], Product_list[3]);
			
send(dataserver,Product_list,(sizeof(int) * n * 2),0);
//printf("products list:\n %s \n", Product_list);
int order_id;	
recv(dataserver,&order_id,4,0);  
					
					char strID[5];
					sprintf(strID,"%d",order_id);  //
					strcpy(send_msg,"Your purchase is successed!\n");
					strcat(send_msg,"Your order ID is ");				
					strcat(send_msg,strID);

				freeInfo(ProductInfo); //free
				free(Product_list);
			}
			else if(strstr(recv_msg,"Buyer: 5")){
				
				//return product
			    char** ProductInfo=getInfo(recv_msg);
				int *retProduct_list=GetReturnProduct(ProductInfo);
				
int ch=5; //purchase request to dataserver
send(dataserver,&ch,4,0);
send(dataserver,retProduct_list,(sizeof(int)*3),0);				
				//returnProduct(retProduct_list);
				strcpy(send_msg,"Your return is successed!\n");
				freeInfo(ProductInfo); //free
				free(retProduct_list);
			}
			else if(strstr(recv_msg,"Buyer: 6")){//view order product
				
				strcpy(send_msg,"Your order list:\n");	
int ch=6;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);

recv(dataserver,recv_server_msg,BUFFER_SIZE,0); 	
			
				//char* buyerOrder=viewCustomerOrder(clientID);
				strcat(send_msg,recv_server_msg);
				//free(buyerOrder); //free
			}
			else if(strstr(recv_msg,"Buyer: 7")){//view order product
				strcpy(send_msg,"Your Bill list:\n");				    
				//char* buyerBill=viewCustomerBill(clientID);
				//strcat(send_msg,buyerBill);
int ch=7;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);

recv(dataserver,recv_server_msg,BUFFER_SIZE,0); 

				strcat(send_msg,recv_server_msg);
				//free(buyerBill); //free
			}
			else if(strstr(recv_msg,"Buyer: 0")){//view order product
							    
				//char* buyerBill=viewCustomerOrder(clientID);
				strcpy(send_msg,"Goodbye!\n");
				exit(0);
			}
		}
		else{
			
			if(strstr(recv_msg,"Seller: 1")){
				if(strstr(recv_msg,"old seller")){
					
				}
				else{
					//save clients input to players information
					
					char** clientInfo=getInfo(recv_msg);
					//int SellerID=addSeller(clientInfo[1],clientInfo[2],clientInfo[3]);
					seller.SellerID=0;
					strcpy(seller.SellerName,clientInfo[1]);
					strcpy(seller.SContactNumber,clientInfo[2]);
					//buyer.CustomerName=atoi(clientInfo[2]);
					strcpy(seller.SellerAddress,clientInfo[3]);
					int SellerID=0;
//race condition
int ch=8;
send(dataserver,&ch,4,0);
send(dataserver,&seller,sizeof(struct sellerInformation),0);

recv(dataserver,&SellerID,4,0); 
 
					clientID=SellerID;
					char strID[5];
					sprintf(strID,"%d",SellerID);  //
					strcpy(send_msg,"Your seller ID is ");				
					strcat(send_msg,strID);
					
				    freeInfo(clientInfo);

				}
			}
			else if(strstr(recv_msg,"Seller: 2")){
				
					//save clients input to players information
					
					char** clientInfo=getInfo(recv_msg);
					//updateSeller(clientID,clientInfo[1],clientInfo[2],clientInfo[3]);
					strcpy(seller.SellerName,clientInfo[1]);
					strcpy(seller.SContactNumber,clientInfo[2]);
					//buyer.CustomerName=atoi(clientInfo[2]);
					strcpy(seller.SellerAddress,clientInfo[3]);
					
//race condition
//LOCK when update buyer information
//pthread_mutex_lock(&mutex_lock);
int ch=9;
send(dataserver,&ch,4,0);
send(dataserver,&seller,sizeof(struct sellerInformation),0);
//UNLOCK	
//pthread_mutex_unlock(&mutex_lock);
					strcpy(send_msg,"Your information has been updated!\n ");				
				    freeInfo(clientInfo);

			}
			else if(strstr(recv_msg,"Seller: 3")){
				
					//add new product
					char** newProduct=getInfo(recv_msg);
					product.productID=atoi(newProduct[1]);
					strcpy(product.productDescription,newProduct[2]);
					product.productQuantity=atoi(newProduct[3]);
					product.productPrice=atoi(newProduct[4]);
					product.sellerID=clientID;
					//addProduct(clientID,newProduct[1],atoi(newProduct[2]),atof(newProduct[3]));
					int productID=0;
//add new product to database
//race condition
int ch=10;
send(dataserver,&ch,4,0);
//send(dataserver,&clientID,4,0);
send(dataserver,&product,sizeof(struct productInformation),0);

recv(dataserver,&productID,4,0); 
					
					//strcpy(send_msg,"Product has been added to your library!\n ");
					char strID[5];
					sprintf(strID,"%d",productID);  //
					strcpy(send_msg,"Your product ID is ");				
					strcat(send_msg,strID);	
					
				    freeInfo(newProduct);

			}
			else if(strstr(recv_msg,"Seller: 4")){  //remove product
				
				char** newProduct=getInfo(recv_msg);
				//seller remove products
				int productID=atoi(newProduct[1]);;
//add new product to database
//race condition
int ch=11;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);
send(dataserver,&productID,4,0);
//send(dataserver,&product,sizeof(struct productInformation),0);

//recv(dataserver,&productID,4,0); 
					
				//removeProduct(clientID,atoi(newProduct[1]));
				strcpy(send_msg,"The product removed!\n ");
				freeInfo(newProduct);
			}
			else if(strstr(recv_msg,"Seller: 5")){ //update products
				
				char** newProduct=getInfo(recv_msg);
				//seller update products
				product.productID=0;
				strcpy(product.productDescription,newProduct[1]);
				product.productQuantity=atoi(newProduct[2]);
				product.productPrice=atoi(newProduct[3]);
int ch=12;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);
send(dataserver,&product,sizeof(struct productInformation),0);

				strcpy(send_msg,"The product's information has been updated!\n ");
				freeInfo(newProduct);
			}
			else if(strstr(recv_msg,"Seller: 6")){ //seller display products
				
				
int ch=13;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);

recv(dataserver,recv_server_msg,BUFFER_SIZE,0); 

				//seller display products
				strcpy(send_msg,"Your products list:\n");
				strcat(send_msg,recv_server_msg);
				//char* SellerProducts=displaySellersProduct(clientID);				
				//strcat(send_msg,SellerProducts);
				//free(SellerProducts);
			}
			else if(strstr(recv_msg,"Seller: 7")){
				
				//seller display order
int ch=14;
send(dataserver,&ch,4,0);
send(dataserver,&clientID,4,0);
recv(dataserver,recv_server_msg,BUFFER_SIZE,0); 

				strcpy(send_msg,"Orders  list:\n");
				strcat(send_msg,recv_server_msg);
				
				//char* SellerOders=displaySellersProduct(clientID);
				//strcat(send_msg,SellerOders);
				//free(SellerOders);
				
			}
			else if(strstr(recv_msg,"Seller: 0")){
							    
				//client exit
				strcpy(send_msg,"Goodbye!\n");
				exit(0);
			}
		}
        
	   	    
	    printf("send data to client:  %d\n", new_socket);
	    printf("%s \n",send_msg);
       //send message
	   rct=send(new_socket,send_msg,BUFFER_SIZE,0);
       if(rct<0){
    	       printf("send failed from %d \n", new_socket );
    	       return;
        }
    
	}
	//send();
}

 
//main function create socket and handle connection from multi clients
int main(){

  
    int connt;           //return results of connection
	//soket information
    int server_socket;   
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    //new socket information
    int new_socket; //create new socket used to when accept new client
    struct sockaddr_in new_addr;
         
    char recv_msg[BUFFER_SIZE];       //store receive message from client
    char send_msg[BUFFER_SIZE];      //store message to send to client
    
    server_socket = socket (AF_INET, SOCK_STREAM,0);   //Create a server socket. 
	
    if(server_socket <0){
        printf("Error in create socket.\n");
        exit(1);
    }
    
    // Configure settings of the server address struct
     //Set all bits of the padding field to 0 
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
   
    // Address family = Internet 
    server_addr.sin_family = AF_INET;

    //Set port number
	printf("PORT %d\n",PORT);
    server_addr.sin_port = htons(PORT);

	
	
    //Set IP address  
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //Bind the address struct to the socket 
    connt=bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(connt <0){
        printf(" binding failed.\n");
        exit(1);
    }
    
     //Listen on the socket, with 5 max connection requests queued 
    if(listen(server_socket,5)==0){
    printf("\nListening\n");
    }else{                 //if lis
    printf("\nlistening failed\n");
    exit(1);
    }
    int ret1=5; //how many child processes can be created
    while (1){
    
	    
    	//Accept call creates a new socket for the incoming connection
        addr_size=sizeof(new_addr);
    	new_socket = accept(server_socket, (struct sockaddr *) &new_addr, &addr_size);
  
    	if(new_socket <0){
        printf("[-]Error in connectiopn.\n");
        //exit(1);
        }
		else {
			if (ret1==0)  { //return 0 if client_queue is full 
				printf(" No thread available now!\n");
							
			}
			else{
					ret1--;
							
					pid_t pid = fork();
					
					
					if (pid < 0) { // fork failed; exit
						printf("fork failed\n");
						exit(1);
					} 
					else if(pid ==0){
							
							printf("Connection accepted from: %s: %d \n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
							//call function to COMMUNICATE with client 
							send_recv(new_socket);
							//when there are 5 clients already?
					}
					else {	// parent process
						close(new_socket);
					}
			}
		}
				
	}
	
}
