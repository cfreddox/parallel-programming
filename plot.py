import matplotlib.pyplot as plt


def plot(
    data1: list,
    time1: list,
    time2: list,
    time4: list,
    image: str,
) -> None:
    """
    Функция для построения графика.
    """
    plt.figure(figsize=(10, 6))
    plt.xlabel("Размер матрицы")
    plt.ylabel("Время, мс")
    plt.grid(True)

    plt.plot(data1, time1, color='blue', label='(8,8)')
    plt.plot(data1, time2, color='red', label='(12,12)')
    plt.plot(data1, time4, color='orange', label='(16,16)')

    plt.title("Производительность")
    plt.legend()
    plt.tight_layout()
    plt.savefig(image)
    plt.close()


def main():
    data1 = [100, 200, 400, 800, 1200, 1600, 2000]

    time_88 = [235.97, 0.354, 0.89, 5.09, 14.32, 32.52, 53.62]
    time_1212 = [214.408, 0.35, 1, 5.07, 13.85, 31.46, 55.143]
    time_1616 = [238.98, 0.34, 0.95, 4.41, 12.81, 26.26, 48.67]

    plot(data1, time_88, time_1212, time_1616, "plot.png")


if __name__ == "__main__":
    main()
