# Data Structures and Algorithms - Polimi

<p align="center">
  <img width="100%" src="https://i.imgur.com/tm9mSuM.png" alt="header" />
</p>
<p align="center">
    <img src="https://i.imgur.com/mPb3Qbd.gif" width="180" alt="Politecnico di Milano"/>
</p>

This repo contains a solution of the 2022/23 Polimi's Data Structures and Algorithm final exam I wrote this summer to keep myself trained.
<br>


## Results

No official results can be given, as I could not take part in the actual exam. Nonetheless, my code runs in similar time and memory consumption to those that achieved honors grades.

## Goal
The application domain is book recommendation. 
The datasets we provide contains interactions of users with books, in particular, if the user attributed to the book a rating of at least 4.
The main goal of the competition is to discover which items (books) a user will interact with.

## Data Structures Description

The main data structure used to store the stations is a Red-Black Tree, implementing the CLRS Red-Black Trees pseudocode.<br>
Cars parked in stations are stored in Linked Lists. This choice was initially made to keep the code simple and was never changed as the code proved to be fast enough. To achieve even better performance, cars may be stored using a priority queue structure, such as a MaxHeap.<br>

## Algorithm Description

The calculation of the shortest path takes advantage of the order of the stations in the tree and visits each station at most twice.<br>
The first visit occurs during the forward pass to find the farthest station that can be reached (to determine if the given destination is reachable), and the second visit adjusts the path as per the requirements of the assignment.<br>
The lookup cost of the starting station, as well as the costs to find the previous or following nodes in the tree, have logarithmic costs. Thus, the overall temporal complexity is $O(n * log n)$ and the space complexity is $O(n)$, where n is the number of stored stations.<br>
