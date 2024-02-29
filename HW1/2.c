#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char name[101];
    struct Node* next;
};

struct Node* addEngineer(struct Node* head, char name[], int position) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->name, name);
    newNode->next = NULL;
    if (position == 0) {
        newNode->next = head;
        return newNode;
    } else {
        struct Node* current = head;
        for (int i = 0; i < position - 1 && current != NULL; i++) {
            current = current->next;
        }
        if (current != NULL) {
            newNode->next = current->next;
            current->next = newNode;
        }
    }
    return head;
}

struct Node* deleteEngineer(struct Node* head, char name[]) {
    if (head == NULL) {
        return NULL;
    }
    if (strcmp(head->name, name) == 0) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    } else {
        struct Node* current = head;
        while (current->next != NULL && strcmp(current->next->name, name) != 0) {
            current = current->next;
        }
        if (current->next != NULL) {
            struct Node* temp = current->next;
            current->next = temp->next;
            free(temp);
        }
        return head;
    }
}

void swapEngineers(struct Node* head, char name1[], char name2[]) {
    if (head == NULL) {
        return;
    }
    struct Node* node1 = head;
    struct Node* prev1 = NULL;
    while (node1 != NULL && strcmp(node1->name, name1) != 0) {
        prev1 = node1;
        node1 = node1->next;
    }

    struct Node* node2 = head;
    struct Node* prev2 = NULL;
    while (node2 != NULL && strcmp(node2->name, name2) != 0) {
        prev2 = node2;
        node2 = node2->next;
    }

    if (node1 == NULL || node2 == NULL) {
        return;
    }

    if (prev1 != NULL) {
        prev1->next = node2;
    } else {
        head = node2;
    }

    if (prev2 != NULL) {
        prev2->next = node1;
    } else {
        head = node1;
    }

    struct Node* temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
}

void printHierarchy(struct Node* head) {
    while (head != NULL) {
        printf("%s ", head->name);
        head = head->next;
    }
}

int main() {
    int numMembers;
    scanf("%d", &numMembers);

    struct Node* head = NULL;

    for (int i = 0; i < numMembers; i++) {
        char name[101];
        scanf("%s", name);
        head = addEngineer(head, name, i);
    }

    int numChanges;
    scanf("%d", &numChanges);

    for (int i = 0; i < numChanges; i++) {
        char action[6];
        scanf("%s", action);

        if (strcmp(action, "add") == 0) {
            char name[101];
            int position;
            scanf("%s %d", name, &position);
            head = addEngineer(head, name, position);
        } else if (strcmp(action, "delete") == 0) {
            char name[101];
            scanf("%s", name);
            head = deleteEngineer(head, name);
        } else if (strcmp(action, "swap") == 0) {
            char name1[101];
            char name2[101];
            scanf("%s %s", name1, name2);
            swapEngineers(head, name1, name2);
        }
    }

    printHierarchy(head);

    return 0;
}
