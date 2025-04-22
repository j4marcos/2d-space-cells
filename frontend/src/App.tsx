// frontend/src/App.tsx
import React, { useEffect, useRef, useState } from 'react';
import GameCanvas from './GameCanvas';

interface GameState {
  players: any[];
  asteroids: any[];
  projectiles: any[];
}

const App: React.FC = () => {
  const ws = useRef<WebSocket | null>(null);
  const [playerId, setPlayerId] = useState<number | null>(null);
  const [state, setState] = useState<GameState>({ players: [], asteroids: [], projectiles: [] });
  const keys = useRef<{ [key: string]: boolean }>({});

  useEffect(() => {
    ws.current = new WebSocket('ws://localhost:9002');

    ws.current.onmessage = (msg) => {
      const data = JSON.parse(msg.data);
      console.log('Received data:', data);
      if (data.type === 'register') {
        setPlayerId(data.player_id);
      } else {
        setState({
          players: data.players ?? [],
          asteroids: data.asteroids ?? [],
          projectiles: data.projectiles ?? [] // fallback defensivo
        });
      }
    };

    const sendCommands = setInterval(() => {
      if (ws.current && playerId !== null) {
        const cmd = {
          type: 'command',
          thrust: keys.current['w'] || false,
          left: keys.current['a'] || false,
          right: keys.current['d'] || false,
          shoot: keys.current[' '],
        };
        ws.current.send(JSON.stringify(cmd));
      }
    }, 100);

    return () => clearInterval(sendCommands);
  }, [playerId]);

  useEffect(() => {
    const handleKey = (e: KeyboardEvent) => {
      keys.current[e.key.toLowerCase()] = e.type === 'keydown';
    };
    window.addEventListener('keydown', handleKey);
    window.addEventListener('keyup', handleKey);
    return () => {
      window.removeEventListener('keydown', handleKey);
      window.removeEventListener('keyup', handleKey);
    };
  }, []);

  return (
    <div>
      <GameCanvas state={state} playerId={playerId} />
    </div>
  );
};

export default App;
