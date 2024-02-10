

#ifndef _DATABASE_H_
#define _DATABASE_H_


//-----------------------------------------------------------------
typedef struct customerInformation {
    int CustomerID;
    char CustomerName[20];
    char ContactNumber[14];
    char CustomerAddress[20];
} BUYERS;

//------------------------------------------------------------------
typedef struct sellerInformation {
    int SellerID;
    char SellerName[20];
    char SContactNumber[14];
    char SellerAddress[20];
} SELLERS;

//------------------------------------------------------------------
typedef struct productInformation {
    int productID;
    char productDescription[20];
    int sellerID;
    int productQuantity;
    int productPrice;
} PRODUCTS;

//-----------------------------------------------------------------
typedef struct billingInformation {
    int OrderID;
    int CustomerID;
    char CustomerAddress[20];
    int totalBill;
} BILLINGS;

//----------------------------------------------------------------
typedef struct customerOrder {
    int OrderID;
    int ProductID;
    int QuantityPurchased;
    char CustomerAddress[20];
    int OrderTotal;
} ORDERS;
//----------------------------------------------------------------

int addSeller(SELLERS );
int addBuyer(BUYERS );
int getClientID(char * );
void updateBuyer(BUYERS );
void updateSeller(SELLERS );


char * viewBills(int );
int * getProductID(int);
char * viewOrders(int );

char * viewProducts();
char * viewSellerProducts(int);

int addProduct(PRODUCTS  );
int getPID(char * );

int getOrderID(char * );
void removeProduct(int ,int );
void updateProduct(int, PRODUCTS );

int purchaseProduct(int , int);
int makeBill(BUYERS ,int );
void makeOrder(int ,BUYERS ,int *,int *,int );

int returnProduct(int , int);
void updateOrder(int ,int , int ,int );
void updateBill(int , int );
//---------------------------------------------------------------------

#endif