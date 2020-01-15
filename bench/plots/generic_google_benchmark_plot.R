#!/usr/bin/env Rscript

library(ggplot2)
library(tidyr)

theme_set(theme_classic())

# Colorblind-friendly palette
cbPalette <- c("#999999", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")

f <- file("stdin", "r")

# Parse a file from Google Benchmark in CSV format.
# You can specify the format for Google Benchmark by appending --benchmark_out_format=csv to the command.
machine_spec <- readLines(f, n = 1)
date_run <- readLines(f, n = 1)
benchmark_data <- read.csv(f, header = TRUE, comment.char = "*")
benchmark_data <- separate(benchmark_data, "name", c("name", "parameter"), sep = "/", convert = TRUE)


ggplot(benchmark_data, aes(x = parameter, y = cpu_time)) +
  geom_path(aes(color = name)) +
  scale_color_manual(values = cbPalette) +
  labs(color = "Type") +
  ylab("CPU Time") +
  xlab("Parameter") +
  ggsave("benchmark_plot.pdf", width = 10, height = 5, device = cairo_pdf)
