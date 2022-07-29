module FrequencyMap(
    freqMap
)where

import qualified Data.Map as Map

--Takes list of real fracs and returns a frequency map of the data

freqMap :: (RealFrac a,Num b)=>[a]->Map.Map a b
freqMap []    = error "freqMap: empty list"
freqMap x=foldl (\map key -> Map.insertWith (+) key 1 map) Map.empty x
