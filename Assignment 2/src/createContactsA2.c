#include "givenA2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>  // For offsetof()

// Helper function to read dynamically allocated strings
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
    FILE *fp = fopen(fileName, "a+b"); // Open for reading and appending
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    struct contact newContact = {0};
    int empId;
    char *firstName = NULL;
    char *lastName = NULL;
    char *email = NULL;

    // Get user input
    printf("Employee Id: ");
    scanf("%d", &empId);
    while (getchar() != '\n'); // Clear input buffer

    printf("First Name: ");
    firstName = readDynamicString();
    
    printf("Last Name: ");
    lastName = readDynamicString();
    
    printf("Email: ");
    email = readDynamicString();

    // Find the end of the file to position new contact
    fseek(fp, 0, SEEK_END);
    long contactPos = ftell(fp);

    // Initialize positions
    newContact.empIdPosn = contactPos + sizeof(struct contact);
    long dataPos = newContact.empIdPosn + sizeof(int);

    // Calculate positions for optional fields
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

    // Link to previous contact
    struct contact lastContact;
    long prevPos = 0;
    long currPos = 0;
    
    // Traverse existing contacts to find last one
    while (1) {
        fseek(fp, currPos, SEEK_SET);
        if (fread(&lastContact, sizeof(struct contact), 1, fp) != 1) break;
        prevPos = currPos;
        currPos = lastContact.next;
        if (lastContact.next == 0) break;
    }

    // Update previous contact's next pointer if needed
    if (prevPos != currPos && prevPos != 0) {
        fseek(fp, prevPos + offsetof(struct contact, next), SEEK_SET);
        fwrite(&contactPos, sizeof(long), 1, fp);
    }

    // Write the new contact structure
    fseek(fp, contactPos, SEEK_SET);
    fwrite(&newContact, sizeof(struct contact), 1, fp);

    // Write employee ID
    fseek(fp, newContact.empIdPosn, SEEK_SET);
    fwrite(&empId, sizeof(int), 1, fp);

    // Write optional fields
    if (newContact.firstNamePosn) {
        fseek(fp, newContact.firstNamePosn, SEEK_SET);
        fputs(firstName, fp);
        fputc('\0', fp);
    }
    if (newContact.lastNamePosn) {
        fseek(fp, newContact.lastNamePosn, SEEK_SET);
        fputs(lastName, fp);
        fputc('\0', fp);
    }
    
    if (newContact.emailPosn) {
        fseek(fp, newContact.emailPosn, SEEK_SET);
        fputs(email, fp);
        fputc('\0', fp);
    }

    // Cleanup
    free(firstName);
    free(lastName);
    free(email);
    fclose(fp);
}