# Vigenere's algorithm

import sys
from cs50 import get_string

# Check if there exist two arguments
if len(sys.argv) != 2:
    print("You have to pass TWO arguments!")
    sys.exit(1)

# Get the key
key = sys.argv[1]
for c in key:
    if not c.isalpha():
        print("There is non-alphabetical character in the key!")
        sys.exit(1)

# Prompt user to enter plain text
plainText = get_string("plaintest: ")
chiperText = ""

# Iterate over each character in plain text
j = 0
for i in range(len(plainText)):
    char = plainText[i]

    if char.isalpha():
        j %= len(key)
        # Convert key's i's character to it's alphabetical index
        k = (ord(key[j]) - 65) if key[j].isupper() else (ord(key[j]) - 97)

        if char.islower():
            # ord("a") equals 97
            char = chr((((ord(plainText[i]) - 97) + k) % 26) + 97)
        else:
            # ord("A") equals 65
            char = chr((((ord(plainText[i]) - 65) + k) % 26) + 65)
        j += 1

    chiperText += char

print(f"ciphertext: {chiperText}")