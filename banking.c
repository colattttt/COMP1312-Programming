#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void createAccount();
void deleteAccount();
void logTransaction(const char *action);
void showSessionInfo();

void logTransaction(const char *action)
{
    FILE *fp = fopen("database/transaction.log", "a");
    if (!fp) return; // handle error

    time_t t = time(NULL);
    fprintf(fp, "%s - %s", action, ctime(&t));

    fclose(fp);
}

void showSessionInfo()
{
    time_t t = time(NULL);
    printf("\n===== BANK SYSTEM SESSION =====\n");
    printf("Date & Time: %s", ctime(&t));

    int count = 0;
    FILE *fp = fopen("database/index.txt", "r"); // must maintain index.txt
    if (fp)
    {
        char line[50];
        while (fgets(line, sizeof(line), fp))
        {
            if (line[0] != '\n') // ignore empty lines
                count++;
        }
        fclose(fp);
    }

    printf("Loaded Accounts: %d\n", count);
    printf("================================\n");
}

int isLettersOnly(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z')))
        {
            return 0; //Found non-letter
        }
        i++;
    }
    return 1; //All letters
}

int accountExists(char *id, int type)
{
    FILE *fp = fopen("database/index.txt", "r");
    if (!fp) return 0; // no accounts

    char file_id[20];
    int file_type;
    while (fscanf(fp, "%19s %d", file_id, &file_type) == 2)
    {
        if (strcmp(file_id, id) == 0 && file_type == type)
        {
            fclose(fp);
            return 1; // account exists
        }
    }

    fclose(fp);
    return 0; // not found
}

void input()
{
    char input[20];
    showSessionInfo();

    while (1) // infinite loop until user chooses exit
    {
        printf("\n--- Main Menu & Session ---\n");
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
            printf("You selected Deposit.\n");
        }
        else if (strcmp(input, "4") == 0 || strcasecmp(input,"withdrawal") == 0)
        {
            logTransaction("Withdrawal");
            printf("You selected Withdrawal.\n");
        }
        else if (strcmp(input, "5") == 0 || strcasecmp(input,"remittance") == 0)
        {
            logTransaction("Remittance");
            printf("You selected Remittance.\n");
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

void createAccount()
{
    char username[50], filepath[100], id[20], pin[5];
    int type, accno;
    FILE *fp;

    printf("\n--- Create Bank Account  ---\n");
    printf("\nEnter your Username: ");
    // Allow up to 49 characters for a string. To avoid crash or memory error if the user types too many characters.
    scanf("%49s", username);

    if (!isLettersOnly(username))
    {
        printf("Username must contain letters only!\n");
        return;
    }

    printf("\nEnter your ID: ");
    scanf("%19s", id);

    // Validate digits only
    for (int i = 0; id[i] != '\0'; i++)
    {
        if (id[i] < '0' || id[i] > '9')
        {
            printf("Invalid ID! Must contain numbers only.\n");
            while(getchar()!='\n'); // clear buffer
            return;
        }
    }

    printf("\nEnter Account Type:");
    printf("\n1. Savings Account");
    printf("\n2. Current Account");
    printf("\nSelect Option: ");
    if (scanf("%d", &type) != 1 || (type != 1 && type != 2))
    {
        printf("Invalid account type! Select 1 or 2.\n");
        while(getchar()!='\n');
        return; // restart
    }
    
    // Check if account already exists
    if (accountExists(id, type))
    {
        printf("You already have this type of account!\n");
        return;
    }
        
    printf("\nEnter your 4-digit PIN: ");
    scanf("%4s", pin);

    if (strlen(pin) != 4)
    {
        printf("PIN must be 4 digits!\n");
        return;
    }

    // Validate 4 digits
    for (int i = 0; i < 4; i++)
    {
        if (pin[i] < '0' || pin[i] > '9')
        {
            printf("Invalid PIN! PIN must contain digits only.\n");
            while(getchar()!='\n');
            return;
        }
    }

    // Generate random unique account number
    srand((unsigned int)time(NULL));
    do
    {
        accno = (rand() % 900000000) + 10000000; 
        sprintf(filepath, "database/%d.txt", accno);
        fp = fopen(filepath, "r");
        if (fp != NULL) fclose(fp);
    } while (fp != NULL);

    // Create new user file
    fp = fopen(filepath, "w");
    if (!fp) 
    {
        printf("Error: Could not create account file.\n");
        return;
    }

    fprintf(fp, "Username: %s\n", username);
    fprintf(fp, "ID: %s\n", id); 
    fprintf(fp, "Account No: %d\n", accno);
    fprintf(fp, "Account Type: %d\n", type);
    fprintf(fp, "PIN: %s\n", pin);
    fprintf(fp, "Amount: 0.00\n");

    fclose(fp);

    logTransaction("Create Account");

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
    FILE *fp_index = fopen("database/index.txt", "r");
    if (!fp_index)
    {
        printf("No accounts found.\n");
        return;
    }

    char accno[20], username[50];
    while (fscanf(fp_index, "%19s %49s", accno, username) == 2)
    {
        strcpy(accnos[total], accno);
        strcpy(usernames[total], username);
        total++;
        if (total >= 100) break; // safety limit
    }
    fclose(fp_index);

    if (total == 0)
    {
        printf("No accounts found.\n");
        return;
    }

    // Show numbered list
    printf("Current accounts:\n");
    for (int i = 0; i < total; i++)
        printf("%d. %s %s\n", i + 1, accnos[i], usernames[i]);

    // Ask user to choose
    int choice;
    printf("Select Option: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > total)
    {
        printf("Invalid choice.\n");
        while(getchar()!='\n'); // clear buffer
        return;
    }

    char selected_acc[20];
    strcpy(selected_acc, accnos[choice - 1]);

    // Ask for last 4 of ID and PIN
    char last4ID[5], pin[5];
    printf("Enter last 4 digits of ID: ");
    scanf("%4s", last4ID);
    printf("Enter 4-digit PIN: ");
    scanf("%4s", pin);

    // Open account file to verify
    char filepath[100];
    sprintf(filepath, "database/%s.txt", selected_acc);

    FILE *fp_acc = fopen(filepath, "r");
    if (!fp_acc)
    {
        printf("Account file not found!\n");
        return;
    }

    char storedID[20] = "", storedPIN[5] = "", line[100];
    while (fgets(line, sizeof(line), fp_acc))
    {
        if (sscanf(line, "ID: %19s", storedID) == 1) continue;
        if (sscanf(line, "PIN: %4s", storedPIN) == 1) continue;
    }
    fclose(fp_acc);

    int len = strlen(storedID);
    if (len < 4 || strcmp(last4ID, storedID + len - 4) != 0 || strcmp(pin, storedPIN) != 0)
    {
        printf("Authentication failed! Account not deleted.\n");
        return;
    }

    // Delete account file
    if (remove(filepath) == 0)
        printf("Account file deleted successfully.\n");
    else
    {
        printf("Error deleting account file.\n");
        return;
    }

    // Update index.txt by skipping the deleted account
    FILE *fp_old = fopen("database/index.txt", "r");
    FILE *fp_new = fopen("database/temp.txt", "w");
    char temp_acc[20], temp_username[50];

    while (fscanf(fp_old, "%19s %49s", temp_acc, temp_username) == 2)
    {
        if (strcmp(temp_acc, selected_acc) != 0)
            fprintf(fp_new, "%s %s\n", temp_acc, temp_username);
    }

    fclose(fp_old);
    fclose(fp_new);

    remove("database/index.txt");
    rename("database/temp.txt", "database/index.txt");

    logTransaction("Delete Account");
}

int main()
{
    input();
    return 0 ;
}