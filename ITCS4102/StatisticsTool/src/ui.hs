import Data.Char
import Control.Monad
import FileRead
import Average
import FrequencyMap
import MinMax
import Data.Typeable
import Median
import System.IO
import Data.Foldable (for_)
import Data.Map
import ZScore
import StdDev
import Variance
import Quartiles
import BestFitEquation
import GraphPlot


main = do
    putStrLn "-----Simple Statistics Calculator-----"
    putStrLn "What would you like to do?"
    putStrLn "a) Find the average."
    putStrLn "b) Find Min and Max."
    putStrLn "c) Find the Median."
    putStrLn "d) Find the Frequency List."
    putStrLn "e) Find the Variance."
    putStrLn "f) Find the Standard Deviation."
    putStrLn "g) Find the Z-score."
    putStrLn "h) Find the First Quartile."
    putStrLn "i) Find the Third Quartile."
    putStrLn "j) Find the Inner Quartile Range."
    putStrLn "k) Find the Line of Best Fit."
    putStrLn "l) Draw a Scatter Plot."
    putStrLn "m) Add to the data set."
    putStrLn "-) Quit the program."

    choice <- getChar
    clear <- getChar --this is janky please don't delete
    let upperChoice = toUpper choice

    d<-getData "statsnumbers.txt"
    let p = [(1.0, 2.0), (2.0, 4.0), (3.0, 8.0), (6.0, 10.0), (7.0, 13.0), (10.0, 11.0)]

    if upperChoice == 'A'
        then do
            putStrLn "Finding the average..."
            putStrLn(show(average d))
            main
    else if upperChoice == 'B'
        then do
            putStrLn "Finding the min and max..."
            putStrLn("min " ++show(getMin d))
            putStrLn("max " ++show(getMax d))
            main
    else if upperChoice == 'C'
        then do
            putStrLn "Finding the median..."
            putStrLn(show(median d))
            main
    else if upperChoice == 'D'
        then do
            putStrLn "Finding the frequency map..."
            for_ (toList (freqMap d)) $ \(q,a) -> putStrLn(show(q) ++ " " ++ show(a))
            main
    else if upperChoice == 'E'
        then do
            putStrLn(show (variance d))
            main
    else if upperChoice == 'F'
        then do
            putStrLn(show (stdDev d))
            main
    else if upperChoice == 'G'
        then do
            putStrLn "Enter the raw score for which you would like to find the Z-score: "
            rawScore <- readLn
            putStrLn "Finding the Z-score..."
            putStrLn(show( zscore (rawScore :: Float) (average d) (stdDev d)))
            main
    else if upperChoice == 'H'
        then do
            putStrLn (show (quartile1 d))
            main
    else if upperChoice == 'I'
        then do
            putStrLn (show (quartile3 d))
            main
    else if upperChoice == 'J'
        then do
            putStrLn (show (innerQuartile d))
            main
    else if upperChoice == 'K'
        then do
            putStr("Slope, Intercept: ")
            putStrLn(show (bestFitEquation p))
            main
    else if upperChoice == 'L'
        then do
            testSP
            putStrLn("Plot drawn in output.png")
            main
    else if upperChoice == 'M'
        then do
            putStrLn "Enter the number: "
            number <- getLine
            appendFile "statsNumbers.txt" ("\n" ++ number)
            main
    else if upperChoice == '-'
        then do
            putStrLn "The program has ended."
    else putStrLn "Invalid input. Ending program."
;
