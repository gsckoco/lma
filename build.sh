if g++-10 -std=c++14 -pipe `pkg-config --cflags gtkmm-3.0` `pkg-config --cflags --libs fmt` `sourcefiles ./src c cpp` -o./out/lmc `pkg-config --libs fmt` `pkg-config --libs gtkmm-3.0`; then
    ./out/lmc
else
    echo "Unable to run, build failed"
fi