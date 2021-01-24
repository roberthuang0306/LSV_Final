import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import os
import sys

# 指定要查詢的路徑
logPath = sys.argv[1]
plotPath = sys.argv[2]
# 列出指定路徑底下所有檔案(包含資料夾)
allFileList = os.listdir(logPath)
allFileList.sort()


for file in allFileList:
    if os.path.isdir(os.path.join(logPath, file)):
        continue
    n = os.path.splitext(file)[0]
    n = n.replace('input', '')
    n = int(n)
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
    #設定大小跟x軸兩端的值
    plt.figure(figsize = (20,10))
    matplotlib.rcParams.update({'font.size': 30})
    plt.xlim(0, max(x))
    plt.ylim(0, max(y))
    #做圖
    plt.scatter(x, y) 
    #視覺化標題跟xy軸名稱
    plt.title(f'n = {n}, output gate number v.s. input gate number', fontsize = 30)
    plt.xlabel('input gate number', fontsize = 30)
    plt.ylabel('output gate number', fontsize = 30)
    #出圖
    plt.savefig(os.path.join(plotPath, os.path.splitext(file)[0]+".png"))