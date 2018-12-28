
# Overview

## Theoretical Assumptions

- The **births** and **deaths** of organisms belonging to a species occur on the same time of the year.  

- The **fitness** of an organism (or unique *Chromosome*) in a species is defined by a **finite** and **exhaustive** set of values or parameters (called *Genes*).

- There will be a **new generation** of organisms every year without fail, unless there are no suitable organisms to breed.

- Organisms of same species may **crossbreed** with their parent or offspring to contribute to the diversity in the genepool.

- An organism may **survive** multiple generations, however the probability of death increases **exponentially** each passing unit of time.

# Representation of an Organism

## 1. Chromosomes

**Structural  syntax :**

|0   |1   |1   |1   |0   |0   |1   |... |1   |0   |  
|----|----|----|----|----|----|----|----|----|----|  

Set of finite values which exhaustively define all hereditary traits (or *possible traits*) in an organism, which does not change during their lifetime.
The smallest unit of a **chromosome** is called a ***gene***.
The **final bit** in chromosome shall represent the ***gender*** of the organism.

## 2. Non - hereditary Traits

**Structural syntax :**

<table>
<tr>
<td>

|15  |
|----|

</td>
<td>

|110 |
|----|

</td>
<td>

|73  |
|----|

</td>
<td>

|... |
|----|

</td>
<td>

|1000  |
|-----|

</td>
</tr>
</table>

Set of finite values which exhaustively define all non-hereditary traits (or *possible traits*) in an organism, which are learned and updated ***gradually*** throughout their lifetime as a **function of time**.

**Example traits :**

|Trait        |Value|
|-------------|-----|
|age          |15   |
|height       |1.1  |
|weight       |73   |
|...          |...  |
|max appetite |1000 |

## 3. Dynamic traits

**Structural syntax :**

<table>
<tr>
<td>

|45  |
|----|

</td>
<td>

|81  |
|----|

</td>
<td>

|27  |
|----|

</td>
<td>

|... |
|----|

</td>
<td>

|100  |
|-----|

</td>
</tr>
</table>

Set of finite values which exhaustively define all dynamic traits (or *possible traits*) in an organism, which changes ***rapidly*** as an effect of interaction with their surroundings as a **function of time**.

**Example traits :**

|Trait      |Value |
|-----------|------|
|speed      |45    |
|vitality   |81    |
|stamina    |27    |
|...        |...   |
|appetite   |100   |

## 4. Generation

|5  |
|---|

Denotes the generation to which the organism was born. Organisms belonging to the same **generation** are termed ***siblings***. Organisms born to the same parent are ***direct siblings***.

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

# Time-Dependency of traits

## Age

**Relation :**

![Age dependency](https://i.ibb.co/FY1nXKy/age.png)

***where,***

**t** = time

![Age dependency](https://i.ibb.co/TrC2Q8t/herbivore-age.png)

## Death probability

**Relation :**

![Death dependency](https://i.ibb.co/8788Hff/death.png)

***where,***

**t** = time
**a, b** = constants

![Death dependency](https://i.ibb.co/QQrVkQy/herbivore-death-prob.png)

## Height

**Relation :**

![Height dependency](https://i.ibb.co/y645rDx/height.png)

***where,***

**t** = time
**a, b, c** = constants

![Height dependency](https://i.ibb.co/ZKcDZPK/herbivore-height.png)

## Weight

**Relation :**

![Weight dependency](https://i.ibb.co/J71x0H8/weight.png)

***where,***

**t** = time
**a, b, c, d** = constants

![Weight dependency](https://i.ibb.co/wzRCPGy/herbivore-weight.png)

## Maximum vitality

**Relation :**

![Maximum Vitality dependency](https://i.ibb.co/cxwh3zr/vitality.png)

***where,***

**t** = time
**a, b, c, d** = constants

![Maximum Vitality dependency](https://i.ibb.co/QvYjVQj/herbivore-max-vit.png)

## Maximum stamina

**Relation :**

![Maximum stamina dependency](https://i.ibb.co/5sxJvBG/stamina.png)

***where,***

**t** = time
**a, b, c, d** = constants

![Maximum stamina dependency](https://i.ibb.co/PcsN2rT/herbivore-max-sta.png)

## Maximum speed

**Relation :**

![Maximum speed dependency](https://i.ibb.co/XWMPvyd/speed.png)

***where,***

**t** = time
**a, b, c, d** = constants

![Maximum speed dependency](https://i.ibb.co/VjwMffP/herbivore-max-speed.png)

## Maximum appetite

**Relation :**

![Maximum appetite dependency](https://i.ibb.co/ZX3HYym/appetite.png)

***where,***

**t** = time
**a, b, c** = constants

![Maximum appetite dependency](https://i.ibb.co/FWsCmb2/herbivore-max-app.png)
