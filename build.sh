#!/bin/bash

PROJECT_DIR=`pwd`
DEPENDENS_SRC_DIR=$PROJECT_DIR/local/src
DEPENDENS_INSTALL_DIR=$PROJECT_DIR/local

RED='\e[1;31m'
EC='\033[m'

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$DEPENDENS_INSTALL_DIR/lib/pkgconfig

echo -e ${RED}changing PKG_CONFIG_PATH to ${EC} ${PKG_CONFIG_PATH}

echo -e ${RED}creating install directory ${EC} $DEPENDENCE_SRC_DIR
mkdir -p ${DEPENDENS_SRC_DIR}

######################################
# get ND
#echo -e ${RED}Fetching autolab-nd ${EC}
#cd ${DEPENDENS_SRC_DIR}
#git clone git@github.com:rtv/autolab-nd.git
#cd ${DEPENDENS_SRC_DIR}/autolab-nd

#echo -e ${RED}Reverting autolab-nd to the correct version ${EC}
#git reset --hard e603ecadda99c8ee6de4cd34c1afd17c406312cb

######################################
# get RAPI
echo -e ${RED}Fetching autolab-rapi ${EC}
cd ${DEPENDENS_SRC_DIR}
git clone git@github.com:jwawerla/autolab-rapi.git
cd ${DEPENDENS_SRC_DIR}/autolab-rapi

echo -e ${RED}Reverting autolab-rapi to the correct version ${EC}
#git reset --hard 4b5038ae10003b6231cb8d7d186df8c229689ab4

######################################
# Install rapi
echo -e ${RED}Installing autolab-rapi ${EC}
cd ${DEPENDENS_SRC_DIR}/autolab-rapi
cmake -DCMAKE_INSTALL_PREFIX=${DEPENDENS_INSTALL_DIR}
make install

######################################
# Install nd
#echo -e ${RED}Installing autolab-nd ${EC}
#cd ${DEPENDENS_SRC_DIR}/autolab-nd
#cmake -DCMAKE_INSTALL_PREFIX=${DEPENDENS_INSTALL_DIR} -DPKG_CONFIG_PATH=$DEPENDENS_INSTALL_DIR/lib/pkconfig
#make install

######################################
# Finishing up

# change back to project dir and we are done
cd $PROJECT_DIR

