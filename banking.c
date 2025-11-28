#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void createAccount();
void deleteAccount();
void deposit();
void withdrawal();
void remittance();
void showSessionInfo();
void logTransaction(const char *msg);

void clearLine() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) 
    {

    }
}

int getPIN(char savedPIN[5]) 
{
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

int chooseInt(const char *prompt, int min, int max) {
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

float getValidAmount(const char *prompt) {
    float amt;
    printf("%s", prompt);
    if (scanf("%f", &amt) != 1) {
        clearLine();
        return -1;
    }
    if (amt <= 0 || amt > 50000) return -1;
    return amt;
}

int selectAccount(char accOut[20]) {
    FILE *fp = fopen("database/index.txt", "r");
    if (!fp) 
    {
        printf("No accounts found.\n");
        return 0;
    }

    char acc[100][20], name[100][50];
    int count = 0;

    while (fscanf(fp, "%19s %49s", acc[count], name[count]) == 2) 
    {
        count++;
    }
    fclose(fp);

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

int updateAmount(const char *accno, float newAmount) {
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

int authenticateAccount(const char *accno, float *returnedBalance, char *returnedType) {
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

void logTransaction(const char *msg) {
    FILE *fp = fopen("database/transaction.log", "a");
    if (!fp) return;
    fprintf(fp, "%s - %s", msg, ctime(&(time_t){time(NULL)}));
    fclose(fp);
}

void showSessionInfo() {
    FILE *fp = fopen("database/index.txt", "r");
    int count = 0;
    char a[20], n[50];

    if (fp) {
        while (fscanf(fp, "%19s %49s", a, n) == 2) count++;
        fclose(fp);
    }

    printf("\n---------- BANK SYSTEM SESSION ----------\n");
    printf("Date & Time: %s", ctime(&(time_t){time(NULL)}));
    printf("Total Accounts: %d\n", count);
    printf("-----------------------------------------\n");
}

void createAccount()
{
    char username[50], id[20], pin[5];
    int type;

    printf("\n---------- Create Bank Account ----------\n");
    printf("\nEnter your username: ");
    // Allow up to 49 characters for a string. To avoid crash or memory error if the user types too many characters.
    scanf("%49s", username);

    for (int i = 0; username[i]; i++)
    {
        if (!isalpha(username[i])) {
            printf("Username must contain letters only!\n");
            return;
        }
    }

    printf("Enter your ID: ");
    scanf("%19s", id);

    // Validate digits only
    for (int i = 0; id[i]; i++)
    {
        if (!isdigit(id[i])) {
            printf("Invalid ID! Must contain numbers only.\n");
            return;
        }
    }

    printf("Enter Account Type:");
    printf("\n1. Savings Account");
    printf("\n2. Current Account");
    type = chooseInt("Enter account type: ", 1, 2);
    if (type == -1) {
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
    char acc[20], name[50];

    if (fp) 
    {
        while (fscanf(fp, "%19s %49s", acc, name) == 2) 
        {
            char fileID[20], fileType[20];
            loadAccountInfo(acc, fileID, NULL, fileType, NULL);
            if (strcmp(id, fileID) == 0 && strcmp(fileType, acctype) == 0) 
            {
                printf("You already have this type of account.\n");
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }
    
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
        fprintf(fp_index, "%d %s\n", accno,username); // or "%d %d\n", accno, type
        fclose(fp_index);
    } 
    else 
    {
        printf("Warning: Could not open index file!\n");
    }

    printf("\nAccount created successfully!\n");
    printf("----------------------------------------\n");
    printf("Username: %s\n",username);
    printf("ID: %s\n",id);
    printf("Account No: %d\n",accno);
    printf("Account Type:%s\n", (type == 1) ? "Savings" : "Current");
    printf("Amount: 0.00\n");
    printf("----------------------------------------");
}

void deleteAccount()
{
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
    FILE *temp = fopen("database/temp.txt", "w");

    char idAcc[20], username[50];

    while (fscanf(old, "%19s %49s", idAcc, username) == 2) {
        if (strcmp(idAcc, accno) != 0) {
            fprintf(temp, "%s %s\n", idAcc, username);
        }
    }

    fclose(old);
    fclose(temp);
    remove("database/index.txt");
    rename("database/temp.txt", "database/index.txt");

    logTransaction("Account Deleted Successfully");
    printf("Account deleted successfully.\n");
}

void deposit()
{
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


    FILE *fp = fopen("database/index.txt", "r");
    if (!fp)
    {
        printf("No accounts found.\n");
        return;
    }


    // while (fscanf(fp, "%s %s", accno, username) == 2)
    // {   
    //     strcpy(listacc[count], accno);
    //     count++;
    // }
    // fclose(fp);

    // if (count == 0)
    // {
    //     printf("No accounts found.\n");
    //     return;
    // }

    // printf("Please choose an option (1-%d):\n\n", count);

    // // Show account list again
    // fp = fopen("database/index.txt", "r");
    // int i = 1;
    // while (fscanf(fp, "%s %s", accno, username) == 2)
    // {
    //     printf("%d. %s (%s)\n", i, accno, username);
    //     i++;
    // }
    // fclose(fp);

    // printf("\nSelect option: ");
    // scanf("%d", &option);

    // if (option < 1 || option > count)
    // {
    //     printf("Invalid choice.\n");
    //     return;
    // }

    // // Selected account number
    // strcpy(selectedAcc, listacc[option - 1]);

    // char pin[5], storedPIN[10];
    // printf("Enter 4-digit PIN: ");
    // scanf("%4s", pin);

    // // Open the selected account file
    // char filepath[100];
    // sprintf(filepath, "database/%s.txt", selectedAcc);

    // FILE *accFile = fopen(filepath, "r");
    // if (!accFile)
    // {
    //     printf("Account not found.\n");
    //     return;
    // }

    // float balance = 0;
    // char line[200], storedID[20]="";
    
    // while (fgets(line, sizeof(line), accFile))
    // {
    //     sscanf(line, "PIN: %9s", storedPIN);
    //     sscanf(line, "Amount: %f", &balance);
    // }
    // fclose(accFile);

    // if (strcmp(pin, storedPIN) != 0)
    // {
    //     printf("Wrong PIN. Deposit cancelled.\n");
    //     return;
    // }

    // // Deposit
    // float depositAmt;
    // printf("Enter deposit amount: ");
    // if (scanf("%f", &depositAmt) != 1 || depositAmt <= 0 || depositAmt > 50000)
    // {
    //     printf("Invalid amount. Enter RM1 to RM50000.\n");
    //     while(getchar()!='\n');
    //     return;
    // }

    // balance += depositAmt;

    // // Update file
    // accFile = fopen(filepath, "r");
    // FILE *temp = fopen("database/temp.txt", "w");

    // while (fgets(line, sizeof(line), accFile))
    // {
    //     if (strncmp(line, "Amount:", 7) == 0)
    //         fprintf(temp, "Amount: %.2f\n", balance);
    //     else
    //         fputs(line, temp);
    // }

    // fclose(accFile);
    // fclose(temp);

    // remove(filepath);
    // rename("database/temp.txt", filepath);

    // logTransaction("Deposit Successful");

    // printf("Deposit successful! \nNew balance: RM %.2f\n", balance);
}

void withdrawal()
{
    char accno[20], username[50];
    char listacc[100][20];
    int option = 0, count = 0;
    char selectedAcc[20];

    FILE *fp = fopen("database/index.txt", "r");
    if (!fp)
    {
        printf("No accounts found.\n");
        return;
    }

    printf("\n------- Withdrawal -------\n");
    while (fscanf(fp, "%s %s", accno, username) == 2)
    {   
        strcpy(listacc[count], accno);
        count++;
    }
    fclose(fp);

    if (count == 0)
    {
        printf("No accounts found.\n");
        return;
    }

    printf("Please choose an option (1-%d):\n\n", count);

    // Show account list again
    fp = fopen("database/index.txt", "r");
    int i = 1;
    while (fscanf(fp, "%s %s", accno, username) == 2)
    {
        printf("%d. %s (%s)\n", i, accno, username);
        i++;
    }
    fclose(fp);

    printf("\nSelect option: ");
    scanf("%d", &option);

    if (option < 1 || option > count)
    {
        printf("Invalid choice.\n");
        return;
    }

    // Selected account number
    strcpy(selectedAcc, listacc[option - 1]);

    char pin[5], storedPIN[10];
    printf("Enter 4-digit PIN: ");
    scanf("%4s", pin);

    // Open the selected account file
    char filepath[100];
    sprintf(filepath, "database/%s.txt", selectedAcc);

    FILE *accFile = fopen(filepath, "r");
    if (!accFile)
    {
        printf("Account not found.\n");
        return;
    }

    float balance = 0;
    char line[200], storedID[20]="";
    
    while (fgets(line, sizeof(line), accFile))
    {
        sscanf(line, "PIN: %9s", storedPIN);
        sscanf(line, "Amount: %f", &balance);
    }
    fclose(accFile);

    if (strcmp(pin, storedPIN) != 0)
    {
        printf("Wrong PIN. Withdrawal cancelled.\n");
        return;
    }

    printf("Current balance: RM %.2f\n", balance);

    float withdrawalAmt;
    printf("Enter withdrawal amount: ");
    if (scanf("%f", &withdrawalAmt) != 1 || withdrawalAmt <= 0 || withdrawalAmt > 50000)
    {
        printf("Invalid amount. Enter RM1 to RM50000.\n");
        while(getchar()!='\n');
        return;
    }

    if (withdrawalAmt > balance)
    {
        printf("Insufficient balance! You only have RM %.2f.\n", balance);
        return;
    }

    balance -= withdrawalAmt;

    // Update file
    accFile = fopen(filepath, "r");
    FILE *temp = fopen("database/temp.txt", "w");

    while (fgets(line, sizeof(line), accFile))
    {
        if (strncmp(line, "Amount:", 7) == 0)
            fprintf(temp, "Amount: %.2f\n", balance);
        else
            fputs(line, temp);
    }

    fclose(accFile);
    fclose(temp);

    remove(filepath);
    rename("database/temp.txt", filepath);

    logTransaction("Withdrawal Successful");

    printf("Withdrawal successful! \nNew balance: RM %.2f\n", balance);
}

void remittance()
{
    char accno[20], username[50];
    char listacc[100][20];
    int sender = 0, receiver = 0, count = 0;

    FILE *fp = fopen("database/index.txt", "r");
    if (!fp)
    {
        printf("No accounts found.\n");
        return;
    }

    printf("\n------- Remittance -------\n");
    while (fscanf(fp, "%s %s", accno, username) == 2)
    {   
        strcpy(listacc[count], accno);
        count++;
    }
    fclose(fp);

    if (count < 2)
    {
        printf("Need at least TWO accounts to perform remittance.\n");
        return;
    }

    printf("Available accounts (1-%d):\n\n", count);

    // Show account list again
    fp = fopen("database/index.txt", "r");
    int i = 1;
    while (fscanf(fp, "%s %s", accno, username) == 2)
    {
        printf("%d. %s (%s)\n", i, accno, username);
        i++;
    }
    fclose(fp);

    // sender
    printf("\nSelect sender account: ");
    scanf("%d", &sender);

    if (sender < 1 || sender > count)
    {
        printf("Invalid sender account.\n");
        return;
    }

    // receiver
    printf("Select receiver account: ");
    scanf("%d", &receiver);

    if (receiver < 1 || receiver > count)
    {
        printf("Invalid receiver account.\n");
        return;
    }

    if (sender == receiver)
    {
        printf("Sender and receiver cannot be the same.\n");
        return;
    }

    char senderAcc[20], receiverAcc[20];
    strcpy(senderAcc, listacc[sender - 1]);
    strcpy(receiverAcc, listacc[receiver - 1]);

    // Sender PIN
    char pin[5], storedPIN[10];
    printf("Enter sender 4-digit PIN: ");
    scanf("%4s", pin);

    char senderfp[100], receiverfp[100];
    sprintf(senderfp, "database/%s.txt", senderAcc);
    sprintf(receiverfp, "database/%s.txt", receiverAcc);

    float senderBalance = 0, receiverBalance = 0;
    char senderType[20] = "", receiverType[20] = "";
    char line[200];

    // read sender file
    FILE *fs = fopen(senderfp, "r");
    if (!fs)
    {
        printf("Sender account file missing.\n");
        return;
    }
    
    while (fgets(line, sizeof(line), fs))
    {
        sscanf(line, "PIN: %9s", storedPIN);
        sscanf(line, "Amount: %f", &senderBalance);
        sscanf(line, "Account Type: %19s", senderType);
    }
    fclose(fs);

    if (strcmp(pin, storedPIN) != 0)
    {
        printf("Wrong sender PIN. Remittance cancelled.\n");
        return;
    }

    // read receiver file
    FILE *fr = fopen(receiverfp, "r");
    if (!fr)
    {
        printf("Receiver account file missing.\n");
        return;
    }
    while (fgets(line, sizeof(line), fr))
    {
        sscanf(line, "Amount: %f", &receiverBalance);
        sscanf(line, "Account Type: %19s", receiverType);
    }
    fclose(fr);

    float amt;
    printf("Enter remittance amount: RM ");
    scanf("%f", &amt);

    if (amt <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }
    if (amt > senderBalance)
    {
        printf("Insufficient balance! You only have RM %.2f.\n", senderBalance);
        return;
    }

    float fee = 0;
    if (strcmp(senderType, "Savings") == 0 && strcmp(receiverType, "Current") == 0)
    {
        fee = amt * 0.02;
    }
    else if (strcmp(senderType, "Current") == 0 && strcmp(receiverType, "Savings") == 0)
    {
        fee = amt * 0.03;
    }

    float total = amt + fee;
    if (total > senderBalance)
    {
        printf("Insufficient balance to cover amount and fee of RM %.2f.\n", total);
        return;
    }

    senderBalance -= total;
    receiverBalance += amt;

    fs = fopen(senderfp, "r");
    FILE *tempS = fopen("database/tempS.txt", "w");
    while (fgets(line, sizeof(line), fs))
    {
        if (strncmp(line, "Amount:", 7) == 0)
            fprintf(tempS, "Amount: %.2f\n", senderBalance);
        else
            fputs(line, tempS);
    }
    fclose(fs);
    fclose(tempS);
    remove(senderfp);
    rename("database/tempS.txt", senderfp);

    fr = fopen(receiverfp, "r");
    FILE *tempR = fopen("database/tempR.txt", "w");
    while (fgets(line, sizeof(line), fr))
    {
        if (strncmp(line, "Amount:", 7) == 0)
            fprintf(tempR, "Amount: %.2f\n", receiverBalance);
        else
            fputs(line, tempR);
    }

    fclose(fr);
    fclose(tempR);
    remove(receiverfp);
    rename("database/tempR.txt", receiverfp);

    logTransaction("Remittance Successful");

    printf("\nRemittance successful!\n");
    printf("Amount transferred: RM %.2f\n", amt);
    printf("Fee charged: RM %.2f\n", fee);
    printf("Sender new balance: RM %.2f\n", senderBalance);
    printf("Receiver new balance: RM %.2f\n", receiverBalance);
}

void input()
{
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
        scanf("%19s", &input);

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
        else if (strcmp(input, "4") == 0 || strcasecmp(input,"withdrawal") == 0)
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
    system("mkdir database >nul 2>&1");
    input();
    return 0 ;
}

// instruction manual (it must consist how to use the code, github link)
//要做instruction manual 还有 documentation （inst manual 是要教user怎样step by step做 account，deposit， withdrawal那些）（documentation 是解释你的code在干嘛 example可以ss一个 function 讲这个function 在干什么 那个东西在zmk）