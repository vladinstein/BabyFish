#pragma once

#include <string>
#include <vector>
#include <array>

typedef uint64_t U64;

// This is a struct containing the game data.
class GameData {

	// Empty bitboard.
	static constexpr U64 EMPTY_BITBOARD{ 0x00000000000000ULL };
	static constexpr U64 A_FILE{ 0x0101010101010101ULL };
	static constexpr U64 A_B_FILES{ 0x0303030303030303ULL };
	static constexpr U64 H_FILE{ 0x8080808080808080ULL };
	static constexpr U64 G_H_FILES{ 0xC0C0C0C0C0C0C0C0ULL };
	static constexpr U64 RANK_8{ 0xFF00000000000000ULL };
	static constexpr U64 RANKS_7_8{ 0xFFFF000000000000ULL };
	static constexpr U64 RANK_1{ 0xFFULL };
	static constexpr U64 RANKS_1_2{ 0xFFFFULL };

	inline static const std::array<U64, 7> ALL_PIECES_EMPTY{ 0x00000000000000ULL, 0x00000000000000ULL, 0x00000000000000ULL,
	0x00000000000000ULL, 0x00000000000000ULL, 0x00000000000000ULL, 0xFFFFFFFFFFFFFFFFULL };

	// Starting bitboards.
	inline static const std::array<U64, 7> ALL_PIECES_START_POS{ 0xFF00000000FF00ULL, 0x4200000000000042ULL, 0x2400000000000024ULL,
	0x8100000000000081ULL, 0x800000000000008ULL, 0x1000000000000010ULL, 0xFFFFFFFFFFFFFFFFULL };
	static constexpr U64 COLOR_START_POS{ 0xFFFFULL };
	static constexpr U64 WHITE_PIECES_START_POS{ 0xFFFFULL };
	static constexpr U64 BLACK_PIECES_START_POS{ 0xFFFF000000000000ULL };

	// Other starting data.
	static constexpr bool ACTIVE_COLOR_START_POS{ true };								// true - white, false - black.
	inline static const std::array<bool, 4> CASTLING_START_POS{ true, true, true, true };
	inline static const std::string EN_PASSANT_TARGET_START_POS{ "-" };
	static constexpr int HALFMOVE_CLOCK_START_POS{ 0 };
	static constexpr int FULLMOVE_NUMBER_START_POS{ 0 };

	// Squares on the bitboard relative to current square.
	static constexpr int ONE_SQUARE_UP{ 8 };
	static constexpr int ONE_SQUARE_DOWN{ -8 };
	static constexpr int TWO_SQUARES_UP{ 16 };
	static constexpr int TWO_SQUARES_DOWN{ -16 };
	static constexpr int ONE_SQUARE_LEFT_ONE_UP{ 7 };
	static constexpr int ONE_SQUARE_RIGHT_ONE_UP{ 9 };
	static constexpr int ONE_SQUARE_LEFT_ONE_DOWN{ -9 };
	static constexpr int ONE_SQUARE_RIGHT_ONE_DOWN{ -7 };
	static constexpr int ONE_SQUARE_UP_TWO_LEFT{ 6 };
	static constexpr int ONE_SQUARE_UP_TWO_RIGHT{ 10 };
	static constexpr int ONE_SQUARE_DOWN_TWO_LEFT{ -10 };
	static constexpr int ONE_SQUARE_DOWN_TWO_RIGHT{ -6 };
	static constexpr int TWO_SQUARES_UP_ONE_LEFT{ 15 };
	static constexpr int TWO_SQUARES_UP_ONE_RIGHT{ 17 };
	static constexpr int TWO_SQUARES_DOWN_ONE_LEFT{ -17 };
	static constexpr int TWO_SQUARES_DOWN_ONE_RIGHT{ -15 };

	// Length of the part of the move string containing the coordinates of one square.
	static constexpr std::size_t LENGTH_ONE_SQUARE_COORDS{ 2 };

	static constexpr int LENGTH_IN_SQUARES_ONE_RANK{ 8 };
	static constexpr int ASCII_LOWER_CASE_A_INT{ 97 };
	static constexpr int ASCII_ONE_INT{ 49 };

	// Default for the player's color is zero-initialized.
	static constexpr bool PLAYER_COLOR_DEFAULT{};

	// Bitboards for board data.
	std::array <U64, 7> m_all_pieces_bitboards{};
	U64 m_color{};
	U64 m_white_pieces{};
	U64 m_black_pieces{};

	// Values for additional data.
	bool m_active_color{};
	bool m_white_king_castling{};
	bool m_white_queen_castling{};
	bool m_black_king_castling{};
	bool m_black_queen_castling{};
	std::string m_en_passant_target{};
	int m_halfmove_clock{};
	int m_fullmove_number{};

	// Player's pieces color (1 - White, 0 - black)
	bool m_player_color{};

public:
	// Add enum enumPiece??
	// Some parts will be removed !!!!!!!!!!!!!!!!!!!!!
	GameData(std::array<U64, 7> all_pieces_bitboards, U64 color, U64 white_pieces, U64 black_pieces, bool active_color,
		std::array<bool, 4> castling_values, std::string en_passant_target, int halfmove_clock, int fullmove_number,
		bool player_color);

	//This fuction replaces digits in the FEN with a number of zeros (1 with 1 zero, 2 - with 2 zeros, 3 - 3 zeros etc)
	static std::string replace_digits_with_zeros(std::string board);

	// This function returns castling values from the current FEN-string.
	static std::array<bool, 4> get_castling_values(std::string fen_castling, std::array<bool, 4> castling_values);

	// This function returns all the data from the FEN-string.
	static std::tuple<std::string, std::array<bool, 4>, bool, std::string, int, int> extract_values_from_fen(std::string fen,
		std::string board, std::array<bool, 4> castling_values, bool active_color, std::string en_passant_target,
		int halfmove_clock, int fullmove_number);

	// Function that creates game object from the FEN string.
	static GameData create_game_object_from_fen(std::string fen);

	// This function creates game object for the starting position.
	static GameData create_game_object_start_pos();

	// Function setting the board position from the FEN-string via the bitboards.
	void set_board_position(std::string board);

	// This function writes white pieces positions into FEN string.
	void append_m_white_pieces_to_fen(std::string& fen, std::size_t bit);

	// This function writes black pieces positions into FEN string.
	void append_m_black_pieces_to_fen(std::string& fen, std::size_t bit);

	// This function writes pieces positions into FEN-to-be string.
	void append_pieces_to_fen(std::string& fen);

	//Is it ok to have this as a method? I think, logically it's a part of a class.
	// This function inserts slashes into the FEN string.
	void insert_slashes_to_fen(std::string& fen);

	// This function replaces zeros (that represent empty squares) with an appropriate number ("000" with 3 etc).
	void replace_zeros_with_digits(std::string& fen);

	// This function appends all the other FEN data (except board position) to the FEN string.
	void append_other_data(std::string& fen);

	// This function writes all the data from the game object to the FEN string.
	void struct_to_fen();

	// This function prints selected bitboard.
	// Functions for adding current board position to the FEN are going to use the same pattend (for every type of pieces).
	void print_bitboard(U64 bitboard) const;

	// Function that prints all the pieces on the board.
	void print_the_board() const;

	// Function that prints different bitboards in the array.
	void print_bitboards() const;

	// This function splits the UCI move string into a vector of 2 or 3 separate strings (2 for each square and 1 for 
	// promotion piece type if needed.
	std::vector<std::string> split_move(std::string move);

	// This function takes each square coods. and returns the bit number of that square (for the bitboard).
	int string_to_bit(std::string square);

	// Function that gets bitboard that has a piece in a particular field.
	std::size_t get_bitboard(int move_from);

	// This function makes a move on the bitboards.
	void make_a_move_bitboards(int move_from, int move_to, std::size_t bitboard_number_from, std::size_t bitboard_number_to);

	// This function converts move string to 2 integers representing "move from" and "move to" positions on the bitboards.
	std::tuple<int, int> move_string_to_int(std::string move);

	// Function that returns white pawn moves.
	std::vector<int> get_white_pawn_moves(int move_from);

	// Function that returns black pawn moves.
	std::vector<int> get_black_pawn_moves(int move_from);

	//Function that returns knight's moves.
	std::vector<int> get_knights_moves(int move_from);

	//Function that returns bishop's moves.
	std::vector<int> get_bishops_moves(int move_from);

	// Function that returns all legit moves.
	std::vector<int> get_legit_moves(size_t bitboard_number_from, int move_from);

	// This function makes a move on all bitboards.
	void make_a_move(int move_from, int move_to);

	// This function sets player's pieces color
	void set_player_color(std::string pl_color);

	// This function make the player's move. It returns 0 if the correct move was entered, it returns 1 if 0 was entered to
	// stop the game. 
	int make_players_move(std::string move);

	// This function generates random legit move for computer.
	std::tuple <int, int> generate_random_move_comp();

	// Function that gets positions of all computer pieces.
	// !!!!!!!!!!!!!!!!!!!!! TEMPORARILY returns only pawns and knights.
	// !!!!!!!!!!!!!!!!!!!!!!
	std::vector<int> get_comp_square_numbers() const;

	// Function that gets positions of all player pieces and empty squares depending on what computer is playing.
	std::vector<int> get_pl_and_empty_square_numbers() const;

	// Function to get a random square from the vector of squares.
	int get_random_square(std::vector<int> squares_numbers);

	// This function represents a game loop.
	void game_loop();
};