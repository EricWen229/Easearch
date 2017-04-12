#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <iostream>

#include "Ui.h"

using namespace std;

int main(int argc, char *argv[]) {
	InitDictionary();
	InitSort();
	cout << "Ready!\n";
	try {
		cppcms::service srv(argc, argv);
		srv.applications_pool().mount(cppcms::applications_factory<Ui>());
		srv.run();
	}
	catch(exception const &e) {
		cerr << e.what() << endl;
	}
}
