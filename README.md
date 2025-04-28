# 2d-space-multiprocess

![alt text](image-2.png)

# Simulador 2D de Física Multithreaded (Projeto SO)

Este repositório contém um simulador 2D de física distribuído em quadrantes, utilizando multithreading em C, servido via HTTP e acessível por um front-end HTML/JavaScript.

## Projeto

- campo de simulaçao de partículas/objetos em um campo 2D, com detecção de colisões (entre objetos e limites)

- divisão de carga de processamento em quadrantes para cálculo paralelo da area total.

- cada quadrante tem uma tread responsavel pelo processamento individual dos objetso dentro da subarea.

- o numero de quadrantes pode ser editado em ./include/manager.h NUM_QUADRANTS
  edite para comparar mais ou menos treads

- interface visual onde é possivel clicar para criar objetos com velocidade aleatoria, e visualizar estatisticas.

## ⚙️ Dependências

- **Compilador C**: GCC (suporte a C99 ou superior)
- **Make**: para automação de build (`make`)
- **Bibliotecas**:
  - `-pthread` para threads POSIX
  - `-lm` para funções matemáticas (`sqrt`, `fmin`)
- **Docker (opcional)**: criar imagem e rodar container

## 🛠️ Como compilar e executar

### Localmente com Make

```bash
# Clonar o repositório
git clone https://github.com/j4marcos/2d-space-cells.git
cd 2d-space-cells

# Build
make clean      # limpa objetos antigos
make            # compila todos os módulos e gera o executável 'simulacao'

# Executar
./simulacao     # inicia o simulador e o servidor HTTP na porta 8080
```

Pronto! agora pode acessar por http://localhost:8080

### Usando Docker Compose

```bash
# Iniciar container (porta 8080)
docker compose up -d
```

## 🚀 Instruções de uso

1. Abra o navegador em `http://localhost:8080`.
2. O canvas exibe o campo de física. As estatísticas aparecem acima (nº de objetos, FPS, threads, tempos de processamento).
3. Clique em qualquer ponto do canvas para criar um novo objeto com velocidade aleatória.
4. Observe colisões entre objetos e com os limites.
5. Para encerrar, pare o processo (`Ctrl+C`) ou o container Docker.

## Limitações

- por causa do buffer overflow, o limite de objetos é 300.

## 🤝 Equipe

| Integrante               | Matrícula   | Responsabilidades               |
| ------------------------ | ----------- | ------------------------------- |
| JOÃO MARCOS CUNHA SANTOS | 20230013339 | Mecânica de colisões e servidor |
| RAFAEL DE FRANÇA SILVA   | 20230012654 | Dockerização e estilização      |
| KAIQUE BEZERRA SANTOS    | 20230013357 | Otimização de threads           |

## 📄 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

---
