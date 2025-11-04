#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void createAccount();

int isLettersOnly(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z')))
        {
            return 0; // ❌ Found non-letter
        }
        i++;
    }
    return 1; // ✅ All letters
}


void input()
{
    int count;

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

        scanf("%d", &count);

        if (count == 1)
        {
            createAccount();
        }
        else if (count == 2)
        {
            printf("You selected Delete Bank Account.\n");
        }
        else if (count == 3)
        {
            printf("You selected Deposit.\n");
        }
        else if (count == 4)
        {
            printf("You selected Withdrawal.\n");
        }
        else if (count == 5)
        {
            printf("You selected Remittance.\n");
        }
        else if (count == 6)
        {
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

    // Create folder only once if not exist
    system("mkdir database >nul 2>&1");

    printf("\n--- Create Bank Account Function ---\n");
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
        
    printf("\nEnter your 4-digit PIN: ");
    scanf("%4s", pin);

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

    srand(time(NULL));
    do
    {
        accno = (rand() % 900000000) + 10000000; 
        sprintf(filepath, "database/%d.txt", accno);
        fp = fopen(filepath, "r");
        if(fp != NULL) fclose(fp);
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
    fprintf(fp, "Amount: 0.00\n");

    fclose(fp);

    printf("\nAccount created successfully!\n");
    printf("----------------------------------------\n");
    printf("Username: %s\n",username);
    printf("ID: %s\n",id);
    printf("Account No: %d\n",accno);
    printf("Account Type:%s\n", (type == 1) ? "Savings" : "Current");
    printf("Amount: 0.00\n");
    printf("----------------------------------------");
}

int main()
{
    input();
    return 0 ;
}