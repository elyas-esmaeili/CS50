from cs50 import get_float

while True:
    f = get_float("Change owed: ")
    if f >= 0.0:
        break

f *= 100
amount = round(f)

numQuarters = amount // 25
amount %= 25

numDimes = amount // 10
amount %= 10

numNickels = amount // 5
amount %= 5

numPennies = amount // 1
amount %= 1

numCoins = numQuarters + numDimes + numNickels + numPennies;

print(numCoins)