#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NAME_SIZE 50
#define ID_SIZE 15
#define PROFIT_MARGIN 0.30

// --- CORE DATA STRUCTURES ---
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

// --- FUNCTION PROTOTYPES ---
void initialize_test_data();
void display_main_menu();
void handle_customer_menu();
void handle_quotes_menu();
void send_contract();
void handle_orders_menu();
void show_analytics();
void press_enter_to_continue();
void free_all_memory();

#endif


