#include "board.cpp"
#include "engine.cpp"
#include "evaluation.cpp"
#include "game_tree.cpp"
#include "move.cpp"
#include "position.cpp"

void interactive() {
    while (1) {
        std::string input;
        std::cout << "Input FEN: ";

        std::getline(std::cin, input);

        if (input.empty()) input = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        Board board = Board::create_from_fen(input);

        board.print_state();
        board.print_legal_moves();
        
        if (!board.is_game_in_progress()) {
            std::cout << "Game over\n\n";
            std::string tmp;
            std::getline(std::cin, input);
            continue;
        }

        while (1) {
            std::string move;
            std::cout << "Input move or exit/fen/e (engine): ";
            std::cin >> move;

            if (move == "exit") {
                return;
            } else if (move == "fen") {
                std::cout << "Exporting fen...\n";
                std::cout << board.export_fen() << "\n\n";
            } else if (move == "e") {
                Move best_move = Engine::solve(board, 2);
                board.execute_move(best_move);
                board.print_state();
                board.print_legal_moves();
            } else {
                board.execute_move_string(move);
                board.print_state();
                board.print_legal_moves();
            }

            if (!board.is_game_in_progress()) {
                std::cout << "Game over\n\n";
                std::string tmp;
                std::getline(std::cin, input);
                break;
            }
        }
    }
}

void run_tests(int max_depth) {
    int test_count = 0, ok_tests = 0, failed_tests = 0;

    std::cout << "Running tests...\n";
    std::string path = "tests";
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        std::ifstream filestream(entry.path());

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(filestream, line)) {
            lines.push_back(line);
        }

        if (lines.size() != 3) {
            std::cout << "Improper test: found " << lines.size() << " lines, expected 2\n";
            continue;
        } 

        std::string comment = lines[0];
        std::string input = lines[1];
        std::vector<std::string> valid_moves;
        std::string buf;
        for (char c : lines[2]) {
            if (c == ' ') {
                valid_moves.push_back(buf);
                buf = "";
            } else buf += c;
        }
        if (!buf.empty()) {
            valid_moves.push_back(buf);
        }

        // input is FEN of board state
        // output is expected move to be played
        
        std::cout << "Running test " << entry.path().filename().string() << ": " << comment << ", board: \"" << input << "\"\n";
        Board board = Board::create_from_fen(input);
        board.print_state();
        Move engine_move = Engine::solve(board, max_depth);

        std::string result = engine_move.to_string();
        test_count++;
        bool ok = false;
        for (std::string valid_result : valid_moves) {
            if (valid_result == result) {
                ok = true;
                break;
            }
        }
        if (ok) {
            std::cout << "Correct output, got " << result << '\n';
            ok_tests++;
        } else {
            std::cout << "Incorrect output: expected any of ";
            for (std::string valid_move : valid_moves) std::cout << valid_move << ", ";
            std::cout << "got " << engine_move.to_string() << '\n';
            failed_tests++;
        }
        std::cout << "\n\n";
    }

    std::cout << "Ran " << test_count << " tests, " << ok_tests << " ok, " << failed_tests << " failed\n";
    if (ok_tests == test_count) {
        std::cout << "All tests passed.\n";
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        interactive();
        return 0;
    }

    int max_depth = 3;
    if (argc == 3) {
        max_depth = atoi(argv[2]);
    }

    if (argc >= 2) {
        std::string arg(argv[1]);
        if (arg == "test") {
            run_tests(max_depth);
        }
    }

    return 0;
}
