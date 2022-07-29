/*
1) Crates a new account
*/
INSERT INTO Customer VALUES(321, 'John', 1223, 'a@b.com', 'pass');
INSERT INTO Account VALUES('pass',321);

/*
	2) For this one, Java / non-SQL would be needed in order to compare the results. The user's input would be checked against the database and 
    would return a T/F for a boolean...
*/

	SELECT * FROM Account WHERE AccID=321;
    
/*
	3) User updated Pass etc
*/
	UPDATE Account
    SET Pass='123'
    WHERE AccID=321;
    
/*
	4) Searching for Books
*/
	SELECT *
    FROM Books
    WHERE ISBN=3445112;
/*
	5) Creating new shopping cart
*/
	INSERT INTO Shopping VALUES(5421, (SELECT CustomerName FROM Customer WHERE AccountID=123), 123, 'Cart', '2015-02-03', '2015-02-03');
    
/*
	6) Adding/Deleting Books from Cart
*/
	INSERT INTO BooksInCart VALUES(5421,1234);
    INSERT INTO BooksInCart VALUES(5423,1232);
    INSERT INTO BooksInCart VALUES(5424,12345);
    DELETE FROM BooksInCart WHERE CarID=5424 AND BooksISBN=12345;
/*
	7) Merging Carts
*/
	INSERT INTO Shopping VALUES(6421, (SELECT CustomerName FROM Customer WHERE AccountID=123), 'Cart', '2015-02-03', '2015-02-03');
	UPDATE BooksInCart
    SET CarID=6421
    WHERE CarID=5421 AND CarID=5424;
    
/*
	8)Changing into Orders - Need a program to sum up the cost of all the books. and then to create a new database.
*/
	INSERT INTO Orders VALUES(3, 123, '2019-09-09', 'S', 123.12, 'C', 'Testing');

/*
	9 and 10 would have to be done with an outside probram
*/

/*
	11) List book information
*/

	SELECT * FROM Books;
/*
	12) Get info of orders assigned to each employee
*/

	SELECT * FROM ManagedOrder JOIN Orders WHERE EmpID=982;
/*
	13) Update order status
*/
	UPDATE Orders SET OrderStatus='N' WHERE OrderID=3;
/*
	14) Insert new book
*/
INSERT INTO Books VALUES(344511,'Jane Smith', 'Death', 2, 2002, 'Non-Fiction', 'Penguin', 5, 12.92);
