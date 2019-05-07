from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    w, h = len(b) + 1, len(a) + 1
    matrix = [[0 for x in range(w)] for y in range(h)]

    # Initialize the matrix
    matrix[0][0] = (0, None)
    for i in range(1, h):
        matrix[i][0] = (i, Operation.DELETED)
    for i in range(1, w):
        matrix[0][i] = (i, Operation.INSERTED)

    for i in range(1, h):
        for j in range(1, w):
            cost = []

            # Substitution cost
            if a[i - 1] == b[j - 1]:
                cost.append(matrix[i - 1][j - 1][0])
            else:
                cost.append(matrix[i - 1][j - 1][0] + 1)

            # Deletion cost
            cost.append(matrix[i - 1][j][0] + 1)

            # Insertion cost
            cost.append(matrix[i][j - 1][0] + 1)

            # Find the minimum cost
            index = cost.index(min(cost))
            if index == 0:
                matrix[i][j] = (cost[index],  Operation.SUBSTITUTED)
            elif index == 1:
                matrix[i][j] = (cost[index], Operation.DELETED)
            elif index == 2:
                matrix[i][j] = (cost[index], Operation.INSERTED)

    return matrix
