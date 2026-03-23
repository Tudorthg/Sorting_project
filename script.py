import random
x = int(input("Input a number: "))
f1 = open("numbers.txt", 'w')
for i in range(0,x):
    f1.write(str(random.randint(1,10000000)))
    f1.write(" ")
f1.close()
