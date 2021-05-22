class Peripherial_Model // interfata, pentru model general
{
  public:
    virtual bool getState() // este on sau of, in cazul geamului on =  deschis
  {
    return peripherial_state;
  }

  virtual void setState(bool b) // schimba stadiul in care e
  {
    if (peripherial_state == b) {
      return;
    }

    peripherial_state = b;
  }

  protected:
    bool peripherial_state; // bool pentru peripherial_model
};

class Window: public Peripherial_Model {

  public: Window(bool f = false) // constructor care e by default false, I assume that the window is closed
  {
    peripherial_state = f;
  }

};