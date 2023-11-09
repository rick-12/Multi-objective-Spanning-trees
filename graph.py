import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('data.txt')

x = data[:, 0]
y = data[:, 1]


plt.plot(x, y)
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Data Plot')
plt.grid(True)
plt.show()