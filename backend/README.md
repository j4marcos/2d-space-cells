# DiepIO Backend - Projeto de Sistemas Operacionais

Este projeto implementa o backend de um jogo multiplayer estilo diep.io com física realista, movimentação espacial e colisões usando C++ e WebSockets. O código é otimizado com multithreading, dividindo o mapa em quadrantes processados paralelamente por threads distintas.

## 📦 Estrutura
```
backend/
├── src/
│   ├── main.cpp       # Ponto de entrada do servidor
│   ├── game.h/cpp     # Estruturas e lógica do mundo do jogo
│   ├── physics.h/cpp  # Física e colisões
│   ├── server.h/cpp   # WebSocket server com websocketpp
├── Makefile           # Compilação
└── README.md          # Este arquivo
```

## ⚙️ Pré-requisitos

- Linux
- `g++` com suporte a C++17
- `make`
- **Bibliotecas**:
  - [websocketpp](https://github.com/zaphoyd/websocketpp) (instale ou adicione como header-only)
  - [Boost](https://www.boost.org/) (`libboost-system` e `libboost-thread`)
  - [nlohmann/json](https://github.com/nlohmann/json) (header-only)

Em sistemas Debian/Ubuntu:
```bash
sudo apt update
sudo apt install g++ make libboost-all-dev
```
Coloque `websocketpp` e `json.hpp` em `/usr/local/include` ou ajuste o `Makefile`.

## 🛠️ Compilação

Navegue até a pasta `backend` e execute:
```bash
make
```
Isso irá gerar o executável `diep_server`.

## 🚀 Execução

Execute o servidor:
```bash
./diep_server
```

O servidor:
- Escuta na porta **9002** via WebSocket
- Aceita múltiplas conexões
- Registra automaticamente jogadores
- Recebe comandos de controle (movimento, tiro)
- Simula movimentação, colisões e envia o estado do jogo a cada 100ms para os clientes conectados

## 🧪 Comandos WebSocket

Formato das mensagens do cliente (JSON):
```json
{
  "type": "command",
  "thrust": true,
  "left": false,
  "right": true,
  "shoot": false
}
```

Resposta inicial do servidor:
```json
{
  "type": "register",
  "player_id": 1
}
```

Estado enviado a cada tick:
```json
{
  "players": [...],
  "asteroids": [...],
  "projectiles": [...]
}
```

## 📌 Observações

- O jogo usa threads para cada quadrante do mapa.
- Toda física e movimentação é simulada no backend.
- O frontend (React) deve se conectar via WebSocket na porta `9002` e interagir com as mensagens acima.

## 📚 Créditos
Projeto criado para a disciplina de **Sistemas Operacionais**, com foco em concorrência, uso de threads, simulação física e redes.

---

Desenvolvido por: [Seu Nome Aqui] ✨
