#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */



//  int fibonacci ( int n)
// {
//   if(n<=1)
//   {
//     return 1;
//   }
//   return fibonacci(n-1)+fibonacci(n-2);
//   // else{
//   //   unsigned int
//   // }
// }

int fibonacci(int n) {
  if (n <= 1) {
    return 1;
  }

  int * fib = new int[n+1];
  fib[0] = 1;
  fib[1] = 1;

  for (int i = 2; i <= n; i++) {
    fib[i] = fib[i-1] + fib[i-2];
  }
  int result = fib[n];
  delete[] fib;
  return result;
}

 int largestfib ( int x){
   int i = 1;
  while(fibonacci(i)<=x){
    i++;
  }
  
  // cout << "largestpossiblefibonacci " << fibonacci(i) << endl; 
  return i-1;
}

void fibonaccicoded ( int n , const vector< int> & sequence , char * array){
  n = n+1;
  int largest_index = largestfib(n);
  // cout << "largest index" << largest_index << endl;
  // char * fibSequence = new char[largest_index+3];
  char * fibSequence = array;
  
  for(int i = 0; i < largest_index; i++)
    fibSequence[i] = '0';

  for(int i = largest_index; i >= 1; i--) {
    if(n == 0) break;
    // cout << n << endl;
    if(n - sequence[i] >= 0) {
      n -= sequence[i];
      fibSequence[i-1] = '1';
    }
  }
  fibSequence[largest_index] = '1';
  fibSequence[largest_index+1] ='\0';
  // cout << "fib: " << fibSequence << endl;

  
  // for(unsigned int i = 0 ; i <= largest_index/2 ; ++i){
  //   char temp = fibSequence[i];
  //   fibSequence[i] = fibSequence[largest_index-i];
  //   fibSequence[largest_index - i ] = temp;
  // }
  // cout << fibSequence << endl;
  // unsigned char result = 0;
  // for(int k = 0 ; k < 8 ; ++k){
  //   result |= (fibSequence[i]=='1') << (7-k);
  // }
  // return result;
}

// void outputing (const char * outFile , char * array){
//   ofstream out (outFile);
//   int n = 0;

//   cout << "FIB: " << array << endl; 
//   unsigned char result = 0;
  

//   out.close();

// }



bool utf8ToFibonacci(const char *inFile,
                     const char *outFile)
{
  // // todo
  // int n = 368 ;
  // cout << fibonaccicoded(n) << endl;
  ifstream infile (inFile , ios::binary);
  ofstream out (outFile , ios::binary);
  if(!infile){
    // cout << "File cannot be read" << endl;
    return false;
  }

  vector< int> sequence;
  for(int i = 0 ; i < 36 ; i++){
    sequence.push_back(fibonacci(i));
  }
  char buffer;
  char * array;
  int n = 0;
  unsigned char write = 0;
  while(infile.read((char *)&buffer,sizeof(char))){
   
    if(!(buffer & 0b10000000)){
      // cout << "1byte buffer " << int(buffer) << endl;
      // cout << "1byte" << endl;
      // cout << "Result :" << (unsigned int) buffer << endl;
      array = new char[largestfib(( int)buffer)+3];
      fibonaccicoded((int)buffer,sequence,array);
      char * head = array;
      // cout << array << endl;
      // swapEndianity(array);
      // cout << array << endl;
      while(*array){
      // cout << "n " << n << endl;
      if(n<8){
      write |= (*array=='1') << (n);
      array++;
      n++;
      }
      else{
        // cout << "write" << endl;
        // cout << hex << int(write) << endl;
        out << write;
        write = 0;
        n = 0;
      }
      }
      delete[] head;
    }
    
    // else if((buffer & 0b11111000)==0b11111000 || (buffer & 0b11111100) == 0b11111100){
    //   // cout << "invalid" << endl;
    //   return false;
    // }
    else if((buffer & 0b11111000) == 0b11110000 ){
      // cout << "4byte" << endl;
      buffer = (buffer << 4);
      buffer = (buffer >> 4);
      // buffer = buffer & 00001111;
       int first = ( int) buffer;
      char buffer2;
      char buffer3;
      char buffer4;
      if(infile.read((char *)&buffer2,sizeof(buffer2))){
        if(!((buffer2 & 0b11000000) == 0b10000000)){
          return false;
        }
        buffer2 = buffer2 << 1 ;
        buffer2 = buffer2 >> 1 ;
        if(infile.read((char *)&buffer3,sizeof(buffer3))){
          if(!((buffer3 & 0b11000000) == 0b10000000)){
          return false;
          }
          buffer3 = buffer3 << 1;
          buffer3 = buffer3 >> 1;
          if(infile.read((char *)&buffer4,sizeof(buffer4))){
            if(!((buffer4 & 0b11000000) == 0b10000000)){
            return false;
          }
            buffer4 = buffer4 << 1;
            buffer4 = buffer4 >> 1;
          }
          else{
            return false;
          }
        }
        else{
          return false;
        }
      }
      else{
        return false;
      }
       int second = ( int) buffer2;
       int third = ( int) buffer3;
       int fourth = ( int) buffer4;
       int result = (first << 18) | (second << 12) | (third <<6) | fourth;
      // cout << "Result: " << result << endl;
      // array = fibonaccicoded(result,sequence);
      if(result > 1114111){
        return false;
      }
      array = new char[largestfib(result)+3];
      fibonaccicoded(result,sequence,array);

      char * head = array;
      // cout << array << endl;
      // SwapEndianity(array);
      while(*array){
      // cout << "n " << n << endl;
      if(n<8){
      write |= (*array=='1') << (n);
      array++;
      n++;
      }
      else{
        // cout << "write" << endl;
        // cout << hex << int(write) << endl;
        out << write;
        write = 0;
        n = 0;
      }
      }
      delete[] head;
    }
    else if((buffer & 0b11110000) == 0b11100000){
      // cout << "3byte" << endl;
      buffer = (buffer << 3);
      buffer = (buffer >> 3);
       int first = ( int) buffer;
      char buffer2;
      char buffer3;
      if(infile.read((char *)&buffer2,sizeof(buffer2))){
        if(!((buffer2 & 0b11000000) == 0b10000000)){
          return false;
        }
        buffer2 = buffer2 << 1 ;
        buffer2 = buffer2 >> 1 ;
        if(infile.read((char *)&buffer3,sizeof(buffer3))){
          if(!((buffer3 & 0b11000000) == 0b10000000)){
          return false;
        }
          buffer3 = buffer3 << 1;
          buffer3 = buffer3 >> 1;
        }
        else{
          return false;
        }
      }
      else{
        return false;
      }
       int second = ( int) buffer2;
       int third = ( int) buffer3;
      // cout << "First: " << first << endl;
      // cout << "Second: " << second << endl;
      // cout << "Third: " << third << endl;
       int result = (first << 12) | (second << 6) | third ;
      // cout << "Result: " << result << endl;
      // cout << fibonaccicoded(result) << endl;
      if(result > 1114111){
        return false;
      }
      array = new char[largestfib(result)+3];
      fibonaccicoded(result,sequence,array);
      char * head = array;
      // cout << array << endl;
      // SwapEndianity(array);
      while(*array){
      // cout << "n " << n << endl;
      if(n<8){
      write |= (*array=='1') << (n);
      array++;
      n++;
      }
      else{
        // cout << "write" << endl;
        // cout << hex << int(write) << endl;
        out << write;
        write = 0;
        n = 0;
      }
      }
      delete[] head;
    }

    else if((buffer & 0b11100000) == 0b11000000){
      // cout << "2byte" << endl;
      buffer = (buffer << 2);
      buffer = (buffer >> 2);
      // buffer = (buffer & 00011111) ;
      // cout << "Buffer: " << (unsigned int)(buffer) << endl;
       int first = ( int) buffer;
      char buffer2;
      if(infile.read((char *)&buffer2,sizeof(buffer2))){
        if(!((buffer2 & 0b11000000) == 0b10000000)){
          return false;
        }
        buffer2 = buffer2 << 1 ;
        buffer2 = buffer2 >> 1 ;
      }
      else{
        // cout << "invalid" << endl;
        return false;
      }
       int second = ( int) buffer2;
       int result = first << 6 | second;
       if(result > 1114111){
        return false;
      }
      // cout << "Result: " << result << endl;
      // cout << fibonaccicoded(result) << endl;
      array = new char[largestfib(result)+3];
      fibonaccicoded(result,sequence,array);
      char * head = array;
      // cout << array << endl;
      // SwapEndianity(array);
      while(*array){
      // cout << "n " << n << endl;
      if(n<8){
      write |= (*array=='1') << (n);
      array++;
      n++;
      }
      else{
        // cout << "write" << endl;
        // cout << hex << int(write) << endl;
        out << write;
        write = 0;
        n = 0;
      }
      }
      delete[] head;
    }
    else{ 
      return false; 
    }
    // else{
    //   cout << "tail" << endl; 
    // }

    // cout << fibonaccicoded(buffer) << endl;
  }
  out << write;

  if(!out){
    return false;
  }

  infile.close();
  out.close();
  
  

  return true; 
}

void printvector(const vector<bool> & array){
  for(unsigned int i = 0 ; i < array.size() ; i++){
    cout << array[i] << " ";
  }
  cout << endl;
}

vector<bool> bytetobits(char buffer , vector<bool> array){
  for(int i = 0 ; i < 8 ; i++){
    bool a = buffer & ( 1 << (i));
    array.push_back(a);
  }
  return array;
}

int decodingfibocoding(vector<bool>& list , const vector<int> &fib) {
    // int n = array.size();
    // vector<int> fib; //initialize Fibonacci sequence
    // fib.push_back(0);
    // fib.push_back(1);
    // while (fib.back() < n) { //compute Fibonacci sequence up to n
    //     int next_fib = fib[fib.size() - 1] + fib[fib.size() - 2];
    //     fib.push_back(next_fib);
    // }
    vector<bool>array;
    int value = 0;
    int found = 0;
    for(unsigned int i = 0 ; i < list.size() ; i++){
      if(!found){
        array.push_back(list[i]);
      }
      if(i > 0 && list[i]== 1 && list[i-1]==1){
        found = 1;
      }
    }
    if(found){
      // cout << "Found" << endl;
      list.erase(list.begin(), list.begin() + array.size());
      // cout << "Size" << array.size() << endl;
      // printvector(array);
      for(unsigned int i = 0 ; i < array.size()-1 ; ++i){
        // if(value > 0x10ffff){
        //   return false;
        // }
        if(array[i]){
          value += fib[i+1];
        }
      }
      return value-1;
    }
    return -1;
}

bool validfibo(vector<bool> & array){
  bool found = false;
  bool zero = true;
  for(unsigned int i = 1 ; i < array.size() ; i++){
    if(array[i]==1){
      zero = false;
    }
    if(array[i]==1 && array[i-1]==1){
      found = true;
    }
  }
  return zero || found ;
}

bool fibonacciToUtf8(const char *inFile,
                     const char *outFile)
{
  // todo
  ifstream file(inFile, ios::binary);
  ofstream out(outFile, ios::binary);
  if (!file || !out)
  {
    return false;
  }
  vector<int> sequence;
  vector<bool> array;
  vector<bool> utf8;
  for(int i = 0 ; i < 36 ; i++){
    sequence.push_back(fibonacci(i));
  }
  char buffer;
  int result;
  while(file.read((char *)&buffer,sizeof(buffer))){
    array = bytetobits(buffer,array);
    // printvector(array);
    result = decodingfibocoding(array,sequence);
    // cout << "Result" << result << endl;
    if(result > 0x10ffff){
      return false;
    }
    if(result>=0){
      if(result <= 127){
        out << (char)result;
      }
      else if(result <= 2047){
        out << (char)(0xc0 | ((result >> 6) & 0x1f));
        out << (char)(0x80 | (result & 0x3f));
      }
      else if(result <= 65535){
        out << (char)(0xe0 | ((result >> 12) & 0x0f));
        out << (char)(0x80 | ((result >> 6) & 0x3f));
        out << (char)(0x80 | (result & 0x3f));
      }
      else{
        out << (char)(0xf0 | ((result >> 18) & 0x07));
        out << (char)(0x80 | ((result >> 12) & 0x3f));
        out << (char)(0x80 | ((result >> 6) & 0x3f));
        out << (char)(0x80 | (result & 0x3f));
      }
    }
    
  }
  result = decodingfibocoding(array,sequence);
  // printvector(array);
  if(!validfibo(array)){
    // cout << "not valid" << endl;
    return false;
  }
  // cout << "Result " << result << endl;
  if(result>0){
    out << (char)result;
  }
  // printvector(array);

  if(!out){
    return false;
  }

  

  return true;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1,
                    const char *file2)
{
  // todo
  ifstream f1(file1, ios::binary);
  ifstream f2(file2, ios::binary);

  f1.seekg(0,f1.end);
  int length1 = f1.tellg();
  f1.seekg(0,f1.beg);

  f2.seekg(0,f2.end);
  int length2 = f2.tellg();
  f2.seekg(0,f2.beg);

  if(length1 != length2 ){
    return false;
  }

  char buffer1;
  char buffer2;
  while(f1.read((char*)&buffer1,sizeof(buffer1)) && f2.read((char *)&buffer2,sizeof(buffer2))){
    if(buffer1 != buffer2){
      return false;
    }
  }
  return true;
  // return (!strcmp(buffer1,buffer2));
}

int main(int argc, char *argv[])
// {  utf8ToFibonacci ( "example/src_2.utf8", "output.fib" );
  {
  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
  assert ( utf8ToFibonacci ( "example/dst_7.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/src_7.fib" ) );
  assert ( utf8ToFibonacci ( "example/dst_8.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/src_8.fib" ) );
  assert ( utf8ToFibonacci ( "example/dst_9.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/src_9.fib" ) );
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );

  // cout << "__________________________________________" << endl;
  fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" );
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
  // cout << "Last case\n";
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
  fibonacciToUtf8 ("example/src_6.fib","output.utf8");
  // cout << "Testing" << endl;
  assert ( ! utf8ToFibonacci("src_12.utf8","output.fib"));

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
