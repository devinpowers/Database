import sys
from json import loads
from re import sub

columnSeparator = "|"

# Dictionary of months used for date transformation
MONTHS = {'Jan':'01','Feb':'02','Mar':'03','Apr':'04','May':'05','Jun':'06',\
        'Jul':'07','Aug':'08','Sep':'09','Oct':'10','Nov':'11','Dec':'12'}

"""
Returns true if a file ends in .json
"""
def isJson(f):
    return len(f) > 5 and f[-5:] == '.json'

"""
Converts month to a number, e.g. 'Dec' to '12'
"""
def transformMonth(mon):
    if mon in MONTHS:
        return MONTHS[mon]
    else:
        return mon

"""
Transforms a timestamp from Mon-DD-YY HH:MM:SS to YYYY-MM-DD HH:MM:SS
"""
def transformDttm(dttm):
    dttm = dttm.strip().split(' ')
    dt = dttm[0].split('-')
    date = '20' + dt[2] + '-'
    date += transformMonth(dt[0]) + '-' + dt[1]
    return date + ' ' + dttm[1]

"""
Transform a dollar value amount from a string like $3,453.23 to XXXXX.xx
"""

def transformDollar(money):
    if money == None or len(money) == 0:
        return money
    return sub(r'[^\d.]', '', money)
    
"""
Parses a single json file. Currently, there's a loop that iterates over each
item in the data set. Your job is to extend this functionality to create all
of the necessary SQL tables for your database.
"""
def parseJson(json_file):
    with open(json_file, 'r') as f:
        items = loads(f.read())['Items'] # creates a Python dictionary of Items for the supplied json file
        f1 = open("Users.dat", "a")
        f2 = open("Bids.dat", "a")
        f3 = open("Categories.dat", "a")
        f4 = open("Items.dat", "a")
        for item in items:
            """
            traverse the items dictionary to extract information from the
            given `json_file' and generate the necessary .dat files to generate
            the SQL tables based on your relation design
            """
            # Get User data:
            # Firstly get the bidder user info:
            Bids = item["Bids"]
            if not Bids == None: 
                for i in range(len(Bids)):
                    bidder = Bids[i]["Bid"]["Bidder"]
                    if not bidder["UserID"] == None:
                        info = "" + '\"' + sub(r'\"','\"\"',bidder["UserID"]) + '\"'
                    else:
                        info = "" + "NULL"
                    if not bidder["Rating"] == None:
                        info += columnSeparator + bidder["Rating"]
                    else:
                        info += columnSeparator + "NULL"
                    if not "Location" in bidder.keys() or bidder["Location"] == None:
                        info += columnSeparator + "NULL"
                    else:
                        info += columnSeparator + '\"' + sub(r'\"','\"\"',bidder["Location"]) + '\"'
                    if not "Country" in bidder.keys() or bidder["Country"] == None:
                        info += columnSeparator + "NULL"
                    else:
                        info += columnSeparator + '\"' + sub(r'\"','\"\"',bidder["Country"]) + '\"'
                    info += "\n"
                    f1.write(info)
                
            # Get the seller user info:
            Seller = item["Seller"]
            SellerID = Seller["UserID"]
            if SellerID == None:
                SellerID = "NULL"
            else:
                SellerID = '\"' + sub(r'\"','\"\"',SellerID) + '\"'
            info = "" + SellerID
            if not Seller["Rating"] == None:
                info += columnSeparator + Seller["Rating"]
            else:
                info += columnSeparator + "NULL"
            if not item["Location"] == None:
                info += columnSeparator + '\"' + sub(r'\"','\"\"',item["Location"]) + '\"'
            else:
                info += columnSeparator + "NULL"
            if not item["Country"] == None:
                info += columnSeparator + '\"' + sub(r'\"','\"\"',item["Country"]) + '\"' + "\n"
            else:
                info += columnSeparator +"NULL" + "\n"
            f1.write(info)
            
            # Get Bids data:
            Bids = item["Bids"]
            if not Bids == None:
                for i in range(len(Bids)):
                    if not item["ItemID"] == None:
                        info = "" + item["ItemID"]
                    else:
                        info = "" + "NULL"
                    bid = Bids[i]["Bid"]
                    if not bid["Bidder"]["UserID"] == None:
                        info += columnSeparator + '\"' + sub(r'\"','\"\"',bid["Bidder"]["UserID"]) + '\"'
                    else:
                        info += columnSeparator + "NULL"
                    if not bid["Time"] == None:
                        info += columnSeparator + transformDttm(bid["Time"])
                    else:
                        info += columnSeparator + "NULL"
                    if not bid["Amount"] == None:
                        info += columnSeparator + transformDollar(bid["Amount"])
                    else:
                        info += columnSeparator + "NULL"
                    info += "\n"
                    f2.write(info)
                
            # Get Categories data:
            Categories = item["Category"]
            ItemID = item["ItemID"]
            if ItemID == None:
                ItemID = "NULL"
            for Category in Categories:
                info = "" + ItemID + columnSeparator + '\"' + sub(r'\"','\"\"',Category) + '\"' + "\n"
                f3.write(info)
            
            # Get Items data:
            info = "" + ItemID + "|" + SellerID 
            if not item["Name"] == None:
                info += columnSeparator + '\"' + sub(r'\"','\"\"',item["Name"]) + '\"' 
            else:
                info += columnSeparator + "NULL"
            if "Buy_Price" in item.keys():
                info += columnSeparator + transformDollar(item["Buy_Price"])
            else:
                info += columnSeparator + "NULL"
            if not item["First_Bid"] == None:
                First_Bid = transformDollar(item["First_Bid"])
            else:
                First_Bid = "NULL"
            if not item["Currently"] == None:
                Currently =  transformDollar(item["Currently"]) 
            else:
                Currently = "NULL"
            if not item["Number_of_Bids"] == None:
                Number_of_Bids = item["Number_of_Bids"]
            else:
                Number_of_Bids = "NULL"
            if not item["Started"] == None:
                Started = transformDttm(item["Started"])
            else:
                Started = "NULL"
            if not item["Ends"] == None:
                Ends = transformDttm(item["Ends"])
            else:
                Ends = "NULL"
            info += columnSeparator + First_Bid + columnSeparator + Currently
            info += columnSeparator + Number_of_Bids + "|" + Started
            info += columnSeparator + Ends
            if item["Description"] == None:
                info += columnSeparator + "NULL" +"\n"
            else:
                info += columnSeparator + '\"' + sub(r'\"','\"\"',item["Description"]) + '\"' +"\n"
            f4.write(info)
            
        f1.close()
        f2.close()
        f3.close()
        f4.close()

"""
Loops through each json files provided on the command line and passes each file
to the parser

"""
def main(argv):
    if len(argv) < 2:
        print(sys.stderr, 'Usage: python skeleton_json_parser.py <path to json files>')
        sys.exit(1)
    # loops over all .json files in the argument
    for f in argv[1:]:
        if isJson(f):
            parseJson(f)
            print("Success parsing " + f)

if __name__ == '__main__':
    main(sys.argv)

