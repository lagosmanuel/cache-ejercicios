# Computer Architecture (UNS)
Exercises on matrix traversal and cache memory miss analysis for the Computer Architecture course at Universidad Nacional del Sur (UNS).

## Exercise 1

Given a square matrix `A` of `NxN` integers, where `N = 32` and integers occupy 4 bytes. The first index represents rows, the second columns, both ranging from `0` to `N-1`. The matrix is stored in memory in row-major order, meaning element `A[i][j]` is located at memory position:
```
base_address_of_A + (i * N + j) * element_size
```

1. Assuming the base address of matrix `A` is `0410H`, what is the base address of the second row? What is the address of component `A[N-1][N-1]`?

Assuming physical memory addresses are 16-bit and there's a physical cache of 512 Bytes for data, implemented with direct mapping and 32-byte lines.

2. Explain how the physical address is divided to access the cache and how many bits each part has.
3. Determine how many blocks the matrix is divided into, what the base address of each block is (for the first 4 rows at least), and the cache index associated with each.

There are two possible ways to traverse the matrix to solve a particular problem:

![](https://github.com/lagosmanuel/cache-ejercicios/blob/main/screenshots/screenshot1.png)

_(**Traversal 1**) and (**Traversal 2**)_

Assume that at the start of each traversal in the data cache, nothing useful is present, and operating with the indices involved in the traversal doesn't imply memory accesses because they're stored in registers:

4. For each traversal, how many data access misses occurred? What was the miss rate?
5. Out of the total misses, what percentage were compulsory misses?
6. Which of the two traversals is more recommended? Justify your answer.
7. What would have happened if the matrix were stored in memory in column-major order, i.e., if element `A[i][j]` were located at memory position `base_address_of_A + (j * N + i) * element_size`?

## Exercise 2

Given two square matrices `A` and `B`, each containing 1024 integers, where each integer is 4 bytes. The first element of `A` (`A[0,0]`) is stored at physical address `0000 4020H` and the first element of `B` at physical address `0001 0800H`.

The system has a 4KB data cache, organized in direct mapping with 16-byte lines and a write-through, write-allocate policy.

1. How many lines are there in the cache?
2. How is the physical address divided into fields and how many bits does each field have to access the cache?
3. Determine how many blocks each matrix is divided into and which set each belongs to.

If initially all entries in the cache are marked as invalid, and during the execution of the following code only `A` and `B` are stored in the cache because `i` and `j` are stored in registers.

```java
for (i = 0; i < 32; i++) {
  for (j=0; j < 32; j++) {
    A[i,j] = i+j;
    B[j,i] = B[j,i] + A[i,j];
  }
}
```

4. How many read and write misses occur? Would this value have changed if the cache, maintaining the same data capacity, had been organized with an associativity degree of 2?
5. How many bytes were written to main memory?
