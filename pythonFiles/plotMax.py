import numpy as np
from numpy import arange
import matplotlib.pyplot as plt
import matplotlib
import os
import sys
from scipy.optimize import curve_fit

def objective(x, a, b, c):
	return a * x + b * x**2 / np.log(x) + c
# 指定要查詢的路徑
logPath = sys.argv[1]
plotPath = sys.argv[2]
# 列出指定路徑底下所有檔案(包含資料夾)
allFileList = os.listdir(logPath)
allFileList.sort()

yMax = []
for file in allFileList:
    if os.path.isdir(os.path.join(logPath, file)):
        continue
    print(file)
    f1 = open( os.path.join(logPath, file), 'r' )
    contents = list(filter(str.strip, f1.readlines()))[:-1]
    f1.close()
    gateNums = []
    for content in contents:
        gateNums.append(list(map(int, (content.replace('\n', '').split(' ')))))
    gateNums = np.array(gateNums)
    #設定xy軸，下面兩個都是一串數字
    y = gateNums[:,0]
    x = gateNums[:,1]
    n = os.path.splitext(file)[0]
    n = int(n.replace('input', ''))
    yMax.append([n,max(y)])

yMax = np.array(yMax)
x = yMax[:,0]
y = yMax[:,1]

plt.figure(figsize = (20,10))
matplotlib.rcParams.update({'font.size': 30})
plt.xlim(0, max(x))
plt.ylim(0, max(y))
#做圖
plt.scatter(x, y) 

plt.title(f'output gates Max number v.s. qubits number', fontsize = 30)
plt.xlabel('qubits number', fontsize = 30)
plt.ylabel('output gates Max number', fontsize = 30)
popt, _ = curve_fit(objective, x, y)
# summarize the parameter values
a, b, c = popt
x_line = arange(min(x), max(x), 1)
# calculate the output for the range
y_line = objective(x_line, a, b, c)
# create a line plot for the mapping function
plt.plot(x_line, y_line, '--', color='red', label=f'y = {b}x^2/ln(x) + {a}x + {c}' )
plt.legend(fontsize=16)
#出圖
plt.savefig(os.path.join(plotPath, "outputMax.png"))