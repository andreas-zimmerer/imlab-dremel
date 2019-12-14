#!/bin/bash

# Make sure you have protobuf installed on your system

# Create a database schema
protoc --plugin=protoc-gen-schema=./protoc-gen-schema.py --proto_path=../.. --schema_out=./gen ../../data/schema.proto
protoc --proto_path=../.. --cpp_out=./gen ../../data/schema.proto

mv ./gen/data/*.pb.h ../../include/imlab/schema_types.h
mv ./gen/data/*.pb.cc ../../src/imlab/schema_types.cc

mv ./gen/data/*.h ../../include/imlab/
mv ./gen/data/*.cc ../../src/imlab/
