// DONE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 9

typedef struct node_t *Node;
struct node_t {
    int data;
    Node next;
};

static bool printRecursivly(Node node, bool printInReverse) {
    if (!(node->next) || !(node->next->next)) {
        printf("%d ", node->data);
        return true;
    }
    if (printInReverse) {
        printRecursivly(node->next->next, true),
        printf("%d ", node->data);
    }
    else {
        printf("%d ", node->data);
        printRecursivly(node->next->next, false);
    }
}

bool printCheckmarkOrdered(Node head) {
    if (!head) {
        return true;
    }
    if (!(head->next)) {
        return printRecursivly(head,false);
    }
    printRecursivly(head, false);
    printRecursivly(head->next, true);
    return true;
}

int main() {
    Node *linkedList = malloc(sizeof(struct node_t*)*(SIZE+1));
    for (int i = 0; i < SIZE; i++) {
        linkedList[i] = malloc(sizeof(struct node_t));
    }
    linkedList[SIZE] = NULL;
    for (int i = 0; i < SIZE; i++) {
        linkedList[i]->data = i+1;
        linkedList[i]->next = linkedList[i+1];
    }
    printCheckmarkOrdered(linkedList[0]);
    return 0;
}