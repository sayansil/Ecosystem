import matplotlib.pyplot as plt
import numpy as np

max_age = 30

plot_for = 'Appetite'
unit = 'cal'

time = np.array(range(0,max_age))

age = time # integer (0, max_age)

max_height = 2 # float (0, inf)
base_height = 0.2 # float (0, max_height)
height_multiplier = 0.55 # float (0, 1)
height = np.minimum(np.maximum(0.5 * (1 + height_multiplier) * np.log(time + 1) - np.power(time / max_age, 2) + base_height, base_height), max_height) # float (0, inf)

max_weight = 65 # float (0, inf)
base_weight = 5 # float (0, max_weight)
weight_multiplier = 0.9 # float (0, 1)
weight = np.minimum(np.maximum(weight_multiplier * max_age * np.log(time + 1) - (0.5/max_age) * np.power(time , 2*(0.75 + weight_multiplier/2)) + base_weight, base_weight), max_weight) # float (0, inf)

base_vitality = 10 # float (0, inf)
vitality_multiplier = 0.75 # float (0, 1)
max_vitality = base_vitality * (vitality_multiplier * 0.5 * np.power(max_age, 0.5) * np.power(np.e, -np.power(time - max_age*0.5, 2)/(max_age * base_vitality)) + 1) # float (0, inf)

base_stamina = 25 # float (0, inf)
stamina_multiplier = 0.25 # float (0, 1)
max_stamina = base_stamina * (stamina_multiplier * 0.5 * np.power(max_age, 0.5) * np.power(np.e, -np.power(time - max_age*0.5, 2)/(max_age * base_stamina)) + 1) # float (0, inf)

speed_multiplier = 0.75 # float (0, 1)
base_speed = 2 # float (0, inf)
max_speed = speed_multiplier * 100 * np.power(np.e, (-1/(speed_multiplier * np.power(max_age, 1.5))) * np.power(time - max_age/2.5, 2)) + base_speed # float (0, inf)

base_appetite = 500 # int (0, inf)
max_appetite = base_appetite + base_appetite *np.power(np.e, (-0.5/np.power(max_age, 1.25)) * np.power(time - max_age/3, 2)) # int (0, inf)

age_on_death = 5 # float (0, inf)
death_prob = np.minimum(1, np.power(np.e, age_on_death*(time/max_age - 1))) # float (0, 1)

fig, ax = plt.subplots(4, 2, sharex=True, figsize=(20,15))

ax[0,0].plot(time, age, '-r')
ax[0,0].set_ylabel("Age (years)")
ax[0,1].plot(time, death_prob, '-b')
ax[0,1].set_ylabel("Death probability")
ax[1,0].plot(time, height, '-g')
ax[1,0].set_ylabel("Height (meters)")
ax[1,1].plot(time, weight, '-k')
ax[1,1].set_ylabel("Weight (kg)")
ax[2,0].plot(time, max_vitality, '-c')
ax[2,0].set_ylabel("Maximum Vitality (HP)")
ax[2,1].plot(time, max_stamina, '-y')
ax[2,1].set_ylabel("Maximum Stamina (Manna)")
ax[3,0].plot(time, max_speed, '-m')
ax[3,0].set_ylabel("Maximum speed (kmph)")
ax[3,0].set_xlabel("Time (years)")
ax[3,1].plot(time, max_appetite, '-k')
ax[3,1].set_ylabel("Maximum appetite (cal)")
ax[3,1].set_xlabel("Time (years)")

plt.show()

