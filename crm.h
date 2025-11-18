#ifndef CRM_H
#define CRM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// --- CONFIG CONSTANTS ---
#define NAME_SIZE 50
#define ID_SIZE 15
#define PROFIT_MARGIN 0.30

// --- STRUCT DEFINITIONS ---

typedef struct Customer {
    int customerId;
    char name[NAME_SIZE];
    char email[NAME_SIZE];
    char company[NAME_SIZE];
    struct Customer *left;
    struct Customer *right;
} Customer;

typedef struct Quote {
    int quoteId;
    int customerId;
    double amount;
    bool accepted;
    char date[ID_SIZE];
    struct Quote *next;
} Quote;

typedef struct Order {
    int orderId;
    int sellerId;
    double totalAmount;
    char product[NAME_SIZE];
    char date[ID_SIZE];
    char status[ID_SIZE];
    struct Order *next;
} Order;

typedef struct Contract {
    int contractId;
    int customerId;
    int relatedQuoteId;
    char documentPath[NAME_SIZE];
    bool sent;
    struct Contract *next;
} Contract;


// --- GLOBALS (ONLY DECLARATIONS HERE) ---
extern Customer *customerRoot;
extern int customerCount;

extern Quote *quoteHead;
extern int quoteCount;

extern Order *orderFront;
extern Order *orderRear;
extern int orderCount;

extern Contract *contractHead;
extern int contractCount;

extern int nextCustomerId;
extern int nextQuoteId;
extern int nextOrderId;
extern int nextContractId;


// --- FUNCTION DECLARATIONS ---

// Utility
void press_enter_to_continue();
bool validate_email(const char *email);
bool validate_date(const char *date);

// Customer / BST
Customer* insert_customer_node(Customer* root, Customer* newCustomer);
Customer* find_customer(int id);
void display_customers_inorder(Customer *root);
void view_customers();
void free_customer_bst(Customer *root);
void add_customer();
void initialize_test_data();

// Quotes
void create_sales_quote();
void view_sales_quotes();
Quote* find_quote_node(int id);

// Orders
void create_order();
void process_order();
void view_orders();

// Contracts
void send_contract();

// Analytics
void show_analytics();

// Menus
void display_main_menu();
void handle_customer_menu();
void handle_quotes_menu();
void handle_orders_menu();

// Memory Cleanup
void free_all_memory();

#endif
