#!/bin/bash

# Make sure you have protobuf installed on your system

# Create a database schema
protoc --plugin=protoc-gen-schema=./protoc-gen-schema.py --proto_path=../.. --schema_out=./gen ../../data/schema.proto

mv ./gen/data/*.cc ../../src/imlab/
mv ./gen/data/*.h ../../include/imlab/
