#!/bin/bash

DIRNAME=$(dirname $0)

# 

BUILDDIR=${DIRNAME}/build
ASSETSDIR=${DIRNAME}/assets

# 

source ${DIRNAME}/python-venv/bin/activate

# 

exodirs=($(find ${DIRNAME}/ -maxdepth 1 -type d -name "exo*"))

for exodir in ${exodirs[@]}
do
    exoname=$(basename $exodir)

    # 

    REPORT_TMP_DIR=${BUILDDIR}/$exoname.report

    # Initializing report

    rm -rf ${REPORT_TMP_DIR}
    mkdir -p ${REPORT_TMP_DIR}
    cp -r $exodir/* ${REPORT_TMP_DIR}/

    # Generating PDF for report

    jupyter nbconvert --to html ${REPORT_TMP_DIR}/rapport.ipynb >/dev/null 2>&1
    weasyprint -s ${ASSETSDIR}/pdf-report.css ${REPORT_TMP_DIR}/rapport.html ${REPORT_TMP_DIR}/rapport.pdf >/dev/null 2>&1

    # Cleaning report files

    rm -rf ${REPORT_TMP_DIR}/rapport.ipynb ${REPORT_TMP_DIR}/rapport.html ${REPORT_TMP_DIR}/.ipynb_checkpoints

    cp ${ASSETSDIR}/Makefile ${REPORT_TMP_DIR}/

    # Creating report archive

    rm -rf ${BUILDDIR}/$exoname.zip
    (cd ${REPORT_TMP_DIR} && zip -q -r ../$exoname.zip .)

    # Cleaning

    rm -rf ${REPORT_TMP_DIR}

    # 

    echo "Report of ${exoname} generated at ${BUILDDIR}/${exoname}.zip"
done