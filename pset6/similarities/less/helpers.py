from bisect import bisect_left
from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Extract lines
    aLines = a.splitlines()
    bLines = b.splitlines()

    common = intersection(aLines, bLines)

    return common


def sentences(a, b):
    """Return sentences in both a and b"""

    aSents = sent_tokenize(a)
    bSents = sent_tokenize(b)

    common = intersection(aSents, bSents)

    return common


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    aSubstrings = createSubstrings(a, n)
    bSubstrings = createSubstrings(b, n)

    common = intersection(aSubstrings, bSubstrings)

    return common


def intersection(a, b):
    """Intersection of list a and b"""

    b.sort()
    common = []

    for i in a:
        if bsearch(b, i):
            if not bsearch(common, i):
                index = bisect_left(common, i)
                common.insert(index, i)

    return common


# Create substirngs of length n from string a
def createSubstrings(a, n):
    substringsList = []
    for i in range((len(a) - n) + 1):
        string = a[i:i + n]
        substringsList.append(string)

    return substringsList


def bsearch(a, x):
    """Binary search function"""
    i = bisect_left(a, x)
    if i != len(a) and a[i] == x:
        return True
    return False