#ifndef SERVER_H
#define SERVER_H

#include "SudokuValidator.h"
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <vector>

// Declaração da função para iniciar o servidor
void startServer();

#endif
