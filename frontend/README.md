# Frontend - DiepIO

Interface web feita com React + TypeScript para o jogo multiplayer com física realista.

## 📦 Estrutura
```
frontend/
├── public/            # Arquivos estáticos
├── src/
│   ├── index.ts       # Ponto de entrada
│   ├── App.tsx        # Componente principal
│   ├── GameCanvas.tsx # Renderiza o jogo no canvas
│   └── api.ts         # (opcional, não usado neste exemplo)
├── package.json       # Dependências e scripts
└── README.md          # Este arquivo
```

## ⚙️ Requisitos

- Node.js 18+
- npm ou yarn

## ▶️ Rodando o Projeto

```bash
cd frontend
npm install
npm run dev
```

Acesse [http://localhost:5173](http://localhost:5173) no navegador.

> O frontend se conecta por WebSocket ao backend em `ws://localhost:9002`.

## ⌨️ Controles

- `W` – impulso
- `A` / `D` – gira a nave
- `Espaço` – dispara projétil

## 🧠 Funcionamento

- Ao conectar, o jogador é registrado automaticamente.
- A cada 100ms, comandos são enviados ao backend.
- O backend responde com o estado do jogo, que é renderizado no canvas.

## 📌 Observações

- O canvas é desenhado manualmente com `CanvasRenderingContext2D`.
- O React apenas gerencia o ciclo de vida e estado.
- Não há roteamento, é uma SPA simples.

---
Desenvolvido para o projeto de Sistemas Operacionais 👾
