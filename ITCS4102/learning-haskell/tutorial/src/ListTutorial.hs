module ListTutorial where

--Lists are homogenous data structures in Haskell, meaning that all of its contents must be of the same data type.

lostNumbers = [4,17,23,45,66]
foundNumbers = [5,18,24,47,67]

--You can attach two lists together using ++ prefix operator. The ++ has to go through the whole list on the left side of the operator. Thu, for bigger lists it is more useful to use the : cons operator if you're putting something at the start of a list. For example 
lostNumbers ++ foundNumbers -> [4,17,23,45,66,5,18,24,47,67]

-- : takes a number (on the left) or a list, and a list (on the right). On the other hand, ++ takes two lists. Even if you want to add just one number, if you're using ++ you must enclose that number in square brackets in order to make it into a list
'A' : 'Small cat' -> 'A Small Cat'
4: [1,2,3] -> [4,1,2,3]

--Lists can contain lists. An empty list is declared as []. A list of lists is [[],[]].!!
--To remove an element from a list by index, !! is used
--for example lostNumbers !!
lostNumbers !! 2 --would remove the element at index 2


--Lists can be compared using <, <=, >, >=, ==. They are compared in lexicographical order. First they compare the heads of the array. ONLY IF the heads are equal does it check the second element. If they aren't, then the expression returns true or false.!!
--This doesn't include == however.!!
[3,2,1] >= [3,5,7] --would return false
[3,2,1]<[5,2,1] --would return true

--Some more list functions:
head [5,4,3,2,1] --head returns the first number of the array, 5
tail [5,4,3,2,1] --returns the array with out the head, [4.3.2.1] 
last[5,4,3,2,1] --returns the last number in the array, 1
init[5,4,3,2,1] --returns the array without the last number, [5,4,3,2]
length[5,4,3,2,1] --returns the length of the array, 5
null [5,4,3,2,1]
null []
lostNumbers == [] --returns if the list is empty or not. If you have a named list, use xs == [] to see if a list is empty
reverse [5,4,3,2,1] --reverses the list, [1,2,3,4,5]
take 3 [5,4,3,2,1] --take has the parameters of a number n and a list, and extracts n numer of elements from the beginning of the list. take 3 [5,4,3,2,1] returns [5,4,3]
drop 3 [5,4,3,2,1] --drop works similarly to take, but instead removes the first n numbers from the list. drop 3 [5,4,3,2,1] returns [2,1]
min[5,3,1] --returns minimum number, 1
max[5,3,1] --returns maximum number, 5
sum[5,3,1] --returns the sum of the list, 9
product[5,3,1] --returns the product of the list, 15
3 `elem` [5,4,3,2,1] --`elem` is a prefix operator that takes in a number and a list and checks if that number is in that list, returns true or false.
-- `elem` is a prefix operator that take in a parameter and list and checks if that parameter is an element of the list. Ex. 4 `elem` [1,2,3,4,5] -> true

--RANGES
--You can use ranges with Haskell, I.E. 
[1..20]--will return a list containing all the numbers between 1 and 20
[A..Z]-- will return a list containing all the alphabet btwn A and Z
[K-Z] --from K to Z
--If you want to specify step, for example, you need to include a second parameter
[3,6..20] --includes all numbers that are multiples of 3
--However, they aren't that smart, so they aren't able to see the powers of 2 i.e. it cannot do [1,2,4,8...100], because you can only specify one step. and some sequences are ambiguous if only given the first couple of terms
--Floating point lists shouldn't also be used with ranges because the results can be odd.!!

--You can use ranges to making infinite lists. Since Haskell is lazy it won't evaluate the infinite list until you need it. For example, if you wanted the first 24 numbers in an infinite list of multiples of 13, you can use take 24 [13,26..]
--Functions that produce infinite lists: 
cycle [1,2,3] --Cycle takes a list and cycles an infinite list. For example, cycle [1,2,3] would return a list of [1,2,3,1,2,3,1,2,3,1,2,3...inf]. and cycle["LOL "] would return ["LOL LOL LOL"]. In order to slice the list with n numbers, use take
repeat 5 --makes an infinite list of the element specified by repeat. Again, use take to slice the list.
replicate 3 10 --Makes a list of 3 that repeats 10. Easier to use than repeat

--LIST COMPREHENSION 
--A certain set has the following type of function: S={x*2 | x ∈ N , X<=10} ; the part behind the pipe (|) is the function, x is the variable, N is the number set, and x<=10 is the predicate
--In Haskell, the above function could be written as take 10 [2,4,..] . But if you wanted to apply a complex function applied on a set, a list comprehension can be used.
 -- For example
 [x*2 | x <- [1..10]] --Is the equivalent of the set function above. the function before the function is as is . the list is the pool of numbers that x is gotten from, so is the equivalent of the x ∈ N , X<=10 and x<- is putting the numbers from the list into x
 [x*2 | x<-[1..10], x*2>=10] --Predicates (which are used to filter) are placed after the binding part and seperated by a comma. In this case the output is going to be numbers from the lists,multiplied by 2, that are greater than 10. 
 [x | x<-[50..100], x `mod` 7 == 3] --Numbers between 50 and 100 where the reminander after being divided by 7 is 3. 
boomBangs [7..13]
boomBangs xs = [ if x < 10 then "BOOM!" else "BANG!" | x <- xs, odd x]   --This list comprehension will take in a list xs and check if its odd via odd x . Afterwards, if it is an odd number and less than 10, it will output BOOM, otherwise it will output bang.
[ x | x <- [10..20], x /= 13, x /= 15, x /= 19]   --Multiple predicates can also be used. In this case, numbners between 10 and 20 that aren't 13, 15, or 19 are shown
[ x*y | x <- [2,5,10], y <- [8,10,11]]   --Multiple lists can be used. When using several lists, comprehensions produce every combination from the lists supplied and join them via the output function. In this case, every product from the two lists are outputted (with the length being 9 elements)
[ x*y | x <- [2,5,10], y <- [8,10,11], x*y > 50]  --Using predicates, this will show the combinations from the list that also are greater than 50
nouns = ["hobo","frog","pope"] 
adjectives = ["lazy","grouchy","scheming"]  
[adjective ++ " " ++ noun | adjective <- adjectives, noun <- nouns]  --This will show every combination from the nouns and adjectives lists
length' xs = sum [1 | _ <- xs] --This is a user made length function. The _ means that the place the lists is being drawn from (a variable), is irrelevant. A _ is used instead. This function replaces all the elements of a function by 1 and adds it up. 
removeNonUppercase st = [ c | c <- st, c `elem` ['A'..'Z']]   --Strings are lists after all, and can be used with lists comprehensions. In this function, a String is passed and non uppercase letters are removed by the prediate c `elem` ['A..'Z'].
xxs = [[1,3,5,2,3,1,2,4,5],[1,2,3,4,5,6,7,8,9],[1,2,4,2,1,6,3,1,3,2,3,6]] 
[ [ x | x <- xs, even x ] | xs <- xxs]  --Nested list comprehensions is also possible when processing list of lists. In this case every odd number in each list inside the list of lists is removed. Its better to write these across several lines however.!!
[[x | x<-xs, even x] --Elements in list xs is checked to see if they are even
    | xs <- xxs] --List in list of lists xxs is passed to xs

--TUPLES
--A Tuple, much like in Python, Ruby and other languages, are a structure that has a finite amount of elements that can be heterogenous
--Tuples are produced with an outer bracket, and elements of the tuple inside parenthesis'. Like so.
[(1,2), (2, True), (3, 'A')]
--Tuples should be used when you know in advance how many elements each piece of data has.
--You cannot write a general function to append a tuple, instead you need to write one if a tuple is a double, trip, quad, etc. 
--Tuples can be compared with each other if their components can be compared (remember its Lexographical), however you cannot compare tuples of different sizes. 
--Tuple Functions
fst (8,11)
fst ("Wow", False) --fst takes a pair and return its first component, in this case being 8 and "Wow" respectively

snd(8,11)
snd("Wow", False) --snd is similar but does it for the second element

zip [1,2,3,4,5] [6,7,8,9] --zip is used to take two lists and make pairs of them. If a list is longer than the other, then the longer list gets cut after the length of the shorter one
zip [1,2,3,4,5] ["Hello", "Goodbye", "Hallo", "Auf Wiedersehen", "Tschuss"] --It can also be used to make tuples of two different lists
zip [1..]["Bonjour", "Au Revoir", "Konnichiwa", "Sayonara"] --It can also be used on infinite lists, since Haskell is lazy it will only evaluate what it needs to evaluate.

--Here is a problem combining tuples and list comprehensions. Which right triangle that has an integer for all sides and the sides are less then or equal to 10 have a perimeter length of 24? 
triangles = [ (a,b,c) | c <- [1..10], b <- [1..10], a <- [1..10] ]   --Here, all the sets of the triangles are made
rightTriangles = [ (a,b,c) | c <- [1..10], b <- [1..c], a <- [1..b], a^2 + b^2 == c^2] --Here there is a stipulation that the triangles must be a right triangle. So C<B<A, and using Pythagoreans Theorm to calculate if its a right triangle
rightTriangles' = [ (a,b,c) | c <- [1..10], b <- [1..c], a <- [1..b], a^2 + b^2 == c^2, a+b+c == 24] --Adding the condition that the perimeter must be 24. A ' is a valid component of a function name in Haskell.
