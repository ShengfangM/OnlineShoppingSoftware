// Author: Shengfang Ma, Reem Alzouri
// Email: shengfang.ma@okstate.edu
// Description: this is the dataserver to communicate with servers

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "database.h"
#define BUFFER_SIZE 8096


//------------------------------------------------------------------

//void *serve_client(void *args);
void serve_client(int);
void *thread_function(void * );
//-----------------------------------------------------------------

int main(){


  int SellerID,found=0,count=0;
  system("clear");
  int serverfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(serverfd==-1)
  {
    perror("Socket failed");
    exit(1);
  }
  printf("Socket connected\n");
  struct sockaddr_in server, client;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(5000);
  server.sin_family=AF_INET;

  int b=bind(serverfd,(struct sockaddr*)&server,sizeof(server));

  if(b>0){
    perror("Bind failure\n");
    exit(1);
  }
    printf("Binding Successfull\n");
    int i=listen(serverfd,20);
    //if(i==0){
	while(1){
      printf("ports are ready listen from client\n");
      int size=sizeof(server);
      int clientfd=accept(serverfd,(struct sockaddr*)&client,&size);
        if(clientfd==-1){
          perror("Accept failure");
          exit(1);
        }
        int * th_client=malloc(sizeof(int));
	    * th_client=clientfd;
		
        pthread_t tid;
        pthread_create(&tid, NULL, thread_function, th_client);

		//serve_client(clientfd);
        printf("\n %s request come from client\n",inet_ntoa(client.sin_addr));
       
    }
}

void * thread_function(void * th_client){
	
	int new_socket= *(int *) th_client;
free(th_client);	
	printf("begin %d",new_socket);	
	serve_client(new_socket);
	

}

void serve_client(int clientfd) {
	 //    struct ThreadArgs *threadArgs = (struct ThreadArgs*) args;
	
	while(1){    
		  
 // printf("client connected.\n");
          
	int opt;
	printf("clientfd is %d \n", clientfd);
    recv(clientfd,&opt,4,0);
	printf("opt is %d \n", opt);

   // BUYERS Buyer;
	//SELLERS Seller;
	//PRODUCTS Product;
	//int clientID;
         if(opt == 1) {//add new buyer
		    BUYERS Buyer1;
            recv(clientfd,&Buyer1,sizeof(BUYERS),0);
			printf("name is %s \n",Buyer1.CustomerName );
            int buyerID=addBuyer( Buyer1);
			printf("buyerID is %d \n",buyerID);
            send(clientfd,&buyerID,4,0);
			printf("Buyer %d registered \n", buyerID);
            break;
		 }
		else if (opt == 2) { //change buyer information
		    BUYERS Buyer2;
            recv(clientfd,&Buyer2,sizeof(BUYERS),0);
			updateBuyer(Buyer2);
            break;
		}

		else if (opt == 3) { //view products
			char * product_info;
			product_info=viewProducts();
			printf("products list:\n %s \n", product_info);
			
            send(clientfd,product_info,BUFFER_SIZE,0);
            
			free(product_info);
            break;
		}
			
		else if (opt == 4) {  //purchase products
			
			int n;
			BUYERS Buyer3;
			recv(clientfd,&Buyer3,sizeof(BUYERS),0);
			recv(clientfd,&n,4,0);
			printf("purchase number %d \n", n);
			int *productlist=(int*)malloc(sizeof(int) * n * 2);
            
			
			recv(clientfd,productlist,(sizeof(int) * n * 2),0);
			//printf("receive data : %d,%d,%d,%d \n",productlist[0], productlist[1],productlist[2], productlist[3]);
			int totalbill=0;
			int *bill_list=(int*)malloc(sizeof(int) * n );
			for(int i = 0; i < n; i++)
            {
				bill_list[i]=purchaseProduct(productlist[i*2+0], productlist[i*2+1]);
				totalbill=totalbill+bill_list[i];
			}
			
			int order_id= makeBill(Buyer3,totalbill);
			makeOrder(order_id,Buyer3,productlist,bill_list,n);
		    send(clientfd,&order_id,4,0);
			printf("order id is %d  \n", order_id);
            printf("Order Made Successfully.\n");
			free(productlist);

            break;
		}
			
		else if (opt == 5) {  //return products
			
			int *Product_list=(int*)malloc(3*sizeof(int));
			recv(clientfd,Product_list,(sizeof(int)*3),0);	
			printf("receive data : %d,%d,%d \n",Product_list[0], Product_list[1],Product_list[2]);
			
				int bill= returnProduct(Product_list[1], Product_list[2]);
				updateOrder(Product_list[0],Product_list[1], Product_list[2],bill);
				updateBill(Product_list[0],  bill);
			
            break;
		}
		
		else if (opt == 6) { //buyers view orders
			int clientID1;
            recv(clientfd,&clientID1,4,0);
			
            char * order_info;
			order_info=viewBills(clientID1);
			send(clientfd,order_info,BUFFER_SIZE,0); 
            printf("orders Added.\n");
			free(order_info);

            break;
		}

		else if (opt == 7) {  //buyers view billings
            int clientID2;
            recv(clientfd,&clientID2,4,0);
			
            char * bill_info;
			bill_info=viewBills(clientID2);
			send(clientfd,bill_info,BUFFER_SIZE,0); 
            printf("billings Added.\n");
			free(bill_info);


            break;
		}

		else if (opt == 8) {  //add new seller
			SELLERS Seller1;
            recv(clientfd,&Seller1,sizeof(SELLERS),0);
			int sellerID=addSeller(Seller1);
            send(clientfd,&sellerID,4,0);
			printf("Seller %d registered \n", sellerID);
			
            break;
		}
			
		else if (opt == 9) {  //update  seller information
			SELLERS Seller2;
            recv(clientfd,&Seller2,sizeof(SELLERS),0);
			updateSeller(Seller2);
           
            break;
		}
		
		else if (opt == 10) { //add new products
			//int clientID;
			//recv(clientfd,&clientID,4,0);
			PRODUCTS Product;
            recv(clientfd,&Product,sizeof(PRODUCTS),0);
			int ProductID=addProduct(Product);
			
            send(clientfd,&ProductID,4,0);
			printf("PRODUCT %d add \n", ProductID);
			
            break;
		}
		
        else if (opt == 11) { //remove products
		
			int ProductID2;
			int clientID3;
			recv(clientfd,&clientID3,4,0);
            recv(clientfd,&ProductID2,4,0);
			removeProduct(ProductID2,clientID3);
            printf("PRODUCT %d removed \n", ProductID2);
            break;
		}


        else if (opt == 12) {  //update products
			int clientID4;
			PRODUCTS Product1;
			recv(clientfd,&clientID4,4,0);
            recv(clientfd,&Product1,sizeof(PRODUCTS),0);;
            updateProduct(clientID4,Product1);
            printf("Bill Created Successfully.\n");

            break;
		}

        else if (opt == 13) {  //view products
			int clientID5;
			recv(clientfd,&clientID5,4,0);
			printf("ID IS %d", clientID5);
			char *products;
            products=viewSellerProducts(clientID5);
			send(clientfd,products,BUFFER_SIZE,0);
            printf("seller products.\n");

            break;
		}
			
        else if (opt == 14) { //view orders
            int clientID6;
			recv(clientfd,&clientID6,4,0);
			char *orders;
            orders=viewOrders(clientID6);
			send(clientfd,orders,BUFFER_SIZE,0);
            printf("seller order.\n");

            break;
		}
			
        else if (opt == 0) {
            exit(0);
		}
        else {
            printf("connection cancelled \n");
            exit(1);
		}
    }
	 
}


