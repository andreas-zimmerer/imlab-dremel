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

Make sure your builds are not failing! <br/>
*Left Sidebar > CI /CD > Pipelines*
