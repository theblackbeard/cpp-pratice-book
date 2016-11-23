#include "std_lib_facilities.h"

class Token {
public:
  char kind;
  double value;
};

class Token_stream {
public:
  Token_stream();
  Token get();
  void putback(Token t);
private:
  bool full{false};
  Token buffer;

};

void Token_stream::putback(Token t){
  if(full) error("putback() into a full buffer");
  buffer  = t;
  full = true;
}

Token get_token();
double primary();
double term();
double expression();


Token get_token(){
  Token t;
  return t;
}

double primary(){
  Token t = ts.get();
  switch (t.kind) {
    case '(':
    {
      double d = expression();
      t = ts.get();
      if(t.kind != ')') error("')' Expected");
      return d;
    }
    case '8':
      return t.value;
    default:
      error("Primary Expected");
  }
}

double term(){
  double left = primary();
  Token t = ts.get();
  while(true){
    switch (t.kind) {
      case '*':
        left *= primary();
        t = ts.get();
        break;
      case '/':
      {
            double d = primary();
            if(d == 0) error("Divide by Zero");
            left /= d;
            t = ts.get();
            break;
      }
      default:
        ts.putback(d);
        return left;
    }
  }
};



double expression()
{
  double left = term();
  Token t = ts.get();
  while(true){
    switch (t.kind) {
      case '+':
        left += term();
        t = ts.get();
        break;
      case '-':
        left -= term();
        t = ts.get();
        break;
      default:
        ts.putback(t);
        return left;
    }
  }
};



int main(){
  try{
    double val = 0;

    while(cin) {
      Token t = ts.get();
      if(t.kind == 'q') break;
      if(t.kind == ';') cout  << "=" << val << '\n';
      else ts.putback(t);
      val = expression();
    }
  }
  catch(exception& e){
    cerr << e.what() << '\n';
    keep_window_open();
    return 1;
  }
  catch(...){
    cerr << "Exception \n";
    keep_window_open();
    return 2;
  }
  return 0;
}
