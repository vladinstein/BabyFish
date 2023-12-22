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
#include <random>
#include "game_class.h"

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

int main()
{
	std::string fen{};
	// U64 test{ ~uint64_t(0) };
	std::cout << "Please, enter the FEN or press enter to start the game from the beginning: ";
	std::getline(std::cin, fen);
	// Add "pl_color" value to the game class (1 - white, 0 - black);
	GameData gameData = fen.length() > 5 ? GameData::create_game_object_from_fen(fen) : GameData::create_game_object_start_pos();
	std::string pl_color{};
	std::cout << "Please, enter w to choose white and b to choose black pieces: ";
	std::getline(std::cin, pl_color);
	gameData.set_player_color(pl_color);
	std::cout << "All pieces:" << '\n';
	gameData.print_the_board();
	if (fen.length() > 5) {
		std::cout << "FEN is: " << fen << std::endl;
	}
	gameData.game_loop();
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
