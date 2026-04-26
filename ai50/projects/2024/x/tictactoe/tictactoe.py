"""
Tic Tac Toe Player
"""

import math

X = "X"
O = "O"
EMPTY = None

PLUS_INFINITY = float(math.inf)
MINUS_INFINITY = float(-math.inf)


def initial_state():
    """
    Returns starting state of the board.
    """
    return [[EMPTY, EMPTY, EMPTY], [EMPTY, EMPTY, EMPTY], [EMPTY, EMPTY, EMPTY]]


def player(board):
    """
    Returns player who has the next turn on a board.
    """
    play_count = 0

    # Count up how many actions was taken from empty board
    for row in board:
        for item in row:
            if item != EMPTY:
                play_count += 1

    # Return player X if it's even plays, otherwise O
    return X if play_count % 2 == 0 else O


def actions(board):
    """
    Returns set of all possible actions (i, j) available on the board.
    """
    actions = set()

    # Loops throught the matrix looking for EMPTY cells
    for i in range(0, 3):
        for j in range(0, 3):
            if board[i][j] == EMPTY:
                # Add the cell to the actions set
                actions.add((i, j))

    return actions


def check_action_validity(board, action):
    row, col = action[0], action[1]

    # Check if the index is safely inside bounds
    if not (0 <= row <= 2 and 0 <= col <= 2):
        raise ValueError

    # Check if the cell isn't already taken
    if board[row][col] != EMPTY:
        raise ValueError

    return


def result(board, action):
    """
    Returns the board that results from making move (i, j) on the board.
    """
    # Check the validity of the action
    check_action_validity(board, action)

    # Deep copy of the board using list comprehension
    board_deep_copy = [[item for item in row] for row in board]

    # Takes the action on copied board
    board_deep_copy[action[0]][action[1]] = player(board)
    return board_deep_copy


def winner(board):
    """
    Returns the winner of the game, if there is one.
    """
    # Initializes a sum array that will be used to count points.
    sum_list = [0 for _ in range(0, 8)]

    # lut table to avoid branching.
    points = {
        X: 1,
        O: -1,
        EMPTY: 0,
    }

    for idx in range(0, 9):
        # Calculate the row and colum indexes.
        row = math.floor(idx / 3)
        col = idx % 3

        cell_point = points[board[row][col]]

        # Calculate row wins.
        sum_list[row] += cell_point

        # Calculate coluns wins.
        sum_list[3 + col] += cell_point

        # Calculate the primary diagonal wins.
        if row == col:
            sum_list[7] += cell_point

        # Calculate the secondary diagonal wins.
        if row == abs(col - 2):
            sum_list[6] += cell_point

    # Calculate the results
    for sum in sum_list:
        # A sum of three points will lead to a win
        val = int(sum / 3)
        if val in (1, -1):
            return X if val == 1 else O

    return None


def terminal(board):
    """
    Returns True if game is over, False otherwise.
    """
    # Check if there is actions left and no one already won, false otherwise
    if len(actions(board)) == 0 or winner(board) is not None:
        return True
    return False


def utility(board):
    """
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    """
    result = {"X": 1, "O": -1}
    return result.get(str(winner(board)), 0)


alpha = MINUS_INFINITY
beta = PLUS_INFINITY


def min_value(board):
    global alpha
    if terminal(board):
        return utility(board)

    v = PLUS_INFINITY
    for action in actions(board):
        v = min(v, max_value(result(board, action)))
        if v < alpha:
            return alpha

    return v


def max_value(board):
    global beta
    if terminal(board):
        return utility(board)

    v = MINUS_INFINITY
    for action in actions(board):
        v = max(v, min_value(result(board, action)))
        if v > beta:
            return v

    return v


def minimize(board):
    global beta
    best_action = None
    beta = PLUS_INFINITY
    # Iterate looking for the action with the min value of maximized action values
    for action in actions(board):
        v = max_value(result(board, action))
        if v < beta:
            best_action = action
            beta = v

    return best_action


def maximize(board):
    global alpha
    best_action = None
    alpha = MINUS_INFINITY
    # Iterate looking for the action with the max value of minimized action values
    for action in actions(board):
        v = min_value(result(board, action))
        if v > alpha:
            best_action = action
            alpha = v

    return best_action


def minimax(board):
    """
    Returns the optimal action for the current player on the board.
    """
    global alpha
    global beta

    if terminal(board):
        return None

    alpha = MINUS_INFINITY
    beta = PLUS_INFINITY
    # Try to maximize if X and minimize if O
    return minimize(board) if player(board) == O else maximize(board)
