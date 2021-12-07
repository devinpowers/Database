JSON_DIR="./ebay_data"
SCRIPT_PATH="./trans.py"
CATEGORY_TB="./Categories.dat"
ITEMS_TB="./Items.dat"
BIDS_TB="./Bids.dat"
USERS_TB="./Users.dat"
rm -f $CATEGORY_TB
rm -f $ITEMS_TB
rm -f $BIDS_TB
rm -f $USERS_TB
for i in $(ls $JSON_DIR); do
  python $SCRIPT_PATH $JSON_DIR/$i
done