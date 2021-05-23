#include <signal.h>

#include "../include/Webhook.hpp"
#include "../include/Controller_MANUAL_PROCESS.hpp"
#include "../include/Controller_AUTO_PROCESS.hpp"

using namespace std;
using namespace Pistache;

//Definition of the manualProcesses of the sensor class

class MPSensorEndPoint {
public:
  explicit MPSensorEndPoint(Address addr): httpEndpoint(std::make_shared < Http::Endpoint > (addr)) {}

  //Initialization of the server. Additional options can be provided here
  void init(size_t thr = 2) {

	auto opts = Http::Endpoint::options().maxRequestSize(30000).threads(static_cast < int > (thr));
	httpEndpoint -> init(opts);

	//Server routes are loaded up
	setupRoutes();
  }

  //Server is started threaded
  void start() {
	httpEndpoint -> setHandler(router.handler());
	httpEndpoint -> serveThreaded();

  }

  //When signaled server shuts down
  void stop() {
	httpEndpoint -> shutdown();
  }

private:
	//Instance of the sensor model
	Webhook webhook;
	Controller_Auto_Process auto_proc_cntrl;
	Controller_Manual_Process man_proc_cntrl;

	// Defining the httpEndpoint and a router.
	std::shared_ptr < Http::Endpoint > httpEndpoint;
	Rest::Router router;
	void setupRoutes() {
	  using namespace Rest;
	  //Defining various endpoints
	  // Generally say that when http://localhost:9080/actions/:actionName is called, the shut_down function should be called.

	  Routes::Post(router, "/action1/:switchoff/:value", Routes::bind( & Controller_Auto_Process::setAction_fereastra, &auto_proc_cntrl));
	  Routes::Get(router, "/action1/:switchoff/", Routes::bind( & Controller_Auto_Process::getAction_fereastra, &auto_proc_cntrl));
	  Routes::Post(router, "/action/:addremove/:value", Routes::bind( & Controller_Auto_Process::setAction_addremove, &auto_proc_cntrl));
	  Routes::Get(router, "/action/:addremove/", Routes::bind( & Controller_Auto_Process::getAction_addremove, &auto_proc_cntrl));
	  Routes::Post(router, "/action2/:addremove/:value", Routes::bind( & Controller_Auto_Process::setAction_interogare, &auto_proc_cntrl));
	  Routes::Get(router, "/action2/:addremove/", Routes::bind( & Controller_Auto_Process::getAction_interogare, &auto_proc_cntrl));

	  Routes::Get(router, "/check_window/:id?", Routes::bind( & Controller_Manual_Process::checkWindow, &man_proc_cntrl));
	  Routes::Get(router, "/close_window/:id?", Routes::bind( & Controller_Manual_Process::closeWindow, &man_proc_cntrl));
	  Routes::Get(router, "/open_window/:id?", Routes::bind( & Controller_Manual_Process::openWindow, &man_proc_cntrl));
	  Routes::Get(router, "/get_aq/:coords?", Routes::bind( & Controller_Manual_Process::getAirQuality, &man_proc_cntrl));
	  Routes::Get(router, "/get_pm25/:coords?", Routes::bind( & Controller_Manual_Process::getPM25, &  man_proc_cntrl));

	  Routes::Post(router, "/webhook", Routes::bind( & Webhook::push, & webhook));
  }
};

int main(int argc, char * argv[]) {

  //This code is needed for gracefull shutdown of the server when no longer needed.
  sigset_t signals;
  if (sigemptyset( & signals) != 0 ||
	sigaddset( & signals, SIGTERM) != 0 ||
	sigaddset( & signals, SIGINT) != 0 ||
	sigaddset( & signals, SIGHUP) != 0 ||
	pthread_sigmask(SIG_BLOCK, & signals, nullptr) != 0) {

	perror("Install signal handler failed!");
	return 1;

  }

  // Set a port on which your server to communicate
  Port port(9080);

  //Number of threads used by the server
  int thr = 2;

  if (argc >= 2) {
	port = static_cast < uint16_t > (std::stol(argv[1]));

	if (argc == 3) {
	  thr = std::stoi(argv[2]);
	}
  }

  Address addr(Ipv4::any(), port);

  cout << "Cores = " << hardware_concurrency() << endl;
  cout << "Using " << thr << "threads " << endl;

  //Instance of the class that defines what the server can do.
  MPSensorEndPoint stats(addr);

  //Initialize and start the server
  stats.init(thr);
  stats.start();

  //Code that waits for the shutdown signal for the server
  int signal = 0;
  int status = sigwait( & signals, & signal);
  if (status == 0) {
	std::cout << "received signal " << signal << std::endl;

  } else {
	std::cerr << "sigwait returns " << status << std::endl;
  }

  stats.stop();
}