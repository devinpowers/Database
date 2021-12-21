import sqlalchemy
import pandas as pd 
from sqlalchemy.orm import sessionmaker
import requests
import json
from datetime import datetime
import datetime
import sqlite3


# Generate your token here:  https://developer.spotify.com/console/get-recently-played/
# Note: You need a Spotify account (can be easily created for free)





def run_spotify_etl():
    DATABASE_LOCATION = "sqlite:///my_played_tracks.sqlite"
    USER_ID = 'devinpowers'
    TOKEN = 'BQBYWihYEqh4trG4SsLjCTSPPrZjh41ial5D7zd9nVZF8vehcRtsJduOvR2Pt0Qo-AZ1eKSGAyp8Y9v8pwa25jQcQAMR4mKRrR2UqPVaDKohsVyuH8bzuyx6V3mkUcxIujIZpsU8OMlf9DwrPRlgwxmdcnANbkc5fBL8dEx3ZxmJyDPewjw'

      # Extract part of the ETL process
 
    headers = {
        "Accept" : "application/json",
        "Content-Type" : "application/json",
        "Authorization" : "Bearer {token}".format(token=TOKEN)
    }
    
    # Convert time to Unix timestamp in miliseconds      
    today = datetime.datetime.now()
    yesterday = today - datetime.timedelta(days=1)
    yesterday_unix_timestamp = int(yesterday.timestamp()) * 1000

    # Download all songs you've listened to "after yesterday", which means in the last 24 hours      
    r = requests.get("https://api.spotify.com/v1/me/player/recently-played?after={time}".format(time=yesterday_unix_timestamp), headers = headers)

    data = r.json()

    song_names = []
    artist_names = []
    played_at_list = []
    timestamps = []

    # Extracting only the relevant bits of data from the json object      
    for song in data["items"]:
        song_names.append(song["track"]["name"])
        artist_names.append(song["track"]["album"]["artists"][0]["name"])
        played_at_list.append(song["played_at"])
        timestamps.append(song["played_at"][0:10])
        
    # Prepare a dictionary in order to turn it into a pandas dataframe below       
    song_dict = {
        "song_name" : song_names,
        "artist_name": artist_names,
        "played_at" : played_at_list,
        "timestamp" : timestamps
    }

    song_df = pd.DataFrame(song_dict, columns = ["song_name", "artist_name", "played_at", "timestamp"])
    
    print(song_df)

if __name__ == "__main__":

    run_spotify_etl()