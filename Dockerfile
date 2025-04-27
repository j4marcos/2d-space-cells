# Usar uma imagem base com GCC e Make
FROM gcc:latest

# Instalar dependências adicionais, se necessário
RUN apt-get update && apt-get install -y make

# Definir diretório de trabalho
WORKDIR /app

# Copiar os arquivos do projeto para o container
COPY . .

# Compilar o projeto usando o Makefile
RUN make

# Expor a porta usada pelo servidor HTTP
EXPOSE 8080

# Comando para executar o programa
CMD ["./simulacao"]