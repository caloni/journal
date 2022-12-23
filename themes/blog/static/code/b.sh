#!/bin/bash
for counter in $(seq 1 255); do cat sample.txt >> bigsample.txt; done
