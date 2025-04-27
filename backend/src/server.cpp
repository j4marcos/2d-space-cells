#include "Server.h"

using json = nlohmann::json;

void startServer()
{
    httplib::Server svr;

    // Rota para validar o Sudoku
    svr.Post("/validate", [](const httplib::Request &req, httplib::Response &res)
             {
        try {
            // Parseia o JSON recebido
            json request_json = json::parse(req.body);

            // Verifica se o campo "board" existe e é uma matriz 9x9
            if (!request_json.contains("board") || !request_json["board"].is_array()) {
                res.status = 400;
                res.set_content("Invalid request: 'board' field is required and must be a 9x9 array.", "text/plain");
                return;
            }

            // Converte o JSON para um vetor de vetores de inteiros
            std::vector<std::vector<int>> board = request_json["board"].get<std::vector<std::vector<int>>>();

            // Valida o tamanho do tabuleiro
            if (board.size() != 9 || board[0].size() != 9) {
                res.status = 400;
                res.set_content("Invalid board: The board must be a 9x9 grid.", "text/plain");
                return;
            }

            // Valida o Sudoku sem threads
            Metrics metricsNoThreads = validateSudokuNoThreads(board);

            // Valida o Sudoku com threads
            Metrics metricsWithThreads = validateSudokuWithThreads(board);

            // Retorna o resultado como JSON
            json response_json = {
                {"no_threads", {
                    {"is_valid", metricsNoThreads.isValid},
                    {"execution_time_ms", metricsNoThreads.executionTimeMs},
                    {"row_checks", metricsNoThreads.rowChecks},
                    {"col_checks", metricsNoThreads.colChecks},
                    {"box_checks", metricsNoThreads.boxChecks}
                }},
                {"with_threads", {
                    {"is_valid", metricsWithThreads.isValid},
                    {"execution_time_ms", metricsWithThreads.executionTimeMs},
                    {"row_checks", metricsWithThreads.rowChecks},
                    {"col_checks", metricsWithThreads.colChecks},
                    {"box_checks", metricsWithThreads.boxChecks}
                }}
            };

            res.set_content(response_json.dump(), "application/json");
        } catch (const std::exception &e) {
            res.status = 500;
            res.set_content(std::string("Internal server error: ") + e.what(), "text/plain");
        } });

    // Rota para validar múltiplos Sudokus
    svr.Get("/validate-multiple", [](const httplib::Request &req, httplib::Response &res)
            {
        try {
            // Gerar 1000 tabuleiros de Sudoku aleatórios
            std::vector<std::vector<std::vector<int>>> boards(1000);
            for (auto &board : boards) {
                board = generateRandomSudoku();
            }

            // Validar sem threads
            Metrics noThreadsMetrics = validateMultipleSudokusNoThreads(boards);

            // Validar com threads
            Metrics withThreadsMetrics = validateMultipleSudokusWithThreads(boards);

            // Retorna o resultado como JSON
            json response_json = {
                {"no_threads", {
                    {"execution_time_ms", noThreadsMetrics.executionTimeMs},
                    {"total_row_checks", noThreadsMetrics.rowChecks},
                    {"total_col_checks", noThreadsMetrics.colChecks},
                    {"total_box_checks", noThreadsMetrics.boxChecks}
                }},
                {"with_threads", {
                    {"execution_time_ms", withThreadsMetrics.executionTimeMs},
                    {"total_row_checks", withThreadsMetrics.rowChecks},
                    {"total_col_checks", withThreadsMetrics.colChecks},
                    {"total_box_checks", withThreadsMetrics.boxChecks}
                }}
            };

            res.set_content(response_json.dump(), "application/json");
        } catch (const std::exception &e) {
            res.status = 500;
            res.set_content(std::string("Internal server error: ") + e.what(), "text/plain");
        } });

    std::cout << "Servidor rodando em http://localhost:9002" << std::endl;

    // Inicia o servidor na porta 9002
    svr.listen("0.0.0.0", 9002);
}

int main()
{
    startServer();
    return 0;
}