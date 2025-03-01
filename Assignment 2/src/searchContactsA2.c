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

int searchContacts(char *fileName, int keyId) {
    FILE *fp = fopen(fileName, "rb");
    if (!fp) return -1;

    struct contact curr;
    long pos = 0;
    int found = 0;

    while (1) {
        fseek(fp, pos, SEEK_SET);
        if (fread(&curr, sizeof(struct contact), 1, fp) != 1) break;

        int currId;
        fseek(fp, curr.empIdPosn, SEEK_SET);
        fread(&currId, sizeof(int), 1, fp);

        if (currId == keyId) {
            char *first = curr.firstNamePosn ? readString(fp) : NULL;
            char *last = curr.lastNamePosn ? readString(fp) : NULL;
            char *email = curr.emailPosn ? readString(fp) : NULL;

            printf("************************\n");
            printf("Employee ID: %d\n", currId);
            if (first) printf("First Name: %s\n", first);
            if (last) printf("Last Name: %s\n", last);
            if (email) printf("Email: %s\n", email);
            printf("************************\n");

            free(first);
            free(last);
            free(email);
            found = 1;
            break;
        }

        if (curr.next == 0) break;
        pos = curr.next;
    }

    fclose(fp);
    return found;
}