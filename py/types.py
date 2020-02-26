type_match = {}
types = ['Normal','Fire','Water','Electric','Grass','Ice','Fighting','Poison','Ground','Flying','Psychic','Bug','Rock','Dragon','Ghost']
for type1 in types:
  type_match[type1] = {}
  for type2 in types:
    type_match[type1][type2] = 1

type_match['Normal']['Rock'] = .5
type_match['Normal']['Ghost'] = 0
type_match['Fire']['Fire'] = .5
type_match['Fire']['Water'] = .5
type_match['Fire']['Grass'] = 2
type_match['Fire']['Ice'] = 2
type_match['Fire']['Bug'] = 2
type_match['Fire']['Rock'] = .5
type_match['Fire']['Dragon'] = .5
type_match['Water']['Fire'] = 2
type_match['Water']['Water'] = .5
type_match['Water']['Grass'] = .5
type_match['Water']['Ground'] = 2
type_match['Water']['Rock'] = 2
type_match['Water']['Dragon'] = .5
type_match['Electric']['Water'] = 2
type_match['Electric']['Electric'] = .5
type_match['Electric']['Grass'] = .5
type_match['Electric']['Ground'] = 0
type_match['Electric']['Flying'] = 2
type_match['Electric']['Dragon'] = .5
type_match['Grass']['Fire'] = .5
type_match['Grass']['Water'] = 2
type_match['Grass']['Grass'] = .5
type_match['Grass']['Poison'] = .5
type_match['Grass']['Ground'] = 2
type_match['Grass']['Flying'] = .5
type_match['Grass']['Bug'] = .4
type_match['Grass']['Rock'] = 2
type_match['Grass']['Dragon'] = .5
type_match['Ice']['Water'] = .5
type_match['Ice']['Grass'] = 2
type_match['Ice']['Ice'] = .5
type_match['Ice']['Flying'] = 2
type_match['Ice']['Dragon'] = 2
type_match['Fighting']['Normal'] = 2
type_match['Fighting']['Ice'] = 2
type_match['Fighting']['Poison'] = .5
type_match['Fighting']['Flying'] = .5
type_match['Fighting']['Psychic'] = .5
type_match['Fighting']['Bug'] = .5
type_match['Fighting']['Rock'] = 2
type_match['Fighting']['Ghost'] = 0
type_match['Poison']['Grass'] = 2
type_match['Poison']['Poison'] = .5
type_match['Poison']['Ground'] = .5
type_match['Poison']['Bug'] = 2
type_match['Poison']['Rock'] = .5
type_match['Poison']['Ghost'] = .5
type_match['Ground']['Fire'] = 2
type_match['Ground']['Electric'] = 2
type_match['Ground']['Grass'] = .5
type_match['Ground']['Poison'] = 2
type_match['Ground']['Flying'] = 0
type_match['Ground']['Bug'] = .5
type_match['Ground']['Rock'] = 2
type_match['Flying']['Electric'] = .5
type_match['Flying']['Grass'] = 2
type_match['Flying']['Fighting'] = 2
type_match['Flying']['Bug'] = 2
type_match['Flying']['Rock'] = .5
type_match['Psychic']['Fighting'] = 2
type_match['Psychic']['Poison'] = 2
type_match['Psychic']['Psychic'] = .5
type_match['Bug']['Fire'] = .5
type_match['Bug']['Grass'] = 2
type_match['Bug']['Fighting'] = .5
type_match['Bug']['Poison'] = 2
type_match['Bug']['Flying'] = .5
type_match['Bug']['Psychic'] = 2
type_match['Bug']['Ghost'] = .5
type_match['Rock']['Fire'] = 2
type_match['Rock']['Ice'] = 2
type_match['Rock']['Fighting'] = .5
type_match['Rock']['Ground'] = .5
type_match['Rock']['Flying'] = 2
type_match['Rock']['Bug'] = 2
type_match['Ghost']['Ghost'] = 2
type_match['Dragon']['Dragon'] = 2

mons = []

mons.append({
  "name": "dewgong",
  "def_types": ['Water', 'Ice'],
  "off_types": ['Ice', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Cloyster",
  "def_types": ['Water', 'Ice'],
  "off_types": ['Ice', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Slowbro",
  "def_types": ['Water', 'Psychic'],
  "off_types": ['Water', 'Psychic'],
  "eff_types": []
})

mons.append({
  "name": "Jynx",
  "def_types": ['Ice', 'Psychic'],
  "off_types": ['Normal', 'Ice'],
  "eff_types": []
})

mons.append({
  "name": "Lapras",
  "def_types": ['Water', 'Ice'],
  "off_types": ['Normal', 'Water', 'Ice'],
  "eff_types": []
})

mons.append({
  "name": "Onix",
  "def_types": ['Rock', 'Ground'],
  "off_types": ['Rock', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Hitmonchan",
  "def_types": ['Fighting'],
  "off_types": ['Ice', 'Fire', 'Electric'],
  "eff_types": []
})

mons.append({
  "name": "Hitmonlee",
  "def_types": ['Fighting'],
  "off_types": ['Fighting'],
  "eff_types": []
})

mons.append({
  "name": "Onix",
  "def_types": ['Rock', 'Ground'],
  "off_types": [],
  "eff_types": []
})

mons.append({
  "name": "Machamp",
  "def_types": ['Fighting'],
  "off_types": ['Fighting'],
  "eff_types": []
})

mons.append({
  "name": "Gengar",
  "def_types": ['Ghost', 'Poison'],
  "off_types": ['Ghost', 'Psychic'],
  "eff_types": []
})

mons.append({
  "name": "Golbat",
  "def_types": ['Poison', 'Flying'],
  "off_types": ['Flying'],
  "eff_types": []
})

mons.append({
  "name": "Haunter",
  "def_types": ['Ghost', 'Poison'],
  "off_types": ['Ghost', 'Psychic'],
  "eff_types": []
})

mons.append({
  "name": "Arbok",
  "def_types": ['Poison'],
  "off_types": ['Normal', 'Poison'],
  "eff_types": []
})

mons.append({
  "name": "Gengar",
  "def_types": ['Ghost', 'Poison'],
  "off_types": ['Ghost', 'Psychic'],
  "eff_types": []
})

mons.append({
  "name": "Gyarados",
  "def_types": ['Water', 'Flying'],
  "off_types": ['Water', 'Dragon', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Dragonair",
  "def_types": ['Dragon'],
  "off_types": ['Normal', 'Dragon'],
  "eff_types": []
})

mons.append({
  "name": "Dragonair",
  "def_types": ['Dragon'],
  "off_types": ['Normal', 'Dragon'],
  "eff_types": []
})

mons.append({
  "name": "Aerodactyl",
  "def_types": ['Rock', 'Flying'],
  "off_types": ['Normal'],
  "eff_types": []
})

mons.append({
  "name": "Dragonite",
  "def_types": ['Dragon', 'Flying'],
  "off_types": ['Normal'],
  "eff_types": []
})

mons.append({
  "name": "Pidgeot",
  "def_types": ['Normal', 'Flying'],
  "off_types": ['Flying'],
  "eff_types": []
})

mons.append({
  "name": "Alakazam",
  "def_types": ['Psychic'],
  "off_types": ['Psychic'],
  "eff_types": []
})

mons.append({
  "name": "Rhydon",
  "def_types": ['Ground', 'Rock'],
  "off_types": ['Normal'],
  "eff_types": []
})

mons.append({
  "name": "Arcanine",
  "def_types": ['Fire'],
  "off_types": ['Fire', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Gyarados",
  "def_types": ['Water', 'Flying'],
  "off_types": ['Dragon', 'Water', 'Normal'],
  "eff_types": []
})

mons.append({
  "name": "Venusaur",
  "def_types": ['Grass', 'Poison'],
  "off_types": ['Grass'],
  "eff_types": []
})

good_types = []
i = 0

for mon in mons:
  good_types.append([])
  for type1 in types:
    def_good = True
    for off_type in mon['off_types']:
      if type_match[off_type][type1] > 1:
        continue
    
    eff = 1
    for def_type in mon['def_types']:
      eff *= type_match[type1][def_type]
    if eff > 1:
      mon["eff_types"].append(type1)
      good_types[i].append(type1)
  i += 1


i = 1
for good_type in good_types:
  print(good_type)
  if i%5 == 0:
    print('\n')
  i+=1

final_types = ['Ice', 'Electric', 'Psychic', 'Water', 'Grass', 'Fire']

for mon in mons:
  no_good_types = True
  for type1 in mon["eff_types"]:
    if type1 in final_types:
      no_good_types = False
  
  if no_good_types:
    print('No good types for ' + mon['name'])
  
