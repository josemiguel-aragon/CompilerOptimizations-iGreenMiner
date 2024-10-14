import matplotlib.pyplot as plt
import pandas as pd


signal_original = pd.read_csv('./aux.csv')
#signal_opt = pd.read_csv('./signal_ghostrunner_-Ounchecked_-Oz.csv')

plt.figure(figsize=(16,6))
plt.plot(signal_original['Main(mA)'], c='g', alpha=0.4, linewidth=1.5, label="Original")
#plt.plot(signal_opt['Main(mA)'], c='b', alpha=0.5, linewidth=1, label="-Ounchecked -Oz")
plt.legend()
#plt.savefig("./signal_compare_ghostrun.png", dpi=300)
plt.show()