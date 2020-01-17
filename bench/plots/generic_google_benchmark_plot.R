#!/usr/bin/env Rscript

library(ggplot2)
library(tidyr)
library("optparse")


option_list <- list(
  make_option(c("-n", "--name"), type="character", default=NULL,
              help="Name of the benchmark. Will influence plot title and the file it is saved to.", metavar="character"),
  make_option(c("-p", "--parameter"), type="character", default=NULL,
              help="If the benchmark has a parameter (plotted on x-axis), how should the axis be labeled?", metavar="character")
);
opt_parser <- OptionParser(option_list=option_list);
opt <- parse_args(opt_parser);

theme_set(theme_minimal())

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
  expand_limits(y = 0) +
  scale_x_log10(breaks = unique(benchmark_data$parameter)) +
  scale_color_manual(values = cbPalette) +
  labs(title = opt$name, color = "Type") +
  ylab("CPU Time") +
  xlab(opt$parameter) +
  ggsave(paste0(opt$name, ".pdf"), width = 10, height = 5, device = cairo_pdf)
