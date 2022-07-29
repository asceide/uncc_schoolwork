module Median(
median
)where
-- Median function takes in list of Num and returns the median

median :: ( Real a, RealFrac a) => [a] -> a
median [] = error "median: empty list"
median [x]=x
median x
  |(length x) `mod` 2==1 = sx!!(((length x) `div` 2))
  |otherwise = (sx!!((((length x) `div` 2)))+(sx!!(((length x) `div` 2)-1))) / 2
  where sx= (mergeSort x)

-- Merge sort used by median

mergeSort :: ( RealFrac a)=>[a]-> [a]
mergeSort []    = error "mergeSort: empty list"
mergeSort [x]=[x]
mergeSort x= mergeSort (take  ( (length x)`div`2) x) `merge` mergeSort (drop ( (length x)`div`2) x)

-- Merge is a utility function for Merge Sort

merge :: ( RealFrac a)=>[a]->[a]->[a]
merge [] y = y
merge x [] = x
merge (x:xs) (y:ys)
  |y<x = [y] ++ merge (ys) ([x]++xs)
  |x<y = [x] ++ merge (xs)([y]++ys)
  |otherwise =[x,y] ++ merge (xs)(ys)
