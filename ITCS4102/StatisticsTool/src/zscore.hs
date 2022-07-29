module ZScore (zscore) where

--This function calculates the Z-score using the formula: (raw score - average) / standard deviation.
--The user is prompted for the raw score in main. 
--The average and standard deviation functions are called in main.

zscore :: Float -> Float -> Float -> Float
zscore x d q = (x - d) / q 
