drop table if exists Items;
drop table if exists Users;
drop table if exists Bids;
drop table if exists Categories;

create table Users(
    UserID varchar(255),
    Rating int,
    Location varchar(255),
    Country varchar(255),
    PRIMARY KEY(UserID)
);

create table Items(
    
    ItemID int,
    SellerID varchar(255),
    Name varchar(255),
    Buy_Price varchar(255),
    First_Bid DECIMAL NOT NULL,
    Currently int,
    Number_of_Bids int,
    Started varchar(255),
    Ends varchar(255),
    Description varchar(255),
    PRIMARY KEY(ItemID),
    FOREIGN KEY (SellerID) REFERENCES Users
);

create table Bids(
    ItemID int,
    UserID varchar(255),
    Time varchar(255),
    Amount int,
    PRIMARY KEY(ItemID, UserID, Amount),
    FOREIGN KEY (ItemID) REFERENCES Items,
    FOREIGN KEY (UserID) REFERENCES Users
);

create table Categories(
    ItemID int,
    Category_Name varchar(255),
    PRIMARY KEY(ItemID, Category_Name),
    FOREIGN KEY (ItemID) REFERENCES Items
);
