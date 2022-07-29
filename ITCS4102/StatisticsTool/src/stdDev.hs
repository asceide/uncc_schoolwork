module StdDev(
    stdDev
)where

import Variance

--stdDev is the square root of variance (v)
--functions to calculate variance are called in main

stdDev :: [Float] -> Float
stdDev = sqrt . variance