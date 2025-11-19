//Version 2
#include "crm.h"

// --- Global Data ---
Customer *customerRoot = NULL; 
int customerCount = 0;
Quote *quoteHead = NULL; 
int quoteCount = 0;
Order *orderFront = NULL; 
Order *orderRear = NULL;  
int orderCount = 0;
Contract *contractHead = NULL; 
int contractCount = 0;

// IDs
int nextCustomerId = 1000;
int nextQuoteId = 2000;
int nextContractId = 3000;
int nextOrderId = 4000;

// --- Utilities ---
void press_enter_to_continue() {
    printf("\n> Press [Enter] to continue...");
    while (getchar() != '\n'); 
    getchar(); 
}

bool validate_email(const char *email) {
    int atCount = 0, dotAfterAt = 0, atPos = -1, len = strlen(email);
    if (len < 5) return false;
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') { atCount++; atPos = i; }
        if (atCount == 1 && email[i] == '.' && i > atPos) dotAfterAt = 1;
    }
    return (atCount == 1 && dotAfterAt && email[0] != '@' && email[len-1] != '.');
}

bool validate_date(const char *date) {
    int d, m, y;
    if (strlen(date) != 10 || date[2] != '-' || date[5] != '-') return false;
    if (sscanf(date, "%d-%d-%d", &d, &m, &y) != 3) return false;
    return (y >= 1900 && y <= 2100 && m >= 1 && m <= 12 && d >= 1 && d <= 31);
}

// --- BST Logic ---
Customer* insert_customer_node(Customer* root, Customer* newCustomer) {
    if (root == NULL) {
        newCustomer->left = NULL; newCustomer->right = NULL; return newCustomer;
    }
    if (newCustomer->customerId < root->customerId) root->left = insert_customer_node(root->left, newCustomer);
    else if (newCustomer->customerId > root->customerId) root->right = insert_customer_node(root->right, newCustomer);
    return root;
}

Customer* find_customer(int id) {
    Customer *curr = customerRoot;
    while (curr != NULL) {
        if (curr->customerId == id) return curr;
        else if (id < curr->customerId) curr = curr->left;
        else curr = curr->right;
    }
    return NULL;
}

void free_customer_bst(Customer *root) {
    if (root == NULL) return;
    free_customer_bst(root->left); free_customer_bst(root->right); free(root);
}

Quote* find_quote_node(int id) {
    Quote *curr = quoteHead;
    while (curr) { if (curr->quoteId == id) return curr; curr = curr->next; }
    return NULL;
}

// --- Initialization ---
void initialize_test_data() {
    Customer *c = (Customer*)malloc(sizeof(Customer));
    c->customerId = nextCustomerId++;
    strcpy(c->name, "Acme Corp"); strcpy(c->email, "admin@acme.com");
    strcpy(c->company, "Acme Inc."); strcpy(c->type, "Company");
    c->loyaltyPoints = 150; // High points to test display
    c->left = NULL; c->right = NULL;
    customerRoot = c; customerCount++;
    printf("System Initialized.\n");
}

// ==========================================
//      ADMIN / BUSINESS FEATURES
// ==========================================

void add_customer() {
    Customer *nc = (Customer*)malloc(sizeof(Customer));
    nc->customerId = nextCustomerId++;
    nc->loyaltyPoints = 0; 
    
    printf("\n--- New Customer Registration ---\n");
    int type;
    printf("1. Company  2. Individual: ");
    scanf("%d", &type);
    strcpy(nc->type, (type == 1) ? "Company" : "Individual");

    printf("Name: "); scanf(" %[^\n]s", nc->name);
    do { printf("Email: "); scanf(" %[^\n]s", nc->email); } while (!validate_email(nc->email));
    
    if (type == 1) { printf("Company Name: "); scanf(" %[^\n]s", nc->company); }
    else strcpy(nc->company, "Individual");

    customerRoot = insert_customer_node(customerRoot, nc);
    customerCount++;
    printf("Success! Customer ID is: [ %d ]\n", nc->customerId);
}

void display_customers_recursive(Customer *root) {
    if (root != NULL) {
        display_customers_recursive(root->left);
        printf("%d\t%-15s\t%s\t\t%d pts\n", root->customerId, root->name, root->type, root->loyaltyPoints);
        display_customers_recursive(root->right);
    }
}

void view_customers() {
    printf("\n--- Customer Database ---\n");
    printf("ID\tName\t\tType\t\tLoyalty\n");
    printf("------------------------------------------------\n");
    display_customers_recursive(customerRoot);
}

// Common order logic used by Admin and Customer
void internal_create_order(int custId) {
    Customer *c = find_customer(custId);
    if (!c) { printf("Customer not found.\n"); return; }

    Order *no = (Order*)malloc(sizeof(Order));
    no->orderId = nextOrderId++;
    no->customerId = custId;
    no->next = NULL;

    printf("\n--- New Order for %s ---\n", c->name);
    printf("Current Points: %d\n", c->loyaltyPoints);
    
    double discount = 0;
    if (c->loyaltyPoints > LOYALTY_THRESHOLD) {
        printf(">> LOYALTY REWARD AVAILABLE! <<\n");
        printf("Applying $%.2f discount.\n", DISCOUNT_AMOUNT);
        discount = DISCOUNT_AMOUNT;
        c->loyaltyPoints -= LOYALTY_THRESHOLD;
    }

    printf("Product Name: "); scanf(" %[^\n]s", no->product);
    printf("Total Amount: $"); scanf("%lf", &no->totalAmount);

    if (discount > 0) {
        if (no->totalAmount > discount) no->totalAmount -= discount;
        else {
            printf("Order value too low for discount. Points returned.\n");
            c->loyaltyPoints += LOYALTY_THRESHOLD;
        }
    }

    do { printf("Date (DD-MM-YYYY): "); scanf(" %s", no->date); } while (!validate_date(no->date));
    strcpy(no->status, "Pending");

    if (!orderFront) { orderFront = no; orderRear = no; }
    else { orderRear->next = no; orderRear = no; }
    orderCount++;
    printf("Order Placed! ID: %d | Final Pay: $%.2f\n", no->orderId, no->totalAmount);
}

void create_order_admin() {
    int id;
    printf("Enter Customer ID: ");
    scanf("%d", &id);
    internal_create_order(id);
}

void process_order() {
    if (!orderFront) { printf("No pending orders.\n"); return; }
    Order *temp = orderFront;
    orderFront = orderFront->next;
    if (!orderFront) orderRear = NULL;

    printf("\n--- Processing Order #%d ---\n", temp->orderId);
    Customer *c = find_customer(temp->customerId);
    if (c) {
        int earned = (int)(temp->totalAmount / 10);
        c->loyaltyPoints += earned;
        printf("Customer %s earned %d Loyalty Points.\n", c->name, earned);
    }
    printf("Order marked COMPLETED.\n");
    free(temp);
    orderCount--;
}

void view_all_orders() {
    printf("\n--- Active Order Queue ---\n");
    Order *curr = orderFront;
    while (curr) {
        printf("#%d | Cust: %d | Item: %s | $%.2f | %s\n", 
            curr->orderId, curr->customerId, curr->product, curr->totalAmount, curr->status);
        curr = curr->next;
    }
}

void create_sales_quote() {
    Quote *nq = (Quote*)malloc(sizeof(Quote));
    nq->quoteId = nextQuoteId++;
    printf("\n--- Generate Quote ---\n");
    printf("Customer ID: "); scanf("%d", &nq->customerId);
    Customer *c = find_customer(nq->customerId);
    if(!c) { printf("Not found.\n"); free(nq); return; }

    printf("Item: "); scanf(" %[^\n]s", nq->itemName);
    printf("Amount: $"); scanf("%lf", &nq->amount);
    do { printf("Date: "); scanf(" %s", nq->date); } while(!validate_date(nq->date));
    
    nq->accepted = false;
    nq->next = quoteHead; quoteHead = nq; quoteCount++;
    printf("Quote #%d Generated.\n", nq->quoteId);
}

void view_sales_quotes() {
    printf("\n--- Sales Quotes ---\n");
    Quote *curr = quoteHead;
    while(curr) {
        printf("#%d | Cust: %d | %s | $%.2f | Accepted: %s\n", 
            curr->quoteId, curr->customerId, curr->itemName, curr->amount, curr->accepted?"Yes":"No");
        curr = curr->next;
    }
}

void send_contract() {
    int qId;
    printf("Enter Quote ID to finalize: "); scanf("%d", &qId);
    Quote *q = find_quote_node(qId);
    if (!q) { printf("Quote not found.\n"); return; }

    Contract *nc = (Contract*)malloc(sizeof(Contract));
    nc->contractId = nextContractId++;
    nc->customerId = q->customerId;
    nc->relatedQuoteId = qId;
    nc->sent = true;
    nc->next = contractHead; contractHead = nc; contractCount++;
    
    q->accepted = true;
    printf("Contract #%d sent to Customer %d.\n", nc->contractId, nc->customerId);
    
    Customer *c = find_customer(q->customerId);
    if (c) {
        int pts = (int)(q->amount / 10);
        c->loyaltyPoints += pts;
        printf("Loyalty Update: +%d points.\n", pts);
    }
}

// --- ANALYTICS WITH LOYALTY BREAKDOWN ---
void print_loyalty_list(Customer *root) {
    if (root) {
        print_loyalty_list(root->left);
        printf("   - ID %d (%s): \t%d Points\n", root->customerId, root->name, root->loyaltyPoints);
        print_loyalty_list(root->right);
    }
}

void show_analytics() {
    printf("\n========================================\n");
    printf("      EXECUTIVE ANALYTICS DASHBOARD\n");
    printf("========================================\n");
    printf("1. RECORDS:\n");
    printf("   - Customers: %d\n   - Quotes: %d\n   - Orders: %d\n", customerCount, quoteCount, orderCount);

    // Sales Calc
    double totalPipe = 0, closedRev = 0;
    Quote *q = quoteHead;
    while(q) {
        totalPipe += q->amount;
        if(q->accepted) closedRev += q->amount;
        q = q->next;
    }
    printf("\n2. FINANCIALS:\n");
    printf("   - Pipeline: $%.2f\n   - Revenue:  $%.2f\n", totalPipe, closedRev);

    // Loyalty Per Customer
    printf("\n3. LOYALTY POINTS PER CUSTOMER:\n");
    if (customerRoot == NULL) printf("   (No customers yet)\n");
    else print_loyalty_list(customerRoot);

    printf("========================================\n");
}

// ==========================================
//      CUSTOMER PORTAL FEATURES
// ==========================================

void customer_view_profile(int myId) {
    Customer *c = find_customer(myId);
    printf("\n--- MY PROFILE ---\n");
    printf("Name: %s\n", c->name);
    printf("Type: %s\n", c->type);
    printf("Loyalty Points Balance: [ %d ]\n", c->loyaltyPoints);
    if (c->loyaltyPoints > LOYALTY_THRESHOLD) 
        printf(">> You have a discount available on your next order! <<\n");
    else
        printf(">> Earn %d more points for a discount.\n", LOYALTY_THRESHOLD - c->loyaltyPoints);
}

void customer_view_my_orders(int myId) {
    printf("\n--- MY ORDER HISTORY ---\n");
    Order *curr = orderFront;
    bool found = false;
    while (curr) {
        if (curr->customerId == myId) {
            printf("Order #%d | %s | $%.2f | %s\n", 
                curr->orderId, curr->product, curr->totalAmount, curr->status);
            found = true;
        }
        curr = curr->next;
    }
    if (!found) printf("No orders found.\n");
}

void customer_portal() {
    int id;
    printf("\n=== CUSTOMER LOGIN ===\n");
    printf("Enter your Customer ID: ");
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); return; }

    Customer *c = find_customer(id);
    if (c == NULL) {
        printf("Error: Customer ID not found. Please register with Business Admin first.\n");
        return;
    }

    printf("Welcome back, %s!\n", c->name);
    int choice;
    do {
        printf("\n--- CUSTOMER MENU (%s) ---\n", c->name);
        printf("1. View My Profile & Points\n");
        printf("2. Place New Order\n");
        printf("3. View My Orders\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: customer_view_profile(id); press_enter_to_continue(); break;
            case 2: internal_create_order(id); press_enter_to_continue(); break;
            case 3: customer_view_my_orders(id); press_enter_to_continue(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid.\n");
        }
    } while (choice != 0);
}

// ==========================================
//      ADMIN PORTAL MENU
// ==========================================
void admin_portal() {
    int choice;
    do {
        printf("\n=== BUSINESS ADMIN DASHBOARD ===\n");
        printf("1. Manage Customers\n");
        printf("2. Sales Quotes\n");
        printf("3. Contracts\n");
        printf("4. Manage Orders\n");
        printf("5. Analytics & Loyalty Report\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: 
                printf("\n[1] Add Customer  [2] View All: ");
                int c1; scanf("%d", &c1);
                if(c1==1) add_customer(); else view_customers();
                press_enter_to_continue();
                break;
            case 2:
                printf("\n[1] Create Quote  [2] View All: ");
                int c2; scanf("%d", &c2);
                if(c2==1) create_sales_quote(); else view_sales_quotes();
                press_enter_to_continue();
                break;
            case 3: send_contract(); press_enter_to_continue(); break;
            case 4:
                printf("\n[1] Create Order [2] Process [3] View All: ");
                int c4; scanf("%d", &c4);
                if(c4==1) create_order_admin(); 
                else if(c4==2) process_order(); 
                else view_all_orders();
                press_enter_to_continue();
                break;
            case 5: show_analytics(); press_enter_to_continue(); break;
            case 0: printf("Returning to Role Selection...\n"); break;
            default: printf("Invalid.\n");
        }
    } while (choice != 0);
}

void free_all_memory() {
    free_customer_bst(customerRoot);
}
