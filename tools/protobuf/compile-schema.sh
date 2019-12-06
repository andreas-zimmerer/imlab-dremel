#!/bin/bash

# Make sure you have protobuf installed on your system

protoc --plugin=protoc-gen-schema=./protoc-gen-schema.py --proto_path=../.. --schema_out=./gen ../../data/schema.proto
