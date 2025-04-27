// frontend/src/SudokuBoard.tsx
import React from 'react';
import { SudokuBoardType } from './types';
import './SudokuBoard.css';

interface Props {
  board: SudokuBoardType;
  setBoard: React.Dispatch<React.SetStateAction<SudokuBoardType>>;
  fixedCells: boolean[][];
}

const SudokuBoard: React.FC<Props> = ({ board, setBoard, fixedCells }) => {
  // Atualiza o valor de uma célula
  const handleChange = (row: number, col: number, value: string) => {
    if (fixedCells[row][col]) return;
    const num = parseInt(value);
    if (value === '' || (num >= 1 && num <= 9)) {
      const newBoard = board.map((r, i) =>
        i === row ? r.map((c, j) => (j === col ? (value === '' ? 0 : num) : c)) : r
      );
      setBoard(newBoard);
    }
  };

  return (
    <div className="sudoku-board">
      {board.map((row, rowIndex) => (
        <div key={rowIndex} className="board-row">
          {row.map((cell, colIndex) => (
            <input
              key={`${rowIndex}-${colIndex}`}
              type="text"
              className={`cell ${fixedCells[rowIndex][colIndex] ? 'fixed' : 'editable'}`}
              value={cell === 0 ? '' : cell}
              onChange={(e) => handleChange(rowIndex, colIndex, e.target.value)}
              maxLength={1}
              readOnly={fixedCells[rowIndex][colIndex]}
            />
          ))}
        </div>
      ))}
    </div>
  );
};

export default SudokuBoard;