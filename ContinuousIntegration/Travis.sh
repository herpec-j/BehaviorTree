#!/bin/bash -e

make run VERBOSE=1 || cat Testing/Temporary/LastTest.log
