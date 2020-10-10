# IMLAB-DREMEL

_Implement [Google Dremel](https://static.googleusercontent.com/media/research.google.com/de//pubs/archive/36632.pdf)'s storage engine in a custom in-memory DB._


The project started out as a small in-memory database written in C++.

The database used Bison and Flex to parse SQL schema information and queries in a REPL environment.
Queries were executed by generating query-specific code at runtime as it was proposed in the paper ["Efficiently Compiling Efficient Query Plans for Modern Hardware" by T. Neumann](https://www.vldb.org/pvldb/vol4/p539-neumann.pdf). It further implemented ["Morsel Driven Parallelism" by Leis et al](http://db.in.tum.de/~leis/papers/morsels.pdf) to efficiently utilize multiple cores during query execution.
Also, similar to [Hyper](http://hyper-db.de/), transactions were executing in isolation by utilizing the `fork` system call.


Later, the database was extended to support nested data by using Google Dremel's memory layout.


Dremel
======

Benchmark (2020-01-13):
```
Run on (8 X 2300 MHz CPU s)
2020-01-13 23:51:29
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                                       Time           CPU Iterations
------------------------------------------------------------------------------
BM_Construct_Complete_FSM                    2866 ns       2854 ns     308817   342.187k items/s
BM_Shredding_nLanguage/1                     2339 ns       2305 ns     229130   423.709k items/s
BM_Shredding_nLanguage/2                     2472 ns       2469 ns     285045   395.547k items/s
BM_Shredding_nLanguage/4                     3483 ns       3480 ns     199078   280.642k items/s
BM_Shredding_nLanguage/8                     5724 ns       5719 ns     126354   170.771k items/s
BM_Shredding_nLanguage/16                   10980 ns      10967 ns      69030   89.0465k items/s
BM_Shredding_nLanguage/32                   20692 ns      20670 ns      35912   47.2446k items/s
BM_Shredding_nLanguage/64                   39186 ns      39148 ns      17596   24.9456k items/s
BM_Shredding_nLanguage/128                  79295 ns      79207 ns       9867   12.3292k items/s
BM_Shredding_nLanguage/256                 153893 ns     153729 ns       4860   6.35248k items/s
BM_Shredding_nLanguage/512                 300106 ns     299793 ns       2341   3.25745k items/s
BM_Shredding_nLanguage/1024                626635 ns     625904 ns       1205   1.56024k items/s
BM_Load_Generated_Dataset/iterations:2        636 ms        636 ms          2   78.6293MB/s   7.86293k items/s
BM_Assemble_Document                         7438 ns       7433 ns      94086   131.383k items/s
BM_Assembly_Generated_Dataset/64             4303 ms       4299 ms          1   23.2629MB/s   372.207k items/s
BM_Assembly_Generated_Dataset/128            2822 ms       2819 ms          1   35.4741MB/s   283.793k items/s
BM_Assembly_Generated_Dataset/256            4128 ms       4123 ms          1   24.2542MB/s   97.0169k items/s
BM_Assembly_Generated_Dataset/512            4189 ms       4184 ms          1   23.9005MB/s   47.8011k items/s
BM_Assembly_Generated_Dataset/1024           4163 ms       4158 ms          1   24.0477MB/s   24.0477k items/s
BM_Assembly_Generated_Dataset/2048           4296 ms       4291 ms          1   23.3025MB/s   11.6512k items/s
BM_Assembly_Generated_Dataset/4096           4234 ms       4229 ms          1   23.6463MB/s   5.91158k items/s
```

