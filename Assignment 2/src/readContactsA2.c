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
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readString(FILE *fp) {
    char *str = NULL;
    int size = 0;
    int capacity = 16;
    int c;

    str = malloc(capacity);
    if (!str) return NULL;

    while ((c = fgetc(fp))) {
        if (c == EOF || c == '\0') break;
        
        if (size >= capacity-1) {
            capacity *= 2;
            char *temp = realloc(str, capacity);
            if (!temp) {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[size++] = c;
    }
    
    str[size] = '\0';
    return str;
}

int readContacts(char *fileName) {
    FILE *fp = fopen(fileName, "rb");
    if (!fp) return -1;

    struct contact curr;
    long pos = 0;
    int recordCount = 0;

    printf("\nPrinting Employee records saved on file:\n");
    
    while (1) {
        // Read contact structure
        fseek(fp, pos, SEEK_SET);
        if (fread(&curr, sizeof(struct contact), 1, fp) != 1) break;

        // Read employee ID
        int empId;
        fseek(fp, curr.empIdPosn, SEEK_SET);
        fread(&empId, sizeof(int), 1, fp);

        // Read optional fields
        char *firstName = NULL;
        char *lastName = NULL;
        char *email = NULL;

        if (curr.firstNamePosn > 0) {
            fseek(fp, curr.firstNamePosn, SEEK_SET);
            firstName = readString(fp);
        }

        if (curr.lastNamePosn > 0) {
            fseek(fp, curr.lastNamePosn, SEEK_SET);
            lastName = readString(fp);
        }

        if (curr.emailPosn > 0) {
            fseek(fp, curr.emailPosn, SEEK_SET);
            email = readString(fp);
        }

        // Print formatted output
        printf("\n*********************************************************\n");
        printf("Employee Id: %d\n", empId);
        if (firstName) printf("First Name: %s\n", firstName);
        if (lastName) printf("Last Name: %s\n", lastName);
        if (email) printf("Email: %s\n", email);
        printf("*********************************************************\n\n");

        // Cleanup
        free(firstName);
        free(lastName);
        free(email);

        // Move to next record
        if (curr.next == 0) break;
        pos = curr.next;
        recordCount++;
    }

    fclose(fp);
    
    if (recordCount == 0) {
        printf("No records found in file.\n");
    }
    
    return 0;
}