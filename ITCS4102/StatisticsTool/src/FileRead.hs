module FileRead(
    getData
)where

import System.IO
import Data.List
import Data.Typeable
import Control.Monad
import qualified Data.Text as Text
import qualified Data.Text.IO as Text
import qualified System.IO.Strict as SIO
import Data.Functor

import Median




readFloat :: String -> Float
readFloat = read


getData:: String -> IO([Float])
getData fileName= do
    contents <- readFile fileName
    let x = map readFloat.words $ contents
    return x
    

{-main = do
    d<-getData "test.txt"
    putStr ("The Data is :"++show (d)++"\n")
    putStr("Median is "++show(median d)++"\n")
    putStr(show (typeOf d))
    writeData-}
