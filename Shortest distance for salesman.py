"""
    문제 출처 백준 -- https://www.acmicpc.net/
    1. 문제(정리)
        A. 핵심 외판원이 이동한 거리의 최솟값이 되도록 한다.
        B. 모든 외판원은 1개 이상의 구역을 할당 받아야 한다.
        C. 모든 도시는 단 한번씩 등장해야한다.
        D. 거리의 계산은 대각 직선을 상용한다.(최단거리)
        E. 모든 외판원은 반드시 시작점으로 순회하여 돌아와야 한다.
        F. 총 N개에 도시 중 K명의 외판원이 배치될 때 최소의 거리의 합은?
    
    2. 핵심 아이디어
        A. 주어진 도시 데이터에서 각 도시간의 거리를 구하여 행렬을 만든다.
        B. 모든 N개의 도시에 한 명의 외판원을 배치한다.(최소 거리의 합 = 0)
        C. 이 도시들 중 가장 가까운 거리를 가진 도시를 연결한다(Merge algo)
            C-1. 이때 최단 거리는 새로 만들어진 들어올 도시와 이미 만들어진 도시의 군집에서 순환이 최소가 되어야 한다.
        D. 외판원의 수가 K명이 될 때까지 Merge를 반복한다.
"""

import random
import math
import numpy as np

"""
# N is number of City
N = 8000
# K is number of Salesman
K = 140

Location = list(0 for i in range(0, N))

# each location's x, y oordinate are under 814,000
for i in range(0, N):
  Location[i] = input([random.randrange(0, 814000), random.randrange(0, 814000)])

for i in Location:
  print(Location[0], Location[1])
"""

# First, make array of city's location

# N is number of City
N = 800
# K is number of Salesman
K = 28

# problem is upper data but that is so huge, then I use downgrade version


# N is number of City
N = 50
# K is number of Salesman
K = 10

Max = (N+K)*100
Location = list(0 for i in range(0, N))

# each location's x, y oordinate are under 814,000
for i in range(0, N):
  Location[i] = [random.randrange(0, Max), random.randrange(0, Max)]

# check condition

for i in Location:
  print(i)


# territory is each salesmen's territory [go round once territory]
# First time, Every salesmen gets one city so array is [one's city, one's city]
territory = list(0 for i in range(0, N))
for i in range(0, N):
  territory[i] = [i, i]

# measureDistance do measure distance p1 point between p2 point
def measureDistance(p1, p2):
  x = p1[0] - p2[0]
  y = p1[1] - p2[1]
  return math.sqrt(x ** 2 + y ** 2)

# First solution : Make every distance array matrix

# Make distance between all cities.
Distance = []

for i in range(0, N):
  Distance.append([])
  for j in range(0, N):
    Distance[i].append(measureDistance(Location[i], Location[j]))

# Short value used when two different territory to merge.
Short = 0

# findShortest() is fine
def findShortest():
  Shortest = Max*2
  point = [0,0] # main point territory array's index, customer point territory array's index
  breakcount = 0
  for i in territory:
    for j in territory:
      for k in j:
        if i[0] == k:
          breakcount = 1
      if breakcount != 1:
        D1 = Distance[i[0]][j[0]] + Distance[i[len(i)-2]][j[len(j)-2]]
        D2 = Distance[i[0]][j[len(j)-2]] + Distance[i[len(i)-2]][j[0]]
        if D1 > D2 :
          if Shortest > D2 :
            Shortest = D2
            point = [i, j]  
            Short = -1 # D2
        else :
          if Shortest > D1:
            Shortest = D1
            point = [i, j]
            Short = 1 # D1 
      breakcount = 0
  return point

# merge two different circle
def merge(t1, t2):
  if Short < 0 : # reverse way
    # t2's array reverse
    for i in t2[0:-1]:
      territory[i].reverse()
    t2.reverse()      
  
  # Merge
  for i in t1[0:-1]:
    index = territory[i].index(t1[-2])
    territory[i] = territory[i][0:index+1] + t2[0:-1] + territory[i][index+1:]
  for i in t2[0:-1]:
    index = territory[i].index(t2[-2])
    territory[i] = territory[i][0:index+1] + t1[0:-1] + territory[i][index+1:]

# action main stream

count = 0
while (N-count) > K :
  shortP = findShortest()
  merge(shortP[0], shortP[1])
  count +=1

# delete the overlap

answer = []
breakcount = 0
for i in territory:
  if i == 0:
    answer.append(i)
  for j in answer:
    for k in j:
      if i[0] == k:
        breakcount = 1
  if breakcount != 1:
    answer.append(i)
  breakcount = 0

for i in answer:
  print(i)