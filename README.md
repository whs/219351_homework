# Webapp homeworks

## Homework 2

Build with `cmake && make`.

```
Benchmarking threaded impl:
9 8 7 6 5 4 3 2 1 0 
99990 99991 99992 99993 99994 99995 99996 99997 99998 99999 
elapsed time = 43.584559

Benchmarking non-threaded impl:
9 8 7 6 5 4 3 2 1 0 
99990 99991 99992 99993 99994 99995 99996 99997 99998 99999 
elapsed time = 77.787568
```

## Homework 3

Run `bench.sh` to build and run.

Interestingly, it is found that the C++ version is faster than C version for hot startup:

```
C: Elapsed time MPI_Wtime is 1.107012
C++: Elapsed time MPI_Wtime is 1.02438
```

## Homework 5

Needs the complete project structure to compile, which is not included.

- Problem 5: Result will be printed
- Problem 6: Run with input and output path as arguments. Both must be HDFS path.
- Problem 7: May take a few days to complete. Run with input as argument, output is printed.
- Problem 8: Run with input and output path as arguments. Both must be HDFS path.

## Homework 6

- Problem4: Result will be saved in `problem4_out` in HDFS
- Problem5: Result will be printed, may be partial solution
