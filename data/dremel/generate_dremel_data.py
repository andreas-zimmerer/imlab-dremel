#!/usr/bin/env python

import json
import random
import string

TOTAL_NUMBER_OF_ENTRIES = 1000
MAX_NUMBER_BACKWARD_LINKS = 20
MAX_NUMBER_FORWARD_LINKS = 20
MAX_NUMBER_NAMES = 10
MAX_NUMBER_LANGUAGES = 5
LIKELIHOOD_OPTIONAL_FIELDS = 1.0

country_codes = ['US', 'DE', 'IT', 'GB']
country_names = ['United States', 'Germany', 'Italy', 'Great Britain']


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
print(json.dumps(output, indent=2))
