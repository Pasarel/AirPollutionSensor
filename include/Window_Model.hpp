#pragma once
#include <utility>

class Peripherial_Model // interfata, pentru model general
{
public:
	virtual bool getState(); // este on sau of, in cazul geamului on =  deschis

	virtual void setState(bool b); // schimba stadiul in care e

protected:
	bool peripherial_state; // bool pentru peripherial_model
};

class Window: public Peripherial_Model {
public: 
	Window(bool f = false); // constructor care e by default false, I assume that the window is closed
	Window(bool f, std::pair<double,double> coords);
	std::pair<double,double> getCoords();
	void setCoords(std::pair<double,double>);
	std::pair<double, double> coords;
};