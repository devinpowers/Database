SELECT COUNT(*)
FROM EbayUsers
WHERE Location = "New York";


SELECT COUNT(*)
FROM EbayUsers
WHERE Location = "Grand Rapids";


//Find in two cities

SELECT COUNT(*)
FROM EbayUsers
WHERE Location = "Miami" OR Location = "Austin";

