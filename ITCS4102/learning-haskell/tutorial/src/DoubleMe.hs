module DoubleMe where

--Functions are defined as follows: function name, parameters, and function work
--Functions are declared with the function name followed by function parameters (which are preceded and proceded with a space), an = sign that is followed by function work
-- + - * / % etc, are all functions bascially. Specifically, they're called infix functions, as they are sandwiched between two parameters. Most of the time functions are prefix functions, as they are only followed by parameters.!!!

doubleMe x = x + x --Doubling a number
doubleUs x y = doubleMe x + doubleMe y

--In Haskell, if statements have a mandatory else, compared to other languages...!! 
--More over, if statements and indeed every expression and function must return something.
--This function doubles the number if its less than 100, otherwise it returns x
doubleSmallNumber x = if x > 100
                                     then x
                                     else x*2


