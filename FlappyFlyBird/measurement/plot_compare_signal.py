import matplotlib.pyplot as plt
import pandas as pd


signal_original = pd.read_csv('./signal_flappyflybird_-Onone_-O0.csv')
signal_opt = pd.read_csv('./signal_flappyflybird_-Ounchecked_-O3.csv')

plt.figure(figsize=(16,6))
plt.plot(signal_original['Main(mA)'], c='g', alpha=0.4, linewidth=1.5, label="Original")
plt.plot(signal_opt['Main(mA)'], c='b', alpha=0.5, linewidth=1, label="-Ounchecked -O3")
plt.legend()
plt.savefig("./signal_compare_flappyflybird_png", dpi=300)
#plt.show()