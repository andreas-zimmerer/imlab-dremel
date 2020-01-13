#!/usr/bin/env python

import json
import csv
import random
import string

TOTAL_NUMBER_OF_ENTRIES = 1000
MAX_NUMBER_BACKWARD_LINKS = 20
MAX_NUMBER_FORWARD_LINKS = 20
MAX_NUMBER_NAMES = 10
MAX_NUMBER_LANGUAGES = 5
LIKELIHOOD_OPTIONAL_FIELDS = 1.0

country_codes = ['en-us', 'en-gb', 'de-de']
country_names = ['us', 'gb', 'de']


def random_string(length=10):
    """Generate a random string of fixed length """
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(length))


def generate_entry(entry_id):
    entry = {'DocId': entry_id}

    # Optionally create a "Links" field
    if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
        entry['Links'] = {}

        # Create a random number of "Backward" links
        number_backward = random.randrange(MAX_NUMBER_BACKWARD_LINKS)
        if number_backward > 0:
            entry['Links']['Backward'] = []
        for b in range(number_backward):
            entry['Links']['Backward'].append(random.randrange(1000))

        # Create a random number of "Forward" links
        number_forward = random.randrange(MAX_NUMBER_FORWARD_LINKS)
        if number_forward > 0:
            entry['Links']['Forward'] = []
        for f in range(number_forward):
            entry['Links']['Forward'].append(random.randrange(1000))

    # Random number of "Name" entries
    number_name = random.randrange(MAX_NUMBER_NAMES)
    if number_name > 0:
        entry['Name'] = []
        for n in range(number_name):
            name = {}

            # Create a random number of "Languages"
            number_language = random.randrange(MAX_NUMBER_LANGUAGES)
            if number_language > 0:
                name['Language'] = []
                for l in range(number_language):
                    country_idx = random.randrange(0, len(country_codes))
                    language = {'Code': country_codes[country_idx]}

                    # Optionally create a "Country"
                    if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
                        language['Country'] = country_names[country_idx]
                    name['Language'].append(language)

            # Optionally create a "Url" for a "Name
            if random.random() < LIKELIHOOD_OPTIONAL_FIELDS:
                name['Url'] = 'http://' + random_string() + '.' + random_string(2)
            entry['Name'].append(name)

    return entry


# Create TOTAL_NUMBER_OF_ENTRIES random entries
random.seed(1234)
output = [generate_entry(i) for i in range(TOTAL_NUMBER_OF_ENTRIES)]


# JSON format
with open('data.json', 'w') as file:
    file.write(json.dumps(output, indent=2))

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


for record in output:
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
