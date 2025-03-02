#include "givenA2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>  // For offsetof()

char* readDynamicString() {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    
    char *str = malloc(strlen(buffer) + 1);
    if (str != NULL) {
        strcpy(str, buffer);
    }
    return str;
}

void createContacts(char *fileName) {
    FILE *fp = fopen(fileName, "rb+");
    if (fp == NULL) {
        fp = fopen(fileName, "wb+");  // If file does not exist, create it
        if (fp == NULL) {
            perror("Error opening file");
            return;
        }
    }

    struct contact newContact = {0};
    int empId;
    char *firstName = NULL;
    char *lastName = NULL;
    char *email = NULL;

    // Get user input
    printf("Employee Id: ");
    scanf("%d", &empId);
    while (getchar() != '\n');

    printf("First Name: ");
    firstName = readDynamicString();
    
    printf("Last Name: ");
    lastName = readDynamicString();
    
    printf("Email: ");
    email = readDynamicString();

    // Move to the end of the file to determine new contact position
    fseek(fp, 0, SEEK_END);
    long contactPos = ftell(fp);

    // Initialize positions
    newContact.empIdPosn = contactPos + sizeof(struct contact);
    long dataPos = newContact.empIdPosn + sizeof(int);

    if (firstName && strlen(firstName) > 0) {
        newContact.firstNamePosn = dataPos;
        dataPos += strlen(firstName) + 1;
    }
    if (lastName && strlen(lastName) > 0) {
        newContact.lastNamePosn = dataPos;
        dataPos += strlen(lastName) + 1;
    }
    if (email && strlen(email) > 0) {
        newContact.emailPosn = dataPos;
        dataPos += strlen(email) + 1;
    }

    // Find the last contact and update its `next` pointer
    struct contact lastContact;
    long prevPos = -1, currPos = 0;

    fseek(fp, 0, SEEK_SET);
    while (fread(&lastContact, sizeof(struct contact), 1, fp) == 1) {
        if (lastContact.next == 0) {  // Last contact in the list
            prevPos = currPos;
        }
        currPos = lastContact.next;
        if (currPos == 0) break;
    }

    // If a previous contact exists, update its `next` pointer
    if (prevPos != -1) {
        fseek(fp, prevPos + offsetof(struct contact, next), SEEK_SET);
        fwrite(&contactPos, sizeof(long), 1, fp);
    }

    // Append new contact
    fseek(fp, contactPos, SEEK_SET);
    fwrite(&newContact, sizeof(struct contact), 1, fp);

    // Write employee ID
    fseek(fp, newContact.empIdPosn, SEEK_SET);
    fwrite(&empId, sizeof(int), 1, fp);

    // Write optional fields
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
