#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function declarations
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void totalBalance(FILE *fPtr);

// Web-compatible functions
void addRecord(FILE *fPtr, int acc, char lname[], char fname[], double bal);
void deleteRecordByAcc(FILE *fPtr, int acc);
void searchAccountByAcc(FILE *fPtr, int acc);
void updateRecordByAcc(FILE *fPtr, int acc, double amount);

int main(int argc, char *argv[]) {
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0,"","",0};
        for (int i = 0; i < 100; i++) {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }
    }

    // 🔥 WEB MODE (Flask support)
    if (argc > 1) {

        if (strcmp(argv[1], "display") == 0)
            displayAll(cfPtr);

        else if (strcmp(argv[1], "total") == 0)
            totalBalance(cfPtr);

        else if (strcmp(argv[1], "search") == 0) {
            int acc = atoi(argv[2]);
            searchAccountByAcc(cfPtr, acc);
        }

        else if (strcmp(argv[1], "add") == 0) {
            int acc = atoi(argv[2]);
            char *lname = argv[3];
            char *fname = argv[4];
            double bal = atof(argv[5]);

            addRecord(cfPtr, acc, lname, fname, bal);
        }

        else if (strcmp(argv[1], "update") == 0) {
            int acc = atoi(argv[2]);
            double amt = atof(argv[3]);
            updateRecordByAcc(cfPtr, acc, amt);
        }

        else if (strcmp(argv[1], "delete") == 0) {
            int acc = atoi(argv[2]);
            deleteRecordByAcc(cfPtr, acc);
        }

        fclose(cfPtr);
        return 0;
    }

    // 🖥️ NORMAL MENU MODE
    while ((choice = enterChoice()) != 8) {
        switch (choice) {
            case 1: textFile(cfPtr); break;
            case 2: updateRecord(cfPtr); break;
            case 3: newRecord(cfPtr); break;
            case 4: deleteRecord(cfPtr); break;
            case 5: displayAll(cfPtr); break;
            case 6: searchAccount(cfPtr); break;
            case 7: totalBalance(cfPtr); break;
            default: printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// ---------------- EXISTING FUNCTIONS ----------------

void displayAll(FILE *fPtr) {
    struct clientData client;
    rewind(fPtr);

    printf("Acct LastName FirstName Balance\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr)) {
        if (client.acctNum != 0)
            printf("%d %s %s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
    }
}

void searchAccount(FILE *fPtr) {
    struct clientData client;
    int acc;

    printf("Enter account number: ");
    scanf("%d", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Not found\n");
    else
        printf("%d %s %s %.2f\n",
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

    printf("Total Balance: %.2f\n", total);
}

void updateRecord(FILE *fPtr) {
    int acc;
    double amt;

    printf("Enter account: ");
    scanf("%d", &acc);

    printf("Enter amount: ");
    scanf("%lf", &amt);

    updateRecordByAcc(fPtr, acc, amt);
}

void newRecord(FILE *fPtr) {
    int acc;
    char lname[15], fname[10];
    double bal;

    printf("Enter acc lname fname balance: ");
    scanf("%d %s %s %lf", &acc, lname, fname, &bal);

    addRecord(fPtr, acc, lname, fname, bal);
}

void deleteRecord(FILE *fPtr) {
    int acc;

    printf("Enter account: ");
    scanf("%d", &acc);

    deleteRecordByAcc(fPtr, acc);
}

// ---------------- WEB FUNCTIONS ----------------

void addRecord(FILE *fPtr, int acc, char lname[], char fname[], double bal) {
    struct clientData client;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account exists\n");
        return;
    }

    client.acctNum = acc;
    strcpy(client.lastName, lname);
    strcpy(client.firstName, fname);
    client.balance = bal;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account added\n");
}

void deleteRecordByAcc(FILE *fPtr, int acc) {
    struct clientData blank = {0,"","",0};

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Deleted\n");
}

void searchAccountByAcc(FILE *fPtr, int acc) {
    struct clientData client;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Not found\n");
    else
        printf("%d %s %s %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
}

void updateRecordByAcc(FILE *fPtr, int acc, double amount) {
    struct clientData client;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Not found\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated\n");
}

// ---------------- MENU ----------------

unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n1.Text File 2.Update 3.Add 4.Delete 5.Display 6.Search 7.Total 8.Exit\n");
    scanf("%u", &choice);

    return choice;
}

void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    rewind(readPtr);

    while (fread(&client, sizeof(struct clientData), 1, readPtr)) {
        if (client.acctNum != 0)
            fprintf(writePtr,"%d %s %s %.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
    }

    fclose(writePtr);
}