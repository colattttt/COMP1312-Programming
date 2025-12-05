#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void clearLine() {
    // Clears the input buffer by removing all characters until the newline.
    // Prevents leftover input from affecting the next user entry.

    int c;

    while ((c = getchar()) != '\n' && c != EOF) 
    {

    }
}

int getPIN(char savedPIN[5]) 
{
    // Reads a PIN from the user and checks that it is exactly 4 digits.
    // Returns 1 if the PIN is valid and stores it in savedPIN, otherwise returns 0.

    char enteredPIN[10];

    printf("Enter 4-digit PIN: ");
    scanf("%9s", enteredPIN);

    if (strlen(enteredPIN) != 4) 
    {
        printf("PIN must be 4 digits!\n");
        while (getchar() != '\n');  
        return 0;
    }

    for (int i = 0; i < 4; i++) 
    {
        if (!isdigit(enteredPIN[i])) 
        {
            printf("Invalid PIN! PIN must contain digits only.\n");
            while (getchar() != '\n'); 
            return 0;
        }
    }

    strcpy(savedPIN, enteredPIN);
    return 1;
}

int chooseInt(const char *prompt, int min, int max) 
{
    // Displays a prompt and reads an integer from the user.
    // Returns the number if it is within [min, max], otherwise returns -1.

    int x;

    printf("%s", prompt);

    if (scanf("%d", &x) != 1) {
        clearLine();  
        return -1;
    }

    if (x < min || x > max) {
        return -1;
    }

    return x;  
}

float getValidAmount(const char *prompt) 
{
    // Prompts the user for a money amount.
    // Returns the value if it's between 1 and 50000; otherwise returns -1.

    float amt;
    printf("%s", prompt);
    if (scanf("%f", &amt) != 1) {
        clearLine();
        return -1;
    }
    if (amt <= 0 || amt > 50000) return -1;
    return amt;
}

int selectAccount(char accOut[20]) 
{
    // Loads all accounts from index.txt and shows them to the user.
    // Lets the user choose an account number from the list.
    // Returns 1 and stores the selected account in accOut, otherwise returns 0.

    FILE *fp = fopen("database/index.txt", "r");
    if (!fp) 
    {
        printf("No accounts found.\n");
        return 0;
    }

    char acc[100][20], name[100][50], text[200];
    int count = 0;

    while (fgets(text, sizeof(text), fp)) 
    {
        text[strcspn(text, "\n")] = '\0';   

        char *bar = strchr(text, '|');
        if (!bar) continue;               

        *bar = '\0';                    
        strcpy(acc[count], text);        
        strcpy(name[count], bar + 1);      

        count++;
    }

    if (count == 0) 
    {
        printf("No accounts found.\n");
        return 0;
    }

    printf("\nAvailable accounts (1-%d):\n", count);
    for (int i = 0; i < count; i++) 
    {
        printf("%d. %s (%s)\n", i + 1, acc[i], name[i]);
    }

    int choice = chooseInt("Select account: ", 1, count);

    if (choice == -1) 
    {
        printf("Invalid choice. Please enter a number between 1 and %d.\n", count);
        return 0;
    }

    strcpy(accOut, acc[choice - 1]);
    return 1;
}

int updateAmount(const char *accno, float newAmount) 
{
    // Updates only the Amount field inside an account file.
    // Copies all lines to a temp file, replaces the Amount line, then replaces the old file with the updated one.
    // Returns 1 on success, 0 on failure. 

    char accountFile[100];
    char tempFile[100];

    sprintf(accountFile, "database/%s.txt", accno);
    sprintf(tempFile, "database/tmp_%s.txt", accno);

    FILE *readFile = fopen(accountFile, "r");
    FILE *writeFile = fopen(tempFile, "w");

    if (!readFile || !writeFile) {
        return 0;
    }

    char currentLine[200];

    while (fgets(currentLine, sizeof(currentLine), readFile)) {

        if (strncmp(currentLine, "Amount:", 7) == 0) {
            fprintf(writeFile, "Amount: %.2f\n", newAmount);
        } else {
            fputs(currentLine, writeFile);
        }
    }

    fclose(readFile);
    fclose(writeFile);

    remove(accountFile);
    rename(tempFile, accountFile);

    return 1;
}

int loadAccountInfo(const char *accno, char *id, char *pin, char *type, float *amt)
{
    // Reads account details (ID, PIN, Type, Amount) from the account file.
    // Copies the data into the provided output variables.
    // Returns 1 if the file was read successfully, otherwise 0.

    char path[100];
    sprintf(path, "database/%s.txt", accno);
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    char line[200];
    char ID[20] = "", PIN[20] = "", TYPE[20] = "";
    float AMT = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "ID: %19s", ID);
        sscanf(line, "PIN: %19s", PIN);
        sscanf(line, "Account Type: %19s", TYPE);
        sscanf(line, "Amount: %f", &AMT);
    }
    fclose(fp);

    if (id) strcpy(id, ID);
    if (pin) strcpy(pin, PIN);
    if (type) strcpy(type, TYPE);
    if (amt) *amt = AMT;

    return 1;
}

int loadAccounts(char acc[100][20], char name[100][50]) 
{
    // Reads all account numbers and names from index.txt.
    // Stores them into the acc[] and name[] arrays.
    // Returns the total number of accounts loaded.

    FILE *fp = fopen("database/index.txt", "r");
    if (!fp) 
    {
        return 0;
    }

    char text[200];
    int count = 0;

    while (fgets(text, sizeof(text), fp)) 
    {
        text[strcspn(text, "\n")] = '\0';   

        char *bar = strchr(text, '|');
        if (!bar) continue;

        *bar = '\0';                 
        strcpy(acc[count], text);   
        strcpy(name[count], bar + 1);  

        count++;
    }

    fclose(fp);
    return count;
}

void showAccounts(char acc[100][20], char name[100][50], int count) 
{
    // Displays the list of accounts with their numbers and names.
    // Used when selecting sender or receiver accounts.

    printf("\nAvailable accounts (1-%d):\n", count);
    for (int i = 0; i < count; i++) 
    {
        printf("%d. %s %s\n", i + 1, acc[i], name[i]);
    }
}

int authenticateAccount(const char *accno, float *returnedBalance, char *returnedType) 
{
    // Verifies an account by checking the user’s PIN against the stored PIN.
    // If the PIN is correct, it can also return the account balance and type.
    // Returns 1 if authentication succeeds, otherwise 0.

    char filePIN[20], userPIN[5], type[20];
    float balance;

    loadAccountInfo(accno, NULL, filePIN, type, &balance);

    if (!getPIN(userPIN)) {
        printf("Invalid PIN.\n");
        return 0;
    }

    if (strcmp(userPIN, filePIN) != 0) {
        printf("Incorrect PIN.\n");
        return 0;
    }

    if (returnedBalance != NULL) {
        *returnedBalance = balance;
    }

    if (returnedType != NULL) {
        strcpy(returnedType, type);
    }

    return 1;
}

void logTransaction(const char *msg) 
{
    // Records a transaction message with a timestamp into transaction.log.
    // Used to track actions like create, delete, deposit, withdrawal, remittance, and quit.

    FILE *fp = fopen("database/transaction.log", "a");
    if (!fp) return;
    fprintf(fp, "%s - %s", msg, ctime(&(time_t){time(NULL)}));
    fclose(fp);
}

void showSessionInfo() 
{
    // This function displays the current session information, including date and time and the total number of accounts found in index.txt.

    FILE *fp = fopen("database/index.txt", "r");
    int count = 0;
    char line[200];

    if (fp) {

        while (fgets(line, sizeof(line), fp)) 
        {
            line[strcspn(line, "\n")] = '\0';   

            char *bar = strchr(line, '|');     
            if (!bar) continue;             

            count++;  
        }

        fclose(fp);
    }

    printf("\n---------- BANK SYSTEM SESSION ----------\n");
    printf("Date & Time: %s", ctime(&(time_t){time(NULL)}));
    printf("Total Accounts: %d\n", count);
    printf("-----------------------------------------\n");
}

void createAccount()
{
    // This function creates a new bank account. It checks the username and 12-digit ID,
    // ensures the user does not already have the same account type, generates a unique
    // account number, saves the account into its own file, updates index.txt,
    // and logs the creation action.

    char username[50], id[20], pin[5];
    int type;

    printf("\n---------- Create Bank Account ----------\n");
    printf("\nEnter your username: ");
    scanf(" %[^\n]", username);

    for (int i = 0; username[i]; i++)
    {
        if (!isalpha(username[i]) && username[i] != ' ') 
        {
            printf("Username can only contain letters and spaces!\n");
            return;
        }
    }

    printf("Enter your ID: ");
    scanf("%19s", id);

    if (strlen(id) != 12) {
        printf("Invalid ID! It must be exactly 12 digits.\n");
        return;
    }


    for (int i = 0; i < 12; i++) {
        if (!isdigit(id[i])) {
            printf("Invalid ID! Only numbers (0-9) are allowed.\n");
            return;
        }
    }

    printf("Select Account Type:");
    printf("\n1. Savings Account");
    printf("\n2. Current Account");
    type = chooseInt("\nEnter account type: ", 1, 2);
    
    if (type == -1) {
        printf("Invalid choice! Please enter 1 or 2.\n");
        return;
    }

    char acctype[20];
    if (type == 1) {
        strcpy(acctype, "Savings");
    } else {
        strcpy(acctype, "Current");
    }

    // Check if this ID already has this type
    FILE *fp = fopen("database/index.txt", "r");
    char text[200];

    while (fgets(text, sizeof(text), fp)) 
    {
        text[strcspn(text, "\n")] = '\0';

        char *bar = strchr(text, '|');
        if (!bar) continue;

        *bar = '\0';          
        char *accNum = text;  
        char *fullName = bar + 1; 

        char fileID[20], fileType[20];
        loadAccountInfo(accNum, fileID, NULL, fileType, NULL);

        if (strcmp(id, fileID) == 0 && strcmp(fileType, acctype) == 0) 
        {
            printf("You already have this type of account.\n");
            fclose(fp);
            return;

        } 
    }

    fclose(fp);
    
    if (!getPIN(pin)) {
    return;
    }

    // Generate unique account number
    srand(time(NULL));
    int accno;
    char path[100];
    
    do {
        accno = (rand() % 90000000) + 10000000;
        sprintf(path, "database/%d.txt", accno);

        fp = fopen(path, "r");
        if (fp != NULL) {
            fclose(fp);
            fp = NULL; 
        }
    } 
    while (fp != NULL);

    // Create new user file
    fp = fopen(path, "w");
    if (!fp) 
    {
        printf("Error: Could not create account file.\n");
        return;
    }

    fprintf(fp, "Username: %s\n", username);
    fprintf(fp, "ID: %s\n", id); 
    fprintf(fp, "Account No: %d\n", accno);
    fprintf(fp, "Account Type: %s\n", acctype);
    fprintf(fp, "PIN: %s\n", pin);
    fprintf(fp, "Amount: 0.00\n");

    fclose(fp);

    logTransaction("Account Created Successfully");

    FILE *fp_index = fopen("database/index.txt", "a");
    if (fp_index) 
    {
        fprintf(fp_index, "%d|%s\n", accno, username);
        fclose(fp_index);
    } 
    else 
    {
        printf("Warning: Could not open index file!\n");
    }

    printf("\nAccount created successfully!\n");
    printf("-----------------------------------------\n");
    printf("Username: %s\n",username);
    printf("ID: %s\n",id);
    printf("Account No: %d\n",accno);
    printf("Account Type:%s\n", (type == 1) ? "Savings" : "Current");
    printf("Amount: 0.00\n");
    printf("-----------------------------------------");
}

void deleteAccount()
{
    // This function deletes an account.
    // It checks the ID and PIN, removes the account's file,
    // and updates index.txt so the deleted account is no longer listed.

    char accno[20];

    printf("\n---------- Delete Bank Account ----------\n");
    if (!selectAccount(accno)) 
    {
        return;
    }

    char fileID[20], pin[20], type[20];
    float bal;
    loadAccountInfo(accno, fileID, pin, type, &bal);

    char last4[5];
    printf("Enter last 4 digits of ID: ");
    scanf("%4s", last4);

    if (strlen(fileID) < 4 || strcmp(last4, fileID + strlen(fileID) - 4) != 0) {
        printf("ID mismatch.\n");
        return;
    }

    if (!authenticateAccount(accno, NULL, NULL)) {
        return;
    }

    char filepath[100];
    sprintf(filepath, "database/%s.txt", accno);

    if (remove(filepath) != 0) {
        printf("Failed to delete account file.\n");
        return;
    }

    printf("Account file deleted.\n");

    FILE *old = fopen("database/index.txt", "r");
    char accList[200][20], nameList[200][50];
    int count = 0;

    if (old)
    {
        char line[200];
        while (fgets(line, sizeof(line), old))
        {
            line[strcspn(line, "\n")] = '\0';

            char *bar = strchr(line, '|');
            if (!bar)
                continue;

            *bar = '\0';
            strcpy(accList[count], line); 
            strcpy(nameList[count], bar + 1); 
            count++;
        }
        fclose(old);
    }

    FILE *newFile = fopen("database/index.txt", "w");
    if (!newFile)
    {
        printf("Error rewriting index file.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(accList[i], accno) != 0)   
        {
            fprintf(newFile, "%s|%s\n", accList[i], nameList[i]);
        }
    }

    fclose(newFile);

    logTransaction("Account Deleted Successfully");
    printf("Account deleted successfully.\n");
}

void deposit()
{
    // This function handles the deposit process: the user selects an account, enters a valid
    // deposit amount, passes PIN authentication, the new balance is calculated
    // and saved, and the transaction is recorded in the log.
    char accno[20];
    printf("\n---------------- Deposit ----------------\n");
    if (!selectAccount(accno)) 
    {
        return;
    }

    float currentBalance;
    if (!authenticateAccount(accno, &currentBalance, NULL))
    {
        return;
    }

    float depositAmount = getValidAmount("Enter deposit amount: RM ");
    if (depositAmount < 0) 
    {
        printf("Invalid amount. Enter 1 to 50000.\n");
        return;
    }

    currentBalance += depositAmount;
    updateAmount(accno, currentBalance);

    logTransaction("Deposit Successful");
    printf("New balance: RM %.2f\n", currentBalance);
}

void withdrawal()
{
    // This function handles the withdrawal process: the user selects an account, authenticates
    // with a PIN, enters a valid amount, the system checks for sufficient funds,
    // updates the balance, and logs the transaction.
    
    char accno[20];
    printf("\n-------------- Withdrawal ---------------\n");
    if (!selectAccount(accno))
    {
        return;
    }

    float currentBalance;
    if (!authenticateAccount(accno, &currentBalance, NULL)) {
        return;
    }
    
    printf("Current balance: RM %.2f\n", currentBalance);

    float withdrawAmount = getValidAmount("Enter withdrawal amount: RM ");
    if (withdrawAmount < 0) 
    {
        printf("Invalid amount. Enter 1 to 50000.\n");
        return;
    }

    if (withdrawAmount > currentBalance) 
    {
        printf("Insufficient balance. You only have RM %.2f.\n", currentBalance);
        return;
    }

    currentBalance -= withdrawAmount;
    updateAmount(accno, currentBalance);

    logTransaction("Withdrawal Successful");
    printf("New balance: RM %.2f\n", currentBalance);
}

void remittance()
{
    // This function handles money transfer between two accounts. The function loads all accounts,
    // allows the user to choose a sender and receiver, verifies PIN for the sender,
    // checks balance and calculates any applicable fee based on account types,
    // updates both account balances, and logs the remittance action.

    char accList[100][20], nameList[100][50];

    int count = loadAccounts(accList, nameList);
    if (count < 2) {
        printf("Need at least 2 accounts for remittance.\n");
        return;
    }

    printf("\n-------------- Remittance ---------------\n");

    showAccounts(accList, nameList, count);

    int senderChoice = chooseInt("Select sender account: ", 1, count);
    if (senderChoice == -1)
    {
        printf("Invalid sender account! Please select a number between 1 and %d.\n", count);
        return;
    }

    int receiverChoice = chooseInt("Select receiver account: ", 1, count);
    if (receiverChoice == -1) 
    {
        printf("Invalid receiver account! Please select a number between 1 and %d.\n", count);
        return;
    }

    if (senderChoice == receiverChoice) 
    {
        printf("Sender and receiver must be different accounts.\n");
        return;
    }

    char senderAcc[20], receiverAcc[20];
    strcpy(senderAcc, accList[senderChoice - 1]);
    strcpy(receiverAcc, accList[receiverChoice - 1]);

    float senderBalance, receiverBalance;
    char senderType[20], receiverType[20];

    if (!authenticateAccount(senderAcc, &senderBalance, senderType)) 
    {
        return;
    }

    loadAccountInfo(receiverAcc, NULL, NULL, receiverType, &receiverBalance);

    float transferAmount = getValidAmount("Enter remittance amount: RM ");
    if (transferAmount < 0) 
    {
        printf("Invalid amount. Enter 1 to 50000.\n");
        return;
    }

    if (transferAmount > senderBalance) 
    {
        printf("Insufficient balance. You only have RM %.2f.\n", senderBalance);
        return;
    }

    float fee = 0.00;
    if (strcmp(senderType, "Savings") == 0 && strcmp(receiverType, "Current") == 0) 
    {
        fee = transferAmount * 0.02;   
    }
    else if (strcmp(senderType, "Current") == 0 && strcmp(receiverType, "Savings") == 0) 
    {
        fee = transferAmount * 0.03;   
    }

    float totalDeduction = transferAmount + fee;

    if (totalDeduction > senderBalance) {
        printf("Insufficient balance to cover amount and fee of RM %.2f.\n", totalDeduction);
        return;
    }

    senderBalance -= totalDeduction;
    receiverBalance += transferAmount;

    updateAmount(senderAcc, senderBalance);
    updateAmount(receiverAcc, receiverBalance);

    logTransaction("Remittance Successful");

    printf("\nRemittance successful!\n");
    printf("Amount transferred: RM %.2f\n", transferAmount);
    printf("Fee charged: RM %.2f\n", fee);
    printf("Sender new balance: RM %.2f\n", senderBalance);
    printf("Receiver new balance: RM %.2f\n", receiverBalance);
}

void menu()
{
    // Displays the main menu and controls overall program flow.
    // This function runs continuously, shows session information,
    // reads the user’s menu choice, and calls the appropriate
    // banking operation (create, delete, deposit, withdrawal,
    // remittance, or exit). Each action is also written to the
    // transaction log. The loop only ends when the user chooses to quit.

    char input[20];

    while (1)
    {
        showSessionInfo();
        printf("\n---------- Main Menu & Session ----------\n");
        printf("1. Create New Bank Account\n");
        printf("2. Delete Bank Account\n");
        printf("3. Deposit\n");
        printf("4. Withdrawal\n");
        printf("5. Remittance\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%19s", input);

        if (strcmp(input,"1") == 0 || strcasecmp(input,"create") == 0)
        {
            logTransaction("Create Account");
            createAccount();
        }
        else if (strcmp(input, "2") == 0 || strcasecmp(input,"delete") == 0)
        {
            logTransaction("Delete Bank Account");
            deleteAccount();
        }
        else if (strcmp(input, "3") == 0 || strcasecmp(input,"deposit") == 0)
        {
            logTransaction("Deposit");
            deposit();
        }
        else if (strcmp(input, "4") == 0 || strcasecmp(input,"withdraw") == 0)
        {
            logTransaction("Withdrawal");
            withdrawal();
        }
        else if (strcmp(input, "5") == 0 || strcasecmp(input,"remittance") == 0)
        {
            logTransaction("Remittance");
            remittance();
        }
        else if (strcmp(input, "6") == 0 || strcasecmp(input,"quit") == 0)
        {
            logTransaction("Exit");
            printf("Exiting program. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }  
    }
}

int main()
{
    // Program entry point. 
    // Starts the banking system by calling the main menu function.
    // The program runs until the user chooses to exit.
    menu();
    return 0 ;
}