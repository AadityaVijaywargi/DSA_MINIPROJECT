#include "functions.h"

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
                free_all_memory();
                break;
            default:
                printf("Invalid choice. Please enter a number from the menu.\n");
                press_enter_to_continue();
                break;
        }
    } while (choice != 0);

    return 0;
}

