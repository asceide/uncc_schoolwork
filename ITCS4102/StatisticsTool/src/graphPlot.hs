module GraphPlot (testSP, drawScatterPlot, drawBarGraph, drawLineGraph) where

import Codec.Picture
import Data.HashSet hiding (null, map, filter, foldr)
import qualified Data.HashSet as Set
import MinMax
import qualified Data.Map as Map

scatterPlotPixelMapper yScale xScale points l x y
    | x2 < 0 = PixelRGB8 255 255 255
    | y2 < 0 = PixelRGB8 255 255 255
    | x2 > 280 = PixelRGB8 255 255 255
    | y2 > 280 = PixelRGB8 255 255 255
    | x2 == 0  = PixelRGB8 0 0 0
    | x2 == 280 = PixelRGB8 0 0 0
    | y2 == 0  = PixelRGB8 0 0 0
    | y2 == 280 = PixelRGB8 0 0 0
    | Set.member ((x1 `div` 7) * 7 ,(y1 `div` 7) * 7) points = PixelRGB8 186 33 9
    | Set.member (x1, y1) l =  PixelRGB8 28 189 13
    | otherwise = PixelRGB8 172 224 232
    where
      x1 = (xScale x)
      y1 = (yScale y)
      x2 = x1 `div` 3
      y2 = y1 `div` 3

lineGraphPixelMapper yScale xScale linePoints x y
    | x2 < 0 = PixelRGB8 255 255 255
    | y2 < 0 = PixelRGB8 255 255 255
    | x2 > 280 = PixelRGB8 255 255 255
    | y2 > 280 = PixelRGB8 255 255 255
    | x2 == 0  = PixelRGB8 0 0 0
    | x2 == 280 = PixelRGB8 0 0 0
    | y2 == 0  = PixelRGB8 0 0 0
    | y2 == 280 = PixelRGB8 0 0 0
    | Set.member (x1, y1) linePoints = PixelRGB8 186 33 9
    | otherwise = PixelRGB8 172 224 232
    where
      x1 = (xScale x)
      y1 = (yScale y)
      x2 = x1 `div` 3
      y2 = y1 `div` 3

barGraphPixelMapper yScale xScale lengths x y
    | x2 < 0 = PixelRGB8 255 255 255
    | y2 < 0 = PixelRGB8 255 255 255
    | x2 > 280 = PixelRGB8 255 255 255
    | y2 > 280 = PixelRGB8 255 255 255
    | x2 == 0  = PixelRGB8 0 0 0
    | x2 == 280 = PixelRGB8 0 0 0
    | y2 == 0  = PixelRGB8 0 0 0
    | y2 == 280 = PixelRGB8 0 0 0
    | (x1 `div` 20) < length(lengths) && (x1 `div` 10) `mod` 2 == 1 && lengths!!(x1 `div` 20)>y1 = PixelRGB8 186 33 9
    | otherwise = PixelRGB8 172 224 232
    where
      x1 = (xScale x)
      y1 = (yScale y)
      x2 = x1 `div` 3
      y2 = y1 `div` 3

--line

lineFromPoints point1 point2 = line a b point1 point2
                     where
                        a = fromIntegral(( snd point2) - (snd point1))/fromIntegral(( fst point2) - (fst point1))
                        b = fromIntegral((snd point2)) - (a * fromIntegral((fst point2)) )

line a b start end
    | a < 1.0 = (foldr (++) ([]) (map generateYs  [( fst start)..(fst end)]))
    | otherwise = (foldr (++) ([]) (map generateXs  [( snd start)..(snd end)]))
    where
      roundY x = round((fromIntegral(x)*a) + b)
      roundX y = round((fromIntegral(y)-b)/a)
      generateYs x = [( x , roundY(x)-1),(x , roundY(x)),( x , roundY(x)+1)]
      generateXs y = [( roundX(y)-1, y),( roundX(y), y),( roundX(y)+1, y)]

--test
testSP = writePng  outputFile ( generateImage (scatterPlotPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(s) bestFitLine) xSize ySize)
         where
            outputFile = "output.png"
            xSize = 900
            ySize = 900
            testData = ( [(0 ,20),(22,22),(66,67),(54,55),(75,79),(80,67),(100, 78)])
            s = Set.fromList(map (\(x , y)-> ((x `div` 7) * 7,(y `div` 7) * 7)) testData)
            bestFitLine = Set.fromList( (lineFromPoints (120, 120) (140, 140)) ++ (line 0.5 20 (0 ,0) (800,800)))

testLG = writePng  outputFile ( generateImage (lineGraphPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(testData)) xSize ySize)
         where
            outputFile = "output.png"
            xSize = 900
            ySize = 900
            d =  [(0 ,20),(22,22),(66,67),(54,55),(75,79),(130,280),(270, 400),(400,200)]
            testData = Set.unions((map (\i -> Set.fromList(lineFromPoints (d!!(i-1))(d!!i))) [1..(length(d)-1)]))


testBG = writePng  outputFile ( generateImage (barGraphPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(d)) xSize ySize)
         where
            outputFile = "output.png"
            xSize = 900
            ySize = 900
            d =  [20, 30, 40, 240, 55, 160, 153, 67, 345]


--Scaling function


scalePoints poits = map (\p -> (scaleX(fst p), scaleY(snd p))) poits
                   where
                      maxX = getMax(map (\p -> (fst p)) poits)
                      maxY = getMax(map (\p -> (snd p)) poits)
                      step = getMax([maxX,maxY])/800
                      scaleX = (\x -> round(x / step))
                      scaleY = (\y -> round(y / step))



scaleYintercept points y = round(y / step)
                   where
                      maxX = getMax(map (\p -> (fst p)) points)
                      maxY = getMax(map (\p -> (snd p)) points)
                      step = getMax([maxX,maxY])/800

--end functions
drawScatterPlot outputPath points = writePng  outputPath ( generateImage (scatterPlotPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(s) Set.empty ) xSize ySize)
                                  where
                                    s = Set.fromList(map (\(x , y)-> ((x `div` 7) * 7,(y `div` 7) * 7)) (scalePoints points))
                                    xSize = 900
                                    ySize = 900


drawBestfitLine outputPath points lineData = writePng  outputPath ( generateImage (scatterPlotPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(s) bestfitLine) xSize ySize)
                                      where
                                        s = Set.fromList(map (\(x , y)-> ((x `div` 7) * 7,(y `div` 7) * 7)) (scalePoints points))
                                        scaledY = scaleYintercept points (snd lineData)
                                        xSize = 900
                                        ySize = 900
                                        bestfitLine = Set.fromList(line (fst lineData) scaledY (0 ,0) (800,800))

drawLineGraph outputPath points = writePng  outputPath ( generateImage (lineGraphPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(pnts)) xSize ySize)
                                where
                                   xSize = 900
                                   ySize = 900
                                   d =  scalePoints points
                                   pnts = Set.unions((map (\i -> Set.fromList(lineFromPoints (d!!(i-1))(d!!i))) [1..(length(d)-1)]))


drawBarGraph outputPath freqmap = writePng  outputPath ( generateImage (barGraphPixelMapper (\y ->870-(y - 30))(\x ->(x - 30))(d)) xSize ySize)
                                where
                                   xSize = 900
                                   ySize = 900
                                   d =  map (\x -> snd x) (Map.toList(freqmap))
