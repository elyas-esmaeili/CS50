import crypt
import sys

flag = 1

def main():
    if len(sys.argv) != 2:
        print("Usage: python crack hash")
        sys.exit(1)

    hashed = sys.argv[1]
    createdPass = ""

    # This loop is use for create permutaion of password
    # i = 0 for one character password and so forth
    for i in range(5):
        if (permutaion(createdPass, 0, i + 1, hashed) == 0):
            break

    sys.exit(0)


def permutaion(createdPass, k, n, hashed):
    """
        createdPass : For guessed password
        k : Is used for create different permutaion
        n : Number of character in the password
        hash = hash that send with command line argument
    """

    global flag
    if k == n:
        newHash = crypt.crypt(createdPass, hashed[:2])

        if newHash == hashed:
            print(f"{createdPass}")
            flag = 0

    else:
        for i in range(ord("A"), ord("z") + 1):
            if flag == 0:
                return flag

            if i == ord("Z"):
                i = ord("a")

            createdPass = createdPass[:k] + chr(i)
            permutaion(createdPass, k + 1, n, hashed)

    return flag


if __name__ == "__main__":
    main()