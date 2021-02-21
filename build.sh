if g++ -std=c++14 `sourcefiles ./src c h cpp hpp` -o./out/lmc; then
    ./out/lmc
else
    echo "Unable to run, build failed"
fi