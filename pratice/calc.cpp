#include "std_lib_facilities.h"

const char number = '8';
const char quit = 'q';
const char print = ';';

const string prompt = "> ";
const string result = "= ";

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
  void ignore(char c);
private:
  bool full{false};
  Token buffer;
};

Token_stream::Token_stream(){}


void Token_stream::putback(Token t){
  if(full) error("putback() into a full buffer");
  buffer  = t;
  full = true;
}

Token Token_stream::get(){
  if(full){
    full = false;
    return buffer;
  }
  char ch;
  cin>>ch;
  switch (ch) {
    case quit:
    case print:
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      return Token{ch};
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
      cin.putback(ch);
      double val;
      cin >> val;
      return Token{number, val};
    }
    default:
      error("Bad Token");
  }
}

void Token_stream::ignore(char c)
{
    if(full && c==buffer.kind){
      full = false;
      return;
    }
    full = false;
    char ch = 0;
    while(cin>>ch) if(ch==c) return;
}

Token_stream ts;
double expression();




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
    case number:
      return t.value;
    case '-':
      return - primary();
    case '+':
      return primary();
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
      case '%':
      {
            int i1 = narrow_cast<int>(left);
            int i2 = narrow_cast<int>(primary());
            if(i2==0) error("%: divide by zero");
            left = i1%i2;
            t = ts.get();
            break;
      }
      default:
        ts.putback(t);
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

void calculate(){
  while(cin) {
    cout << prompt;
    Token t = ts.get();
    while(t.kind == print) t=ts.get();
    if(t.kind == quit) return;
    ts.putback(t);
    cout << result << expression() << '\n';
  }
}

void clean_up_mess()
{
   ts.ignore(print);
}

int main(){
  try{
    calculate();
    keep_window_open();
    return 0;
  }
  catch(exception& e){
    cerr << e.what() << '\n';
    clean_up_mess();
  }
  catch(...){
    cerr << "Exception \n";
    keep_window_open();
    return 2;
  }
  return 0;
}
