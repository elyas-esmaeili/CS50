from cs50 import get_int

cardNumber = get_int("Number: ")

totalSum = cardId = counter = 0

while cardNumber != 0:

    if cardNumber < 100 and cardNumber > 9:
        cardId = cardNumber
    elif cardNumber == 4:
        cardId = 4

    rem = cardNumber % 10
    cardNumber //= 10

    if (counter % 2) != 0:
        rem *= 2
        rem = (rem % 10) + (rem // 10)

    totalSum += rem
    counter += 1

if (totalSum % 10) == 0:

    if cardId == 4:
        if counter == 13 or counter == 16:
            print("VISA")
        else:
            print("INVALID")

    elif cardId == 34 or cardId == 37:
        if counter == 15:
            print("AMEX")
        else:
            print("INVALID")

    elif (cardId == 51 or cardId == 52 or
          cardId == 53 or cardId == 54 or
          cardId == 55):
        if counter == 16:
            print("MASTERCARD")
    else:
        print("INVALID")

else:
    print("INVALID")