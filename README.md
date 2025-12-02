1.0 Program Overview

The Banking System Application is a console-based program that enables users to perform basic banking operations using only native C programming features. The system supports creating new bank accounts, deleting existing accounts, depositing money, withdrawing money, and performing remittances. Each time the program runs, it also displays a session summary that shows the current date, time, and total number of registered accounts. All account data is stored in a dedicated database folder. Inside this folder, the file index.txt maintains a list of all accounts, including their usernames. In contrast, individual account details are stored in separate text files, each named after its account number (for example, 12345678.txt). The program also maintains a transaction.log file, which records every action performed within the system, including account creation, deposits, withdrawals, remittances, account deletions, and system exit events. This structure enables the program to operate as a comprehensive yet straightforward banking system, utilizing only text-file storage.


2.0 Starting the Program

Step 1: Open your terminal in VS code and compile the source code using: “gcc banking.c -o banking”.

Step 2: Execute the compiled program with: “./banking”.

Step 3: Once the program starts, it will display the current date and time, the total number of existing   accounts, and the Main Menu options.	


3.0 Main Menu Overview

The terminal presents the following options:
1.	Create New Bank Account
2.	Delete Bank Account
3.	Deposit
4.	Withdrawal
5.	Remittance
6.	Quit

Users may enter the option number (1–6) or type the command name (e.g., create, delete, deposit, withdraw, remittance, and quit).

4.0 Creating a New Bank Account

Step 1: Select option 1 or type command “create” to create new bank account.

Step 2: Enter your username (letters and spaces only).

Step 3: Enter your ID number (digits only).

Step 4: Choose the account type: 

•	Enter 1 for Savings 

•	Enter 2 for Current

Step 5: Create a 4-digit PIN.

Step 6: The system will check for valid username, ID, and duplicate account type.

Step 7: A unique 8-digit account number will be generated automatically.

Step 8: The account file is created and added to the system database.

Step 9: A summary of your new account details is displayed.

A log entry is added to transaction.log.


5.0 Deleting a Bank Account

Step 1: Select option 2 or type command “delete” to delete a bank account.

Step 2: Choose the account from the list of available accounts.

Step 3: Enter the last four digits of the account holder’s ID.

Step 4: Enter the account PIN for authentication.

Step 5: If authentication succeeds:

•	The account file is deleted

•	The index file is rewritten to remove the deleted entry

•	A confirmation message appears.

•	A log entry is added to transaction.log.

Step 6: If authentication fails:

•	The account file will not be deleted

•	The index file will remain the same

•	The program will let user to select again the option to do


6.0 Depositing Money (Deposit)

Step 1: Select option 3 or type command “deposit” to depositing money into a bank account.

Step 2: Choose an account from the list.

Step 3: Enter the account PIN for authentication.

Step 4: If authentication succeeds:

•	Enter the deposit amount (RM 1 to RM 50,000).

•	The system updates the account balance.

•	A new balance is displayed.

•	A log entry is added to transaction.log.

Step 5: If authentication fails:

•	A message “Incorrect PIN” appears.

•	The program will let user to select again the option to do.


7.0 Withdrawing Money (Withdraw)

Step 1: Select option 4 or type command “withdraw” to withdraw money from a bank account.

Step 2: Choose an account to withdraw from.

Step 3: Enter the correct PIN for authentication.

Step 4: If authentication succeeds:

•	Enter the withdrawal amount (RM 1 to RM 50,000).

•	The system checks if the balance is sufficient.

•	The withdrawal is processed, and the balance is updated.

•	A new balance is displayed.

•	A log entry is added to transaction.log.

Step 5: If authentication fails:

•	A message “Incorrect PIN” appears.

•	The program will let user to select again the option to do.


8.0 Transferring Money (Remittance)

Step 1: Select option 5 or type command “remittance” to transfer money from a bank account.

Step 2: The system will list all available accounts.

Step 3: Choose the sender account.

Step 4: Choose the receiver account

Note: Sender and receiver must be different accounts

Step 5: Authenticate using the sender’s PIN.

Step 6: If authentication succeeds:

•	Enter the remittance amount (RM 1 to RM 50,000).

•	The system calculates any applicable fees from savings to current will incur a 2% of remittance fee. From current to savings will incur a 3% remittance fee. For same type there will be no remittance fee.

•	The sender’s balance is deducted (amount + fee).

•	The receiver’s balance is increased (amount only).

•	The system displays a summary of the transfer.

•	A log entry is added to transaction.log.

Step 7: If authentication fails:

•	A message “Incorrect PIN” appears.

•	The program will let user to select again the option to do.


9.0 Session Information Display

Every time the menu is shown, the system updates the user with:

•	Current date and time

•	Total number of registered accounts

•	Main menu options

This information is refreshed automatically and helps users monitor the state of the system.


10.0 Exiting the Program

Step 1: Select option 6 or type command “quit” to quit from the program.

Step 2: The system saves an “Exit” entry in the transaction log.

Step 3: The program closes safely.


11.0 Files Used by the System

All program data is stored inside the database folder:

index.txt	will stores all account numbers and usernames

<accno>.txt	stores individual account files containing username, ID, account type, PIN, and balance

transaction.log	records all activities (create, deposit, withdrawal, remittance, delete, exit)

This structure allows the system to function entirely without external databases or libraries.


12.0 Additional Notes

1.	Usernames must only contain letters and spaces.

2.	IDs must be numeric.

3.	PINs must be exactly 4 digits.

4.	The system prevents duplicate account types for the same user.

5.	The remittance function includes automatic fee calculation.

6.	The system uses text files for portability and simplicity.

