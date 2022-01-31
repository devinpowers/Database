

import sqlite3

connection  = sqlite3.connect('citi_bikes_2020.db')
cursor = connection.cursor()

with open('citibikes2020.csv', 'r') as file:
    no_records = 0
    next(file) ## skip header row
    for row in file:
        cursor.execute("INSERT INTO CitiBikes VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", row.split(","))
        
        connection.commit()
        no_records +=1
        
connection.close()
print('\n{} Records Transferred'.format(no_records))



print("DONE!")

