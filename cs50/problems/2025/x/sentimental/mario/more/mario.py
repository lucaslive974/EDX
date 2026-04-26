from cs50 import get_int

value = -1

while(value < 1 or value > 8):
    value = get_int("Height: ")

for i in range(value + 1):
    if(i == 0):
        continue
    print(f"{" " * (value - i)}{"#" * i}  {"#" * i}")
