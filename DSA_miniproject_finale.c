#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // character handling functions

// --- Configuration Constants ---
#define NAME_SIZE 50
#define ID_SIZE 15
#define PROFIT_MARGIN 0.30

// --- CORE DATA STRUCTURES ---

typedef struct Customer {
    int customerId;
    char name[NAME_SIZE];
    char email[NAME_SIZE];
    char company[NAME_SIZE];
    // BST pointers for Customer structure
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

// --- GLOBAL DATA STORAGE (UPDATED for Customer BST) ---

Customer *customerRoot = NULL; // Head is now Root
int customerCount = 0;

Quote *quoteHead = NULL;
int quoteCount = 0;

Order *orderFront = NULL; 
Order *orderRear = NULL;  
int orderCount = 0;

Contract *contractHead = NULL; 
int contractCount = 0;

int nextCustomerId = 1000;
int nextQuoteId = 2000;
int nextContractId = 3000;
int nextOrderId = 4000;

// --- UTILITY FUNCTIONS ---

void press_enter_to_continue() {
    printf("\nPress [Enter] to return to the menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    getchar(); 
}

// Validate email format
bool validate_email(const char *email) {
    int atCount = 0;
    int dotAfterAt = 0;
    int atPosition = -1;
    int len = strlen(email);
    
    if (len < 5) return false; // Minimum: a@b.c
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atCount++;
            atPosition = i;
        }
        if (atCount == 1 && email[i] == '.' && i > atPosition) {
            dotAfterAt = 1;
        }
    }
    
    // Must have exactly one @, at least one dot after @, and not start/end with @ or .
    if (atCount != 1 || !dotAfterAt || email[0] == '@' || email[0] == '.' || 
        email[len-1] == '@' || email[len-1] == '.') {
        return false;
    }
    
    return true;
}

// Validate date format DD-MM-YYYY
bool validate_date(const char *date) {
    int day, month, year;
    
    // Check format: exactly 10 characters with dashes at positions 2 and 5
    if (strlen(date) != 10 || date[2] != '-' || date[5] != '-') {
        return false;
    }
    
    // Check if all other characters are digits
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(date[i])) {
            return false;
        }
    }
    
    // Parse the date
    if (sscanf(date, "%d-%d-%d", &day, &month, &year) != 3) {
        return false;
    }
    
    // Validate ranges
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Check days in month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Leap year check
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    if (day > daysInMonth[month - 1]) {
        return false;
    }
    
    return true;
}

// --- CUSTOMER BST FUNCTIONS ---

// BST Insertion Helper
Customer* insert_customer_node(Customer* root, Customer* newCustomer) {
    if (root == NULL) {
        newCustomer->left = NULL;
        newCustomer->right = NULL;
        return newCustomer;
    }

    if (newCustomer->customerId < root->customerId) {
        root->left = insert_customer_node(root->left, newCustomer);
    } else if (newCustomer->customerId > root->customerId) {
        root->right = insert_customer_node(root->right, newCustomer);
    }
    // Ignoring equal case as IDs are unique
    return root;
}

// BST Search for Customer
Customer* find_customer(int id) {
    Customer *current = customerRoot;
    while (current != NULL) {
        if (current->customerId == id) {
            return current;
        } else if (id < current->customerId) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

// In-order traversal to print customers
void display_customers_inorder(Customer *root) {
    if (root != NULL) {
        display_customers_inorder(root->left);
        printf("%d\t%s\t\t%s\t%s\n", 
               root->customerId, 
               root->name, 
               root->email, 
               root->company);
        display_customers_inorder(root->right);
    }
}

// Display Customers (Menu option helper)
void view_customers() {
    printf("\n--- Customers List (BST In-Order Traversal - %d Total) ---\n", customerCount);
    if (customerRoot == NULL) {
        printf("Error: No customers found.\n");
        return;
    }
    printf("ID\tName\t\tEmail\t\tCompany\n");
    printf("----------------------------------------------------------\n");
    display_customers_inorder(customerRoot);
}

// Free all memory in the Customer BST (Post-order traversal)
void free_customer_bst(Customer *root) {
    if (root == NULL) return;
    free_customer_bst(root->left);
    free_customer_bst(root->right);
    free(root);
}

// --- END CUSTOMER BST FUNCTIONS ---


Quote* find_quote_node(int id) {
    Quote *current = quoteHead;
    while (current != NULL) {
        if (current->quoteId == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void initialize_test_data() {
    Customer *initialCustomer = (Customer*)malloc(sizeof(Customer));
    if (initialCustomer == NULL) {
        printf("Error: Memory allocation failed during initialization.\n");
        return;
    }
    initialCustomer->customerId = nextCustomerId++;
    strcpy(initialCustomer->name, "Acme Corp");
    strcpy(initialCustomer->email, "contact@acme.com");
    strcpy(initialCustomer->company, "Acme Inc.");
    initialCustomer->left = NULL;
    initialCustomer->right = NULL;

    customerRoot = initialCustomer; // Set as the initial root
    customerCount++;

    printf("System initialized with one test customer (ID: 1000).\n");
}

// --- CUSTOMER FUNCTIONS (MODIFIED for BST) ---

void add_customer() {
    Customer *newCustomer = (Customer*)malloc(sizeof(Customer));
    if (newCustomer == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    newCustomer->customerId = nextCustomerId++;
    
    printf("\n--- Add New Customer ---\n");
    printf("Enter Name: ");
    scanf(" %[^\n]s", newCustomer->name);
    
    // Email validation loop
    do {
        printf("Enter Email: ");
        scanf(" %[^\n]s", newCustomer->email);
        if (!validate_email(newCustomer->email)) {
            printf("Invalid email format! Please enter a valid email (e.g., user@example.com)\n");
        }
    } while (!validate_email(newCustomer->email));
    
    printf("Enter Company: ");
    scanf(" %[^\n]s", newCustomer->company);

    // Insert into BST
    customerRoot = insert_customer_node(customerRoot, newCustomer);
    
    customerCount++;
    printf("Customer added successfully! ID: %d\n", newCustomer->customerId);
}

// --- ORDER FUNCTIONS (MODIFIED to remove (Enqueue) and (Dequeue) printouts) ---

void create_order() {
    Order *newOrder = (Order*)malloc(sizeof(Order));
    if (newOrder == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    newOrder->orderId = nextOrderId++;
    newOrder->next = NULL;

    printf("\n--- Create New Order ---\n");
    printf("Enter Seller ID: ");
    if (scanf("%d", &newOrder->sellerId) != 1) {
        printf("Invalid input.\n");
        free(newOrder);
        return;
    }

    printf("Enter Total Amount: $");
    if (scanf("%lf", &newOrder->totalAmount) != 1) {
        printf("Invalid input.\n");
        free(newOrder);
        return;
    }
    printf("Enter Product Name: ");
    scanf(" %[^\n]s", newOrder->product);
    
    // Date validation loop
    do {
        printf("Enter Date (DD-MM-YYYY): ");
        scanf(" %s", newOrder->date);
        if (!validate_date(newOrder->date)) {
            printf("Invalid date format! Please use DD-MM-YYYY (e.g., 15-03-2024)\n");
        }
    } while (!validate_date(newOrder->date));
    
    strcpy(newOrder->status, "Pending"); 

    if (orderRear == NULL) {
        orderFront = newOrder;
        orderRear = newOrder;
    } else {
        orderRear->next = newOrder;
        orderRear = newOrder;
    }
    orderCount++;
    // REMOVED: Order created and added to queue! (Enqueue)
    printf("Order created and added to queue! ID: %d\n", newOrder->orderId); 
}

void process_order() {
    if (orderFront == NULL) {
        printf("\n--- Process Order ---\n");
        printf("Error: The order queue is empty. Nothing to process.\n");
        return;
    }

    Order *temp = orderFront;
    orderFront = orderFront->next;

    if (orderFront == NULL) {
        orderRear = NULL;
    }

    printf("\n--- Order Processed ---\n"); // REMOVED: (Dequeue)
    printf("Order ID: %d (Seller ID: %d)\n", temp->orderId, temp->sellerId);
    printf("Product: %s | Amount: $%.2f\n", temp->product, temp->totalAmount);
    printf("Order marked as COMPLETED and removed from the queue.\n");
    
    free(temp); 
    orderCount--;
}

void view_orders() {
    printf("\n--- Orders Queue (%d Total) ---\n", orderCount);
    if (orderFront == NULL) {
        printf("Error: No orders in the queue.\n");
        return;
    }
    printf("ID\tSeller ID\tAmount\t\tProduct\t\tStatus\n");
    printf("----------------------------------------------------------------\n");

    Order *current = orderFront;
    while(current != NULL) {
        printf("%d\t%d\t\t$%.2f\t\t%s\t\t%s\n",
            current->orderId,
            current->sellerId,
            current->totalAmount,
            current->product,
            current->status
        );
        current = current->next;
    }
}

// --- QUOTE FUNCTIONS ---

void create_sales_quote() {
    Quote *newQuote = (Quote*)malloc(sizeof(Quote));
    if (newQuote == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    newQuote->quoteId = nextQuoteId++;
    
    printf("\n--- Create New Sales Quote ---\n");
    printf("Enter Customer ID: ");
    if (scanf("%d", &newQuote->customerId) != 1) {
        printf("Invalid input.\n");
        free(newQuote);
        return;
    }
    // find_customer now uses the BST search
    if (find_customer(newQuote->customerId) == NULL) {
        printf("Error: Customer ID %d not found.\n", newQuote->customerId);
        free(newQuote);
        return;
    }

    printf("Enter Quote Amount: $");
    if (scanf("%lf", &newQuote->amount) != 1) {
        printf("Invalid input.\n");
        free(newQuote);
        return;
    }
    
    // Date validation loop
    do {
        printf("Enter Date (DD-MM-YYYY): ");
        scanf(" %s", newQuote->date);
        if (!validate_date(newQuote->date)) {
            printf("Invalid date format! Please use DD-MM-YYYY (e.g., 15-03-2024)\n");
        }
    } while (!validate_date(newQuote->date));
    
    newQuote->accepted = false; 

    newQuote->next = quoteHead;
    quoteHead = newQuote;

    quoteCount++;
    printf("Sales Quote created successfully! ID: %d\n", newQuote->quoteId);
}

void view_sales_quotes() {
    printf("\n--- Sales Quotes List (%d Total) ---\n", quoteCount);
    if (quoteHead == NULL) {
        printf("Error: No quotes found.\n");
        return;
    }
    printf("ID\tCust ID\tAmount\tDate\t\tAccepted\n");
    printf("--------------------------------------------------\n");
    
    Quote *current = quoteHead;
    while(current != NULL) {
        printf("%d\t%d\t$%.2f\t%s\t%s\n",
            current->quoteId,
            current->customerId,
            current->amount,
            current->date,
            current->accepted ? "Yes" : "No"
        );
        current = current->next;
    }
}

// --- CONTRACT FUNCTION ---

void send_contract() {
    // Check if quotes exist
    if (quoteHead == NULL) {
        printf("\n--- Send Contract ---\n");
        printf("Error: No quotes available. Please create a quote first.\n");
        return;
    }
    
    Contract *newContract = (Contract*)malloc(sizeof(Contract));
    if (newContract == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    
    int qId;
    printf("\n--- Send Contract ---\n");
    printf("Enter Quote ID to base contract on: ");
    if (scanf("%d", &qId) != 1) {
        printf("Invalid input.\n");
        free(newContract);
        return;
    }

    Quote *associatedQuote = find_quote_node(qId);

    if (associatedQuote != NULL) {
        newContract->contractId = nextContractId++;
        newContract->relatedQuoteId = qId;
        newContract->customerId = associatedQuote->customerId;
        
        char temp_id[10];
        sprintf(temp_id, "%d", newContract->contractId);
        strcpy(newContract->documentPath, "doc/contract_");
        strcat(newContract->documentPath, temp_id);
        strcat(newContract->documentPath, ".txt");
        newContract->sent = true;

        newContract->next = contractHead;
        contractHead = newContract;

        contractCount++;
        
        printf("Contract (ID: %d) successfully generated and 'sent' to customer %d.\n", 
               newContract->contractId, newContract->customerId);
        
        associatedQuote->accepted = true;
        printf("Associated Quote (ID: %d) marked as ACCEPTED.\n", qId);

    } else {
        printf("Error: Quote ID not found.\n");
        free(newContract);
    }
}

// --- ANALYTICS ---

void show_analytics() {
    printf("\n--- CRM Analytics Dashboard ---\n");
    
    printf("Total Records:\n");
    printf("  - Customers: %d\n", customerCount);
    printf("  - Sales Quotes: %d\n", quoteCount);
    printf("  - Orders in Queue: %d\n", orderCount);
    printf("  - Contracts Sent: %d\n", contractCount);

    int acceptedQuotes = 0;
    double totalQuoteValue = 0.0;
    double acceptedQuoteValue = 0.0;

    Quote *currentQuote = quoteHead;
    while(currentQuote != NULL) {
        totalQuoteValue += currentQuote->amount;
        if (currentQuote->accepted) {
            acceptedQuotes++;
            acceptedQuoteValue += currentQuote->amount;
        }
        currentQuote = currentQuote->next;
    }
    
    double conversionRate = (quoteCount > 0) ? ((double)acceptedQuotes / quoteCount) * 100.0 : 0.0;

    printf("\nSales Performance:\n");
    printf("  - Quotes Accepted: %d\n", acceptedQuotes);
    printf("  - Quote Conversion Rate: %.2f%%\n", conversionRate);
    printf("  - Total Quoted Value: $%.2f\n", totalQuoteValue);
    printf("  - Total Accepted Value (Potential Revenue): $%.2f\n", acceptedQuoteValue);

    double totalOrderRevenue = 0.0;
    Order *currentOrder = orderFront;
    while(currentOrder != NULL) {
        totalOrderRevenue += currentOrder->totalAmount;
        currentOrder = currentOrder->next;
    }

    printf("\nOrder Value:\n");
    printf("  - Total Pending Order Revenue: $%.2f\n", totalOrderRevenue);

    double orderProfit = totalOrderRevenue * PROFIT_MARGIN;
    double potentialProfit = acceptedQuoteValue * PROFIT_MARGIN;

    printf("\nProfit Analysis (Based on %.0f%% Margin):\n", PROFIT_MARGIN * 100);
    printf("  - **Pending Order Profit (Guaranteed):** **$%.2f**\n", orderProfit);
    printf("  - **Potential Profit (From All Accepted Quotes):** **$%.2f**\n", potentialProfit);
    printf("  - Total Cost of Goods Sold (COGS) in Queue: $%.2f\n", totalOrderRevenue * (1.0 - PROFIT_MARGIN));
}

// --- MENU HANDLERS ---

void display_main_menu() {
    printf("\n====================================\n");
    printf("  Simple C-CRM Command Line Platform\n");
    printf("====================================\n");
    printf("1. Customer Management\n");
    printf("2. Sales Quotes\n");
    printf("3. Send Contract (Simulate)\n");
    printf("4. Order Management\n");
    printf("5. Show Analytics\n");
    printf("0. Exit\n");
    printf("------------------------------------\n");
    printf("Enter your choice: ");
}

void handle_customer_menu() {
    int choice;
    do {
        printf("\n--- Customer Management ---\n");
        printf("1. Add New Customer (BST Insert)\n");
        printf("2. View All Customers (BST In-Order)\n"); 
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0; 
            while (getchar() != '\n'); 
        }

        switch (choice) {
            case 1:
                add_customer();
                press_enter_to_continue();
                break;
            case 2:
                view_customers();
                press_enter_to_continue();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void handle_quotes_menu() {
    int choice;
    do {
        printf("\n--- Sales Quotes Management ---\n");
        printf("1. Create New Sales Quote\n");
        printf("2. View All Sales Quotes\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0;
            while (getchar() != '\n');
        }

        switch (choice) {
            case 1:
                create_sales_quote();
                press_enter_to_continue();
                break;
            case 2:
                view_sales_quotes();
                press_enter_to_continue();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void handle_orders_menu() {
    int choice;
    do {
        printf("\n--- Order Management ---\n");
        printf("1. Create New Order\n");
        printf("2. View All Orders\n");
        printf("3. Process Oldest Order\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0;
            while (getchar() != '\n');
        }

        switch (choice) {
            case 1:
                create_order();
                press_enter_to_continue();
                break;
            case 2:
                view_orders();
                press_enter_to_continue();
                break;
            case 3:
                process_order();
                press_enter_to_continue();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

// --- Memory Cleanup Function ---
void free_all_memory() {
    printf("Freeing allocated memory...\n");
    // Free Customer BST (Post-order traversal)
    free_customer_bst(customerRoot);

    // Free Quote Linked List
    Quote *currentQuote = quoteHead;
    Quote *tempQuote;
    while (currentQuote != NULL) {
        tempQuote = currentQuote;
        currentQuote = currentQuote->next;
        free(tempQuote);
    }
    
    // Free Order Queue
    Order *currentOrder = orderFront;
    Order *tempOrder;
    while (currentOrder != NULL) {
        tempOrder = currentOrder;
        currentOrder = currentOrder->next;
        free(tempOrder);
    }

    // Free Contract Linked List
    Contract *currentContract = contractHead;
    Contract *tempContract;
    while (currentContract != NULL) {
        tempContract = currentContract;
        currentContract = currentContract->next;
        free(tempContract);
    }

    printf("Memory freed successfully.\n");
}

// --- Main Program ---
int main() {
    int choice;
    printf("Welcome to the C-CRM Simulator!\n");
    initialize_test_data();
    
    do {
        display_main_menu();

        if (scanf("%d", &choice) != 1) {
            choice = -1; 
            while (getchar() != '\n'); 
        }

        switch (choice) {
            case 1:
                handle_customer_menu();
                break;
            case 2:
                handle_quotes_menu();
                break;
            case 3:
                send_contract();
                press_enter_to_continue();
                break;
            case 4:
                handle_orders_menu();
                break;
            case 5:
                show_analytics();
                press_enter_to_continue();
                break;
            case 0:
                printf("\nExiting C-CRM. Goodbye!\n");
                free_all_memory(); // Added memory cleanup call
                break;
            default:
                printf("Invalid choice. Please enter a number from the menu.\n");
                press_enter_to_continue();
                break;
        }
    } while (choice != 0);

    return 0;
}
