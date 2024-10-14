import matplotlib.pyplot as plt
import pandas as pd


df = pd.read_csv('./aux.csv')
df2 = pd.read_csv('./signal.csv')

plt.plot(df['Main(mA)'])
plt.plot(df2['Unnamed: 0'], df2['Main(mA)'])
plt.show()