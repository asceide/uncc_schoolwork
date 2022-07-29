module TypeClassesTut where

--Haskell has a static type system, which means that the type of every expression is known at compile time. The program won't compile if you try to divide a float by a boolean, for example.
-- "::" is 'Has type of' 
--Functions also has types. It is good coding practice to specify the type of the function 
--Using the function from lists in removing lowercase letters from a list
    removeNonUppercase :: [Char] -> [Char]  --This means that the function removeNonUppercase takes in a parameter of a list of char (or string) and maps it to a list of char/string, or rather erturns a list of char/string
    removeNonUppercase st = [ c | c <- st, c `elem` ['A'..'Z']]   
    addThree :: Int -> Int -> Int -> Int  --In this example, addThree takes in 3 Ints and returns an Int. The parameters and return function are separated by ->. The return type is the last item
    addThree x y z = x + y + z  
--Different type of Data types
--Int: Integer Number, bounded by 2^32
--Integer: Integer number, unbounded
--Float
--Double
--Bool
--Char

--Haskell has a much more powerful version of generics. Function that have variable types declarations are called polymorphic functions. 
--for example
head :: [a] -> a --This function takes a list of any type and returns a list of that same type
fst :: (a, b) -> a  --This function takes a tuple of any type, and returns something of the first type in the tuple

--TYPECLASSES
--A type class is a sort of interface that defines a behavior. If a type is of a typeclass, that means it supports and implements the behavior that a typeclass describes.!!
--Examples
-- == is a function much like * + - / . If a function is comprised by special characters, then it is considered an infix function by default. In order to examine its type, pass it to another function, or call it as a prefix function, it must be surrounded by paranthesis. 
(==) :: (Eq a) => a -> a -> Bool -- Everything before => is read as a class constraint. It can be read as follows: function == has a type of : this function takes in two parameters of the same time and returns a boolean. The two values MUST be part of the Eq class (this is the class constraint). All Haskell type except for IO and functions are part of the Eq class
--Elem has a function type of (Eq a) => a -> [a] -> Bool because it uses == to check the values in the list.
--Typeclasses
--Eq is used for equality testing == is /= is not. If Eq is used as a type contraint, it is uses == or /= somewhere in its definition.!! 
--Ord is used for comparison functions using >, <. >=, <= . For a type to be in Ord, it must also be in the Eq typeclass
--Ordering returns GT, LT, EQ for Greater than, less than and equals to
--Show typeclass memebers can be presented as a String. All types except for functions can be part of the show typeclass
--Read takes in a String and returns a type that is the member of type read
read "8.2" + 3.8  --returns 12
show 3 --returns "3"
--If you were to use just read "4", an error would be thrown because it doesn't know what type we want to be returned.!!
--In order to fix this, you would use explicit type annotations to let the compiler know what type of value you want to be returned. This is done using again ::
read "5" :: Int  --Returns an integer 5
read "5" :: Float --Returns a float 5.0
(read "5" :: Float) * 4  --returns a float 5*4=20.0
read "[1,2,3,4]" :: [Int]   --returns a list of Ints
read "(3, 'a')" :: (Int, Char) --returns a tuple of int, char
--Enum are sequentially ordered types, meaning that they can be enumerated. They have predecessors and successors which can be obtained by pred and succ. This class contains (), Bool, Char, Ordering, Int, Integer, Float and Double
--Bounded are members with an upper and lower bound. Tuples are also bounded if its members are bounded.!!
--Num is the numeric typeclass, its member being able to behave as numbers. 
--Integral is a numeric typeclass whose members consist of Int and Integer
--Floating is the same but for floating numbers, Float and Double
--fromIntegral allows you to divide Integers and floating numbers. Using length as an example, you would use it as
fromIntegral (length [1,2,3,4]) + 3.2  --