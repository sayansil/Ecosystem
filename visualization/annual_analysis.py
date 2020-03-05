import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
from matplotlib.backends.backend_pdf import PdfPages

kind = "deer"

filepath = "../data/csv/yearly_" + kind + ".csv"
df = pd.read_csv(filepath).dropna()

theoretical_columns = [
    'theoretical_maximum_base_height',
    'theoretical_maximum_base_weight',
    'theoretical_maximum_base_speed',
    'theoretical_maximum_base_appetite',
    'theoretical_maximum_base_stamina',
    'theoretical_maximum_base_vitality',
    'theoretical_maximum_height',
    'theoretical_maximum_weight',
    'theoretical_maximum_speed',
    'theoretical_maximum_speed_multiplier',
    'theoretical_maximum_height_multiplier',
    'theoretical_maximum_weight_multiplier',
    'theoretical_maximum_stamina_multiplier',
    'theoretical_maximum_vitality_multiplier']


def save():
    figs = []
    fig = get_mortality_graphs()
    figs.append(fig)
    fig = get_demographic_graphs()
    figs.append(fig)
    fig = get_copulation_graphs()
    figs.append(fig)
    fig = get_dependency_graphs()
    figs.append(fig)
    fig1, fig2 = get_average_graphs()
    figs.append(fig1)
    figs.append(fig2)

    with PdfPages('multipage_pdf.pdf') as pdf:
        for fig in figs:
            plt.figure(fig.number)
            pdf.savefig()

def get_mortality_graphs():
    """
        age_on_death
        fitness_on_death
        age_fitness_on_death_ratio
        max_age
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Mortality Graphs')
    gs = GridSpec(3, 2, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.set_title('Age affecting Death')
    x = df.index
    y = df['age_on_death']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[0, 1])
    ax.set_title('Fitness affecting Death')
    x = df.index
    y = df['fitness_on_death']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, :])
    ax.set_title('Age vs. Fitness affecting Death')
    x = df.index
    y = df['age_fitness_on_death_ratio']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Ratio")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, :])
    ax.set_title('Max age with time')
    x = df.index
    y = df['max_age']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Max age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_demographic_graphs():
    """
        male
        female
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Demographic Graphs')
    gs = GridSpec(1, 1, figure=fig)

    ax = fig.add_subplot(gs[:, :])
    ax.set_title('Population')
    x = df.index
    y = df['male']
    ax.plot(x, y, '-b', label='Male')
    x = df.index
    y = df['female']
    ax.plot(x, y, '-r', label='Female')
    ax.set_ylabel("Population")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_copulation_graphs():
    """
        matable_male
        matable_female
        mating_age_start
        mating_age_end
        conceiving_probability
        mutation_probability
        offsprings_factor
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Copulation Graphs')
    gs = GridSpec(4, 2, figure=fig)

    ax = fig.add_subplot(gs[0, :])
    ax.set_title('Matable population')
    x = df.index
    y = df['matable_male']
    ax.plot(x, y, '-b', label='Male')
    x = df.index
    y = df['matable_female']
    ax.plot(x, y, '-r', label='Female')
    ax.set_ylabel("Matable Population")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, 0])
    ax.set_title('Mating Start')
    x = df.index
    y = df['mating_age_start']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, -1])
    ax.set_title('Mating End')
    x = df.index
    y = df['mating_age_end']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Age")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, 0])
    ax.set_title('Mutation')
    x = df.index
    y = df['mutation_probability']
    ax.plot(x, y, '-r')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, -1])
    ax.set_title('Conceive')
    x = df.index
    y = df['conceiving_probability']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Probability")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[3, :])
    ax.set_title('Multiple offprings')
    x = df.index
    y = df['offsprings_factor']
    ax.plot(x, y, '-b')
    ax.set_ylabel("Factor")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_dependency_graphs():
    """
        height_on_stamina
        weight_on_stamina
        height_on_vitality
        weight_on_vitality
        height_on_speed
        weight_on_speed
        stamina_on_speed
        vitality_on_speed
        vitality_on_appetite
        stamina_on_appetite
    """
    fig = plt.figure()
    fig.set_size_inches(8, 10)
    fig.subplots_adjust(hspace=0.3)
    fig.subplots_adjust(wspace=0.3)
    fig.suptitle('Dependency Graphs')
    gs = GridSpec(3, 2, figure=fig)

    ax = fig.add_subplot(gs[0, 0])
    ax.set_title('Factors affecting Stamina')
    x = df.index
    y = df['height_on_stamina']
    ax.plot(x, y, '-r', label='Height')
    x = df.index
    y = df['weight_on_stamina']
    ax.plot(x, y, '-b', label='Weight')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[0, -1])
    ax.set_title('Factors affecting Vitality')
    x = df.index
    y = df['height_on_vitality']
    ax.plot(x, y, '-r', label='Height')
    x = df.index
    y = df['weight_on_vitality']
    ax.plot(x, y, '-b', label='Weight')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[1, :])
    ax.set_title('Factors affecting Speed')
    x = df.index
    y = df['height_on_speed']
    ax.plot(x, y, '-r', label='Height')
    x = df.index
    y = df['weight_on_speed']
    ax.plot(x, y, '-b', label='Weight')
    x = df.index
    y = df['stamina_on_speed']
    ax.plot(x, y, '-c', label='Stamina')
    x = df.index
    y = df['vitality_on_speed']
    ax.plot(x, y, '-g', label='Vitality')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig.add_subplot(gs[2, :])
    ax.set_title('Factors affecting Appetite')
    x = df.index
    y = df['vitality_on_appetite']
    ax.plot(x, y, '-g', label='Vitality')
    x = df.index
    y = df['stamina_on_appetite']
    ax.plot(x, y, '-c', label='Stamina')
    ax.set_ylabel("Factor")
    ax.legend(loc="upper right")
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig

def get_average_graphs():
    """
        average_generation
        average_age
        average_height
        average_weight
        average_static_fitness
        average_immunity
        average_death_factor
        average_max_speed_at_age
        average_max_appetite_at_age
        average_max_stamina_at_age
        average_max_vitality_at_age
        average_vision_radius
    """
    fig1 = plt.figure()
    fig1.set_size_inches(8, 10)
    fig1.subplots_adjust(hspace=0.3)
    fig1.subplots_adjust(wspace=0.9)
    fig1.suptitle('Average Graphs 1')
    gs = GridSpec(3, 6, figure=fig1)

    ax = fig1.add_subplot(gs[0, 0:3])
    ax.set_title('Generation')
    x = df.index
    y = df['average_generation']
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[0, 3:6])
    ax.set_title('Age')
    x = df.index
    y = df['average_age']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 0:3])
    ax.set_title('Height')
    x = df.index
    y = df['average_height']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[1, 3:6])
    ax.set_title('Weight')
    x = df.index
    y = df['average_weight']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[2, 0:2])
    ax.set_title('Static Fitness')
    x = df.index
    y = df['average_static_fitness']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[2, 2:4])
    ax.set_title('Immunity')
    x = df.index
    y = df['average_immunity']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig1.add_subplot(gs[2, 4:6])
    ax.set_title('Death Factor')
    x = df.index
    y = df['average_death_factor']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    fig2 = plt.figure()
    fig2.set_size_inches(8, 10)
    fig2.subplots_adjust(hspace=0.3)
    fig2.subplots_adjust(wspace=0.3)
    fig2.suptitle('Average Graphs 2')
    gs = GridSpec(3, 2, figure=fig2)

    ax = fig2.add_subplot(gs[0, 0])
    ax.set_title('Max speed at age')
    x = df.index
    y = df['average_max_speed_at_age']
    ax.plot(x, y, '-g')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[0, -1])
    ax.set_title('Max appetite at age')
    x = df.index
    y = df['average_max_appetite_at_age']
    ax.plot(x, y, '-b')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, 0])
    ax.set_title('Max stamina at age')
    x = df.index
    y = df['average_max_stamina_at_age']
    ax.plot(x, y, '-r')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[1, -1])
    ax.set_title('Max vitality at age')
    x = df.index
    y = df['average_max_vitality_at_age']
    ax.plot(x, y, '-c')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    ax = fig2.add_subplot(gs[2, :])
    ax.set_title('Max vision radius')
    x = df.index
    y = df['average_vision_radius']
    ax.plot(x, y, '-m')
    for tick in ax.get_yticklabels():
        tick.set_rotation(45)

    return fig1, fig2