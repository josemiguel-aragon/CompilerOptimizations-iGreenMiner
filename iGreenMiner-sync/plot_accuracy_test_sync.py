import matplotlib.pyplot as plt
import numpy as np


if __name__ == '__main__':
    plt.figure(figsize=(12,7))
    
    approaches = ["onlypc", "iphone15", "iphone11_nomonsoon", "iphone11_monsoon", "periodicsample", "periodicsamplecsv"]
    
    all_plots = []
    for approach in approaches:
        data = np.loadtxt(f"./accuracy_test_{approach}.csv", delimiter=',')
        all_plots.append(data)
    
    plt.boxplot(all_plots, labels = approaches, notch = True)
    plt.ylabel("Run time (s)")
    plt.axhline(y = 1, color = 'indianred', linestyle = '--', label="App run time")
    plt.legend()
    plt.show()
        