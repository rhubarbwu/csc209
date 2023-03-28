# Linked-Lists

![Source: [`www.educative.io`](https://www.educative.io/)](lec06/ll.png){width=75%}

A very common data structure that maintains ordering with easy insertions/deletions is the linked-list (LL).

## Linked-List Nodes (LLNode)

Here's a sample `struct` implementation of a linked-list node.

```c
typedef struct llnode {
	struct llnode * next;
	int data;
} LLNode;
```

Each holds a pointer `next` to another `LLNode`, so they can refer to each other.

```c
LLNode b = {NULL, 1};  // b doesn't point to any other node.
LLNode a = {&b, 0};    // a.next is a pointer to b
```

`LLNode` structs are often dynamically allocated and referenced by pointers.

```c
LLNode *c = malloc(sizeof(LLNode)),
	   *d = malloc(sizeof(LLNode)),
	   *e = malloc(sizeof(LLNode));
(*c).next = d;
d->next = e;
```

## Linked-Lists: Pointers

The head -- representing the first node or entry point -- is just like any other node, but its address is the one that's typically passed around to represent the entire linked-lists.

Pointers to `LLNode` objects can be `NULL`, so you may want to check before accessing `next` or `data`. But this can be useful to check for the end of a linked-list!

```c
LLNode *node; // suppose it was initialized somewhere...
while (node != NULL) { // iterate until the end
	// suppose you're doing some stuff with node
	node = node->next;
}
```

The tail is the "last" node in the list, and stores a `NULL`-pointer for `next`.

- If you want to stop at a specific node, you can pass the pointer to a *pseudo-*tail.

  ```c
  while (node != tail && node != NULL)
      node = node->next;
  ```

## Linked-Lists: Other Info

1. How big is a linked-list?
2. How can one make a linked-list lightweight?
3. Does its size depend on the objects whose order they represent?
4. Are LLs finite? Does iteration always terminate?
5. What's the complexity of a search? Insertion? Deletion? Update?

## Linked-Lists: Other Info

1. How big is a linked-list? What about the head?

   - _Depends on what's stored (pointers, primitives, structs?) in the nodes. The head is the same size as any node._

2. How can one make a linked-list lightweight?

   - _Just use pointers to anything that is bigger than a pointer!_

3. Does its size depend on the objects whose order they represent?

   - _Depends on how many primitives and pointers you've got._

4. Are LLs finite? Does iteration always terminate?

   - _LLs themselves are bounded (by application or memory) but iteration can be infinite if nodes pointers form a cycle._

5. What's the complexity of a search? Insertion? Deletion? Update?

   - _All of these are linear: $\mathcal O(n)$! These can be improved by combining LL pointers with other data structures you might see in CSC263/265 and CSC373._

## Linked-Lists: Operations

Aside from searching, you can insert/delete/update/move nodes relatively easy.

```c
LLNode *prev, *new, *target; // suppose they're initialized somewhere...
while (node != NULL) {
	if (node != target) {
		new = node->next; prev = node; node = new;
		continue; }
	switch (op) { // assume node == target
		case INSERT: // insert new after target
			new->next = node->next->next;
			node->next = new; break;
		case DELETE: // remove target
			// how would you handle special case of deleting head?
			prev->next = node->next;
			recursive_free(node); break;
		case UPDATE: node->data = 209; break;
	} break; }
```

## Linked-Lists: Operations

Moving `target` can be done by removing but not freeing `target`, and then re-inserting somewhere else in the LL. There's probably other stuff you might do!

### Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/linkedList.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/linkedList.zip)
