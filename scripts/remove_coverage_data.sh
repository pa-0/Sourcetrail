#!/bin/bash

fd --type=file "gcno" --exec rm
fd --type=file "gcov" --exec rm
fd --type=file "gcda" --exec rm
