import matplotlib.pyplot as plt
import pandas as pd
import matplotlib

import matplotlib.pylab as pylab
params = {'axes.labelsize': 38,
         'xtick.labelsize': 35,
         'ytick.labelsize': 35}
pylab.rcParams.update(params)


signal_original = pd.read_csv('./signal_coreml_-Onone_-O0.csv')
signal_opt = pd.read_csv('./signal_coreml_-Ounchecked_-O0.csv')

fig =plt.figure(figsize=(16,10))
ax = fig.gca()
plt.plot(signal_original['Main(mA)'], c='g', alpha=0.4, linewidth=1.5, label="Original")
plt.plot(signal_opt['Main(mA)'], c='b', alpha=0.5, linewidth=1, label="-Ounchecked -O0")
plt.legend(fontsize=35)
plt.ylabel('Amperage(mA)')
plt.xlabel('Sample number')
ax.xaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter(useMathText=True))
ax.ticklabel_format(style="sci", axis="x", scilimits=(0,2))

fig.tight_layout(pad=1.0)
plt.savefig("./signal_compare_coreml.jpg", dpi=300)
#plt.show()