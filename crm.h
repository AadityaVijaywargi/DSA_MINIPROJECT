//Version 2
#ifndef CRM_H
#define CRM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// --- Constants ---
#define NAME_SIZE 50
#define ID_SIZE 15
#define LOYALTY_THRESHOLD 100
#define DISCOUNT_AMOUNT 100.00
#define PROFIT_MARGIN 0.30

// --- Data Structures ---
typedef struct Customer {
    int customerId;
    char name[NAME_SIZE];
    char email[NAME_SIZE];
    char company[NAME_SIZE];
    char type[15];         
    int loyaltyPoints;      
    struct Customer *left; 
    struct Customer *right;
} Customer;

typedef struct Quote {
    int quoteId;
    int customerId;
    char itemName[NAME_SIZE];
    double amount;
    bool accepted;
    char date[ID_SIZE];
    struct Quote *next;
} Quote;

typedef struct Order {
    int orderId;
    int customerId;
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

// --- Globals ---
extern Customer *customerRoot; 
extern int customerCount;
extern Quote *quoteHead;
extern int quoteCount;
extern Order *orderFront; 
extern Order *orderRear;  
extern int orderCount;
extern Contract *contractHead; 
extern int contractCount;

// --- Prototypes ---
void initialize_test_data();
void press_enter_to_continue();
bool validate_email(const char *email);
bool validate_date(const char *date);

// Core Data Functions
Customer* insert_customer_node(Customer* root, Customer* newCustomer);
Customer* find_customer(int id);
Quote* find_quote_node(int id);
void free_all_memory();

// Business/Admin Features
void admin_portal();
void add_customer();
void view_customers(); // Shows everyone
void create_order_admin();
void process_order();
void view_all_orders();
void create_sales_quote();
void view_sales_quotes();
void send_contract();
void show_analytics(); // Includes Loyalty Leaderboard

// Customer Features
void customer_portal();
void customer_place_order(int myId);
void customer_view_my_orders(int myId);
void customer_view_profile(int myId);

// Menus
void main_role_selection_menu();

#endif
