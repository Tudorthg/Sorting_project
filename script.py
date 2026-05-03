import random
import os
x = int(input("Enter number of files to generate: "))
A = []
for i in range(x):
    A.append(int(input("Number of elements in a file: ")))

folder = "C:/Users/Tudor/PyCharmMiscProject/Numbers"
os.makedirs(folder, exist_ok=True)

for i in range(x):
    filename = os.path.join(folder, f"numbers_{A[i]}.txt")
    with open(filename, "w") as f:
        for j in range(A[i]):
            f.write(str(random.randint(0, 100000)))
            f.write(" ")

os.startfile(folder)

