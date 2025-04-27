// frontend/src/App.tsx
import React, { useState } from 'react';
import SudokuBoard from './SudokuBoard';
import { SudokuBoardType } from './types';
import './App.css';

const App: React.FC = () => {
  // Tabuleiro inicial vazio
  const initialBoard: SudokuBoardType = Array(9).fill(null).map(() => Array(9).fill(0));
  const [board, setBoard] = useState<SudokuBoardType>(initialBoard);
  const [fixedCells, setFixedCells] = useState<boolean[][]>(
    Array(9).fill(null).map(() => Array(9).fill(false))
  );
  const [error, setError] = useState<string | null>(null);

  // Gera um novo tabuleiro chamando o backend
  const generateBoard = async () => {
    try {
      const response = await fetch('/api/generate', { method: 'GET' });
      const data = await response.json();
      if (data.board) {
        setBoard(data.board);
        const newFixedCells = data.board.map((row: number[]) =>
          row.map((cell: number) => cell !== 0)
        );
        setFixedCells(newFixedCells);
        setError(null);
      } else {
        setError('Erro ao gerar tabuleiro');
      }
    } catch (err) {
      setError('Falha na conexão com o servidor');
    }
  };

  // Resolve o tabuleiro chamando o backend
  const solveBoard = async () => {
    try {
      const response = await fetch('/api/solve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ board }),
      });
      const data = await response.json();
      if (data.solvedBoard) {
        setBoard(data.solvedBoard);
        setError(null);
      } else {
        setError('Não foi possível resolver o tabuleiro');
      }
    } catch (err) {
      setError('Falha na conexão com o servidor');
    }
  };

  // Limpa o tabuleiro
  const resetBoard = () => {
    setBoard(initialBoard);
    setFixedCells(Array(9).fill(null).map(() => Array(9).fill(false)));
    setError(null);
  };

  return (
    <div className="app-container">
      <h1>Sudoku</h1>
      {error && <p className="error">{error}</p>}
      <SudokuBoard board={board} setBoard={setBoard} fixedCells={fixedCells} />
      <div className="controls">
        <button onClick={generateBoard}>Novo Jogo</button>
        <button onClick={solveBoard}>Resolver</button>
        <button onClick={resetBoard}>Limpar</button>
      </div>
    </div>
  );
};

export default App;