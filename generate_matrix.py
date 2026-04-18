import argparse
from random import randint


def args_parse():
    """Парсит аргументы командной строки."""
    parser = argparse.ArgumentParser(
        prog="Make random matrix"
    )
    parser.add_argument(
        "-f", "--file",
        type=str,
        help="file for matrix",
        required=True
    )
    parser.add_argument(
        "--N",
        type=int,
        help="number of rows",
        required=True
    )
    parser.add_argument(
        "--M",
        type=int,
        help="number of cols",
        required=True
    )
    parser.add_argument(
        "--nmax",
        type=int,
        help="max value",
        required=True
    )
    parser.add_argument(
        "--nmin",
        type=int,
        help="min value",
        required=True
    )
    return parser.parse_args()


def generate_matrix(filename: str, n: int, m: int, min, max):
    with open(filename, "w") as f:
        f.write(f"{n}\t{m}\n")
        for i in range(0, n):
            for j in range(0, m):
                if j != m-1:
                    f.write(f"{randint(min, max)} ")
                else:
                    f.write(f"{randint(min, max)}\n")


if __name__ == "__main__":
    args = args_parse()
    if args.nmin > args.nmax:
        print("Минимальное число должно быть больше максимального")
    else:
        generate_matrix(args.file, args.N, args.M, args.nmin, args.nmax)
