
#include <stdlib.h>

typedef struct llnode {
    struct llnode *next;
    int data;
} LLNode;

int main() {
    // initialize both next and data
    LLNode *head = malloc(sizeof(LLNode));

    // adding nodes to the end of the LL
    for (int i = 0; i < 10000; i++) {
        LLNode *new = malloc(sizeof(LLNode));
        new->data = i;  // also initialize next

        head->next = new;  // change head to tail
        head = new;        // change head to tail
    }

    // there are memory leaks! not everything is freed...
    while (head != NULL) {
        LLNode *temp = head->next;
        free(head);
        head = temp;
    }

    return 0;
}