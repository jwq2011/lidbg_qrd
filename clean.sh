#=======================================================================================
#	FileName    : 
#	Description : Make ALL
#       Date:         2012/02/03
#=======================================================================================

cd shell && ./all_clean.sh
cd ..
rm ./build_cfg.mk
#find . -name "*.ko" -type f -exec rm  {} \;

