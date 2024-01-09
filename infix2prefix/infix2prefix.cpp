#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

//================================================================================================
//
//  STRING.HPP
//
//================================================================================================
class String {
public:
            String        ();                               //Empty string
            String        (char);                           //String('x')
            String        (const char[]);                   //String("abc")
            String        (const String&);                  //Copy Constructor
            ~String       ();                               //Destructor
    void    swap          (String&);                        //Constant time swap
    String& operator=     (String);                         //Assignment Copy

      int     capacity      ()                        const;  //Max chars that can be stored
    int     length        ()                        const;  //Actual number of chars in string
    char&   operator[]    (int);                            //Accessor/Modifier
    char    operator[]    (int)                     const;  //Accessor
    
    String& operator+=    (const String&);
  bool    operator==    (const String&)           const;
  bool    operator<     (const String&)           const;
  String  substr        (int, int)                const;  //sub from staring to ending position
  int     findch        (int,  char)              const;  //Location of charater starting at position
  int     findstr       (int,  const String&)     const;  //Location of string starting at a position
    std::vector<String> split(char)                 const;

    friend  std::ostream& operator<<(std::ostream&, const String&);
    friend  std::istream& operator>>(std::istream&, String&);

private:
    char    *str;                                           //Pointer to char[]
    int     stringSize;                                     //Size includes NULL terminator
};

String  operator+       (String,        const String&);
bool    operator==      (const char[],  const String&);
bool    operator==      (char,          const String&);
bool    operator<       (const char[],  const String&);
bool    operator<       (char,          const String&);
bool    operator<=      (const String&, const String&);
bool    operator!=      (const String&, const String&);
bool    operator>=      (const String&, const String&);
bool    operator>       (const String&, const String&);


//================================================================================================
//
//  STRING.CPP
//
//===============================================================================================

std::vector<String> String::split(char ch) const {
  std::vector<String> strVector;
  int start = 0, end;
  String sub;

  while(start < stringSize){
    end = this->findch(start, ch);
    if (end == -1)
      end = this->length();
    sub = substr(start, end - 1);
    strVector.push_back(sub);
    if(end != this->length())
      start = end + 1;
    else
      start = stringSize;
  }
  return strVector;  
}

std::istream& operator>>(std::istream& input, String& object){
  String temp;
  char ch;
  input >> ch;
  while (ch != ';'){
    temp += ch;
    input >> ch;
  }
  object = temp;
  /*do {
    input >> ch;
    object += ch;
    } while (!input.eof());*/
  //input.getline(object.str, '\n');
  //std::cout << "object is: " << object << std::endl;
  
  return input;
}

std::ostream& operator<<(std::ostream& out, const String& object){
  for(int i = 0; i < object.length(); ++i){
    out << object.str[i];
  }
  return out;
}

String& String::operator+= (const String& rhs){
  int length = this->length();
  int rhsLength = rhs.length();
  
  String temp = str;
  delete[] str;
  stringSize = length + rhsLength + 1;
  str = new char[stringSize];

  for(int i = 0; i < length; ++i)
    str[i] = temp[i];
  for(int i = 0; i < rhsLength; ++i){
    str[(i + length)] = rhs.str[i];
  }

  str[(stringSize-1)] = 0;
  return *this;
}

char& String::operator[](int index){
  if (index < 0 || index >= stringSize){
    std::cout << "index is out of range";
    return str[0];
  }
  else
    return str[index];
}

char String::operator[](int index) const {
  if(index < 0 || index >= stringSize){
    std::cout << "index is out of range";
    return str[0];
  }
  else
    return str[index];
}

String::String(){
  stringSize = 1;
  str = new char[stringSize];
  str[0] = 0;
}

String::String(char ch){
  if (ch == '\0'){
    stringSize = 1;
    str = new char[stringSize];
    str[0] = 0;
  }
  else {
    stringSize = 2;
    str = new char[stringSize];
    str[0] = ch;
    str[1] = 0;
  }
     
}

String::String(const char ch[]){
  int len = 0;
  while (ch[len] != 0) ++len;

  stringSize = len + 1;
  str = new char[stringSize];
  
  for (int i = 0; i < stringSize; ++i)
    str[i] = ch[i];
  str[len] = 0;
}

//copy constructor
String::String(const String& actual){
  stringSize = actual.stringSize;
  str = new char[stringSize];
  for(int i = 0; i < stringSize; ++i)
    str[i] = actual.str[i];
}

//Destructor
String::~String(){
  delete[] str;
}

//Time swap
void String::swap(String& rhs){
  char *temp = str;
  str = rhs.str;
  rhs.str = temp;
  char sizeTemp = stringSize;
  stringSize = rhs.stringSize;
  rhs.stringSize = sizeTemp;
}

//Assignment copy
String& String:: operator=(String rhs) {
  if (str == rhs.str) return *this;
  delete[] str;
  stringSize = rhs.stringSize;
  str = new char[stringSize];
  for(int i = 0; i < stringSize; ++i)
    str[i] = rhs.str[i];
  return *this;
}

int String::capacity() const {
  return stringSize - 1;
}

int String::length() const{
  return capacity();
}
bool String::operator== (const String& rhs) const{
  int length = this->length();
  int rhsLength = rhs.length();

  if(length != rhsLength) return false;
  for (int i = 0; i < length; ++i){
    if (str[i] != rhs.str[i]) return false;
  }
  return true;
}

bool String::operator< (const String& rhs) const{
  int i = 0;
  while ((str[i] != 0) && (rhs.str[i] != 0) && (str[i] == rhs.str[i]))
    ++i;
  return str[i] < rhs.str[i];
}

String String::substr(int start, int end) const{
  String result;
  
  if (start < 0) start = 0;
  if (end < start) return result;
  if (end >= length()) end = length() - 1;
  
    for (int i = start; i <= end; ++i){
      result += str[i];
    }
  return result;
}

int String::findch (int start, char ch) const{
  if (start < 0) start = 0;
  if (start >= length()) return -1;

  int i = start;
  while ((str[i] != 0) && (str[i] !=ch)) ++i;
  if (str[i] == ch)
    return i;
  else
    return -1;
}


int String::findstr (int start, const String& chArray) const{
  if (start < 0) start = 0;
  if (start >= length()) return -1;
  int location = start;

  while (chArray.length() + location <= length()){
    String temp = substr(location, location + chArray.length() - 1);
    if (chArray == temp)
      return location;
    else
      ++location;
  }
  return -1;
}

String operator+ (String first, const String& second){
  String result = first;
  result += second;
  return result;
}

bool operator!= (const String& one, const String& two){
  if (one == two)
    return false;
  return true;
}


//===============================================================================================
//
//  STACK.HPP
//
//===============================================================================================

template<typename T> 
class Node {
public:
  Node():next(0) {};
  Node(const T& item) : data(item), next(0){};
  T data;
  Node<T>* next;
};


// CLASS INV:
//
template <typename T>
class stack {
public:
              stack     () : tos(0){};
              stack     (const stack<T>&);
              ~stack    ();
    void      swap      (stack<T>&);
    stack<T>& operator= (stack<T>);
    
	bool      empty     () const;
	bool      full      () const;
	T         top       () const;
	T         pop       ();
	void      push      (const T&);
  
private:
	Node<T>   *tos;
};

template <typename T>
stack<T>::stack(const stack<T>& actual) : stack<T>(){
  Node<T>* temp = actual.tos;
  Node<T>* bottom;

  while(temp != 0){
    if (tos == 0){
      tos = new Node<T>(temp->data);
      bottom = tos;
    }
    else{
      bottom->next = new Node<T>(temp->data);
      bottom = bottom->next;
    }
    temp = temp->next;
  }
}

template <typename T>
stack<T>::~stack(){
  Node<T>* temp = 0;
  while(tos != 0){
    temp = tos;
    tos = tos->next;
    delete temp;
  }
}

template <typename T>
void stack<T>::swap(stack<T>& rhs){
  Node<T>* temp = tos;
  tos = rhs.tos;
  rhs.tos = temp;
}

template <typename T>
stack<T>& stack<T>::operator= (stack<T> rhs){
  swap(rhs);
  return *this;
}

template <typename T>
bool stack<T>::empty() const {
  return tos == 0;
}

template <typename T>
bool stack<T>::full() const {
  Node<T>* temp = new(std::nothrow) Node<T>();
  if(temp == 0) return true;
  delete temp;
  return false;
}

template <typename T>
T stack<T>::top() const{
  assert(!empty());
  return tos->data;
}

template <typename T>
T stack<T>::pop(){
  assert(!empty());
  T result = tos->data;
  Node<T>* temp = tos;
  tos = tos->next;
  delete temp;
  return result;
}

template <typename T>
void stack<T>::push(const T& item){
  assert(!full());
      Node<T>* temp = new Node<T>(item);
      temp->next = tos;
      tos = temp;
 }


//==================================================================================================
//
// lab implementation here
//
//==================================================================================================

void infix2prefix(String&);

int main(){
  
  std::ifstream file("data3-1.txt");
  if(!file){
    std::cout << "Stream failed to open" << std::endl;
    return 1;
  }/*
  String expression;
  while(file){
    file >> expression;
    infix2prefix(expression);
  }
*/

   String result;
  do{
    file >> result;
    infix2prefix(result);
    //std::cout << "Postfix Expression: " << std::endl;
  }while (!file.eof());
}
void infix2prefix(String& expr){
  if(expr == "") exit(1);
  stack<String> result;

  String temp(expr);
  std::cout << "Infix expression: " << temp << std::endl;

  String right, left, oper;
  int length = temp.length();
  int i = 0;
  while (i < length && temp[i] != ';'){
    if(temp[i] == ')'){
      right = result.pop();
      oper = result.pop();
      left = result.pop();
      result.push(oper + left + right);
    }
    else {
      if (temp[i] != '('){
      	result.push(temp[i]);
      }
    }
    ++i;
  }
  temp = result.top();
  std::cout << "Prefix Expression: " << temp << std::endl;
}
/*
void infix2prefix(String expression){
  String token;
  std::vector<String> parts;
  parts = expression.split(' ');
  stack<String> s;

  int i = 0;
  while(i <= parts.size()){
    token = parts[i];
    String rhs, lhs, op;
    if(token == ')'){
      rhs = s.pop();
      op = s.pop();
      lhs = s.pop();
      s.push(op + lhs + rhs);
    }
    else if(token != '(') {
      String temp(token);
      s.push(token);
    }
    ++i;
    String result = s.pop();
    std::cout << result;
  }*/
  
  
    
