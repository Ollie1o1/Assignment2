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
#include <string.h>
#include <ctype.h>
#include <stddef.h>  // For offsetof()

int isValidName(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) { 
            if (str[i] != ' ') return 0; // Allow spaces in names
        }
    }
    return 1;
}

char* readDynamicString() {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char *str = malloc(strlen(buffer) + 1);
    if (str != NULL) {
        strcpy(str, buffer);
    }
    return str;
}

void createContacts(char *fileName) {
    FILE *fp = fopen(fileName, "rb+");
    if (fp == NULL) {
        fp = fopen(fileName, "wb+");
        if (fp == NULL) {
            return;
        }
    }

    struct contact newContact = {0};
    int empId;
    char *firstName = NULL;
    char *lastName = NULL;
    char *email = NULL;

    // Get valid Employee ID
    while (1) {
        printf("Employee Id (1-9999): ");
        if (scanf("%d", &empId) == 1 && empId >= 1 && empId <= 9999) {
            while (getchar() != '\n'); // Clear input buffer
            break;
        }
        while (getchar() != '\n'); // Clear invalid input
    }

    // Get valid First Name
    while (1) {
        printf("First Name: ");
        firstName = readDynamicString();
        if (firstName != NULL && isValidName(firstName)) break;
        free(firstName);
    }

    // Get valid Last Name
    while (1) {
        printf("Last Name: ");
        lastName = readDynamicString();
        if (lastName != NULL && isValidName(lastName)) break;
        free(lastName);
    }

    // Get Email
    printf("Email: ");
    email = readDynamicString();

    // Calculate positions
    fseek(fp, 0, SEEK_END);
    long contactPos = ftell(fp);
    newContact.empIdPosn = contactPos + sizeof(struct contact);
    long dataPos = newContact.empIdPosn + sizeof(int);

    // Update position pointers for optional fields
    if (firstName != NULL && strlen(firstName) > 0) {
        newContact.firstNamePosn = dataPos;
        dataPos += strlen(firstName) + 1;
    }
    if (lastName != NULL && strlen(lastName) > 0) {
        newContact.lastNamePosn = dataPos;
        dataPos += strlen(lastName) + 1;
    }
    if (email != NULL && strlen(email) > 0) {
        newContact.emailPosn = dataPos;
        dataPos += strlen(email) + 1;
    }

    // Find last contact in linked list
    struct contact lastContact;
    long prevPos = -1;
    long currentPos = 0;

    // Follow next pointers
    do {
        fseek(fp, currentPos, SEEK_SET);
        if (fread(&lastContact, sizeof(struct contact), 1, fp) != 1) break;
        prevPos = currentPos;
        currentPos = lastContact.next;
    } while (currentPos != 0);

    // Update previous contact's next pointer
    if (prevPos != -1) {
        fseek(fp, prevPos + offsetof(struct contact, next), SEEK_SET);
        fwrite(&contactPos, sizeof(long), 1, fp);
    }

    // Write new contact structure
    newContact.next = 0;
    fseek(fp, contactPos, SEEK_SET);
    fwrite(&newContact, sizeof(struct contact), 1, fp);

    // Write data fields
    fseek(fp, newContact.empIdPosn, SEEK_SET);
    fwrite(&empId, sizeof(int), 1, fp);

    if (newContact.firstNamePosn) {
        fseek(fp, newContact.firstNamePosn, SEEK_SET);
        fwrite(firstName, strlen(firstName) + 1, 1, fp);
    }
    if (newContact.lastNamePosn) {
        fseek(fp, newContact.lastNamePosn, SEEK_SET);
        fwrite(lastName, strlen(lastName) + 1, 1, fp);
    }
    if (newContact.emailPosn) {
        fseek(fp, newContact.emailPosn, SEEK_SET);
        fwrite(email, strlen(email) + 1, 1, fp);
    }

    // Cleanup
    free(firstName);
    free(lastName);
    free(email);
    fclose(fp);
}