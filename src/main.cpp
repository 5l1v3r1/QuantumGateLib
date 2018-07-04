#include <iostream>
#include "../include/circuit/circuit.h"

template<class T>
[[deprecated]] void f(T){}

using namespace qlib;

int main(void) {

	u_ptr<Measure> poyo = UniquePtrFactory<Measure>::create(nullptr);
	u_ptr<UnitaryOp> poyo2 = UniquePtrFactory<UnitaryOp>::create("Hadamard", std::vector<std::string>({"hoge", "poyo"}), nullptr);

	return 0;
}
     
