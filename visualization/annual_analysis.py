import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

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
average_columns = [
    'average_generation',
    'average_age',
    'average_height',
    'average_weight',
    'average_max_speed_at_age',
    'average_max_appetite_at_age',
    'average_max_stamina_at_age',
    'average_max_vitality_at_age',
    'average_static_fitness',
    'average_immunity',
    'average_death_factor',
    'average_vision_radius']
dependency_columns = [
    'height_on_stamina',
    'weight_on_stamina',
    'height_on_vitality',
    'weight_on_vitality',
    'vitality_on_appetite',
    'stamina_on_appetite',
    'height_on_speed',
    'weight_on_speed',
    'stamina_on_speed',
    'vitality_on_speed',
    'age_on_death',
    'fitness_on_death',
    'age_fitness_on_death_ratio']

def get_mortality_plot():
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

    ax1 = fig.add_subplot(gs[0, 0])
    ax1.set_title('Age affecting Death')
    x = df.index
    y = df['age_on_death']
    ax1.plot(x, y, '-r')
    ax1.set_ylabel("Factor")
    for tick in ax1.get_yticklabels():
        tick.set_rotation(45)

    ax2 = fig.add_subplot(gs[0, 1])
    ax2.set_title('Fitness affecting Death')
    x = df.index
    y = df['fitness_on_death']
    ax2.plot(x, y, '-b')
    ax2.set_ylabel("Factor")
    for tick in ax2.get_yticklabels():
        tick.set_rotation(45)

    ax3 = fig.add_subplot(gs[1, :])
    ax3.set_title('Age vs. Fitness affecting Death')
    x = df.index
    y = df['age_fitness_on_death_ratio']
    ax3.plot(x, y, '-b')
    ax3.set_ylabel("Ratio")
    for tick in ax3.get_yticklabels():
        tick.set_rotation(45)

    ax4 = fig.add_subplot(gs[2, :])
    ax4.set_title('Max age with time')
    x = df.index
    y = df['max_age']
    ax4.plot(x, y, '-b')
    ax4.set_ylabel("Max age")
    for tick in ax4.get_yticklabels():
        tick.set_rotation(45)

    plt.show()

def get_demographic_plot():
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

    ax1 = fig.add_subplot(gs[:, :])
    ax1.set_title('Population')
    x = df.index
    y = df['male']
    ax1.plot(x, y, '-b', label='Male')
    x = df.index
    y = df['female']
    ax1.plot(x, y, '-r', label='Female')
    ax1.set_ylabel("Population")
    ax1.legend(loc="upper right")
    for tick in ax1.get_yticklabels():
        tick.set_rotation(45)

    plt.show()

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

    ax1 = fig.add_subplot(gs[0, :])
    ax1.set_title('Matable population')
    x = df.index
    y = df['matable_male']
    ax1.plot(x, y, '-b', label='Male')
    x = df.index
    y = df['matable_female']
    ax1.plot(x, y, '-r', label='Female')
    ax1.set_ylabel("Matable Population")
    ax1.legend(loc="upper right")
    for tick in ax1.get_yticklabels():
        tick.set_rotation(45)

    ax2 = fig.add_subplot(gs[1, 0])
    ax2.set_title('Mating Start')
    x = df.index
    y = df['mating_age_start']
    ax2.plot(x, y, '-b')
    ax2.set_ylabel("Age")
    for tick in ax2.get_yticklabels():
        tick.set_rotation(45)

    ax3 = fig.add_subplot(gs[1, -1])
    ax3.set_title('Mating End')
    x = df.index
    y = df['mating_age_end']
    ax3.plot(x, y, '-r')
    ax3.set_ylabel("Age")
    for tick in ax3.get_yticklabels():
        tick.set_rotation(45)

    ax4 = fig.add_subplot(gs[2, 0])
    ax4.set_title('Mutation')
    x = df.index
    y = df['mutation_probability']
    ax4.plot(x, y, '-r')
    ax4.set_ylabel("Probability")
    for tick in ax4.get_yticklabels():
        tick.set_rotation(45)

    ax5 = fig.add_subplot(gs[2, -1])
    ax5.set_title('Conceive')
    x = df.index
    y = df['conceiving_probability']
    ax5.plot(x, y, '-b')
    ax5.set_ylabel("Probability")
    for tick in ax5.get_yticklabels():
        tick.set_rotation(45)

    ax6 = fig.add_subplot(gs[3, :])
    ax6.set_title('Multiple offprings')
    x = df.index
    y = df['offsprings_factor']
    ax6.plot(x, y, '-b')
    ax6.set_ylabel("Factor")
    for tick in ax6.get_yticklabels():
        tick.set_rotation(45)

    plt.show()