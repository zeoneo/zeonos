rm -rf zeonos_build/*
cd zeonos_build
cmake -DCPU_ARCH=arm ..
make VERBOSE=1 generate_map_file generate_list_file