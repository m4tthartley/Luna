if [ ! -d "debug" ]; then
	mkdir debug
fi

pushd debug

clang -x c++ ../code/main.cpp -o luna

popd