import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import itertools

from scipy.stats import mannwhitneyu, spearmanr
from IPython.display import set_matplotlib_formats
set_matplotlib_formats('retina', quality=100)

import matplotlib.pylab as pylab
params = {'axes.labelsize': 26,
         'xtick.labelsize': 20,
         'ytick.labelsize': 20}
pylab.rcParams.update(params)

def export_legend(legend, filename="legend.png"):
    fig  = legend.figure
    fig.canvas.draw()
    bbox  = legend.get_window_extent().transformed(fig.dpi_scale_trans.inverted())
    fig.savefig(filename, dpi=300, bbox_inches=bbox)


swift_opt = ['-Onone', '-O', '-Ounchecked']
llvm_opt = ['-O0', '-O3', '-Oz']
bench = "ghostrun"

if __name__ == '__main__':
    ec_plots = []
    time_plots = []
    labels = []

    # Level of significance
    alpha = 0.05

    df = pd.DataFrame()
    df2 = pd.DataFrame()

    count = 0

    for swift in swift_opt:
        for llvm in llvm_opt:
            print(f'ec_measurements_{bench}_{swift}_{llvm}.csv')
            ec_plots.append(np.loadtxt(f'ec_measurements_{bench}_{swift}_{llvm}.csv', delimiter=',')[0:30])
            time_plots.append(np.loadtxt(f'time_measurements_{bench}_{swift}_{llvm}.csv', delimiter=',')[0:30])
            labels.append(f'{swift.replace('unchecked','un').replace('none','no').replace('size','si')}_{llvm}')
            df[labels[-1].replace('-','')] = ec_plots[-1]
            df2[labels[-1].replace('-','')] = time_plots[-1]


            print(np.median(time_plots[-1]))
            print(np.median(ec_plots[-1]))

            if count != 0:
                print(np.median(time_plots[0])/np.median(time_plots[-1]))
                print(np.median(ec_plots[0])/np.median(ec_plots[-1]))
                print( (np.median(time_plots[0])/np.median(time_plots[-1])) / (np.median(ec_plots[0])/np.median(ec_plots[-1])) )

                print("Improvements")
                print((np.median(time_plots[-1]) - np.median(time_plots[0]))/np.median(time_plots[-1]) * 100)
                print((np.median(ec_plots[-1]) - np.median(ec_plots[0]))/np.median(ec_plots[-1]) * 100)
            print("Correlation value")
            s = spearmanr(time_plots[-1], ec_plots[-1])
            print(s.correlation)
            print(s.pvalue)

            count += 1

    print("\nALL CORRELATION")
    print(spearmanr(list(itertools.chain.from_iterable(time_plots)), list(itertools.chain.from_iterable(ec_plots))).correlation)

    df.to_csv("ec_measurements_df.csv")
    df2.to_csv("time_measurements_df.csv")

    fig, ax1 = plt.subplots(nrows=1, ncols=2, figsize=(16, 6))
    ax1[0].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
             np.median(np.array(ec_plots), axis=1), color='indianred', alpha=0.75, label="_no_legend")
    aux2 = ax1[0].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
             np.array(ec_plots).min(axis=1), color='k', linestyle='dashed', linewidth=0.75, alpha=0.75, label='Line of best fit')
    ax1[0].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
             np.array(ec_plots).max(axis=1), color='k', linestyle='dashed', linewidth=0.75, alpha=0.75, label='_no_legend')
    aux3 = ax1[0].fill_between(range(1,len(llvm_opt) * len(swift_opt)+1),
                     np.array(ec_plots).min(axis=1), np.array(ec_plots).max(axis=1), color='mediumseagreen', alpha=0.25, label="Range of Min and Max")
    bp = ax1[0].boxplot(ec_plots, labels=labels, patch_artist=True)
    for patch in bp['boxes']:
        patch.set(facecolor='w')

    ax1[0].set_ylabel('Energy consumption(J)')
    ax1[0].set_xticklabels(labels=labels, rotation=60, ha="right")


    ax1[1].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
             np.median(np.array(time_plots), axis=1), color='indianred', alpha=0.75, label="_no_legend")
    ax1[1].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
            np.array(time_plots).min(axis=1), color='k', linestyle='dashed', linewidth=0.75, alpha=0.75, label='Line of best fit')
    ax1[1].plot(range(1,len(llvm_opt) * len(swift_opt)+1),
            np.array(time_plots).max(axis=1), color='k', linestyle='dashed', linewidth=0.75, alpha=0.75, label='_no_legend')
    ax1[1].fill_between(range(1,len(llvm_opt) * len(swift_opt)+1),
            np.array(time_plots).min(axis=1), np.array(time_plots).max(axis=1), color='mediumseagreen', alpha=0.25, label="Range of Min and Max")
    bp = ax1[1].boxplot(time_plots, labels=labels, patch_artist=True)
    for patch in bp['boxes']:
        patch.set(facecolor='w')
    ax1[1].set_ylabel('Run time(s)')
    ax1[1].set_xticklabels(labels=labels, rotation=60, ha="right")

    aux = plt.legend()
    fig.tight_layout(pad=1.0)
    #plt.show()
    plt.savefig(f"./bp-{bench}.png", dpi=300)

    export_legend(aux)




