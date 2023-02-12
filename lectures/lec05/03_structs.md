# Structs

## Structures: Motivation & Basics

Data is often structured, like in classes in object-oriented programming such as in Java, or relational database schemes like SQL.

In C, we use the `struct`, which is a collection of _members_:

```c
struct [structure tag] {
   member definition;
   ...
   member definition;
} [one or more structure variables];
```

- Can be dynamically or statically allocated.
- Can declare arrays of structs, pointers to structs...

### Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/structs.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/structs.zip)

## Structures: As a Type

A basic declaration can use an anonymous type.

```c
struct { float lon, lat; } a, b;
```

However, commonly a `struct` is declared as an explicit type;

```c
struct coordinate { float lon, lat; };
float euclidean(struct coordinate a, struct coordinate b);
```

Additionally, you can use `typedef` to create an alias for it.

```c
typedef struct coordinate {
    float lon, lat;
} Coordinate;
double euclidean(Coordinate a, Coordinate b);
float manhattan(Coordinate a, Coordinate b);
short time_zone(Coordinate a, Coordinate b);
```

### Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/structs.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/structs.zip)

## Structures: Alignment

As memory addresses implied, pieces of memory are aligned on the smallest granularity of 4 bytes. Members of a `struct` are aligned on their largest member.

What is `sizeof(struct student)`? What if we reorder the members?

```c
struct student {
    char school[21]; // 21
    int student_num; // +4 = 25, round to 28
    char name[21]; // +21 = 49, round to 52
}; // 52
```

Consecutive members of the same size can be packed.

```c
struct student {
    int student_num; // 4
    char name[21], school[21]; // +21+21 = +42 = 46, round to 48
}; // 48
```

Alignment is based on order and size of members. Although there exist compiler optimizations that reorder the members to reduce memory footprint.

## Structures: Accessing Members

As seen before, members can be directly accessed using the dot `.` notation.

```c
#include "math.h"
typedef struct coordinate { float lon, lat; } Coordinate;
float manhattan(Coordinate a, Coordinate b) {
    return abs(b.lon - a.lon) + abs(b.lat - a.lat)
}
double euclidean(Coordinate a, Coordinate b) {
    double dlon = (double)b.lon - (double)a.lon;
    double dlat = (double)b.lat - (double)a.lat;
    return sqrt(pow(dlon, 2) + pow(dlat, 2));
}
```

## Structures: Composition

A `struct` can contain anything; you can nest `struct`'s...

```c
typedef struct box {
    Coordinate c1, c2;
} Box;
```

And access inner members...

```c
Coordinate c1 = {1.7, -2.3}, c2 = {3.08, 9.81};
Box b = {c1, c2};
int lon1 = b.c1.lon;
int lat2 = b.c2.lat;
```

## Structures: Pointers

Passing `struct`'s by value results in deep copies, which are consume a lot of stack memory. Instead, as with arrays, we can pass `struct`'s by pointers.

```c
typedef struct coordinate { float lon, lat; } Coordinate;
typedef struct box {
    Coordinate *c1, *c2;
} Box;
```

- Why are we still storing two `float` values in `Coordinate` and not `float*`?

Then we use pointer operators to access through pointers.

```c
double range(Box b) {
    return euclidean(*(b.c1), *(b.c2));
}
```

Commonly, the arrow `->` notation is used instead for readability.

```c
double area(Box b) {
    return abs(b.c2->lon - b.c1->lon) * abs(b.c2->lat - b.c1->lat);
}
```

## Structures: Linked-Lists: A Taster

A very common data structure that maintains ordering with easy insertions/deletions is the linked-list (LL). Here's a sample `struct` implementation.

```c
typedef struct llnode {
	struct llnode * next;
	int data;
} LLNode;
```

Each `LLNode` holds a pointer `next` to another `LLNode`, so they can refer to each other. More next week...

### Arnold's Code

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/linkedList.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/linkedList.zip)

### Homework

- Your lab exercise this week will be to implement the `ArrayList`.
- A common interview question is reversing a linked-list. Try this too!
