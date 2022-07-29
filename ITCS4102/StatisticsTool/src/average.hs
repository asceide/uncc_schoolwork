module Average(
    average
)where

import Data.List

average :: [Float] -> Float
average list = realToFrac (sum list) / genericLength list


{- main = do
list1 = [1.0, 2.0] :: [Float]
let list1 = [1.0, 2.0, 3.0, 4.0, 5.0]
putStr(show  (typeOf list1)) -}