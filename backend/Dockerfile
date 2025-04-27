# Base da imagem
FROM ubuntu:24.04

# Configuração para evitar prompts interativos
ENV DEBIAN_FRONTEND=noninteractive

# Instalação de dependências necessárias
RUN apt-get update && \
    apt-get install -y g++ make cmake wget nlohmann-json3-dev && \
    apt-get clean

# Baixa o httplib.h
RUN wget -O /usr/local/include/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

# Diretório de trabalho
WORKDIR /app

# Copia os arquivos do projeto para o contêiner
COPY . /app

# Compila o projeto usando o Makefile
RUN echo "Compilando o projeto SudokuValidator..." && \
    make && \
    test -f bin/SudokuValidator && echo "Compilação OK" || (echo "Compilação FALHOU" && exit 1)

# Porta exposta
EXPOSE 9002

# Comando para executar o programa
CMD ["bin/SudokuValidator"]