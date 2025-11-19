#include "crm.h"

int main() {
    int roleChoice;
    initialize_test_data();

    do {
        printf("\n====================================\n");
        printf("   C-CRM: LOYALTY EDITION v2.0\n");
        printf("====================================\n");
        printf("Who are you?\n");
        printf("1. Business Admin\n");
        printf("2. Customer\n");
        printf("0. Close App\n");
        printf("------------------------------------\n");
        printf("Enter Role: ");
        
        if (scanf("%d", &roleChoice) != 1) {
            roleChoice = -1;
            while (getchar() != '\n'); 
        }

        switch (roleChoice) {
            case 1: 
                admin_portal(); 
                break;
            case 2: 
                customer_portal(); 
                break;
            case 0: 
                printf("Shutting down...\n"); 
                free_all_memory(); 
                break;
            default: 
                printf("Invalid selection.\n");
        }
    } while (roleChoice != 0);

    return 0;
}
