#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <map>
#include <tuple>
#include <utility>
#include <bitset>
#include <algorithm>

typedef uint64_t U64;

const U64 EMPTY_BITBOARD{ 0x00000000000000ULL };
const U64 WHITE_PAWNS_START_POS{ 0xFF000000000000ULL };
const U64 BLACK_PAWNS_START_POS{ 0xFF00ULL };
const U64 WHITE_KNIGHTS_START_POS{ 0x4200000000000000ULL };
const U64 BLACK_KNIGHTS_START_POS{ 0x42ULL };
const U64 WHITE_BISHOPS_START_POS{ 0x2400000000000000ULL };
const U64 BLACK_BISHOPS_START_POS{ 0x24ULL };
const U64 WHITE_ROOKS_START_POS{ 0x8100000000000000ULL };
const U64 BLACK_ROOKS_START_POS{ 0x81ULL };
const U64 WHITE_QUEEN_START_POS{ 0x800000000000000ULL };
const U64 BLACK_QUEEN_START_POS{ 0x8ULL };
const U64 WHITE_KING_START_POS{ 0x1000000000000000ULL };
const U64 BLACK_KING_START_POS{ 0x10ULL };
const U64 WHITE_PIECES_START_POS{ 0xFFFF000000000000ULL };
const U64 BLACK_PIECES_START_POS{ 0xFFFFULL };
const bool ACTIVE_COLOR_START_POS{ true };
const std::array<bool, 4> CASTLING_START_POS{ true, true, true, true };
const std::string EN_PASSANT_TARGET_START_POS{ "-" };
const int HALFMOVE_CLOCK_START_POS{ 0 };
const int FULLMOVE_NUMBER_START_POS{ 0 };

#define set_bit(b, i) ((b) |= (1ULL << i))
#define get_bit(b, i) ((b) & (1ULL << i))
#define clear_bit(b, i) ((b) &= ~(1ULL << i))

enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1
};

/*
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
*/

void replace_digits_with_zeros(std::string& fen);

// This is a struct containing the game data.
class GameData {
	// Bitboards for board data.
	U64 m_white_pawns {};
	U64 m_black_pawns {};
	U64 m_white_knights {};
	U64 m_black_knights {};
	U64 m_white_bishops {};
	U64 m_black_bishops {};
	U64 m_white_rooks {};
	U64 m_black_rooks {};
	U64 m_white_queens {};
	U64 m_black_queens {};
	U64 m_white_king {};
	U64 m_black_king {};
	U64 m_white_pieces {};
	U64 m_black_pieces {};
	// Values for additional data.
	bool m_active_color {};
	bool m_white_king_castling {};
	bool m_white_queen_castling {};
	bool m_black_king_castling {};
	bool m_black_queen_castling {};
	std::string m_en_passant_target {};
	int m_halfmove_clock {};
	int m_fullmove_number {};

public:
	GameData(U64 white_pawns, U64 black_pawns, U64 white_knights, U64 black_knights, U64 white_bishops, U64 black_bishops,
		U64 white_rooks, U64 black_rooks, U64 white_queens, U64 black_queens, U64 white_king, U64 black_king, U64 white_pieces,
		U64 black_pieces, bool active_color, std::array<bool, 4> castling_values, std::string en_passant_target, int halfmove_clock, int fullmove_number)
		: m_white_pawns { white_pawns }
		, m_black_pawns { black_pawns }
		, m_white_knights { white_knights }
		, m_black_knights { black_knights }
		, m_white_bishops { white_bishops }
		, m_black_bishops { black_bishops }
		, m_white_rooks { white_rooks }
		, m_black_rooks { black_rooks }
		, m_white_queens { white_queens }
		, m_black_queens { black_queens }
		, m_white_king { white_king }
		, m_black_king { black_king }
		, m_white_pieces { white_pieces }
		, m_black_pieces { black_pieces }
		, m_active_color { active_color }
		, m_white_king_castling { castling_values[0] }
		, m_white_queen_castling { castling_values[1] }
		, m_black_king_castling { castling_values[2] }
		, m_black_queen_castling { castling_values[3] }
		, m_en_passant_target { en_passant_target }
		, m_halfmove_clock { halfmove_clock }
		, m_fullmove_number{ fullmove_number }
	{
	}
	// Function setting the board position from the FEN-string via the bitboards.
	void set_board_position(std::string board) {
		// Map connecting FEN string values with pointers to particular bitboards.
		std::map<std::string, U64*> fen_bit{ {"P", &m_white_pawns }, { "N", &m_white_knights }, { "B", &m_white_bishops },
		{ "R", &m_white_rooks }, {"Q", &m_white_queens}, { "K", &m_white_king }, {"p", &m_black_pawns }, { "n", &m_black_knights },
		{ "b", &m_black_bishops }, { "r", &m_black_rooks }, {"q", &m_black_queens}, { "k", &m_black_king }
		};
		std::cout << "FEN string board only: " << board << '\n';
		// Looping through the modified (we replaced each number x with x zeros) board part of the FEN string.
		for (std::size_t i = 0; i < board.length(); i++) {
			// If the character is not 0, we need to set the bit in the bitboard.
			if (board[i] != '0') {
				// Create a string from a character to be able to use a map.
				std::string s(1, board[i]);
				// Create a pointer to a particular bitboard that we need, according to the letter. 
				U64* pBitboard = fen_bit[s];
				// Set that bit on that bitboard.
				set_bit(*pBitboard, i);
				// If the letter is uppercase, we also set a bit in the m_white_pieces bitboard.
				if (std::isupper(board[i]))
					set_bit(m_white_pieces, i);
				// If the letter is lowercase, we set a bit in the m_black_pieces bitboard.
				else
					set_bit(m_black_pieces, i);
			}
		}
	}

	// This function writes white pieces positions into FEN string.
	void append_m_white_pieces_to_fen(std::string& fen, std::size_t bit) {
		// Write the appropriate letter (white pieces) into the future FEN string.
		if (get_bit(m_white_pawns, bit))			fen[bit] = 'P';
		else if (get_bit(m_white_knights, bit))	fen[bit] = 'N';
		else if (get_bit(m_white_bishops, bit))	fen[bit] = 'B';
		else if (get_bit(m_white_rooks, bit))		fen[bit] = 'R';
		else if (get_bit(m_white_queens, bit))	fen[bit] = 'Q';
		else									fen[bit] = 'K';
	}

	// This function writes black pieces positions into FEN string.
	void append_m_black_pieces_to_fen(std::string& fen, std::size_t bit) {
		// Write the appropriate letter (black pieces) into the future FEN string.
		if (get_bit(m_black_pawns, bit))          fen[bit] = 'p';
		else if (get_bit(m_black_knights, bit))   fen[bit] = 'n';
		else if (get_bit(m_black_bishops, bit))   fen[bit] = 'b';
		else if (get_bit(m_black_rooks, bit))     fen[bit] = 'r';
		else if (get_bit(m_black_queens, bit))    fen[bit] = 'q';
		else									fen[bit] = 'k';
	}

	// This function writes pieces positions into FEN-to-be string.
	void append_pieces_to_fen(std::string& fen) {
		for (std::size_t bit = 0; bit < 64; bit++) {
			// Check bitboards for white and black pieces and call the function for the white/black accordingly.
			if (get_bit(m_white_pieces, bit))
				append_m_white_pieces_to_fen(fen, bit);
			else if (get_bit(m_black_pieces, bit))
				append_m_black_pieces_to_fen(fen, bit);
		}
	}

	//Is it ok to have this as a method? I think, logically it's a part of a class.
	// This function inserts slashes into the FEN string.
	void insert_slashes_to_fen(std::string& fen) {
		// Use a stringstream.
		std::stringstream ss;
		// Insert position zero outside the loop to prevent it from inserting a slash at the zero position.
		ss << fen[0];
		for (std::size_t i = 1; i < fen.length(); i++) {
			// If it's an eights position, insert a slash into the stringstream.
			if (i % 8 == 0) ss << '/';
			// Insert current letter into the stringstream.
			ss << fen[i];
		}
		// Assigning FEN string variable the value of the stringstream converted to a string.
		fen = ss.str();
	}

	// This function replaces zeros (that represent empty squares) with an appropriate number ("000" with 3 etc).
	void replace_zeros_with_digits(std::string& fen) {
		//Find first occurance of a zero in the string.
		std::size_t zero_found = fen.find_first_of("0");
		// Keep going until done.
		while (zero_found != std::string::npos) {
			std::size_t zero_idx = zero_found;
			// Check how many zeros in a row.
			while (fen[zero_idx] == '0') zero_idx++;
			// Index of a last zero minus index of a first found zero is the number of empty squares. 
			std::size_t num_empty_fields = zero_idx - zero_found;
			// Turn it into a string.
			std::string num_empty_fields_str = std::to_string(num_empty_fields);
			// Replacing zeros with a digit.
			fen.replace(zero_found, num_empty_fields, num_empty_fields_str);
			// Find next occurance of a zero.
			zero_found = fen.find_first_of("0", zero_found + 1);
		}
	}

	// This function appends all the other FEN data (except board position) to the FEN string.
	void append_other_data(std::string& fen) {
		fen.append(" ");
		std::map<bool, std::string> color_map{ {false, "b"}, {true, "w"} };
		fen.append(color_map[m_active_color]);
		fen.append(" ");
		if (m_white_king_castling == true)
			fen.append("K");
		if (m_white_queen_castling == true)
			fen.append("Q");
		if (m_black_king_castling == true)
			fen.append("k");
		if (m_black_queen_castling == true)
			fen.append("q");
		if (fen.back() == ' ')
			fen.append("-");
		fen.append(" ");
		if (m_en_passant_target == "")
			fen.append("-");
		else
			fen.append(m_en_passant_target);
		fen.append(" ");
		fen = fen + std::to_string(m_halfmove_clock);
		fen.append(" ");
		fen = fen + std::to_string(m_fullmove_number);
	}

	// This function writes all the data from the game object to the FEN string.
	void struct_to_fen() {
		// Create a string of 64 zeros for 64 positions on a board.
		std::string fen(64, '0');
		append_pieces_to_fen(fen);
		insert_slashes_to_fen(fen);
		// Now there are zeros left representing the empty fields. Turn those zeros into digits ("000" to 3 etc).
		replace_zeros_with_digits(fen);
		append_other_data(fen);
		//Need to add all other data (we already have that func.).
		std::cout << fen << std::endl;
	}

	// This function prints selected bitboard.
	// Functions for adding current board position to the FEN are going to use the same pattend (for every type of pieces).
	void print_bitboard(U64 bitboard) const {
		std::cout << "\n";
		for (int rank = 0; rank < 8; rank++) {
			for (int file = 0; file < 8; file++) {
				if (!file)
					std::cout << 8 - rank << ' ';
				int square = rank * 8 + file;
				// Print bit state (either 1 or 0).
				(get_bit(bitboard, square)) ? std::cout << 1 : std::cout << 0;
				std::cout << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << "  a b c d e f g h" << "\n";
		std::cout << "\n";
		std::cout << "Bitboard:  " << +bitboard << "\n";
		std::cout << "\n";
	}

	void print_the_board() const {
		U64 all_pieces = m_white_pieces | m_black_pieces;
		print_bitboard(all_pieces);
	}

};

// This function prints a human-readable ascii board representation.
//void print_board_ascii(FenData& game) {
//	std::map<int, std::string> fen_map{ {-4, "r"}, {-3, "b"}, {-2, "n"}, {-1, "p"}, {-5, "q"}, {-6, "k"},
//	{4, "R"}, {3, "B"}, {2, "N"}, {1, "P"}, {5, "Q"}, {6, "K"}, {0, "."} };
//	for (std::size_t i = 2; i < game.board.size() - 2; i++)
//	{
//		for (std::size_t j = 2; j < game.board[i].size() - 2; j++)
//		{
//			std::cout << fen_map[game.board[i][j]];
//		}
//		std::cout << std::endl;
//	}
//}

//FenData make_a_move(FenData& game, std::string& move) {
//	std::cout << move << '\n';
//	return game;
//}

// Is it possible to move the loop into a different func?
// This function returns black pawn moves.
//std::vector <std::pair<size_t, size_t>> get_black_pawn_moves (FenData& game) {
//	std::vector <std::pair<size_t, size_t>> black_pawn_moves = {};
//	for (std::size_t i = 2; i < game.board.size() - 2; i++)
//	{
//		for (std::size_t j = 2; j < game.board[i].size() - 2; j++)
//		{
//			if (i == 3 && game.board[i][j] == -1 && game.board[i + 2][j] == 0)
//				black_pawn_moves.push_back(std::make_pair(i + 2, j));
//			if (game.board[i][j] == -1 && game.board[i + 1][j] == 0)
//				black_pawn_moves.push_back(std::make_pair(i + 1, j));
//			if (game.board[i][j] == -1 && game.board[i + 1][j - 1] > 0 && game.board[i + 1][j - 1] < 7)
//				black_pawn_moves.push_back(std::make_pair(i + 1, j - 1));
//			if (game.board[i][j] == -1 && game.board[i + 1][j + 1] > 0 && game.board[i + 1][j + 1] < 7)
//				black_pawn_moves.push_back(std::make_pair(i + 1, j + 1));
//		}
//	}
//	return black_pawn_moves;
//}

// This function returns white pawn moves.
//std::vector <std::pair<size_t, size_t>> get_white_pawn_moves(FenData& game) {
//	std::vector <std::pair<size_t, size_t>> white_pawn_moves = {};
//	for (std::size_t i = 2; i < game.board.size() - 2; i++)
//	{
//		for (std::size_t j = 2; j < game.board[i].size() - 2; j++)
//		{
//			if (i == 8 && game.board[i][j] == 1 && game.board[i - 2][j] == 0)
//				white_pawn_moves.push_back(std::make_pair(i - 2, j));
//			if (game.board[i][j] == 1 && game.board[i - 1][j] == 0)
//				white_pawn_moves.push_back(std::make_pair(i - 1, j));
//			if (game.board[i][j] == 1 && game.board[i - 1][j - 1] > 0 && game.board[i - 1][j - 1] < 7)
//				white_pawn_moves.push_back(std::make_pair(i - 1, j - 1));
//			if (game.board[i][j] == 1 && game.board[i - 1][j + 1] > 0 && game.board[i - 1][j + 1] < 7)
//				white_pawn_moves.push_back(std::make_pair(i - 1, j + 1));
//		}
//	}
//	return white_pawn_moves;
//}

//This fuction replaces digits in the FEN with a number of zeros (1 with 1 zero, 2 - with 2 zeros, 3 - 3 zeros etc)
void replace_digits_with_zeros(std::string& fen) {
	//Find first occurance of digits in the FEN-string.
	std::size_t found = fen.find_first_of("0123456789");
	// Keep going until done.
	while (found != std::string::npos) {
		// This turns the digit char into an integer.
		int empty_squares_digit = fen[found] - '0';
		// Turning the integer into size_t to be able to create a string with it (this will be the length of the string).
		// (!!!!!) Need an error-check function here (!!!!!)
		// Check if it's positive.
		std::size_t zeros_length = static_cast<unsigned int>(empty_squares_digit);
		// Creating a string with appropriate number of zeros.
		std::string zeros(zeros_length, '0');
		// Replacing a digit with an appropriate number of zeros.
		fen.replace(found, 1, zeros);
		// Find next occurance of any of the digits.
		found = fen.find_first_of("0123456789", found + zeros_length);
	}
}


// This function should be part of the constructor.
// This method sets castling values from the FEN in the current game object.
std::array<bool, 4> get_castling_values(std::string& fen_castling, std::array<bool, 4> castling_values) {
	for (char& castling_char : fen_castling) {
		switch (castling_char) {
		case 'K':
			castling_values[0] = true;
			break;
		case 'k':
			castling_values[1] = true;
			break;
		case 'Q':
			castling_values[2] = true;
			break;
		case 'q':
			castling_values[3] = true;
			break;
		}
	}
	return castling_values;
}

// This function is going to extract data from the FEN.
std::tuple<std::string, std::array<bool, 4>, bool, std::string, int, int> extract_values_from_fen(std::string fen) {
	std::istringstream ssfen(fen);
	std::cout << fen << '\n';
	std::array<bool, 4> castling_values = { false, false, false, false };
	bool active_color{};
	std::string en_passant_target{};
	int halfmove_clock{};
	int fullmove_number{};
	std::string castling_string{};
	std::string skip{};
	std::string active_color_str{};
	ssfen >> skip >> active_color_str >> castling_string >> en_passant_target >> halfmove_clock >> fullmove_number;
	// Remove other values from the last element of rows array, leaving only the board data.
	std::map<std::string, bool> color_map{ {"b", false}, {"w", true} };
	active_color = color_map[active_color_str];
	castling_values = get_castling_values(castling_string, castling_values);
	std::string board = fen.substr(0, fen.find(" "));
	board.erase(std::remove(board.begin(), board.end(), '/'), board.end());
	replace_digits_with_zeros(board);
	return std::make_tuple(board, castling_values, active_color, en_passant_target, halfmove_clock, fullmove_number);
}

GameData create_game_object(std::string fen) {
	std::string board{};
	std::array<bool, 4> castling_values = { false, false, false, false };
	bool active_color{};
	std::string en_passant_target{};
	int halfmove_clock{};
	int fullmove_number{};
	tie(board, castling_values, active_color, en_passant_target, halfmove_clock, fullmove_number) = extract_values_from_fen(fen);
	U64 white_pawns = EMPTY_BITBOARD;
	U64 black_pawns = EMPTY_BITBOARD;
	U64 white_knights = EMPTY_BITBOARD;
	U64 black_knights = EMPTY_BITBOARD;
	U64 white_bishops = EMPTY_BITBOARD;
	U64 black_bishops = EMPTY_BITBOARD;
	U64 white_rooks = EMPTY_BITBOARD;
	U64 black_rooks = EMPTY_BITBOARD;
	U64 white_queens = EMPTY_BITBOARD;
	U64 black_queens = EMPTY_BITBOARD;
	U64 white_king = EMPTY_BITBOARD;
	U64 black_king = EMPTY_BITBOARD;
	U64 white_pieces = EMPTY_BITBOARD;
	U64 black_pieces = EMPTY_BITBOARD;
	GameData gameData{ white_pawns, black_pawns, white_knights, black_knights, white_bishops, black_bishops, white_rooks,
		black_rooks, white_queens, black_queens, white_king, black_king, white_pieces, black_pieces, active_color,
		castling_values, en_passant_target, halfmove_clock, fullmove_number };
	gameData.set_board_position(board);
	return gameData;
}


int main()
{
	std::string fen;
	std::cout << "Please, enter the FEN or the first move: ";
	std::getline(std::cin, fen);
	GameData gameData = create_game_object(fen);
	std::cout << "FEN is: " << fen << std::endl;
	std::cout << "All pieces:" << '\n';
	gameData.print_the_board();
	fen.erase(std::remove(fen.begin(), fen.end(), '/'), fen.end());
	replace_digits_with_zeros(fen);
	std::cout << "FEN with zeros: " << fen << std::endl;
	std::cout << "Current a8: " << fen[a8] << std::endl;
	std::cout << "Current b2: " << fen[b2] << std::endl;
	//std::vector <std::pair<size_t, size_t>> piece_moves = {};
	//print_board_ascii(game);
	//print_board_ascii(game);
	//piece_moves = get_black_pawn_moves(game); 
	//std::vector <std::pair<size_t, size_t>> white_pawn_moves = get_white_pawn_moves(game);
	//piece_moves.insert(std::end(piece_moves), std::begin(white_pawn_moves), std::end(white_pawn_moves));
	//for (std::pair<size_t, size_t> pawn_move : piece_moves)
	//	std::cout << pawn_move.first << pawn_move.second << std::endl;
	gameData.struct_to_fen();
	return 0;
}
