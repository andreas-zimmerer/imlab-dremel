# IMLAB

## Task 1

* Implement the TPC-C [New Order](/data/new_order.sql) transaction and storage for all [relations](/data/schema.sql) as a standalone C++ program.
* Implement the [load procedures](/include/imlab/database.h) to populate the database with the data provided in the [\*.tbl](https://db.in.tum.de/teaching/ws1718/imlab/tpcc_5w.tar.gz) files.
* Translate the *New Order* transaction to C++ using the [mapping](/include/imlab/infra/types.h) from SQL to C data types.
    * **integer**: int32_t
    * **char**, **varchar**: length plus character array (do not use pointers or std::string)
    * **timestamp**: uint64_t (seconds since 01.01.1970)
    * **numeric(x, y)**: int64_t<br/>
    (x is the total number of digits, y is number of digits after the decimal separator, for example numeric(10,2) x=12.3 is represented as 1230)
* Implement the [New Order](/data/new_order.sql) transaction as [C++ function](/include/imlab/database.h).
* Execute the *New Order* transaction 1 million times with [random input](/tools/imlabdb.cc).
* Print the number of tuples in the tables before and after executing the transactions.

How many *New Order* transactions per second can your implementation execute?

Useful files:
* [CMake configuration (CMakeLists.txt)](/CMakeLists.txt)
* [Type header (include/imlab/infra/types.h)](/include/imlab/infra/types.h)
* [Key hashing (include/imlab/infra/key.h)](/include/imlab/infra/hash.h)
* [Database header (include/imlab/database.h)](/include/imlab/database.h)
* [Database defs (src/database.cc)](/src/database.cc)
* [Database tests (test/database_test.cc)](/test/database_test.cc)
* [Executable (tools/imlabdb.cc)](/tools/imlabdb.cc)

Useful links:
* [TPC-C website](http://www.tpc.org/tpcc/)
* [TPC-C specification](http://www.tpc.org/tpc_documents_current_versions/pdf/tpc-c_v5.11.0.pdf)
* [TPC-C data (5 warehouses)](https://db.in.tum.de/teaching/ws1718/imlab/tpcc_5w.tar.gz)

## Task 2

* Use *bison* and *flex* to write a parser for SQL schema files.
    * You can assume, that all attributes are declared NOT NULL.
    * You must support primary keys.
    * You can ignore secondary indexes (for now).
* Write a schema compiler, that generates C++ code from the [schema representation](include/imlab/schemac/schema_parse_context.h).
    * You must implement the CRUD operations for every relation. (**C**reate, **R**ead, **U**pdate, **D**elete).
    * It must further be possible to lookup a tuple by its primary key (if any).
    * Use the schema compiler to generate code for the [schema](data/schema.sql) and add it to your repository.
* Re-implement the [New Order](data/new_order.sql) transaction as [C++ function](include/imlab/database.h) using your generated code.

The project skeleton for this task contains a grammar & scanner for a toy format called **FOO**, which looks as follows:

```
foo bar1 {
    a integer,
    b char{20}
};

foo bar2 {
    c integer
};
```

You can use the executable `schemac` to parse a **FOO** file with the following command:
```
./schemac --in ~/Desktop/example.foo --out_cc /tmp/example.cc --out_h /tmp/example.h
```

First learn how bison & flex work together and how they are used in the following files:
* [FOO grammar](tools/schemac/schema_parser.y)
* [FOO scanner](tools/schemac/schema_scanner.l)
* [CMake integration](tools/schemac/local.cmake)
* [Parse context H](include/imlab/schemac/schema_parse_context.h)
* [Parse context CC](tools/schemac/schema_parse_context.cc)
* [Parser tests](test/schemac/schema_parser_test.cc)

Then use the parsed schema to generate C++ code with a schema compiler:
* [Schema compiler H](include/imlab/schemac/schema_compiler.h)
* [Schema compiler CC](tools/schemac/schema_compiler.cc)

## Task 3

* Implement the [Delivery](data/delivery.sql) transaction.
* Execute 1 million transactions [composed of a mix](tools/imlabdb.cc) of [New Order](data/new_order.sql) (90%) and [Delivery](data/delivery.sql) (10%) transactions.
* Implement [this analytical query](data/olap.sql) using STL datastructures for the join implementation.<br/>
    * Do not use any existing existing index structures.
    * Follow [this interface](include/imlab/database.h).
    * The result of the query [our dataset](https://db.in.tum.de/teaching/ws1718/imlab/tpcc_5w.tar.gz) should be *1367872001.25*.
* Execute OLTP and OLAP queries concurrently.
    * Execute 1 million transactions of the NewOrder/Delivery mix.
    * Run the analytical concurrently using the **fork** system call (in [imlabdb.cc](tools/imlabdb.cc)).
    * Whenever an analytical query is finished, create a new snapshot using fork, such that exactly one snapshot and query is active at any given time.
    * [This example](data/fork_example.cc) for using fork may be helpful.
* Implement your own lazy hash table using [this interface](include/imlab/infra/hash_table.h).
* Measure the runtimes of the analytical query with the STL datastructure and your hash table.

## Task 4

* Implement query compilation of relational algebra trees to C++ code. Use the produce/consume (push) model.
* You need to support the following operators: [table scan](include/imlab/algebra/table_scan.h), [selection](include/imlab/algebra/selection.h), [print](include/imlab/algebra/print.h) and [inner join](include/imlab/algebra/inner_join.h).
* Test your code to make sure, that the [IUs](include/imlab/algebra/iu.h) are correctly propagated through the operators.
* Manually build an algebra tree for [this SQL query](data/queryc_1.sql).
* Write a [query compiler](tools/queryc/queryc.cc) that generates C++ code for the static algebra tree.
* Add the generated C++ code to your repository and execute it in [imlabdb.cc](tools/imlabdb.cc) using the print operator.

## Task 5

* Implement a simple parser for the following subset of SQL:
    * The select clause contains one or more attribute names.
    * The from clause contains one or more relation names.
    * The where clause is optional and can contain one or more selections (connected by "and").
    * You only need to support selections of the form "attribute = attribute" and "attribute = constant".
    * You can assume that each relation is referenced only once, and that all attribute names are unique.

* Implement a semantic analysis, which takes the parsed SQL statement and the schema as input and computes an algebra tree that can be used as input to your code generator.
    * Report errors when non-existing attributes or tables are referenced.
    * You should report an error if a table has no join condition (i.e., a cross product would be necessary).
    * Build left-deep join trees based on the order of the relations in the from clause.

* Implement a REPL ("read-eval-print-loop") for your database system:
    * Read one line from standard input.
    * Analyze it (report any errors and abort if necessary).
    * Generate C++ code and store it in a temporary file.
    * Call a C++ compiler and generate a temporary shared library (.so file).
    * Using `dlopen`, `dlsym`, and `dlclose` you can now load the shared library and execute the query in it (have a look at [this example](data/dlopen)), you man need to compile with `-fPIC` and `-rdynamic`.
    * Measure and report the compilation and the execution times separately.

* Test your database with [these queries](data/queryc_2.sql).

## Task 6

* Implement morsel-driven parallelism in your query engine.
    * Use `parallel_for` in your table scans. [TBB](https://www.threadingbuildingblocks.org/docs/help/tbb_userguide/parallel_for.html)
    * Make your hash table of task 3 thread-safe and build it multi-threaded with `enumerable_thread_specific`. [TBB](https://www.threadingbuildingblocks.org/docs/help/reference/thread_local_storage/enumerable_thread_specific_cls.html)
* Skim through [this paper list](https://gitlab.db.in.tum.de/imlab19/project-topics) and find a project topic that you want to work on for the rest of this course.

How many transactions per second can your implementation execute?

Make sure your builds are not failing! <br/>
*Left Sidebar > CI /CD > Pipelines*



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
