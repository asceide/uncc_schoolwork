module BestFitEquation(bestFitEquation) where

import Data.List

bestFitEquation :: [(Float,Float)] -> (Float,Float)
bestFitEquation list = (m, b)
    where
        x = map (\p -> (fst p)) list
        y = map (\p -> (snd p)) list
        x2 = map (\p -> ((fst p)^2)) list
        xy = map (\p -> (fst p) * (snd p)) list
        n = genericLength list
        nxy = sum(xy) * n
        sumx = sum x
        sumy = sum y
        nx2 = n * (sum x2)
        sumxsumy = (sum x) * (sum y)
        sumxsquared = (sum x)^2
        sumx2 = sum x2
        m = (subtract nxy sumxsumy) / (subtract sumx2 sumxsquared)
        mx = m * (sum x)
        b = (subtract sumy mx) / n

        --m = subtract((n * (sum (xy)) (sum x) * (sum y))) `div` subtract((n * (sum (x^2)) (sum x)^2))
        --b = subtract((sum y) m*(sum x)) `div` n