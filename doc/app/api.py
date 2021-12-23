

import requests
import json
reponse = requests.get('http://api.stackexchange.com/2.2/questions?order=desc&sort=activity&site=stackoverflow')

for data in reponse.json()['items']:

    if data['answer_count'] == 0:

        print(data['title'])
        print(data['link'])
    else:
        print("SKipped")

    print()

