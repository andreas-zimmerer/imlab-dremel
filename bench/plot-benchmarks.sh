#!/bin/bash

cd ../cmake-build-release

./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_construct_fsm.csv" --benchmark_filter=BM_Construct_FSM_Fields
./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_assemble_document.csv" --benchmark_filter=BM_Assemble_Document
./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_assemble_dataset.csv" --benchmark_filter=BM_Assembly_Generated_Dataset.*
./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_drill_json.csv" --benchmark_filter=BM_Apache_Drill_Json

cd ../bench/plots

cat bench_construct_fsm.csv | ./generic_google_benchmark_plot.R --name "FSM Construction Time" --parameter "Number of Fields in the FSM"
cat bench_assemble_document.csv | ./generic_google_benchmark_plot.R --name "Document Assemling Time" --parameter "Number of Fields in the Assembled Document"
cat bench_assemble_dataset.csv | ./generic_google_benchmark_plot.R --name "Assembling a generated Dataset of 50 MiB" --parameter "Average Size of a Record from the Dataset in Bytes"
cat bench_drill_json.csv | ./generic_google_benchmark_plot.R --name "Assembling a generated Dataset of 50 MiB with Apache Drill" --parameter "Average Size of a Record from the Dataset in Bytes"
