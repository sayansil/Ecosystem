
# Assumptions

## Theoretical Assumptions

- The **births** and **deaths** of organisms belonging to a species occur on the same time of the year.  

- The **fitness** of an organism (unique *Chromosome*) in a species is defined by a **finite** and **exhaustive** set of values or parameters (called *Genes*).

- There will be a **new generation** of organisms every year without fail, unless there are no suitable organisms to breed.

- Organisms of same species may **crossbreed** with their parent or offspring to contribute to the diversity in the genepool.

- An organism may **survive** multiple generations, however the probability of death increases **exponentially** each passing unit of time.

# Representation of an Organism

## 1. Chromosomes

|0   |1   |1   |1   |0   |0   |1   |... |1   |0   |  
|----|----|----|----|----|----|----|----|----|----|  

Set of finite values which exhaustively define all hereditary traits (or *possible traits*) in an organism, which does not change during their lifetime.
The smallest unit of a **chromosome** is called a ***gene***.
The **final bit** in chromosome shall represent the ***gender*** of the organism.

## 2. Non - hereditary Traits

|0   |0   |1   |0   |1   |0   |1   |... |1   |1   |
|----|----|----|----|----|----|----|----|----|----|

Set of finite values which exhaustively define all non-hereditary traits (or *possible traits*) in an organism, which are learned and updated throughout their lifetime.

## 3. Generation

|5  |
|---|

Denotes the generation to which the organism was born. Organisms belonging to the same **generation** are termed ***siblings***. Organisms born to the same parent are ***direct siblings***.

## 4. Age

|2  |
|---|

Number of years since birth.

```AGE = recent GENERATION - individual GENERATION```

## 5. Fitness

|0.52  |
|------|

A measure of survivality of the organism with respect to their species. A value of **0** denotes ***inevitable extinction***, and a value of **1** denotes an ***ideal organism*** in the proposed ecosystem.

# Mechanism

## Initialization

Initialize the ecosystem with some amount of **randomly generated** organisms (each with a specific **chromosome**) for every species, keeping their traits under acceptable and feasible bounds.

## Adaptation

Changes in **non-hereditary traits** as a stimuli to environmental factors. The rate or extent of change may be affected by the **chromosome** of the organism.

## Evaluation

Calculate the **fitness** of each living organism by their ability to *sustain* and *survive*, and assign the evaluated fitness score to it.

## Cross-over

Breeding of pairs of organisms selected at **random** (or *guided random*) to produce zero, one or multiple offsprings following the next **generation**. The **chromosomes** of the offsprings are directly derived from that of their parent's **chromosomes**.

## Mutation

Slight modification at random to the **genome** of the offsprings born in the current generation, to maintain diversity in the **genepool** of the species and not let one **chromosome** *dominate* the **population**.

## Selection

The organisms from past **generations** are removed to raise the **average fitness** of the species in the ecosystem. The **fitness** and **non-hereditary traits** may have a direct effect on the **selection** of an individual to survive another year.

## Repeat

The process consisting of steps **Adaptation** to **Selection** repeats itself forever, unless the species becomes extinct after a **Selection** wipes off all organisms of the speceies.