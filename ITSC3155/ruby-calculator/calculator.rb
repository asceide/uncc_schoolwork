#This is just a simple ruby calculator trying to get practice with ruby. I will TRY to implement UI, but there aren't 
#any guarantees
quitFlag=false
#  There are several types of loop.  If you want to do a conditional statement, then you can use a while/do while (which doesn't need explaining)
# Until is basically the opposite of the conditional while loop,. While x is y, do. Vs Until  x is y, do.
# For loop is basically For element in array do x. However, there is a better way to iterate through loops in Ruby
# Using .each, you can loop through each element of the array (done by x.each do y). If you need indexing, use  x.each_with_index do |element, index| y
# All loop statements must end in an end statement, as is the case with if statements.
until quitFlag==true
    puts "Simple Calculator v.01.043020\n"
    puts "Do you want to quit? (Y\\N) "
    gets.chomp=="Y" ? (quitFlag=true ) : (puts "Yes!") #When getting user input, gets also includes a newline character (\n). If you want to get this comparison correct, you have to call gets with chomp to remove the newline character
end