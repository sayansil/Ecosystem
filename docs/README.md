
<p align="center">

![build][build-link] ![cpp][cpp-link] [![GPLv3 License](https://img.shields.io/badge/license-GPL%20v3-yellow.svg)][license-link] [![Hits][hits-link]](https://hits.seeyoufarm.com)


[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3821/badge)][cii-link]

</p>

# Overview

An ecosystem simulation tool where we create some organisms and leave them be. They adapt and improve every generation on their own. We, users can control ***directly controllable*** environmental features (like *climate* and *rainfall*, unlike *population*) from the top, and see what effect does it have on a future generation of the species.

# Abstract

>*“It is not the strongest or the most intelligent who will survive but those who can best manage change.”*
>           - **Leon C. Megginson**

Environmental data collected worldwide points to **one obvious fact**: *the conditions are going to change drastically over the next 50 - 100 years.* Our world today is plagued by a variety of environmental imbalances caused by the harmful activities of humans. Issues like *global warming, gradual rising of sea levels, climatic changes, extinction of animals* are wreaking havoc. Going by the saying, change is the only aspect of life which is constant, our ability to adapt to change will be thoroughly tested, failing to do so will lead to *human extinction.*

In this work, we present a novel **Ecosystem Simulator** where the user *plays the role of God*. Our toolkit helps the user **mimic a real-world ecosystem** where he can introduce new organisms with custom characteristics and also bring changes to the ecosystem and then observe their behaviour and reaction to those changes. Thus the user can be looked upon as The God of the ecosystem who has the power to bring any change with which he can **simulate the problems happening worldwide** and gain valuable insights on how the organisms try to adapt to the change to ensure their survival. Studying the overall behaviour of the ecosystem will provide us with a wealth of knowledge and be far better prepared on how we should be **equipped to handle the future.**

Let us start by understanding an ecosystem, which is defined as a *complex interrelationship of living organisms and their physical habitat together in a unit of space*. The abundance of life forces necessary for the thriving of living organisms has led to a diverse range of ecosystems in our planet which have been unpredictable till now. With the passage of time ecosystems may undergo drastic changes due to climatic stimulus or artificial interference. Many have tried to predict the end results of this constant change in ecosystems among which the most notable one is **Charles Darwin** with his **theory of evolution** and survival of the fittest. By the virtue of this theory, the one who is fit to adapt to the change in its surroundings will eventually survive and the rest will be filtered off, contributing to the evolution of the species as a whole in the new ecosystem. This picture of the future is rather *ambiguous* as it does not specifically tells us about the features or traits of the organisms which will be dominant by that time, leaving much to our imagination.

A system which could approximately figure out the generic trend of the traits in species given an ecosystem will not only help us study the future forms of the currently existing species but also help us distinguish rather endangered organisms among the rest. This however cannot be achieved using a single mathematical formula, rather a simulation of the organisms and their interdependence among one another and their surroundings over a period of time can help us arrive at a similar goal.



# Theoretical Assumptions

- The **births** and **deaths** of organisms belonging to a species occur on the same time of the year.

- The **fitness** of an organism (or unique *Chromosome*) in a species is defined by a **finite** and **exhaustive** set of values or parameters (called *Genes*).

- There will be a **new generation** of organisms every year without fail, unless there are no suitable organisms to breed.

- Organisms of same species may **crossbreed** with their parent or offspring to contribute to the diversity in the genepool.

- An organism may **survive** multiple generations, however the probability of death increases **exponentially** each passing unit of time.

# Representation of an Organism

## 1. Chromosomes

**Type :** ***ordered list of Bits***

**Structural  syntax :**

<table>
  <tr>
    <th>0</th>
    <th>1</th>
    <th>1</th>
    <th>1</th>
    <th>0</th>
    <th>0</th>
    <th>1</th>
    <th>...</th>
    <th>1</th>
    <th>0</th>
  </tr>
</table>

Set of finite values which exhaustively define all hereditary traits (or *possible traits*) in an organism, which does not change during their lifetime.
The smallest unit of a **chromosome** is called a ***gene***.
The **final bit** in chromosome shall represent the ***gender*** of the organism.

## 2. Non - hereditary Traits

**Type :** ***unordered list of Float***

**Structural syntax :**

<table>
  <tr>
    <th>
    <table>
    <tr>
    <th>15
    </th>
    </tr>
    </table>
    </th>
    <th>
    <table>
    <tr>
    <th>1.1
    </th>
    </tr>
    </table>
    </th>
    <th>
    <table>
    <tr>
    <th>73
    </th>
    </tr>
    </table>
    </th>
    <th>...</th>
    <th><table>
    <tr>
    <th>1000
    </th>
    </tr>
    </table>
    </th>
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

**Type :** ***unordered list of Float***

**Structural syntax :**

<table>
  <tr>
    <th>
    <table>
    <tr>
    <th>45
    </th>
    </tr>
    </table>
    </th>
    <th>
    <table>
    <tr>
    <th>81
    </th>
    </tr>
    </table>
    </th>
    <th>
    <table>
    <tr>
    <th>27
    </th>
    </tr>
    </table>
    </th>
    <th>...</th>
    <th><table>
    <tr>
    <th>100
    </th>
    </tr>
    </table>
    </th>
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

**Type :** ***Integer***

Denotes the generation to which the organism was born. Organisms belonging to the same **generation** are termed ***siblings***. Organisms born to the same parent are ***direct siblings***.

## 5. Fitness

**Type :** ***Float***

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

![Height dependency](https://i.ibb.co/qmfk6MX/height.png)

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



[build-link]: https://github.com/sayansil/Ecosystem/workflows/build/badge.svg?branch=ubuntu
[license-link]: https://github.com/sayansil/Ecosystem/blob/master/LICENSE
[hits-link]: https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fsayansil%2FEcosystem
[cpp-link]: https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B
[cii-link]: https://bestpractices.coreinfrastructure.org/projects/3821
