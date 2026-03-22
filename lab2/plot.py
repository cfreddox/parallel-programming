import matplotlib.pyplot as plt


def plot(
    data1: list,
    time1: list,
    time2: list,
    time4: list,
    time8: list,
    time12: list,
    image: str,
) -> None:
    """
    Функция для построения графика.
    """
    plt.figure(figsize=(10, 6))
    plt.xlabel("Размер матрицы")
    plt.ylabel("Время, мс")
    plt.grid(True)

    plt.plot(data1, time1, color='blue', label='1 поток')
    plt.plot(data1, time2, color='red', label='2 потока')
    plt.plot(data1, time4, color='orange', label='4 потока')
    plt.plot(data1, time8, color='green', label='8 потоков')
    plt.plot(data1, time12, color='purple', label='12 потоков')

    plt.title("Производительность в зависимости от количества потоков")
    plt.legend()
    plt.tight_layout()
    plt.savefig(image)
    plt.close()


def main():
    data1 = [200, 400, 800, 1200]

    time1 = [193, 1624, 7637, 21778]
    time2 = [139, 520, 4714, 15449]
    time4 = [101, 310, 2608, 10663]
    time8 = [90, 210, 2209, 5697]
    time12 = [71, 206, 1769, 5399]

    plot(data1, time1, time2, time4, time8, time12, "plot.png")


if __name__ == "__main__":
    main()
