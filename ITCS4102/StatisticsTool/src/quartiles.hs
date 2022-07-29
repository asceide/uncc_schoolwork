module Quartiles(
    quartile1, quartile3, innerQuartile
)where

import Data.List

{- sort list
list / 4
quartile 1 and 3 (2 is median) -}

quartile1 :: [Float] -> Float
quartile1 list = sortedList!!index
    where 
        sortedList = sort list
        index = ((genericLength sortedList) + 1) `div` 4

quartile3 :: [Float] -> Float
quartile3 list = sortedList!!index
    where 
        sortedList = sort list
        index = (((genericLength sortedList) + 1) `div` 4) * 3


innerQuartile :: [Float] -> Float
innerQuartile list = (quartile3 list) - (quartile1 list)