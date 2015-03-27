#!/bin/bash -e

make VERBOSE=1 run || cat Testing/Temporary/LastTest.log
