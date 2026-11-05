#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "payment.h"

int acceptPayment(double amount, char paymentMethod[], int size)
{
    int choice, bankChoice, i;
    char upiID[100];
    char cardName[60];
    char cardNumber[30];
    char expiryDate[10];
    char cvv[10];

    const char *banks[] = {
        "State Bank of India (SBI)",
        "HDFC Bank",
        "ICICI Bank",
        "Axis Bank",
        "Punjab National Bank (PNB)",
        "Bank of Baroda",
        "Canara Bank",
        "Kotak Mahindra Bank"
    };

    int bankCount = 8;
    int valid;

    if (amount <= 0)
    {
        printf("\nInvalid payment amount!\n");
        return 0;
    }

    printf("\n====================================");
    printf("\n          PAYMENT GATEWAY");
    printf("\n====================================");
    printf("\nTicket Amount: Rs. %.2f", amount);

    printf("\n\nSelect Payment Method:");
    printf("\n1. UPI");
    printf("\n2. Debit Card");
    printf("\n3. Credit Card");
    printf("\n4. Net Banking");
    printf("\n5. Cash at Counter");
    printf("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("\nInvalid input.\n");
        return 0;
    }

    switch (choice)
    {
        case 1:
        {
            printf("\nEnter UPI ID (example: name@bank): ");
            scanf("%99s", upiID);

            if (strchr(upiID, '@') == NULL ||
                strchr(upiID, '@') == upiID ||
                strchr(upiID, '@')[1] == '\0')
            {
                printf("\nInvalid UPI ID format.\n");
                return 0;
            }

            snprintf(paymentMethod, size, "%s", "UPI");

            printf("\nUPI ID: %s", upiID);
            break;
        }

        case 2:
        case 3:
        {
            printf("\nEnter Cardholder Name: ");
            scanf(" %59[^\n]", cardName);

            printf("Enter Card Number (16 digits): ");
            scanf("%29s", cardNumber);

            valid = (strlen(cardNumber) == 16);

            for (i = 0; cardNumber[i] != '\0'; i++)
            {
                if (!isdigit((unsigned char)cardNumber[i]))
                {
                    valid = 0;
                    break;
                }
            }

            if (!valid)
            {
                printf("\nInvalid card number. Use 16 digits.\n");
                return 0;
            }

            printf("Enter Expiry Date (MM/YY): ");
            scanf("%9s", expiryDate);

            if (strlen(expiryDate) != 5 ||
                expiryDate[2] != '/' ||
                !isdigit((unsigned char)expiryDate[0]) ||
                !isdigit((unsigned char)expiryDate[1]) ||
                !isdigit((unsigned char)expiryDate[3]) ||
                !isdigit((unsigned char)expiryDate[4]))
            {
                printf("\nInvalid expiry date format.\n");
                return 0;
            }

            printf("Enter CVV (3 digits): ");
            scanf("%9s", cvv);

            if (strlen(cvv) != 3 ||
                !isdigit((unsigned char)cvv[0]) ||
                !isdigit((unsigned char)cvv[1]) ||
                !isdigit((unsigned char)cvv[2]))
            {
                printf("\nInvalid CVV format.\n");
                return 0;
            }

            if (choice == 2)
                snprintf(paymentMethod, size, "%s", "Debit Card");
            else
                snprintf(paymentMethod, size, "%s", "Credit Card");

            printf("\nCard details format accepted.");
            break;
        }

        case 4:
        {
            printf("\n========== SELECT YOUR BANK ==========\n");

            for (i = 0; i < bankCount; i++)
            {
                printf("%d. %s\n", i + 1, banks[i]);
            }

            printf("Select bank (1-%d): ", bankCount);

            if (scanf("%d", &bankChoice) != 1 ||
                bankChoice < 1 || bankChoice > bankCount)
            {
                printf("\nInvalid bank selection.\n");
                return 0;
            }

            snprintf(paymentMethod, size, "Net Banking - %s",
                     banks[bankChoice - 1]);

            printf("\nSelected Bank: %s", banks[bankChoice - 1]);
            break;
        }

        case 5:
            snprintf(paymentMethod, size, "%s", "Cash at Counter");

            printf("\nPlease pay Rs. %.2f at the reservation counter.",
                   amount);

            printf("\nCash payment is pending until received by the counter.\n");
            return 1;

        default:
            printf("\nInvalid payment method selected!\n");
            return 0;
    }

    printf("\n\n====================================");
    printf("\nPayment request created successfully.");
    printf("\nMethod: %s", paymentMethod);
    printf("\nAmount: Rs. %.2f", amount);
    printf("\n====================================\n");
    return 1;
}

