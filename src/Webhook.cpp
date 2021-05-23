#include "../include/Webhook.hpp"

Webhook::Webhook() {
	output_lock = boost::interprocess::file_lock(filename);
}

void Webhook::push(const Rest::Request & request, Http::ResponseWriter response) {
	try {
		json print = json::parse(request.body());
		output_lock.lock();
		output.open(filename);
		output << print << std::endl;
		output.close();
		output_lock.unlock();
		response.send(Http::Code::Ok, to_string(json(
			{
				{"code", 200}, 
				{"state", "wrote to file"}
			}))
		);
	}
	catch (json::parse_error exc) {
		response.send(Http::Code::Ok, to_string(json(
			{
				{"code", 400}, 
				{"state", "input was badly formatted"}
			}))
		);
	}
	catch (const std::exception &exc) {
		response.send(Http::Code::Ok, to_string(json(
			{
				{"code", 500}, 
				{"state", exc.what()}
			}))
		);
	}
	catch (...) {
		response.send(Http::Code::Ok, to_string(json(
			{
				{"code", 500}, 
				{"state", "internal server error, soz"}
			}))
		);
	}
}