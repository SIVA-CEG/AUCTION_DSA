#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Item (Doubly Linked List)
typedef struct Item {
    int itemId;
    char itemName[50];
    float basePrice;
    float currentBid;
    struct Item *prev, *next;
    char soldTo[50];
    float soldPrice;
} Item;

// Structure for Participant Registration
typedef struct Participant {
    char participantId[10];  // Participant ID (combination of letter and number)
    char name[50];
    char contact[50];
    float totalSpent;
    struct Participant *next;
} Participant;

// Function prototypes
void enterAsAuctioneer();
void enterAsParticipant();
void addItem(char *name, float basePrice);
void printItems();
void printSoldItems();
void registerParticipant(char *name, char *contact);
void viewParticipants();
void startAuction();
void generateParticipantId(char *id);
void increaseBid();

// Global variables
Item *head = NULL;          // Head of the doubly linked list (Items)
Item *soldHead = NULL;      // Head of the sold items list
Participant *participantList = NULL; // Head of participant linked list
Item *currentItem = NULL;   // Pointer to the current item in the auction

// Function to automatically generate unique participant ID
void generateParticipantId(char *id) {
    static int participantCount = 1;
    snprintf(id, 10, "P%03d", participantCount++);
}

// Function to add an item to the doubly linked list (Auctioneer's task)
void addItem(char *name, float basePrice) {
    static int itemIdCounter = 1;
    Item *newItem = (Item *)malloc(sizeof(Item));
    newItem->itemId = itemIdCounter++;
    strcpy(newItem->itemName, name);
    newItem->basePrice = basePrice;
    newItem->currentBid = basePrice; // Set the current bid to the base price initially
    newItem->soldTo[0] = '\0';       // Initially, the item is not sold
    newItem->soldPrice = 0;
    newItem->prev = NULL;
    newItem->next = head;

    if (head != NULL) {
        head->prev = newItem;
    }
    head = newItem;
    currentItem = newItem;  // Set the current item to the newly added item

    printf("Item added: %s (ID: %d) with base price %.2f\n", name, newItem->itemId, basePrice);
}

// Function to print all available items
void printItems() {
    Item *temp = head;
    if (temp == NULL) {
        printf("No items available.\n");
        return;
    }

    printf("Items available for auction:\n");
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Current Bid: %.2f\n", temp->itemId, temp->itemName, temp->currentBid);
        temp = temp->next;
    }
}

// Function to print all sold items
void printSoldItems() {
    Item *temp = soldHead;
    if (temp == NULL) {
        printf("No items have been sold yet.\n");
        return;
    }

    printf("Sold Items:\n");
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Sold Price: %.2f, Buyer: %s\n", temp->itemId, temp->itemName, temp->soldPrice, temp->soldTo);
        temp = temp->next;
    }
}

// Function to register a participant (Participants can register themselves)
void registerParticipant(char *name, char *contact) {
    Participant *newParticipant = (Participant *)malloc(sizeof(Participant));

    // Generate a unique ID for the participant
    generateParticipantId(newParticipant->participantId);
    
    strcpy(newParticipant->name, name);
    strcpy(newParticipant->contact, contact);
    newParticipant->totalSpent = 0;  // Initially no money spent
    newParticipant->next = participantList;
    participantList = newParticipant;

    printf("Participant registered: %s (ID: %s)\n", name, newParticipant->participantId);
}

// Function to view all participants
void viewParticipants() {
    Participant *temp = participantList;
    if (temp == NULL) {
        printf("No participants registered.\n");
        return;
    }

    printf("Registered Participants:\n");
    while (temp != NULL) {
        printf("ID: %s, Name: %s, Contact: %s\n", temp->participantId, temp->name, temp->contact);
        temp = temp->next;
    }
}

// Function to automatically increase the bid based on the current price
void increaseBid() {
    if (currentItem->currentBid < 10000) {
        currentItem->currentBid += 1000;
    } else if (currentItem->currentBid < 50000) {
        currentItem->currentBid += 2500;
    } else if (currentItem->currentBid < 100000) {
        currentItem->currentBid += 5000;
    } else if (currentItem->currentBid < 200000) {
        currentItem->currentBid += 10000;
    } else if (currentItem->currentBid < 500000) {
        currentItem->currentBid += 25000;
    } else if (currentItem->currentBid < 1000000) {
        currentItem->currentBid += 50000;
    } else {
        currentItem->currentBid += 100000;
    }
}

// Function to start the auction (Start bidding on items)
void startAuction() {
    if (head == NULL) {
        printf("No items in the auction to start.\n");
        return;
    }

    Participant *participant = participantList;
    if (participant == NULL) {
        printf("No participants registered for the auction.\n");
        return;
    }

    int allSkipped = 0;

    // Auction loop - Auction each item
    while (currentItem != NULL) {
        // Display available items and sold items before starting the auction
        printf("\n--- Auctioning Item ---\n");
        printItems();
        printSoldItems();

        printf("\n--- Auctioning Item: %s (Current Bid: %.2f) ---\n", currentItem->itemName, currentItem->currentBid);

        allSkipped = 1;  // Assume all participants skip the bid initially

        // Loop through participants and allow them to place bids
        Participant *tempParticipant = participantList;
        while (tempParticipant != NULL) {
            int bidChoice;
            printf("Participant %s (ID: %s), do you want to bid on item %s (Current Bid: %.2f)? (Enter 1 to bid, 0 to skip): ", 
                   tempParticipant->name, tempParticipant->participantId, currentItem->itemName, currentItem->currentBid);
            scanf("%d", &bidChoice);

            if (bidChoice == 1) {
                // Automatically increase bid based on the current bid
                increaseBid();
                tempParticipant->totalSpent = currentItem->currentBid;  // Update total spent
                printf("Bid placed! New price: %.2f\n", currentItem->currentBid);
                allSkipped = 0;  // At least one participant has bid
            } else if (bidChoice == 0) {
                printf("%s chose not to bid.\n", tempParticipant->name);
            } else {
                printf("Invalid choice. Please enter 1 to bid or 0 to skip.\n");
            }

            tempParticipant = tempParticipant->next;  // Move to next participant
        }

        // If all participants skipped, stop the auction for this item
        if (allSkipped == 1) {
            printf("No bids placed on item %s. The item will not be sold.\n", currentItem->itemName);
        } else {
            // Move the current item to the sold list after bidding
            strcpy(currentItem->soldTo, "Alice");  // Simulate that Alice bought the item
            currentItem->soldPrice = currentItem->currentBid;

            // Add the item to the sold items list
            Item *soldItem = currentItem;
            currentItem = currentItem->next;
            soldItem->next = soldHead;
            soldItem->prev = NULL;
            if (soldHead != NULL) {
                soldHead->prev = soldItem;
            }
            soldHead = soldItem;

            // Remove the item from the available items list
            if (soldItem->prev != NULL) {
                soldItem->prev->next = soldItem->next;
            } else {
                head = soldItem->next;
            }
            if (soldItem->next != NULL) {
                soldItem->next->prev = soldItem->prev;
            }
        }
    }

    printf("\nAuction finished!\n");
    // Print total money spent by each participant
    printf("Total Money Spent by Participants:\n");
    Participant *temp = participantList;
    while (temp != NULL) {
        printf("%s (ID: %s) spent: %.2f\n", temp->name, temp->participantId, temp->totalSpent);
        temp = temp->next;
    }
}

// Function for auctioneer menu
void enterAsAuctioneer() {
    int choice;
    while (1) {
        printf("\n--- Auctioneer Menu ---\n");
        printf("1. Add Item\n");
        printf("2. View Items\n");
        printf("3. View Sold Items\n");
        printf("4. Start Auction\n");
        printf("5. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char name[50];
                    float basePrice;
                    printf("Enter Item Name: ");
                    scanf("%s", name);
                    printf("Enter Base Price: ");
                    scanf("%f", &basePrice);
                    addItem(name, basePrice);
                }
                break;
            case 2:
                printItems();
                break;
            case 3:
                printSoldItems();
                break;
            case 4:
                startAuction();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function for participant menu
void enterAsParticipant() {
    char name[50], contact[50];
    printf("Enter Participant Name: ");
    scanf("%s", name);
    printf("Enter Participant Contact: ");
    scanf("%s", contact);
    registerParticipant(name, contact);
}

// Main function to drive the program
int main() {
    int choice;
    while (1) {
        printf("\n--- Auction Management System ---\n");
        printf("1. Enter as Auctioneer\n");
        printf("2. Enter as Participant\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                enterAsAuctioneer();
                break;
            case 2:
                enterAsParticipant();
                break;
            case 3:
                printf("Exiting Auction System.\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
