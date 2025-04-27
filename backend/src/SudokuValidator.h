#ifndef SUDOKU_VALIDATOR_H
#define SUDOKU_VALIDATOR_H

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

// Estrutura para armazenar métricas de validação
struct Metrics
{
    bool isValid;           // Resultado da validação
    double executionTimeMs; // Tempo de execução em milissegundos
    int rowChecks;          // Contador de verificações em linhas
    int colChecks;          // Contador de verificações em colunas
    int boxChecks;          // Contador de verificações em sub-boxes
};

// Declaração das funções de validação
Metrics validateSudokuNoThreads(std::vector<std::vector<int>> &board);
Metrics validateSudokuWithThreads(std::vector<std::vector<int>> &board);
Metrics validateMultipleSudokusNoThreads(const std::vector<std::vector<std::vector<int>>> &boards);
Metrics validateMultipleSudokusWithThreads(const std::vector<std::vector<std::vector<int>>> &boards);
std::vector<std::vector<int>> generateRandomSudoku();

#endif