#!/bin/bash

# Make sure you have protobuf installed on your system

# Create a database schema
protoc --plugin=protoc-gen-schema=./protoc-gen-schema.py --proto_path=../../data --schema_out=./gen ../../data/schema.proto
protoc --proto_path=../../data --cpp_out=./gen ../../data/schema.proto
