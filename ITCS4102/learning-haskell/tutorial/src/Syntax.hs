module Syntax where

--PATTERN MATCHING - Specifying patterns to which some data should conform to and then checking to see if it does, while deconstructiing the data according to the patterns
--For example:
lucky :: (Integral a) => a -> String --This is function lucky that has a type of Integer a and returns a String. A is being type restricted to just Integers.
lucky 7 = "LUCKY NUMBER SEVEN"
lucky x = "Sorry you're out of luck pal!" --The function evaluates as so: It gets the number and checks from top to bottom. If it matches a pattern it stops there and gives the output( in this case 7). Otherwise, any other number would match the pattern at lucky x
--Or for example, if you wanted a function that says the number between 1 and 5 and nothing otherwise
    sayMe :: (Integral a) => a -> String  
    sayMe 1 = "One!"  
    sayMe 2 = "Two!"  
    sayMe 3 = "Three!"  
    sayMe 4 = "Four!"  
    sayMe 5 = "Five!"  
    sayMe x = "Not between 1 and 5"   --If sayMe x were to be moved to before sayMe 1, the function would always say Not Between 1 and 5 because the pattern matching goes from top to bottom

--For example, using recursion
    factorial :: (Integral a) => a -> a  
    factorial 0 = 1  
    factorial n = n * factorial (n - 1)  
--If factorial n=n*factorial(n-1) was above the first line (factorial 0=1), then the recursion would never end.!

--Pattern matching may also fail, like so: 
    charName :: Char -> String  
    charName 'a' = "Albert"  
    charName 'b' = "Broseph"  
    charName 'c' = "Cecil"  
    --If you were to input anything other than that, then the program would tell you that the pattern matching is non-exhaustive. As a result, you should always include a catch all with a variable like x

--Pattern Matchin can be used on tuples:
--Without Pattern Matching
addVectors :: (Num a) => (a, a) -> (a, a) -> (a, a)  
addVectors a b = (fst a + fst b, snd a + snd b) 
--With pattern matching
addVectors :: (Num a) => (a, a) -> (a, a) -> (a, a)  
addVectors (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)  
--Notice how instead of a and b and using the fst and snd function, a pattern is laid out and you can avoid having to call those other functions in order to make the calculation with the tuple. 
    first :: (a, b, c) -> a  
    first (x, _, _) = x  
      
    second :: (a, b, c) -> b  
    second (_, y, _) = y  
      
    third :: (a, b, c) -> c  
    third (_, _, z) = z  
--Since there aren't any functions for triples, you can use pattern matching to get the variable for the third in a tuple, you can also do the same with fourth, firth, etc 
--Pattern Matching can also be done with list comprehentions
xs = [(1,3), (4,3), (2,4), (5,3), (5,6), (3,1)] 
[a+b | (a,b) <- xs]  --Would out put [4,7,6,8,11,4]
--If a match failed, it would just go to the next element. 

--Lists can also be used in pattern matching. You can match them with empty lists or patterns that involve : and the empty list. Remember that a list is basically 1:2:3:[] etc. an x:xs pattern would bind the head of a list to x and the rest of the list to xs. Even if the list were to be one element long, xs would just be an empty list []
--If you wanted to bind, for example, 3 elements to variables and the rest of the list to another variable, you can use x:y:z:xs to accomplish that. 
--Implementing a self made head function
    head' :: [a] -> a  
    head' [] = error "Can't call head on an empty list, dummy!"  
    head' (x:_) = x  --If you want to bind several variables, even if one of them is _, you have to enclose them in paranthesis

    tell :: (Show a) => [a] -> String  
    tell [] = "The list is empty"  
    tell (x:[]) = "The list has one element: " ++ show x  
    tell (x:y:[]) = "The list has two elements: " ++ show x ++ " and " ++ show y  
    tell (x:y:_) = "This list is long. The first two elements are: " ++ show x ++ " and " ++ show y   --x:[] and x:y:[] can be written as [x] and [x,y] because of the syntax sugar, but since x:y:_ matches any list greater than 2, it cannot be written in square brackets
   --Using lists, patternmatching and recursion
    length' :: (Num b) => [a] -> b  
    length' [] = 0  
    length' (_:xs) = 1 + length' xs  --Since we are binding xs to the rest of the list and removing _/the head, the recursion would keep going until xs is an empty list
    --or doing the same for sum
    sum' :: (Num a) => [a] -> a  
    sum' [] = 0  
    sum' (x:xs) = x + sum' xs  --Again, binding the head to x and the rest of the list to xs, and then doing recursion until the list is empty
--Suppose you want to call back the original list. This can be accomplished by setting name followed by a @ before the pattern like so
    capital :: String -> String  
    capital "" = "Empty string, whoops!"  
    capital all@(x:xs) = "The first letter of " ++ all ++ " is " ++ [x]  

--GUARDS
--Guards are a way to test if one or more values are true or false. 
    --Example
        bmiTell :: (RealFloat a) => a -> String  
        bmiTell bmi  
            | bmi <= 18.5 = "You're underweight, you emo, you!"  
            | bmi <= 25.0 = "You're supposedly normal. Pffft, I bet you're ugly!"  
            | bmi <= 30.0 = "You're fat! Lose some weight, fatty!"  
            | otherwise   = "You're a whale, congratulations!"  
    --This function shows the usage of Guards. Guards are pipes that follow a function name and parameters. The expression is evaluated. If it is true, the function body is done, otherwise it keeps going down the guards. It is similar to an if-else tree.
    --Speaking of otherwise, otherwise is the catch all for guards. It evaluates to true if all the other expressions are false. 
    --You can also use guards to get parameters and evaluate functions, as shown below by changing the parameters of bmiTell to weight and hight
        bmiTell :: (RealFloat a) => a -> a -> String  
        bmiTell weight height  
            | weight / height ^ 2 <= 18.5 = "You're underweight, you emo, you!"  
            | weight / height ^ 2 <= 25.0 = "You're supposedly normal. Pffft, I bet you're ugly!"  
            | weight / height ^ 2 <= 30.0 = "You're fat! Lose some weight, fatty!"  
            | otherwise                 = "You're a whale, congratulations!"  
--When using guards, the function name and parameters do not need an = sign after words. In fact it will cause an error if you do include it
--You can also use guards to create your own infix function like so
    --Example
        myCompare :: (Ord a) => a -> a -> Ordering  
        a `myCompare` b  
            | a > b     = GT  
            | a == b    = EQ  
            | otherwise = LT  
        --You can declare infix functions with ` ` and parameters on either side. The function definition on top is like normal, however. 
--Instead of repeating an expression over and over like in bmiTell, you can use where to define it. 
    --Example
        bmiTell :: (RealFloat a) => a -> a -> String  
         bmiTell weight height  
            | bmi <= 18.5 = "You're underweight, you emo, you!"  
            | bmi <= 25.0 = "You're supposedly normal. Pffft, I bet you're ugly!"  
            | bmi <= 30.0 = "You're fat! Lose some weight, fatty!"  
            | otherwise   = "You're a whale, congratulations!"  
            where bmi = weight / height ^ 2  
    --You can use where after the guards (with the same indentations) to define some sort of expression. 
    --The names declared in a function are only limited to that function in scope. Using the previous example you can also define stuff like 18.5, 25.5 etc with names
    --You can also use pattern matching, i.e. with
    ... 
      where bmi = weight / height ^ 2  
      (skinny, normal, fat) = (18.5, 25.0, 30.0) 
    --You can also evaluate functions inside of where blocks
        calcBmis :: (RealFloat a) => [(a, a)] -> [a]  
        calcBmis xs = [bmi w h | (w, h) <- xs]  
        where bmi weight height = weight / height ^ 2  
    --Where can also be nested
--LET BINDINGS
--Let bindings are really similar to Where bindings. Let bindings let you bind variables everywhere and are themselves expressions. However, they are local so they don't span across guards. As with any construct that uses names, pattern matching can also be used. 
    --Example
        cylinder :: (RealFloat a) => a -> a -> a  
        cylinder r h = 
            let sideArea = 2 * pi * r * h  
                 topArea = pi * r ^2  
             in  sideArea + 2 * topArea  
    --The essential form of let is let <bindings> in expression. The difference between let and where is that let bindings are expressions themselves, where as where bindings are constructs. As a result, you can place let statements anywhere, as so:
        --Example
        4 * (let a = 9 in a + 1) + 2  
        [let square x = x * x in (square 5, square 3, square 2)]  
        --If you want to declare bindings inline, you can use semicolons to sepearate them
        (let a = 100; b = 200; c = 300 in a*b*c, let foo="Hey "; bar = "there!" in foo ++ bar)
        (let (a,b,c) = (1,2,3) in a+b+c) * 100   --Tuple pattern matching
        --You can use them inside set comprehensions
        calcBmis :: (RealFloat a) => [(a, a)] -> [a]  
        calcBmis xs = [bmi | (w, h) <- xs, let bmi = w / h ^ 2]  
            calcBmis :: (RealFloat a) => [(a, a)] -> [a]  
          calcBmis xs = [bmi | (w, h) <- xs, let bmi = w / h ^ 2, bmi >= 25.0]  --Set comprehension with let. You cannot use bmi in the (w,h) <- xs part, but otherwise names defined after the pipe can be seen everywhere. This also means that you can ommit in when using set comprehensions with let
--CASE
--Case are similar to the ones in imperative languages and in if/else/then and let bindings. You can do pattern matching and expression evaluations. 
    head' :: [a] -> a  
    head' [] = error "No head for empty lists!"  
    head' (x:_) = x  

    head' :: [a] -> a  
    head' xs = case xs of [] -> error "No head for empty lists!"  
             (x:_) -> x  
    --Case follows a simple syntax : case expression of pattern -> result
    --Pattern matching on function parameters can only be done when defining functions. Cases can be used anywhere, as in: 
    describeList :: [a] -> String  
    describeList xs = "The list is " ++ case xs of [] -> "empty."  
                                                   [x] -> "a singleton list."   
                                                   xs -> "a longer list."  
    --or alterntively
    describeList :: [a] -> String  
    describeList xs = "The list is " ++ what xs  
        where what [] = "empty."  
              what [x] = "a singleton list."  
              what xs = "a longer list."  
