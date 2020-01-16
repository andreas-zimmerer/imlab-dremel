#!/usr/bin/env python

"""
Call this script like:
./generate_dremel_data.py <number> <size>

 <number>   Number of records to generate.
 <size>     Average size of one record in bytes.
"""

import sys
import json
import csv
import random
import string
import os.path

if len(sys.argv) != 3:
    print('Script requires two arguments: <number> and <size>')
    sys.exit(0)

TOTAL_NUMBER_OF_RECORDS = int(sys.argv[1])
AVG_SIZE_OF_RECORD = int(sys.argv[2])
LIKELIHOOD_OPTIONAL_FIELDS = 1  # AVG_SIZE only works with 1 right now

country_codes = ['en-us', 'en-gb', 'de-de']
country_names = ['us', 'gb', 'de']


def random_string(length=10):
    """Generate a random string of fixed length """
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(length))


def calculate_record_size(record):
    size = 0
    if type(record) == dict:
        for key, value in record.items():
            size += calculate_record_size(value)
    elif type(record) == list:
        for l in record:
            size += calculate_record_size(l)
    elif type(record) == int:
        size += 8  # 64 bit integer
    elif type(record) == str:
        size += len(record)
    else:
        print(type(record))
    return size


def generate_record(entry_id):
    entry = {'DocId': entry_id}

    # Optionally create a "Links" field
    if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
        # Create a random number of "Backward" links
        avg_bytes_links = (AVG_SIZE_OF_RECORD - 8) / 2
        number_backward = random.randrange(0, round(avg_bytes_links / 2 / 8) * 2)
        if number_backward > 0:
            if 'Links' not in entry:
                entry['Links'] = {}
            entry['Links']['Backward'] = []
        for b in range(number_backward):
            entry['Links']['Backward'].append(random.randrange(1000))

        # Create a random number of "Forward" links
        number_forward = random.randrange(0, round(avg_bytes_links / 2 / 8) * 2)
        if number_forward > 0:
            if 'Links' not in entry:
                entry['Links'] = {}
            entry['Links']['Forward'] = []
        for f in range(number_forward):
            entry['Links']['Forward'].append(random.randrange(1000))

    avg_bytes_names = (AVG_SIZE_OF_RECORD - 8) / 2
    number_name = random.randrange(0, round(avg_bytes_names / 41) * 2)
    if number_name > 0:
        entry['Name'] = []
        for n in range(number_name):  # because avg. 3 languages per name -> size of name is on average 28
            name = {}

            # Create a random number of "Languages"
            number_language = random.randrange(0, 3 * 2)  # on average 3 languages per name
            if number_language > 0:
                name['Language'] = []
                for l in range(number_language):  # one language field has on average a size of 6
                    country_idx = random.randrange(0, len(country_codes))
                    language = {'Code': country_codes[country_idx]}

                    # Optionally create a "Country"
                    if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
                        language['Country'] = country_names[country_idx]
                    name['Language'].append(language)

            # Optionally create a "Url" for a "Name
            if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
                name['Url'] = 'http://' + random_string() + '.' + random_string(2)

            if number_language > 0 or 'Url' in name:  # only add name if it contains something
                entry['Name'].append(name)

    return entry


# Check if file already exists
json_filename = 'generated_data_' + str(TOTAL_NUMBER_OF_RECORDS) + '_' + str(AVG_SIZE_OF_RECORD) + '.json'
if os.path.isfile(json_filename):
    sys.exit(0)

# Create TOTAL_NUMBER_OF_ENTRIES random entries
random.seed(1234)
records = [generate_record(i) for i in range(TOTAL_NUMBER_OF_RECORDS)]

total_size = 0
for r in records:
    total_size += calculate_record_size(r)
print('Generated ' + str(TOTAL_NUMBER_OF_RECORDS) + ' random records with total size of ' + str(total_size) + ' bytes.')
print('Avg. ' + str(total_size / TOTAL_NUMBER_OF_RECORDS) + ' bytes per record.')

# JSON format
with open(json_filename, 'w') as file:
    file.write(json.dumps(records, indent=2))

# DB columnar format in CSV
docId_file = open('t_docId.csv', mode='w')
docId_writer = csv.DictWriter(docId_file, fieldnames=['DocId'])
docId_writer.writeheader()
links_backward_file = open('t_links_backward.csv', mode='w')
links_backward_writer = csv.DictWriter(links_backward_file, fieldnames=['DocId', 'Links.Backward'])
links_backward_writer.writeheader()
links_forward_file = open('t_links_forward.csv', mode='w')
links_forward_writer = csv.DictWriter(links_forward_file, fieldnames=['DocId', 'Links.Forward'])
links_forward_writer.writeheader()
name_language_code_file = open('t_name_language_code.csv', mode='w')
name_language_code_writer = csv.DictWriter(name_language_code_file, fieldnames=['DocId', 'Name.Language.Code'])
name_language_code_writer.writeheader()
name_language_country_file = open('t_name_language_country.csv', mode='w')
name_language_country_writer = csv.DictWriter(name_language_country_file, fieldnames=['DocId', 'Name.Language.Country'])
name_language_country_writer.writeheader()
name_url_file = open('t_name_url.csv', mode='w')
name_url_writer = csv.DictWriter(name_url_file, fieldnames=['DocId', 'Name.Url'])
name_url_writer.writeheader()


for record in records:
    docId = record['DocId']
    docId_writer.writerow({'DocId': docId})
    if 'Links' in record:
        if 'Backward' in record['Links']:
            for b in record['Links']['Backward']:
                links_backward_writer.writerow({'DocId': docId, 'Links.Backward': b})
        if 'Forward' in record['Links']:
            for f in record['Links']['Forward']:
                links_forward_writer.writerow({'DocId': docId, 'Links.Forward': f})
    if 'Name' in  record:
        for n in record['Name']:
            if 'Language' in n:
                for l in n['Language']:
                    code = l['Code']
                    name_language_code_writer.writerow({'DocId': docId, 'Name.Language.Code': code})
                    if 'Country' in l:
                        country = l['Country']
                        name_language_country_writer.writerow({'DocId': docId, 'Name.Language.Country': country})
            if 'Url' in n:
                url = n['Url']
                name_url_writer.writerow({'DocId': docId, 'Name.Url': url})

links_backward_file.close()
links_forward_file.close()
name_language_code_file.close()
name_language_country_file.close()
name_url_file.close()
