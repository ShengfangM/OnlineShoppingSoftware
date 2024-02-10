// Author: Shengfang Ma
// Email: shengfang.ma@okstate.edu
// Description: this is the server which call function to handle communication with clients

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"
//--------------------------------------------------------------------
char customerFIle[]={"CustomerInformation.txt"};
char sellerFile[]={"SellerInformation.txt"};
char productFile[]={"ProductInformation.txt"};
char billFile[]={"BillingInformation.txt"};
char orderFile[]={"CustomerOrder.txt"};

//--------------------------------------------------------------------


#define BUFFER_SIZE 8096

#define MAX_PRODUCT 500
#define MAX_BUYER 1000
#define MAX_SELLER 100
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//new seller register
int addSeller( SELLERS seller_info) {
	
    seller_info.SellerID=getClientID(sellerFile);
    printf("Buyer Id is-------------------------------------: %d \n",seller_info.SellerID);
	
	FILE *fp;
    fp=fopen (sellerFile,"a+");
    fprintf(fp,"%d %s %s %s \n", seller_info.SellerID,seller_info.SellerName,seller_info.SContactNumber, seller_info.SellerAddress);

    fclose(fp);
	
	return seller_info.SellerID;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//new buyer register
int addBuyer( BUYERS buyer_info) {
	
    buyer_info.CustomerID=getClientID(customerFIle);
    printf("Buyer Id is-------------------------------------: %d \n",buyer_info.CustomerID);
	
	FILE *fp;
    fp=fopen (customerFIle,"a+");
    fprintf(fp,"%d %s %s %s \n", buyer_info.CustomerID,buyer_info.CustomerName,buyer_info.ContactNumber, buyer_info.CustomerAddress);

    fclose(fp);

	return buyer_info.CustomerID;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//get Id for new client
int getClientID(char * filename)
{
    FILE *fp;
    int value=0;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		char name[20];
		char number[14];
		char address[20];

		while (!feof(fp)) {		
			fscanf(fp,"%d %s %s %s \n", &value,name,number, address);
			
		}
		fclose(fp);
	}

    return value+1;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//updata buyer_info
void updateBuyer(BUYERS buyer_info){
	
	
    int value=0;
	char tempfile[]="temp.txt";
	FILE *fp;
	fp=fopen(customerFIle,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		char name[20];
		char number[14];
		char address[20];

		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {	
	
			fscanf(fp,"%d %s %s %s \n", &value,name,number, address);
			//printf("ID is %d \n", buyer_info.CustomerID);
			//printf("value is %d \n", value);
			if (value==buyer_info.CustomerID){
				
				strcpy(name,buyer_info.CustomerName);
				strcpy(number,buyer_info.ContactNumber);
				strcpy(address,buyer_info.CustomerAddress);
			}
			fprintf(fp1,"%d %s %s %s \n", value,name,number, address);

			
		}
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, customerFIle);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }

	}

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//updata seller_info
void updateSeller(SELLERS seller_info){
		
    int value=0;
	char tempfile[]="temp.txt";
	FILE *fp;
    fp=fopen(sellerFile,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		char name[20];
		char number[14];
		char address[20];

		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %s %s \n", &value,name,number, address);
			if (value==seller_info.SellerID){
				
				strcpy(name,seller_info.SellerName);
				strcpy(number,seller_info.SContactNumber);
				strcpy(address,seller_info.SellerAddress);
				
				//fprintf(fp,"%d %s %s %s \n", seller_info.SellerID,seller_info.SellerName,seller_info.SContactNumber, seller_info.SellerAddress);

			}
			fprintf(fp1,"%d %s %s %s \n", value,name,number, address);

		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, sellerFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }

	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//view product_info
char * viewProducts( ){
	
	char  * products;
	products =  malloc(BUFFER_SIZE);

	FILE *fp;
    int value=0;
	int i=0;
	char buffer[256];
    fp=fopen(productFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		printf("1Products database failed\n");
		return NULL;		
    }
	else{
			
			strcpy(products,"product ID, product Description, seller ID, product Quantity, product Price \n ");
		while (fgets(buffer, 500, fp) != NULL) {		
			strcat(products,buffer);
			i++;
		}

		fclose(fp);
		//printf("1. products list %s", products);
		return products;		
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//view oders belong to buyer
char * viewBills( int buyerID){
	
	char  * billings;
	billings =  malloc(BUFFER_SIZE);

	FILE *fp;
    int i=0;
	char buffer[256];
    fp=fopen(billFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		return NULL;		
    }
	else{
		int orderID; 
		char address[20];
		int totalBill;
		int value=0;
		strcpy(billings,"order ID, address, total bill \n ");
		while (!feof(fp)) {		
			fscanf(fp,"%d %d %s %d \n", &orderID, &value,address,&totalBill );
			if (value==buyerID){
				
			char strNum[25];
			sprintf(strNum,"%d",orderID); 
			strcat(billings,strNum);
			strcat(billings,", ");
			
			strcat(billings,address);
			strcat(billings,", ");
			
			sprintf(strNum,"%d",totalBill); 
			strcat(billings,strNum);			
			strcat(billings,"\n");
			
		}

	}

		fclose(fp);			
		return billings;		
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//seller add products 
int  addProduct(PRODUCTS product_info){
	
	//product_info.productID=getPID(productFile);
    //printf("product Id is-------------------------------------: %d \n",product_info.productID);
	
	FILE *fp;
    fp=fopen (productFile,"a+");
    fprintf(fp,"%d %s %d %d %d\n", product_info.productID,product_info.productDescription,product_info.sellerID, product_info.productQuantity, product_info.productPrice);

    fclose(fp);

	return product_info.productID;

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//get Id for new client
int getPID(char * filename)
{
    FILE *fp;
    int value=0;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		char name[20];
		int number;

		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &value,name, &number, &number, &number);
			
		}
		fclose(fp);
	}

	
    return value+1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//updata product
void updateProduct(int seller_id,PRODUCTS product_info){

    //int value=0;
	char tempfile[]="temp.txt";
	FILE *fp;
    fp=fopen(productFile,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		int productID; 
		char decription[20];
		int quantity;
		int value=0;
		int price;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &productID,decription, &value,&quantity,&price );
			if (productID==product_info.productID && value==seller_id){
				
				strcpy(decription,product_info.productDescription);
				value=product_info.sellerID;
				quantity=product_info.productQuantity;
				price=product_info.productPrice;
				
			}
			fprintf(fp1,"%d %s %d %d %d \n", productID,decription, value,quantity,price );
		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, productFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }

	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//remove product
void removeProduct(int prodID , int seller_id){
	
	char tempfile[]="temp.txt";
	FILE *fp;
    fp=fopen(productFile,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		int productID; 
		char decription[20];
		int quantity;
		int SID=0;
		int price;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &productID,decription, &SID,&quantity,&price );
			if (productID ==prodID && seller_id ==SID){
				
			}
			else{
				fprintf(fp1,"%d %s %d %d %d \n", productID,decription, SID,quantity,price );
			}
		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, productFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }

	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//seller view products belong to seller
char* viewSellerProducts(int sellerID){
	
	char  * products;
	products =  malloc(BUFFER_SIZE);

	FILE *fp;
    int i=0;
	char buffer[256];
    fp=fopen(productFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		return NULL;		
    }
	else{
		int prodID; 
		char description[20];
		int sID;
		int quantity=0;
		int price=0;
		strcpy(products,"product ID, product Description, seller ID, product Quantity, product Price \n ");
		while (!feof(fp)) {	
		
			fscanf(fp,"%d %s %d %d %d\n", &prodID,description, &sID, &quantity,&price );
			if (sID==sellerID){
				
			char strNum[25];
			sprintf(strNum,"%d",prodID); 
			strcat(products,strNum);
			strcat(products,", ");
			
			strcat(products,description);
			strcat(products,", ");
			
			sprintf(strNum,"%d",sID); 
			strcat(products,strNum);
			strcat(products,", ");
			
			sprintf(strNum,"%d",quantity); 
			strcat(products,strNum);
			strcat(products,", ");
			
			sprintf(strNum,"%d",price); 
			strcat(products,strNum);
			strcat(products,"\n");
			
		}

	}
		fclose(fp);			
		return products;		
	}

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//view oders belong to sellers
//view oders belong to sellers
char * viewOrders( int sellerID){
	
	char  * orders;
	orders =  malloc(BUFFER_SIZE);

	FILE *fp;
    int i=0;
	char buffer[256];
    fp=fopen(orderFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		return NULL;		
    }
	else{
		int * productID=getProductID(sellerID);
		
		int orderID; 
		char address[20];
		int totalBill;
		int value=0;
		int quantity;
		
		strcpy(orders,"order ID, product ID, product quantity, deliver address\n ");
		while (!feof(fp)) {		
			fscanf(fp,"%d %d %d %s %d \n", &orderID, &value,&quantity,address,&totalBill );
			for(i=0;i<productID[0]-1;i++){
			if (value==productID[i+1]){
				
			char strNum[25];
			
			sprintf(strNum,"%d",orderID); 
			strcat(orders,strNum);
			strcat(orders,", ");
			sprintf(strNum,"%d",value); 
			strcat(orders,strNum);
			strcat(orders,", ");
			sprintf(strNum,"%d",quantity); 
			strcat(orders,strNum);
			strcat(orders,", ");
			strcat(orders,address);
			strcat(orders,", ");
			
			sprintf(strNum,"%d",totalBill); 
			strcat(orders,strNum);			
			strcat(orders,"\n");
			}
			
			}
			
		}
		fclose(fp);
		return orders;		
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//get products belongs to the seller
int * getProductID(int sellerID){
	
	int * prodID;
	prodID=(int *)malloc(sizeof(int)*100);
	
    int i=0;
	char buffer[256];
	FILE *fp;
    fp=fopen(productFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		return NULL;		
    }
	else{
		int product_id; 
		char decription[20];
		int quantity;
		int value=0;
		int price;
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &product_id,decription, &value,&quantity,&price );
			if (value==sellerID){
				prodID[i]=product_id;
				i++;
			}
		}
		
		int * productID;
	    productID=(int *)malloc(sizeof(int)*i);
		productID[0]=i;
		for (int j=1; j<i; j++ ){
			productID[i]=prodID[i-1];
		}
		fclose(fp);
		free(prodID);
		return productID;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//PURCHASE product
int purchaseProduct(int product_id, int product_quantity){

    int value=0;
	char tempfile[]="temp.txt";
	FILE *fp;
    fp=fopen(productFile,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		int productID; 
		char decription[20];
		int quantity;
		int sID=0;
		int price;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &productID,decription, &sID,&quantity,&price );
			if (product_id==productID){
				printf("product_id %d, productID %d\n", product_id, productID);
				if(product_quantity>quantity) {
					value=-1;
				}
				else{
					quantity=quantity-product_quantity;
					value=price*product_quantity;
				}
				
				
			}
			fprintf(fp1,"%d %s %d %d %d \n", productID,decription, sID,quantity,price );
		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, productFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }
       
	}
	return value;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//make order
int makeBill(BUYERS Buyer,int totalbill){
	
	int OrderID;
	
	OrderID=getOrderID(billFile);
    printf("product Id is-------------------------------------: %d \n",OrderID);
	
	FILE *fp;
    fp=fopen (billFile,"a+");
    fprintf(fp,"%d %d %s %d\n", OrderID, Buyer.CustomerID, Buyer.CustomerAddress,totalbill);

    fclose(fp);

	return OrderID;
} 

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//get Id for new order
int getOrderID(char * filename)
{
    FILE *fp;
    int value=0;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		char address[20];
		int number;
		int number2;

		while (!feof(fp)) {		
			fscanf(fp,"%d  %d %s %d \n", &value,&number, address, &number2);
			
		}
		fclose(fp);
	}

	
    return value+1;
}
//
//
void makeOrder(int order_id,BUYERS Buyer,int *productlist,int *bill_list,int n){
	
	FILE *fp;
    fp=fopen (orderFile,"a+");
	for (int i=0; i<n; i++){
    fprintf(fp,"%d %d %d %s %d\n", order_id, productlist[i*2+0], productlist[i*2+1], Buyer.CustomerAddress,bill_list[i]);
	}
    fclose(fp);

}
//

int returnProduct(int product_id, int product_quantity){

    int value=0;
	char tempfile[]="temp.txt";
	FILE *fp;
    fp=fopen(productFile,"r+");
    if(fp==NULL)
    {
		fclose(fp);
    }
	else{
		int productID; 
		char decription[20];
		int quantity;
		int sID=0;
		int price;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %s %d %d %d \n", &productID,decription, &sID,&quantity,&price );
			if (product_id==productID){
				
					quantity=quantity+product_quantity;
					value=price*product_quantity;
				}
				
			fprintf(fp1,"%d %s %d %d %d \n", productID,decription, sID,quantity,price );
		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, productFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }
       
	}
	return value;
}
//
//
void updateBill(int order_id, int bill){
	
	char tempfile[]="temp.txt";
	int value;
	FILE *fp;
    int i=0;
    fp=fopen(billFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		//return NULL;		
    }
	else{
		int OrderID; 
		char address[20];
		int CustomerID;
		int totalBill=0;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %d %s %d \n", &OrderID, &CustomerID,address,&totalBill );
		
			if (OrderID==order_id ){
				totalBill=totalBill-bill;
			}
			fprintf(fp1,"%d %d %s %d \n", OrderID, CustomerID,address,totalBill );
			
			
		}
		
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, billFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }
	   
			
	}
	
}

//
void updateOrder(int order_id,int product_id, int product_num,int bill){
		
	char tempfile[]="temp.txt";

	FILE *fp;
    int i=0;
    fp=fopen(orderFile,"r");
    if(fp==NULL)
    {
		fclose(fp);
		//return NULL;		
    }
	else{
		int orderID; 		
		int productID;
		int quantity=0;
		char address[20];
		int OrderTotal;
		
		FILE *fp1;
		fp1=fopen(tempfile,"w");
		
		while (!feof(fp)) {		
			fscanf(fp,"%d %d %d %s %d \n", &orderID, &productID, &quantity,address,&OrderTotal );
			if (orderID==order_id && product_id==productID){
				product_num=product_num-quantity;
				OrderTotal=OrderTotal-bill;
			}	
			fprintf(fp1,"%d %d %d %s %d \n", orderID, productID, quantity,address,OrderTotal );			
		}
		fclose(fp);
		fclose(fp1);
		int ret = rename(tempfile, orderFile);
	
	   if(ret == 0) {
		  printf("File renamed successfully");
	   } else {
		  printf("Error: unable to rename the file");
	   }
	   		
	}
	
}
