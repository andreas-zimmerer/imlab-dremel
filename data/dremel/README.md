# Dremel Data

This directory should contain the input data for the Dremel engine.

The example data can be randomly generated with
```
./generate_dremel_data.py > dremel_data.json
```

This will generate a json file with 100000 random entries.
All parameters for generating the data can be adjusted in `generate_dremel_data.py`.

The schema of the generated data follows [`schema.proto`](/data/schema.proto)
