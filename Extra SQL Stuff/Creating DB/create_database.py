


import sqlite3

# Creating Database using SQLite:
# .connect("name_of_data_base_to_Create.db")
    
conn = sqlite3.connect('citi_bikes_2023.db')

## Cursor enables traversal over the records in our database
cur = conn.cursor()



## We create a table in our DataBase and label each of the headers
## For each column

sql = """
    CREATE TABLE CitiBikes (
        Trip_Duration INTEGER,
        StartTime TEXT,
        StopTime TEXT,
        StartStationID INTEGER,
        StartStationName TEXT,
        StartStationLat DECIMAL,
        StartStationLong DECIMAL,
        EndStationID INTEGER,
        EndStationName TEXT,
        EndStationLat DECIMAL,
        EndStationlog DECIMAL,
        BikeId INTEGAR,
        Usertype TEXT,
        Birthyear TEXT,
        Gender INTEGER
        
        )
    """
    
cur.execute(sql)
print('Database has been created')

conn.commit()
conn.close()
    


