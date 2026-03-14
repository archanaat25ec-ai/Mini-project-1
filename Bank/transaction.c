#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void totalBalance(FILE *fPtr);

int main() {
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 8) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            case 5:
                displayAll(cfPtr);
                break;
            case 6:
                searchAccount(cfPtr);
                break;
            case 7:
                totalBalance(cfPtr);
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

void displayAll(FILE *fPtr) {
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum != 0) {
            printf("%-6d%-16s%-11s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

void searchAccount(FILE *fPtr) {
    struct clientData client;
    int acc;

    printf("Enter account number to search: ");
    scanf("%d", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Account not found\n");
    else
        printf("Account: %d %s %s Balance: %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
}

void totalBalance(FILE *fPtr) {
    struct clientData client;
    double total = 0;

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum != 0)
            total += client.balance;
    }

    printf("Total bank balance: %.2f\n", total);
}

void updateRecord(FILE *fPtr) {
    struct clientData client;
    int acc;
    double amount;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found\n");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);
    printf("Enter amount (+deposit / -withdraw): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated balance: %.2f\n", client.balance);
}

void newRecord(FILE *fPtr) {
    struct clientData client = {0,"","",0};
    int acc;

    printf("Enter new account number: ");
    scanf("%d", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account already exists\n");
        return;
    }

    printf("Enter LastName FirstName Balance: ");
    scanf("%s %s %lf", client.lastName, client.firstName, &client.balance);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
}

void deleteRecord(FILE *fPtr) {
    struct clientData blank = {0,"","",0};
    struct clientData client;
    int acc;

    printf("Enter account number to delete: ");
    scanf("%d", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Account not found\n");
    else {
        fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);
        printf("Account deleted\n");
    }
}

void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        printf("File cannot be created\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,"%-6s%-16s%-11s%10s\n","Acct","Last Name","First Name","Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr)) {
        if (client.acctNum != 0)
            fprintf(writePtr,"%d %s %s %.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
    }

    fclose(writePtr);
    printf("Text file created successfully\n");
}

unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
    printf("1. Export accounts to text file\n");
    printf("2. Deposit / Withdraw\n");
    printf("3. Add new account\n");
    printf("4. Delete account\n");
    printf("5. Display all accounts\n");
    printf("6. Search account\n");
    printf("7. Show total bank balance\n");
    printf("8. Exit\n");
    printf("Enter choice: ");

    scanf("%u",&choice);
    return choice;
}