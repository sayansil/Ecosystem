import matplotlib.pyplot as plt

from herbivore import Deer

deer = Deer(Deer.get_random_chromosome_structure(), 1)

age = []
max_vitality_at_age = []
max_stamina_at_age = []
max_appetite_at_age = []
height = []
weight = []
fitness = []
death_probability = []

for i in range(50):
    d = deer.get_all_stats()
    age.append(i + 1)
    max_vitality_at_age.append(d['max_vitality_at_age'])
    max_stamina_at_age.append(d['max_stamina_at_age'])
    max_appetite_at_age.append(d['max_appetite_at_age'])
    height.append(d['height'])
    weight.append(d['weight'])
    fitness.append(d['fitness'])
    death_probability.append(d['death_probability'])
    deer.increment_age()

fig = plt.figure()

ax1 = fig.add_subplot(421)
ax1.plot(age, age)
ax1.set_xlabel('age')

ax2 = fig.add_subplot(422)
ax2.plot(age, fitness)
ax2.set_xlabel('fitness')

ax3 = fig.add_subplot(423)
ax3.plot(age, max_vitality_at_age)
ax3.set_xlabel('max_vitality_at_age')

ax4 = fig.add_subplot(424)
ax4.plot(age, max_stamina_at_age)
ax4.set_xlabel('max_stamina_at_age')

ax5 = fig.add_subplot(425)
ax5.plot(age, max_appetite_at_age)
ax5.set_xlabel('max_appetite_at_age')

ax6 = fig.add_subplot(426)
ax6.plot(age, height)
ax6.set_xlabel('height')

ax7 = fig.add_subplot(427)
ax7.plot(age, weight)
ax7.set_xlabel('weight')

ax8 = fig.add_subplot(428)
ax8.plot(age, death_probability)
ax8.set_xlabel('death_probability')

plt.show()