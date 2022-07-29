module MinMax where
--Finds the minimum, gets a list of Floats and returns an Float
getMin :: [Float] -> Float --Maybe is used incase an error occurs
getMin [] = error "empty list" --If the list/head is empty, nothing is returned
getMin [x] = x --If it consists of just one number
getMin (x:xs) = mins x (getMin xs) --Takes the function and functor that takes x and function that takes xs and returns a functor that contains cs

mins :: Float -> Float -> Float --Function to compare the two numbers
mins a b
    | a > b = b
    | a < b = a
    | a == b = a

getMax :: [Float] -> Float --Same as above but to find the max.
getMax [] = error "empty list"
getMax [x] = x
getMax (x:xs) = maxi x (getMax xs)

maxi :: Float -> Float -> Float
maxi a b
    | a > b = a
    | a < b = b
    | a == b = a