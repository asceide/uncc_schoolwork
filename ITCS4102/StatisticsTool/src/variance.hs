module Variance (variance) where

import Data.List
import Average

--get length of the list to use in variance calculation
count::[Float] -> Float
count = fromIntegral . length

variance :: [Float] -> Float
variance list = (1 / count list) * (Prelude.sum $ map (\x -> (x - (avgList))^2) list)
    where avgList = average list
