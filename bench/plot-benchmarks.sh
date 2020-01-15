#!/bin/bash

cd ../cmake-build-release

#./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_construct_fsm.csv" --benchmark_filter=BM_Construct_FSM_Fields
#./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_assemble_document.csv" --benchmark_filter=BM_Assemble_Document
#./dremel_benchmark --benchmark_out_format=csv --benchmark_out="../bench/plots/bench_assemble_dataset.csv" --benchmark_filter=BM_Assembly_Generated_Dataset.*

cd ../bench/plots

cat bench_construct_fsm.csv | ./generic_google_benchmark_plot.R
cat bench_assemble_document.csv | ./generic_google_benchmark_plot.R
cat bench_assemble_dataset.csv | ./generic_google_benchmark_plot.R
