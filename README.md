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

How many transactions per second can your implementation execute?

Make sure your builds are not failing! <br/>
*Left Sidebar > CI /CD > Pipelines*
