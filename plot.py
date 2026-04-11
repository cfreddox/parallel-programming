import matplotlib.pyplot as plt


def plot(
    data1: list,
    time1: list,
    time2: list,
    time4: list,
    time8: list,
    image: str,
) -> None:
    """
    Функция для построения графика.
    """
    plt.figure(figsize=(10, 6))
    plt.xlabel("Размер матрицы")
    plt.ylabel("Время, мс")
    plt.grid(True)

    plt.plot(data1, time1, color='blue', label='1 ядро')
    plt.plot(data1, time2, color='red', label='2 ядра')
    plt.plot(data1, time4, color='orange', label='4 ядра')
    plt.plot(data1, time8, color='green', label='8 ядрер')

    plt.title("Производительность в зависимости от количества потоков")
    plt.legend()
    plt.tight_layout()
    plt.savefig(image)
    plt.close()


def main():
    data1 = [100, 200, 400, 800, 1200, 1600, 2000]

    time1 = [0, 4, 30, 226, 942, 2348, 4505]
    time2 = [0, 2, 16, 125, 529, 1341, 2476]
    time4 = [0, 1, 8, 79, 394, 847, 1696]
    time8 = [0, 1, 8, 87, 301, 784, 1590]

    plot(data1, time1, time2, time4, time8, "plot.png")


if __name__ == "__main__":
    main()
