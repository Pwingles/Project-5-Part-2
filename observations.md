# Observations

## Run 1: Single Probing, Compaction OFF (N=1024)
13 and a half million total probes, with an average number of 545 probes.

I notice taht the effective load factor is full meaning that its either active or tombstones in the list.

The table size is only 1279 in which the avg probe is half of that. so every new element inserted has to traverse potentially half the list.

This is terribly inefficient.

**Metrics:**
- **Load Factor:** 80%
- **Effective Load Factor:** 100%
- **Tombstone Fraction:** 19%
- **Free Fraction:** 0%
- **Avg Probes:** 547.32
- **Compactions:** 0

## Run 2: Single Probing, Compaction ON (N=1024)
First this I noticed when viewing each compaction is that the avg number of probes goes up quickly for the first few until it plateaus around 15 going into 16.

Comparing Single No compaction to single compaction I can see that Compaction has a significant effect on the Average number of propes of only 16 as opposed to the previous 545. Which brings the total amount of probes to 394099

and with compaction the effective load factor never hits 100%, ending the run with 92%, with only 12% of that being tombstone.
The elements available map now shoes Dashes showing more breaking points.

This whole trial took a total of 26 compactions.

**Metrics:**
- **Load Factor:** 80%
- **Effective Load Factor:** 92%
- **Tombstone Fraction:** 12%
- **Free Fraction:** 7%
- **Avg Probes:** 16.18
- **Compactions:** 26

## Run 3: Double Probing, Compaction OFF (N=1024)
With double probing no compaction, I can see that we have roughly the same amount of Avg number of probes and Single probing no compaction.
with 13.7 million probes it is very similar to single probe no compaction as well.

I believe this is due to the nature smaller table size compared to number of elements being inserted. So more duplicate hashing will happen until it finds a spot.

The effective load factor hits 100%

with 19% being tombstone.

Double probing doesn't seem to function well without compaction either.

**Metrics:**
- **Load Factor:** 80%
- **Effective Load Factor:** 100%
- **Tombstone Fraction:** 19%
- **Free Fraction:** 0%
- **Avg Probes:** ~547 (Similar to Run 1)
- **Compactions:** 0

## Run 4: Double Probing, Compaction ON (N=1024)
I noticed that double probing with compaction has a overall lower avg number of compactions then the other 3 methods.
at roughly 6.4

The totcal compaction count is 32 which required more than single proping

with a tomstone fraction of only 13%

Looking at the maps you can see the clusters are shorter in length. and the elements deleted map have less to it. But the most important map is the elements available map is very scattered.

This is the best method yet with only 157566 probes.

**Metrics:**
- **Load Factor:** 80%
- **Effective Load Factor:** 93%
- **Tombstone Fraction:** 13%
- **Free Fraction:** 6%
- **Avg Probes:** 6.43
- **Compactions:** 32

## D3 Histogram Analysis (N=32768, Double Probing)
the blue bars count gradually grew in size each cluster peaking at 17-32 runs an then slowly diminsihing until 129-193 bin

While the orange bars are significantly taller than the blue bars from the 1-16 bins being the tallest and dropping off at 64 to non existent

ohh so the run length bins are hte clusters. meaning we have a higher volume of clusters after compaction

## D3 LRU Profile Analysis
When viewing into the LRU profiles I notice that the average probing between double hasing with compact vs single hashing with compaction that The av probes on the double is aobut 1/3 of the amount compared to single probing.

In terms of elapsed time I would have assumed that single hasing would have been less efficient with the amount of "avg" probes but as the size of N got larger it seems double hashing costed more time.

wheN i veiew occupancy tried i can see that the tombstones for both hover below 15% and i believe that this is due to the compaction constantly cleaning up the tombstones before it can reach a higher %
