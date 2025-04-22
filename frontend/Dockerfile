# frontend/Dockerfile
FROM node:18-alpine

# Diretório da aplicação
WORKDIR /app

# Copia os arquivos de configuração primeiro (melhor cache)
COPY package*.json ./

# Instala dependências
RUN npm install

# Copia o restante do código
COPY . .

# Expõe a porta padrão do Vite
EXPOSE 5173

# Inicia em modo dev acessível externamente
CMD ["npm", "run", "dev", "--", "--host"]
