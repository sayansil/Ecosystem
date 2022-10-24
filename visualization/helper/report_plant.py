import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec


def get_figs(year_map: dict) -> list:
    figs = []
    
    figs.extend(get_demographic_graphs(year_map))
    figs.extend(get_copulation_graphs(year_map))
    figs.extend(get_dependency_graphs(year_map))
    figs.extend(get_average_graphs(year_map))
    figs.extend(get_theoretical_graphs(year_map))

    return figs


def get_XY(year_map: dict, key: str):
    X, Y = [], []

    for year, species_data in year_map.items():
        X.append(int(year))
        Y.append(float(getattr(species_data.get('stats'), key)))
    
    return X, Y

def get_demographic_graphs(year_map: dict) -> list:
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Demographic Graphs')
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[:, :])
    ax.set_title('Population')
    x, y = [], []
    for year, species_data in year_map.items():
        x.append(int(year))
        y.append(
            int(species_data.get('population').matablePopulation.malePopulation) + 
            int(species_data.get('population').nonMatablePopulation.malePopulation) +
            int(species_data.get('population').matablePopulation.femalePopulation) + 
            int(species_data.get('population').nonMatablePopulation.femalePopulation)
        )
    ax.plot(x, y, '-b')
    ax.set_ylabel("Population")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return [fig]

def get_copulation_graphs(year_map: dict) -> list:
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Copulation Graphs')
    gs = GridSpec(4, 2, figure=fig)

    ax = fig.add_subplot(gs[0, :])
    ax.set_title('Matable population')
    x, y = [], []
    for year, species_data in year_map.items():
        x.append(int(year))
        y.append(
            int(species_data.get('population').matablePopulation.malePopulation) +
            int(species_data.get('population').matablePopulation.femalePopulation)
        )
    ax.plot(x, y, '-r')
    ax.set_ylabel("Matable Population")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, 0])
    ax.set_title('Mating Start')
    x, y = get_XY(year_map, "matingAgeStart")
    ax.plot(x, y, '-b')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, -1])
    ax.set_title('Mating End')
    x, y = get_XY(year_map, "matingAgeEnd")
    ax.plot(x, y, '-r')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, 0])
    ax.set_title('Mutation')
    x, y = get_XY(year_map, "mutationProbability")
    ax.plot(x, y, '-r')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, -1])
    ax.set_title('Conceive')
    x, y = get_XY(year_map, "conceivingProbability")
    ax.plot(x, y, '-b')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[3, :])
    ax.set_title('Multiple offprings')
    x, y = get_XY(year_map, "offspringsFactor")
    ax.plot(x, y, '-b')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return [fig]

def get_dependency_graphs(year_map: dict) -> list:
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Dependency Graphs')
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.set_title('Factors affecting Vitality')
    x, y = get_XY(year_map, "heightOnVitality")
    ax.plot(x, y, '-r', label='Height')
    x, y = get_XY(year_map, "weightOnVitality")
    ax.plot(x, y, '-b', label='Weight')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_average_graphs(year_map: dict) -> list:
    fig1 = plt.figure()
    fig1.set_size_inches(8, 10)
    fig1.subplots_adjust(hspace=0.3)
    fig1.subplots_adjust(wspace=0.3)
    fig1.suptitle('Average Graphs 1')
    gs = GridSpec(2, 2, figure=fig1)

    ax = fig1.add_subplot(gs[0, 0])
    ax.set_title('Generation')
    x, y = get_XY(year_map, "generation")
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[0, 1])
    ax.set_title('Age')
    x, y = get_XY(year_map, "age")
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 0])
    ax.set_title('Height')
    x, y = get_XY(year_map, "height")
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 1])
    ax.set_title('Weight')
    x, y = get_XY(year_map, "weight")
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    fig2 = plt.figure()
    fig2.set_size_inches(8, 10)
    fig2.subplots_adjust(hspace=0.3)
    fig2.subplots_adjust(wspace=0.3)
    fig2.suptitle('Average Graphs 2')
    gs = GridSpec(2, 2, figure=fig2)

    ax = fig2.add_subplot(gs[0, 0])
    ax.set_title('Max vitality at age')
    x, y = get_XY(year_map, "maxVitalityAtAge")
    ax.plot(x, y, '-c')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)
    
    ax = fig2.add_subplot(gs[0, -1])
    ax.set_title('Max age with time')
    x, y = get_XY(year_map, "maxAge")
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, 0])
    ax.set_title('Static Fitness')
    x, y = get_XY(year_map, "staticFitness")
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, -1])
    ax.set_title('Immunity')
    x, y = get_XY(year_map, "immunity")
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return [fig1, fig2]

def get_theoretical_graphs(year_map: dict) -> list:
    fig1 = plt.figure()
    fig1.set_size_inches(8, 10)
    fig1.subplots_adjust(hspace=0.3)
    fig1.subplots_adjust(wspace=0.3)
    fig1.suptitle('Theoretical Graphs 1')
    gs = GridSpec(3, 2, figure=fig1)

    ax = fig1.add_subplot(gs[0, 0])
    ax.set_title('Max base height')
    x, y = get_XY(year_map, "theoreticalMaximumBaseHeight")
    ax.plot(x, y, '-b')
    ax.set_ylabel("meters")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[0, -1])
    ax.set_title('Max height')
    x, y = get_XY(year_map, "theoreticalMaximumHeight")
    ax.plot(x, y, '-b')
    ax.set_ylabel("meters")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 0])
    ax.set_title('Max base weight')
    x, y = get_XY(year_map, "theoreticalMaximumBaseWeight")
    ax.plot(x, y, '-r')
    ax.set_ylabel("kg")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, -1])
    ax.set_title('Max weight')
    x, y = get_XY(year_map, "theoreticalMaximumWeight")
    ax.plot(x, y, '-r')
    ax.set_ylabel("kg")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[2, 0:2])
    ax.set_title('Max base vitality')
    x, y = get_XY(year_map, "theoreticalMaximumBaseVitality")
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    fig2 = plt.figure()
    fig2.set_size_inches(8, 10)
    fig2.subplots_adjust(hspace=0.3)
    fig2.subplots_adjust(wspace=0.3)
    fig2.suptitle('Theoretical Graphs 2')
    gs = GridSpec(2, 2, figure=fig2)

    ax = fig2.add_subplot(gs[0, 0:2])
    ax.set_title('Max vitality multiplier')
    x, y = get_XY(year_map, "theoreticalMaximumVitalityMultiplier")
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, 0])
    ax.set_title('Max height multiplier')
    x, y = get_XY(year_map, "theoreticalMaximumHeightMultiplier")
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, -1])
    ax.set_title('Max weight multiplier')
    x, y = get_XY(year_map, "theoreticalMaximumWeightMultiplier")
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig1, fig2
