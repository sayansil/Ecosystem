
# Assumptions

## Theoretical Assumptions

- The **births** and **deaths** of organisms belonging to a species occur on the same time of the year.  

- The **fitness** of an organism (unique *Chromosome*) in a species is defined by a **finite** and **exhaustive** set of values or parameters (called *Genes*).

- There will be a **new generation** of organisms every year without fail, unless there are no suitable organisms to breed.

- Organisms of same species may **crossbreed** with their parent or offspring to contribute to the diversity in the genepool.

- An organism may **survive** multiple generations, however the probability of death increases **exponentially** each passing unit of time.

# Representation of an Organism

## 1. Chromosome Structure

|0   |1   |1   |1   |0   |0   |1   |... |1   |0   |  
|----|----|----|----|----|----|----|----|----|----|  

Set of finite values which exhaustively define all traits (or *possible traits*) in an organism.
The smallest unit of a **chromosome** is called a ***gene***.

## 2. Generation

|5  |
|---|

Denotes the generation to which the organism was born. Organisms belonging to the same **generation** are termed ***siblings***. Organisms born to the same parent are ***direct siblings***.

## Age

|2  |
|---|

Number of years since birth.

```AGE = recent GENERATION - individual GENERATION```

## Fitness

|0.52  |
|------|

A measure of survivality of the organism with respect to their species. A value of **0** denotes ***inevitable extinction***, and a value of **1** denotes an ***ideal organism*** in the proposed ecosystem.