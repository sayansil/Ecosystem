from flask import Flask, jsonify, request
import json
import sys
import os
ECOSYSTEM_ROOT = os.environ['ECOSYSTEM_ROOT']
sys.path.insert(1, os.path.join(ECOSYSTEM_ROOT, 'Cpp/build/python'))
import pyecosystem as pyeco

app = Flask('app')

available = True

with open('keys.json') as config_file:
  configs = json.load(config_file)
  keys = configs['API-KEYS']

@app.route('/query/<kingdom>/<species>')
def main(kingdom, species):
  global available
  global keys

  if available:
    key = request.values.get('api-key', '')
    if key not in keys:
      return jsonify({
        "status": "failure",
        "log": "invalid api key",
        "data": ""
      })

    available = False

    full_species_name = kingdom + '/' + species

    initial_organism_count = int(request.values.get('initial_count', '200'))
    years_to_simulate = int(request.values.get('years', '100'))

    print("Simulating " + str(years_to_simulate) + " years with " + \
      str(initial_organism_count) + " " + species + "(s) of kingdom " + kingdom + ".")

    obj = pyeco.pyecosystem(True)

    obj.reset_species(full_species_name)

    while initial_organism_count > 0:
        obj.spawn_organism(full_species_name, 10, False)
        initial_organism_count-=1

    while years_to_simulate > 0:
        obj.happy_new_year(True)
        obj.remember_species(full_species_name)
        years_to_simulate-=1

    data = obj.get_annual_data(full_species_name)

    available = True
    return jsonify({
      "status": "success",
      "log": "",
      "data": data
    })
  return jsonify({
    "status": "failure",
    "log": "Server Busy. A process is already running.",
    "data": ""
  })

@app.route('/')
def ok():
  return "Ok."

@app.route('/help/menu')
def menu():
  return jsonify({
    "animal":["deer", "lion"],
    "plant": ["bamboo"]
  })

@app.route('/help/schema')
def schema():
  return jsonify({
    "animal": ["YEAR", "MALE", "FEMALE", "M_MALE", "M_FEMALE", "C_PROB", \
        "M_AGE_START", "M_AGE_END", "MX_AGE", "MT_PROB", "OF_FACTOR", "AGE_DTH", \
        "FIT_DTH", "AFR_DTH", "HT_SP", "HT_ST", "HT_VT", "WT_SP", "WT_ST", "WT_VT", \
        "VT_AP", "VT_SP", "ST_AP", "ST_SP", "TMB_AP", "TMB_HT", "TMB_SP", "TMB_ST", \
        "TMB_VT", "TMB_WT", "TM_HT", "TM_SP", "TM_WT", "TMM_HT", "TMM_SP", "TMM_ST", \
        "TMM_VT", "TMM_WT", "SL_FACTOR", "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", \
        "AVG_WT", "AVGMA_AP", "AVGMA_SP", "AVGMA_ST", "AVGMA_VT", "AVG_SFIT", \
        "AVG_DTHF", "AVG_VIS"],
    "plant": ["YEAR", "POP", "M_POP", "C_PROB", "M_AGE_START", "M_AGE_END", "MX_AGE", \
        "MT_PROB", "OF_FACTOR", "AGE_DTH", "FIT_DTH", "AFR_DTH", "HT_VT", "WT_VT", \
        "TMB_HT", "TMB_VT", "TMB_WT", "TM_HT", "TM_WT", "TMM_HT", "TMM_VT", "TMM_WT", \
        "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", "AVG_WT", "AVGMA_VT", "AVG_SFIT", "AVG_DTHF"]
  })

@app.route('/available')
def status():
  global available
  return str(available)


app.run(host='0.0.0.0', port=8080)
