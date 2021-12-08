SELECT COUNT(*) FROM (
	SELECT * FROM Items INNER JOIN Users on Items.UserID = Users.UserID 
	WHERE rating > 1000 GROUP BY Items.UserID
);