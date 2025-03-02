#include "givenA2.h"
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
    if (!fp) {
        printf("Error opening file\n");
        return -1;
    }

    struct contact curr;
    long pos = 0;
    int recordCount = 0;

    printf("\nPrinting Employee records saved on file:\n");
    
    while (1) {
        fseek(fp, pos, SEEK_SET);
        if (fread(&curr, sizeof(struct contact), 1, fp) != 1) break;

        int empId;
        fseek(fp, curr.empIdPosn, SEEK_SET);
        fread(&empId, sizeof(int), 1, fp);

        char *firstName = curr.firstNamePosn ? readString(fp) : NULL;
        char *lastName = curr.lastNamePosn ? readString(fp) : NULL;
        char *email = curr.emailPosn ? readString(fp) : NULL;

        printf("\n*********************************************************\n");
        printf("Employee Id: %d\n", empId);
        if (firstName) printf("First Name: %s\n", firstName);
        if (lastName) printf("Last Name: %s\n", lastName);
        if (email) printf("Email: %s\n", email);
        printf("*********************************************************\n\n");

        free(firstName);
        free(lastName);
        free(email);

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
