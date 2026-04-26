import pytest
import tictactoe as ttt


@pytest.fixture
def board():
    return ttt.initial_state()


@pytest.fixture
def mid_game_board(board):
    """
    A board with 3 plays.
    """
    board[0][0] = ttt.X
    board[1][1] = ttt.O
    board[1][0] = ttt.X


@pytest.fixture
def win_game_x(board):
    board[0][0] = ttt.X
    board[1][1] = ttt.X
    board[2][2] = ttt.X


@pytest.fixture
def win_game_o(board):
    board[0][2] = ttt.O
    board[1][1] = ttt.O
    board[2][0] = ttt.O


@pytest.fixture
def tied_board():
    return [[ttt.X, ttt.X, ttt.O], [ttt.O, ttt.X, ttt.X], [ttt.X, ttt.O, ttt.O]]


class TestTicTacToe:
    def test_should_return_player_X(self, board):
        """
        Initial state should return player X.
        """
        assert ttt.player(board) == ttt.X

    def test_mid_game_should_return_player_O(self, board, mid_game_board):
        assert ttt.player(board) == ttt.O

    def test_should_return_nine_possible_actions(self, board):
        """
        Initial state should return a set of actions with nine options.
        """
        assert len(ttt.actions(board)) == 9

    def test_mid_game_should_return_six_possible_actios(self, board, mid_game_board):
        """
        A board with 3 plays should return 6 possible actions
        """
        actions = ttt.actions(board)
        assert len(actions) == 6
        assert actions == {(0, 2), (0, 1), (1, 2), (2, 0), (2, 1), (2, 2)}

    @pytest.mark.parametrize(
        "action",
        [
            (3, 2),
            (-1, 2),
            (0, 3),
            (0, -1),
        ],
    )
    def test_invalid_moves(self, board, action):
        with pytest.raises(ValueError):
            ttt.check_action_validity(board, action)

    def test_checked_board_cell_must_throw(self, board):
        board[0][1] = ttt.X
        with pytest.raises(ValueError):
            ttt.check_action_validity(board, (0, 1))

    def test_result_should_return_a_new_board(self, board):
        _board_1 = ttt.result(board, (1, 1))
        expected_board = [
            [ttt.EMPTY, ttt.EMPTY, ttt.EMPTY],
            [ttt.EMPTY, ttt.X, ttt.EMPTY],
            [ttt.EMPTY, ttt.EMPTY, ttt.EMPTY],
        ]

        assert _board_1 == expected_board

        _board_2 = ttt.result(_board_1, (0, 0))
        expected_board[0][0] = ttt.O

        assert _board_2 == expected_board

        assert board is not _board_1
        assert _board_1 is not _board_2

    def test_winners_empty_board(self, board):
        assert ttt.winner(board) is None

    @pytest.mark.parametrize("player", [ttt.X, ttt.O])
    def test_winner_player_horizontal(self, board, player):
        board[0][0] = player
        board[0][1] = player
        board[0][2] = player

        assert ttt.winner(board) == player

    @pytest.mark.parametrize("player", [ttt.X, ttt.O])
    def test_winner_player_vertically(self, board, player):
        board[0][0] = player
        board[1][0] = player
        board[2][0] = player

        assert ttt.winner(board) == player

    @pytest.mark.parametrize("player", [ttt.X, ttt.O])
    def test_winner_player_primary_diagonal(self, board, player):
        board[0][0] = player
        board[1][1] = player
        board[2][2] = player

        assert ttt.winner(board) == player

    @pytest.mark.parametrize("player", [ttt.X, ttt.O])
    def test_winner_player_secondary_diagonal(self, board, player):
        board[0][2] = player
        board[1][1] = player
        board[2][0] = player

        assert ttt.winner(board) == player

    def test_tied_board_return_winner_none(self, tied_board):
        assert ttt.winner(tied_board) is None

    def test_terminal_true_on_tied_board(self, tied_board):
        assert ttt.terminal(tied_board)

    def test_terminal_false_on_empty(self, board):
        assert ttt.terminal(board) == False

    def test_terminal_true_on_win(self, board, win_game_x):
        assert ttt.terminal(board) == True

    def test_utility_zero_on_empty(self, board):
        assert ttt.utility(board) == 0

    def test_utility_zero_on_tie(self, tied_board):
        assert ttt.utility(tied_board) == 0

    def test_utility_zero_on_midgame(self, board, mid_game_board):
        assert ttt.utility(board) == 0

    def test_utility_on_X_win(self, board, win_game_x):
        assert ttt.utility(board) == 1

    def test_utility_on_O_win(self, board, win_game_o):
        assert ttt.utility(board) == -1

    def test_minimax_none_on_terminal(self, tied_board):
        assert ttt.minimax(tied_board) is None

    def test_minimax_blocks_three_in_a_row(self, board, mid_game_board):
        action = ttt.minimax(board)
        assert action == (2, 0)

    def test_minimax_hits_three_in_a_row(self):
        board = [
            [ttt.X, ttt.O, ttt.EMPTY],
            [ttt.O, ttt.EMPTY, ttt.EMPTY],
            [ttt.X, ttt.O, ttt.X],
        ]

        assert ttt.minimax(board) == (1, 1)
