CREATE DATABASE BOOKSTORE;

USE BOOKSTORE;
/*The following statements will create the individual tables for the database
*/
CREATE TABLE Customer
(AccountID DECIMAL(5,0),
CustomerName CHAR(35) NOT NULL,
PhoneNum DECIMAL(10,0),
Email CHAR(35),
Pass CHAR(35),
CONSTRAINT pk_AccountID PRIMARY KEY(AccountID));

CREATE TABLE Account
(Pass CHAR(35),
AccID DECIMAL(5,0),
CONSTRAINT fk_AccID FOREIGN KEY(AccID) REFERENCES Customer(AccountID));


CREATE TABLE Books
(ISBN DECIMAL(13,0),
AuthorName Char(35) NOT NULL,
Title Char(35) NOT NULL,
Edition DECIMAL(2,0),
BookYear DECIMAL(4,0),
Category CHAR(20),
Publisher CHAR(35) NOT NULL,
StockQuan DECIMAL(5,0),
Price DECIMAL(5,2),
CONSTRAINT pk_ISBN PRIMARY KEY(ISBN));


CREATE TABLE Orders
(OrderID DECIMAL(10,0),
AcctID DECIMAL(5,0),
OrderDate DATE,
OrderStatus CHAR(3),
TotalPrice DECIMAL(6,2),
ShippingAddress CHAR(35),
PaymentMethod CHAR(2),
BillAddress CHAR(35),
CONSTRAINT pk_OrderID PRIMARY KEY(OrderID),
CONSTRAINT fk_AcctID FOREIGN KEY(AcctID) REFERENCES Customer(AccountID));

CREATE TABLE Ordered(
ISBN DECIMAL(23,0),
OrID DECIMAL(10,0),
CONSTRAINT fk_OrID FOREIGN KEY(OrID) REFERENCES Orders(OrderID));


CREATE TABLE CreditCard
(CardID CHAR(15),
AcID DECIMAL(5,0),
CardNum DECIMAL(20,0),
NameOnCard CHAR(35) NOT NULL,
CVV DECIMAL(3,0),
CONSTRAINT pk_CardID PRIMARY KEY (CardID),
CONSTRAINT fk_AcID FOREIGN KEY(AcID) REFERENCES Customer(AccountID));



CREATE TABLE Shopping(
CartID DECIMAL(10,0),
CustoNa CHAR(35),
AccouID DECIMAL(5,0),
CartName CHAR(35),
DateCreated DATE,
DateUpdated DATE,
CONSTRAINT pk_CartID PRIMARY KEY (CartID),
CONSTRAINT fk_AccouID FOREIGN KEY (AccouID) REFERENCES Customer(AccountID));

CREATE TABLE BooksInCart(
CarID DECIMAL(10,0),
BooksISBN DECIMAL(23,0),
CONSTRAINT fk_CarID FOREIGN KEY(CarID) REFERENCES Shopping(CartID));

CREATE TABLE Employee(
EmployeeID DECIMAL (23,0),
EmpName CHAR(35),
EmpAddress CHAR(35),
EmpSalary DECIMAL(23,2),
Manager BOOLEAN,
CONSTRAINT pk_EmployeeID PRIMARY KEY (EmployeeID));

CREATE TABLE ManagedOrder(
OrdID DECIMAL (23,0),
EmpID DECIMAL (23,0),
CONSTRAINT fk_EmpID FOREIGN KEY(EmpID) REFERENCES Employee(EmployeeID));


/*The following statements will populate the database initially
*/
INSERT INTO Customer VALUES(123,'John Smith', 12345678, 'abc@def.com','test');
INSERT INTO Account VALUES('test',123);

INSERT INTO Books VALUES(3445112,'Jane Smith', 'Life', 1, 2000, 'Non-Fiction', 'Penguin', 5, 12.92);
INSERT INTO Orders VALUES(456, '123', '2016-03-07', 'S', 123.12, '1234 Main St', 'D', '1234 Main St');
INSERT INTO Ordered VALUES(3445112, 456);
INSERT INTO CreditCard VALUES(12345, 123, 1234556, 'John D. Smith', 123);
INSERT INTO Shopping VALUES(543, 'John Smith', 123, 'Jan 7 Cart', '2016-12-06', '2016-12-07');
INSERT INTO BooksInCart VALUES(543,12321);
INSERT INTO Employee VALUES(982, 'Gretchen Koenigberg', '987 Kikoe Road', 421.22, 0);
INSERT INTO ManagedOrder VALUES(456,982);
