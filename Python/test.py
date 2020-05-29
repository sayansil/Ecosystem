from flask import Flask
import sys
import os
ECOSYSTEM_ROOT = os.environ['ECOSYSTEM_ROOT']
sys.path.insert(1, os.path.join(ECOSYSTEM_ROOT, 'Cpp/build'))
import pyecosystem as pyeco

app = Flask('app')

@app.route('/test')
def main():
  initial_organism_count = 200
  years_to_simulate = 10

  obj = pyeco.pyecosystem(True)

  while initial_organism_count > 0:
      obj.spawn_animal('deer', 10, False)
      initial_organism_count-=1

  while years_to_simulate > 0:
      obj.happy_new_year(True)
      obj.remember_species('animal/deer')
      years_to_simulate-=1

  data = obj.get_annual_data('animal/deer')
  return data

@app.route('/')
def ok():
  return "Tested. Ok."

app.run(host='0.0.0.0', port=8080)