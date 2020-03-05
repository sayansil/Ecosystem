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
copulation_columns = [
    'matable_male',
    'matable_female',
    'conceiving_probability',
    'mating_age_start',
    'mating_age_end',
    'mutation_probability',
    'offsprings_factor']
demographic_columns = [
    'male',
    'female'
]


def get_mortality_plot(df):
    """
        age_on_death
        fitness_on_death
        age_fitness_on_death_ratio
        max_age
    """
    fig, axs = plt.subplots(3, sharex=True)
    fig.set_size_inches(8.27, 11.69)
    fig.subplots_adjust(hspace=0.2)
    fig.suptitle('Mortality Graphs')

    axs[0].set_title('Factors affecting Death')
    x = df.index
    y = df['age_on_death']
    axs[0].plot(x, y, '-r')
    x = df.index
    y = df['fitness_on_death']
    axs[0].plot(x, y, '-b')
    axs[0].set_ylabel("Factors")
    for tick in axs[0].get_yticklabels():
        tick.set_rotation(45)

    axs[1].set_title('Age vs. Fitness affecting Death')
    x = df.index
    y = df['age_fitness_on_death_ratio']
    axs[1].plot(x, y, '-b')
    axs[1].set_ylabel("Ratio")
    for tick in axs[1].get_yticklabels():
        tick.set_rotation(45)

    axs[2].set_title('Max age with time')
    x = df.index
    y = df['max_age']
    axs[2].plot(x, y, '-b')
    axs[2].set_ylabel("Max age")
    for tick in axs[1].get_yticklabels():
        tick.set_rotation(45)