
import random
import struct
import argparse


def generate_maze(height, width, obstacle_probability):
    maze = [[0 for _ in range(width)] for _ in range(height)]

    for i in range(height):
        for j in range(width):
            if random.random() < obstacle_probability:
                maze[i][j] = 1

    return maze


def save_maze(maze, output_path):
    with open(output_path, "wb") as f:
        f.write(struct.pack('i', len(maze)))
        f.write(struct.pack('i', len(maze[0])))

        for i in range(len(maze)):
            for j in range(len(maze[0])):
                f.write(struct.pack('B', maze[i][j]))


def parse_args():
    parser = argparse.ArgumentParser(description="Generate a maze.")
    parser.add_argument('output_path', type=str,
                        help="Caminho do arquivo onde a maze sera salva.")
    parser.add_argument('n_linhas', type=int, help="Numero de linhas.")
    parser.add_argument('n_colunas', type=int, help="Numero de colunas.")
    parser.add_argument('prob', type=float, default=0.3,
                        help="Probabilidade de uma celula ser um obstaculo.")
    return parser.parse_args()


def main():
    args = parse_args()
    maze = generate_maze(args.n_linhas, args.n_colunas, args.prob)

    # MAZES QUADRADAS: descomente o codigo abaixo para tornar as diagonais livres
    """
    for i in range(len(maze) - 1, -1, -1):
        if i < len(maze) and i < len(maze[0]):
            maze[i][i] = 0
            maze[i][len(maze) - i - 1] = 0
    """

    save_maze(maze, args.output_path)


if __name__ == "__main__":
    main()
