import sys
import matplotlib.pyplot as plt

wallClockTime = dict()
num_threads = 2

if len(sys.argv) < 1:
        print("Please input the file name to draw the graph")
        sys.exit()

fileName = sys.argv[1]
fileNameObj = open(fileName, 'r')

for line in fileNameObj.readlines():
        lineArr = line.split()
        if lineArr:
                if lineArr[0] == 'real':
                        wallClockTime[num_threads] = (lineArr[1].split('.')[1])[:-1]
                        num_threads +=2

plt.plot(list( wallClockTime.keys()),[float(i) for i in  list(wallClockTime.values())])
plt.xlabel('X-axis --- No of threads ')
plt.ylabel('Y-axis --- Wall clock time in seconds')


plt.show()
