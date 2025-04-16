#!/bin/bash

DIRNAME=$(dirname $0)

PYTHON_VENV_PATH=${DIRNAME}/python-venv

mkdir -p $PYTHON_VENV_PATH
python -m venv $PYTHON_VENV_PATH

source $PYTHON_VENV_PATH/bin/activate

python -m pip install --upgrade pip
python -m pip install jupyterlab
python -m pip install weasyprint
python -m pip install pandas
python -m pip install matplotlib
python -m pip install numpy
python -m pip install scipy