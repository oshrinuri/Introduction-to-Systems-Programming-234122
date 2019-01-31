// DONE & WORKING

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 8

typedef int (*opFunction)(int,int);

typedef struct node_t *Node;
struct node_t {
    int n;
    Node next;
};

int list_max(int a, int b) { return a > b ? a : b; }
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return a / b; }

static int listLen(Node head) {
    int len = 0;
    Node curr = head;
    while (curr != NULL) {
        len++;
        curr = curr->next;
    }
    return len;
}

static Node arrayToList(int *array, int len) {
    Node head = NULL;
    for (int i = len - 1; i >= 0; i--) {
        Node newNode = malloc(sizeof(*newNode));
        newNode->next = head;
        newNode->n = array[i];
        head = newNode;
    }
    return head;
}

static void printList(Node head) {
    Node curr = head;
    while (curr != NULL) {
        curr->next?printf("%d -> ", curr->n):printf("%d ",curr->n);
        curr = curr->next;
    }
}

static Node createList() {
    Node *list = malloc(sizeof(struct node_t *) * (SIZE + 1));
    for (int i = 0; i < SIZE; ++i) {
        list[i] = malloc(sizeof(struct node_t));
    }
    list[SIZE] = NULL;
    for (int i = 0; i < SIZE; ++i) {
        list[i]->n = rand() % 101;
        list[i]->next = list[i + 1];
    }
    return list[0];
}

Node listOperator(Node list1, Node list2, opFunction op_func) {
    if (!list1 || !list2 || listLen(list1) != listLen(list2)) {
        return NULL;
    }
    int arrLength = listLen(list1);
    int *resultArr = malloc(sizeof(int) * arrLength);
    for (int i = 0; i < arrLength; ++i) {
        resultArr[i] = op_func(list1->n, list2->n);
        list1 = list1->next;
        list2 = list2->next;
    }
    return arrayToList(resultArr, arrLength);
}

Node maxElements(Node* lists, int n) {
    if (n==0) return NULL;
    if (n==1) { return listOperator(lists[0],lists[0],list_max); }
    Node curr_max_list = listOperator(lists[0], lists[1], list_max);
    for (int j = 2; j < n; ++j) {
        curr_max_list = listOperator(curr_max_list, lists[j], list_max);
    }
    return curr_max_list;
}

int main() {
    time_t t;
    srand((unsigned) time(&t));
    Node head0 = createList();
    Node head1 = createList();
    Node head2 = createList();
    Node head3 = createList();
    Node head4 = createList();
    Node head5 = createList();

    Node* arrayOfLists = malloc(sizeof(*arrayOfLists)*6);
    arrayOfLists[0] = head0;
    arrayOfLists[1] = head1;
    arrayOfLists[2] = head2;
    arrayOfLists[3] = head3;
    arrayOfLists[4] = head4;
    arrayOfLists[5] = head5;

    printf("All lists:\n");
    printList(head0);
    printf("\n");
    printList(head1);
    printf("\n");
    printList(head2);
    printf("\n");
    printList(head3);
    printf("\n");
    printList(head4);
    printf("\n");
    printList(head5);

    // Node newHead = listOperator(head1, head2, mul);
    Node maxHead = maxElements(arrayOfLists,6);
    printf("\nMax list:\n");
    printList(maxHead);

    Node* arrayOfListsOne = malloc(sizeof(*arrayOfLists)*1);
    arrayOfListsOne[0] = head0;
    Node maxHeadOne = maxElements(arrayOfListsOne,1);
    printf("\nMax list with one Node:\n");
    printList(maxHeadOne);
}