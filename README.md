# Data Structures and Algorithms 2022/23- Polimi

<p align="center">
  <img width="100%" src="https://walkccc.me/CLRS/img/13.1-2-1.png" alt="header" />
</p>

This repo contains a solution of the 2022/23 Polimi's Data Structures and Algorithm final exam I wrote this summer to keep myself trained.
<br>


## Results

No official results can be given, as I could not take part in the actual exam. Nonetheless, my code runs in similar time and memory consumption to those that achieved honors grades.

## Goal

Consider an highway described as a sequence of service stations. Each service station is located at a distance from the beginning of the expressway expressed in kilometers by a positive integer or zero. There are no two service stations with the same distance: each service station is therefore uniquely identified by its distance from the beginning of the expressway. Each service station is equipped with a fleet of rental electric vehicles. Each vehicle is characterized by its range, given by a charge of the batteries, expressed in kilometers, by a positive integer. The fleet of vehicles at a single station comprises at most 512 vehicles. Renting a car from station s, it is possible to reach all those whose distance from s is less than or equal to the car's range. A journey is identified by a sequence of service stations where the driver makes a stop. It begins at a service station and ends at another, passing through zero or more intermediate stations. Assume that the driver cannot turn back during the journey and rents a new car whenever stopping at a service station. Therefore, given two consecutive stops s and t, t must always be farther from the start than s, and t must be reachable using one of the available vehicles at s. The goal of the project is as follows: given a pair of stations, plan the route with the fewest number of stops between them. In case there are multiple routes with the same minimum number of stops (i.e., tied), the route that favors stops with the shortest distance from the beginning of the expressway should be chosen. In other words, consider the set of n tied routes P = {p1, p2, . . . pn} where each route is a tuple of m elements pi = ⟨pi,1, pi,2, . . . pi,m⟩ representing the distance from the start of the expressway of each stop in order of traversal. The unique route pi should be chosen such that there is no other pj with the same k final stops preceded by a stop with a shorter distance, i.e., ∄j, k: ⟨pi,m−k+1, . . . pi,m⟩ = ⟨pj,m−k+1, . . . pj,m⟩ ∧ pj,m−k < pi,m−k. Here's an example of an expressway. In this example, the correct route between the station at distance 20 and the one at distance 50 is 20→30→50 (not 20→45→50). Similarly, 50→30→20 is the correct route between the station at distance 50 and the one at distance 20 (thus from right to left).

## Data Structures Description

The main data structure used to store the stations is a Red-Black Tree, implementing the CLRS Red-Black Trees pseudocode.

Cars parked in stations are stored in Linked Lists. This choice was initially made to keep the code simple and was never changed as the code proved to be fast enough. To achieve even better performance, cars may be stored using a priority queue structure, such as a MaxHeap.

## Algorithm Description

The calculation of the shortest path takes advantage of the order of the stations in the tree and visits each station at most twice.

The first visit occurs during the forward pass to find the farthest station that can be reached (to determine if the given destination is reachable), and the second visit adjusts the path as per the requirements of the assignment.

The lookup cost of the starting station, as well as the costs to find the previous or following nodes in the tree, have logarithmic costs.

The overall temporal complexity is $O(n \log n)$ and the space complexity is $O(n)$, where $n$ is the number of stored stations.

## A.Y. 2021-22 Solution

[WordChecker](https://github.com/Jacopopiazza/WordChecker)
