#!/bin/bash

sudo scp $CI_PROJECT_DIR/src/SimpleBash/cat/s21_cat root@192.168.56.102:/usr/local/bin/
sudo scp $CI_PROJECT_DIR/src/SimpleBash/grep/s21_grep root@192.168.56.102:/usr/local/bin/