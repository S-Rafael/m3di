#!/usr/bin/env python3
# 
#   Copyright (C) 2021 Rafael M. Siejakowski.
#   All rights reserved.
#
###############################################################
#
# Script for generating the JSON data files from the SnapPy
# oriented cusped census. Requires the 'xnappy' module.
#
MAX_TETRAHEDRA = 5

import snappy
import xnappy

Census = snappy.OrientableCuspedCensus
query  = "tets <= %d AND cusps = 1"%MAX_TETRAHEDRA
triangulations = Census.find(where=query)
for T in triangulations:
	X = xnappy.Manifold(T)
	X.save_m3di_json(T.name() + ".json")

