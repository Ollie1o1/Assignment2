/*
Student Name: Oliver Raczka
Student ID: 1341067
Due Date: March 2, 2025
Course: CIS*2500
I have exclusive control over this submission via my password.
By including this header comment, I certify that:
1) I have read and understood the policy on academic integrity.
2) I have completed Moodle's module on academic integrity.
3) I have achieved at least 80% on the academic integrity quiz
I assert that this work is my own. I have appropriate acknowledged
any and all material that I have used, be it directly quoted or
paraphrased. Furthermore, I certify that this assignment was written
by me in its entirety.
*/
#include "givenA2.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char choice[10];
    int menuChoice, empId, result;
    
    // Stage 1: Create initial contacts
    do {
        printf("Do you wish to enter a new contact (Yes or No)?: ");
        scanf("%s", choice);
        if (strcmp(choice, "Yes") == 0) {
            createContacts(argv[1]);
        }
    } while (strcmp(choice, "No") != 0);

    // Stage 2: Menu system
    do {
        printf("\nEnter a choice:\n1 to print, 2 to search, 3 to create more, -1 to exit: ");
        scanf("%d", &menuChoice);

        switch(menuChoice) {
            case 1:
                if (readContacts(argv[1])) {
                    printf("Error reading file\n");
                }
                break;
            case 2:
                do { // Get valid Employee ID
                    printf("Search for Employee Id? ");
                    scanf("%d", &empId);
                } while(empId < 1 || empId > 9999);
                
                result = searchContacts(argv[1], empId);
                if (result == 0) {
                    printf("No match found.\n");
                } else if (result == -1) {
                    printf("Error opening file\n");
                }
                break;
            case 3:
                createContacts(argv[1]);
                break;
            case -1:
                printf("Exiting\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (menuChoice != -1); 

    return 0;
}