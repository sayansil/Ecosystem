
<p align="center">

<a href="https://github.com/sayansil/Ecosystem">
  <img src="https://raw.githubusercontent.com/sayansil/Ecosystem/master/docs/res/eco.png" />
</a>

</p>



<!-- Badges -->

<p align="center">
<a href="https://github.com/sayansil/Ecosystem/actions">
        <img src="https://github.com/sayansil/Ecosystem/workflows/build/badge.svg?branch=ubuntu" /></a>

<a href="https://github.com/sayansil/Ecosystem/actions">
        <img src="https://github.com/sayansil/Ecosystem/workflows/test/badge.svg?branch=ubuntu" /></a>


<a href="https://github.com/sayansil/Ecosystem/blob/master/LICENSE">
        <img src="https://img.shields.io/badge/license-GPL%20v3-yellow.svg" /></a>
        
<a href="https://www.codefactor.io/repository/github/sayansil/ecosystem/overview/master">
        <img src="https://www.codefactor.io/repository/github/sayansil/ecosystem/badge/master" alt="CodeFactor" /></a>
</p>


<p align="center">
<a href="https://bestpractices.coreinfrastructure.org/projects/3821">
        <img src="https://bestpractices.coreinfrastructure.org/projects/3821/badge" /></a>

<a href="https://isocpp.org/files/papers/p0636r0.html">
        <img src="https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B" /></a>

</p>

# Overview

An ecosystem simulation tool where we create some organisms and leave them be. They adapt and improve every generation on their own. We, users can control ***directly controllable*** environmental features (like *climate* and *rainfall*, unlike *population*) from the top, and see what effect does it have on a future generation of the species.


# Theoretical Assumptions

- The **births** and **deaths** of organisms belonging to a species occur on the same time of the year.

- The **fitness** of an organism (or unique *Chromosome*) in a species is defined by a **finite** and **exhaustive** set of values or parameters (called *Genes*).

- There will be a **new generation** of organisms every year without fail, unless there are no suitable organisms to breed.

- Organisms of same species may **crossbreed** with their parent or offspring to contribute to the diversity in the genepool.

- An organism may **survive** multiple generations, however the probability of death increases **exponentially** each passing unit of time.

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


# Usage

### Requirements

- For visualization,

```shell
pip install -r visualization/requirements.txt
```

- For webapp,

```shell
pip install -r Python/requirements.txt
```

### Setup

- Navigate to the root directory of the project and set the environment variable `ECOSYSTEM_ROOT`

```shell
cd Ecosystem
export ECOSYSTEM_ROOT=`pwd`
```
- Import all git submodules

```shell
git submodule update --recursive --remote
```

- Create a **build** directory in `Cpp` and build the project using `CMake`

```shell
cd Cpp & mkdir build & cd build
cmake ..
make
```

- Create folders with the name of required species in the directory of their kingdom.

```shell
mkdir ../../data/json/animal/<animal-species>
mkdir ../../data/json/plant/<plant-species>
```
For example,

```shell
mkdir ../../data/json/animal/deer
mkdir ../../data/json/plant/bamboo
mkdir ../../data/json/animal/lion
```

- Run the setup file generated in the build directory to create/populate required files and databases

```shell
./setup/setup
```

- Open and edit the pre-populated `base.json` and `modify.json` files for the species which were defined earlier. The files will be found inside the directories `data/json/<kingdom>/<species>`

