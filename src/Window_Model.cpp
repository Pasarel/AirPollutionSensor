#include "../include/Window_Model.hpp"

bool Peripherial_Model::getState() // este on sau of, in cazul geamului on =  deschis
{
    return peripherial_state;
}

void Peripherial_Model::setState(bool b) // schimba stadiul in care e
{
    if (peripherial_state == b) {
        return;
    }

    peripherial_state = b;
}

Window::Window(bool f = false) // constructor care e by default false, I assume that the window is closed
{
    peripherial_state = f;
}