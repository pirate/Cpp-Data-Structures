#a fundamental understanding of programming:

# 9!

# is the same as

product = 1
for x in list:
    product = product*x

# but expressed in a different way is this:

1*2*3*4*5*6*7*8*9 == 9(8(7(6(5(4(3(2(1)))))))) == 1(2(3(4(5(6(7(8(9))))))))

#which is the essential piece of understanding recursive programming

#because all you have to do is generate a function that does this

1(2(3(4(5(6(7(8(9))))))))

def function(x,y=1):
    return x*y

function(9,function(8, function(7, function(6, function(5, function(4, function(3, function(2)))))
# which we already know is equal to
function(2, function(3, function(4, function(5, function(6, function(7, function(8, function(9))))


