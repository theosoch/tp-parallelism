#!/bin/bash

DIRNAME=$(dirname $0)

# 

source ${DIRNAME}/python-venv/bin/activate

# 

exodirs=($(find ${DIRNAME}/doc -type d -name "exo*"))

for exodir in ${exodirs[@]}
do
    exoname=$(basename $exodir)
    rm -rf ${DIRNAME}/build/$exoname.report
    mkdir -p ${DIRNAME}/build/$exoname.report
    cp -r $exodir/* ${DIRNAME}/build/$exoname.report/

    jupyter nbconvert --to html ${DIRNAME}/build/$exoname.report/rapport.ipynb >/dev/null 2>&1
    weasyprint -s ${DIRNAME}/assets/pdf-report.css ${DIRNAME}/build/$exoname.report/rapport.html ${DIRNAME}/build/$exoname.report/rapport.pdf >/dev/null 2>&1

    rm -rf ${DIRNAME}/build/$exoname.report/rapport.ipynb ${DIRNAME}/build/$exoname.report/rapport.html ${DIRNAME}/build/$exoname.report/.ipynb_checkpoints

    cp ${DIRNAME}/assets/Makefile ${DIRNAME}/build/$exoname.report/
    cp ${DIRNAME}/src/$exoname.c ${DIRNAME}/build/$exoname.report/main.c
    cp ${DIRNAME}/src/$exoname.h ${DIRNAME}/build/$exoname.report/main.h

    rm -rf ../$exoname.zip
    (cd ${DIRNAME}/build/$exoname.report && zip -r ../$exoname.zip .)
done