#include "SudokuValidator.h"

// Função sem threads
Metrics validateSudokuNoThreads(std::vector<std::vector<int>> &board)
{
    Metrics metrics = {true, 0.0, 0, 0, 0};
    auto start = std::chrono::high_resolution_clock::now();

    // Verifica se o tabuleiro é 9x9
    if (board.size() != 9 || board[0].size() != 9)
    {
        metrics.isValid = false;
        auto end = std::chrono::high_resolution_clock::now();
        metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
        return metrics;
    }

    // Verifica linhas
    for (int i = 0; i < 9 && metrics.isValid; i++)
    {
        std::vector<bool> seen(10, false);
        for (int j = 0; j < 9; j++)
        {
            int num = board[i][j];
            metrics.rowChecks++;
            if (num < 1 || num > 9 || seen[num])
            {
                metrics.isValid = false;
                break;
            }
            seen[num] = true;
        }
    }

    // Verifica colunas
    for (int j = 0; j < 9 && metrics.isValid; j++)
    {
        std::vector<bool> seen(10, false);
        for (int i = 0; i < 9; i++)
        {
            int num = board[i][j];
            metrics.colChecks++;
            if (num < 1 || num > 9 || seen[num])
            {
                metrics.isValid = false;
                break;
            }
            seen[num] = true;
        }
    }

    // Verifica sub-boxes 3x3
    for (int box = 0; box < 9 && metrics.isValid; box++)
    {
        std::vector<bool> seen(10, false);
        int rowStart = (box / 3) * 3;
        int colStart = (box % 3) * 3;
        for (int i = rowStart; i < rowStart + 3; i++)
        {
            for (int j = colStart; j < colStart + 3; j++)
            {
                int num = board[i][j];
                metrics.boxChecks++;
                if (num < 1 || num > 9 || seen[num])
                {
                    metrics.isValid = false;
                    break;
                }
                seen[num] = true;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return metrics;
}

// Função com threads
Metrics validateSudokuWithThreads(std::vector<std::vector<int>> &board)
{
    Metrics metrics = {true, 0.0, 0, 0, 0};
    if (board.size() != 9 || board[0].size() != 9)
    {
        metrics.isValid = false;
        return metrics;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::atomic<bool> valid(true);
    std::mutex metricsMutex;

    // Função para validar linhas
    auto validateRows = [&]()
    {
        for (int i = 0; i < 9 && valid.load(); i++)
        {
            std::vector<bool> seen(10, false);
            for (int j = 0; j < 9; j++)
            {
                int num = board[i][j];
                {
                    std::lock_guard<std::mutex> lock(metricsMutex);
                    metrics.rowChecks++;
                }
                if (num < 1 || num > 9 || seen[num])
                {
                    valid.store(false);
                    return;
                }
                seen[num] = true;
            }
        }
    };

    // Função para validar colunas
    auto validateCols = [&]()
    {
        for (int j = 0; j < 9 && valid.load(); j++)
        {
            std::vector<bool> seen(10, false);
            for (int i = 0; i < 9; i++)
            {
                int num = board[i][j];
                {
                    std::lock_guard<std::mutex> lock(metricsMutex);
                    metrics.colChecks++;
                }
                if (num < 1 || num > 9 || seen[num])
                {
                    valid.store(false);
                    return;
                }
                seen[num] = true;
            }
        }
    };

    // Função para validar sub-boxes individuais
    auto validateBox = [&](int boxIndex)
    {
        int rowStart = (boxIndex / 3) * 3;
        int colStart = (boxIndex % 3) * 3;
        std::vector<bool> seen(10, false);

        for (int i = rowStart; i < rowStart + 3 && valid.load(); i++)
        {
            for (int j = colStart; j < colStart + 3; j++)
            {
                int num = board[i][j];
                {
                    std::lock_guard<std::mutex> lock(metricsMutex);
                    metrics.boxChecks++;
                }
                if (num < 1 || num > 9 || seen[num])
                {
                    valid.store(false);
                    return;
                }
                seen[num] = true;
            }
        }
    };

    // Lançar threads
    std::thread rowThread(validateRows);
    std::thread colThread(validateCols);
    std::vector<std::thread> boxThreads;

    for (int i = 0; i < 9; i++)
    {
        boxThreads.emplace_back(validateBox, i);
    }

    // Aguardar todas as threads
    rowThread.join();
    colThread.join();
    for (auto &t : boxThreads)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    metrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    metrics.isValid = valid.load();
    return metrics;
}

// Valida múltiplos Sudokus sem usar threads
Metrics validateMultipleSudokusNoThreads(const std::vector<std::vector<std::vector<int>>> &boards) {
    Metrics combinedMetrics = {true, 0.0, 0, 0, 0};
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &board : boards) {
        Metrics m = validateSudokuNoThreads(const_cast<std::vector<std::vector<int>>&>(board));

        combinedMetrics.rowChecks += m.rowChecks;
        combinedMetrics.colChecks += m.colChecks;
        combinedMetrics.boxChecks += m.boxChecks;

        if (!m.isValid) {
            combinedMetrics.isValid = false;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    combinedMetrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return combinedMetrics;
}

// Valida múltiplos Sudokus usando threads
Metrics validateMultipleSudokusWithThreads(const std::vector<std::vector<std::vector<int>>> &boards) {
    Metrics combinedMetrics = {true, 0.0, 0, 0, 0};
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    std::mutex metricsMutex;
    std::atomic<bool> overallValid(true);

    for (const auto &board : boards) {
        threads.emplace_back([&]() {
            Metrics m = validateSudokuWithThreads(const_cast<std::vector<std::vector<int>>&>(board));

            std::lock_guard<std::mutex> lock(metricsMutex);
            combinedMetrics.rowChecks += m.rowChecks;
            combinedMetrics.colChecks += m.colChecks;
            combinedMetrics.boxChecks += m.boxChecks;

            if (!m.isValid) {
                overallValid.store(false);
            }
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    combinedMetrics.executionTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    combinedMetrics.isValid = overallValid.load();
    return combinedMetrics;
}

// Função para gerar um tabuleiro de Sudoku aleatório
std::vector<std::vector<int>> generateRandomSudoku()
{
    std::vector<std::vector<int>> board(9, std::vector<int>(9, 0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 9);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = dis(gen);
        }
    }
    return board;
}
