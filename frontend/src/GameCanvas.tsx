// frontend/src/GameCanvas.tsx
import React, { useEffect, useRef } from 'react';

interface GameObject {
  x: number;
  y: number;
  r?: number;
  angle?: number;
  id?: number;
}

interface Props {
  state: {
    players: GameObject[];
    asteroids: GameObject[];
    projectiles: GameObject[];
  };
  playerId: number | null;
}

const GameCanvas: React.FC<Props> = ({ state, playerId }) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw asteroids
    ctx.fillStyle = 'gray';
    for (const a of state.asteroids) {
      ctx.beginPath();
      ctx.arc(a.x, a.y, a.r || 30, 0, Math.PI * 2);
      ctx.fill();
    }

    // Draw projectiles
    ctx.fillStyle = 'red';
    for (const p of state.projectiles) {
      ctx.beginPath();
      ctx.arc(p.x, p.y, 3, 0, Math.PI * 2);
      ctx.fill();
    }

    // Draw players
    for (const p of state.players) {
      ctx.save();
      ctx.translate(p.x, p.y);
      ctx.rotate(p.angle || 0);
      ctx.beginPath();
      ctx.moveTo(15, 0);
      ctx.lineTo(-10, 10);
      ctx.lineTo(-10, -10);
      ctx.closePath();
      ctx.fillStyle = p.id === playerId ? 'cyan' : 'blue';
      ctx.fill();
      ctx.restore();
    }
  }, [state]);

  return <canvas ref={canvasRef} width={800} height={600} style={{ background: '#111' }} />;
};

export default GameCanvas;
