#include "../include/APIClient.hpp"

int main(int argc, char * argv[]) {
	APIClient c;
	c.init();
	c.fetch();
}