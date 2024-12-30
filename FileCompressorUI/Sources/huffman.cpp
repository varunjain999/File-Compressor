#include "huffman.hpp"
#include <iostream>
//------------------------------------------------------
 void huffman::createArr(){
    for(int i=0;i<128;i++){
        arr.push_back(new node());
        arr[i]->data=i;
        arr[i]->freq=0;
    }
  }
 //------------------------------------------------------ 
  int huffman::binToDec(string str){
      int res=0;
      for(auto it: str){
        res=res*2+it-'0';
      }
      return res;
  }

//------------------------------------------------------
  string huffman::decToBin(int num){
    string temp="",res="";

   while(num>0){
    temp+=(num%2+'0');
    num/=2;
   }
   
   res.append(8-temp.length(),'0');
   for(int i=temp.length()-1;i>=0;i--){
    res+=temp[i];
   }
  
  return res;
  }
//------------------------------------------------------
   void huffman::createMinHeap(){
    char c;
    inFile.open(inFileName,ios::in);
    inFile.get(c);

    while(!inFile.eof()){
        arr[c]->freq++;
        inFile.get(c);
    }
    inFile.close();

    for(int i=0;i<128;i++){
        if(arr[i]->freq>0){
            minHeap.push(arr[i]);
        }
    }
  }
//------------------------------------------------------

  void huffman::createTree(){
  node* left,*right;
  
  priority_queue<node*,vector<node*>,compare> tempPQ(minHeap);

  while(tempPQ.size()!=1){
     left=tempPQ.top();
     tempPQ.pop();

     right=tempPQ.top();
     tempPQ.pop();

     root=new node();
     root->freq=left->freq+right->freq;

     root->left=left;
     root->right=right;
     tempPQ.push(root);
     }
}

//------------------------------------------------------

 void huffman::traverse(node* r,string s){  //now we will call traverse to put 
// values into the leaf nodes of createTree
       if(r->left==NULL && r->right==NULL){
        r->code=s;
        return;
       }

       traverse(r->left,s+'0');
       traverse(r->right,s+'1');


}

//------------------------------------------------------

 void huffman::buildTree(char ch,string& savedCode){   //we take the stored characters in compressed file and make tree out of it

     node* curr=root;
     for(int i=0;i<savedCode.length();i++){
      if(savedCode[i]=='0'){
          if(curr->left==NULL){
            curr->left=new node();
          }
          curr=curr->left;
      }

      else if(savedCode[i]=='1'){
        if(curr->right==NULL){
          curr->right=new node();
        }
        curr=curr->right;
      }
     }
     curr->data=ch;
}

//------------------------------------------------------
   void huffman::createCodes(){
    traverse(root,"");
  }
//------------------------------------------------------

   void huffman::saveEncodedFile(){
    inFile.open(inFileName,ios::in);
    outFile.open(outFileName,ios::out | ios::binary);
    string in="";
    string s="";
    char c;



       // characters ke codes aise store honge
        //   no of characters

           in+=(char)minHeap.size();
           priority_queue<node*,vector<node*>,compare> tempPQ(minHeap);

           while(!tempPQ.empty()){
              node* curr=tempPQ.top();
              in+=curr->data;
        //    curr->data   s=000000000(126times)1curr->code
     //                                   |
   //                         pehele 1 ke baad code start hai
              s.assign(127-curr->code.length(),'0');
              s+='1';
              s+=curr->code;

//   ab s ko 8-8 bits mei tod do=> total 16bytes(=128bits)
//128 bits isliye coz there are 128 characters in ASCII
        //  let code for a =101...then it is stored as
        //  in=00000000(8bits),00000000....,00000001,10100000(16times)
          
        //  aise saare codes store honge 16bytes mei
              in+=(char)binToDec(s.substr(0,8));
              for(int i=0;i<15;i++){
                s=s.substr(8);
                in+=(char)binToDec(s.substr(0,8));
              }
              tempPQ.pop();
            }
            s.clear();

 /*
      ab yaha jo be text tha use encode karenge
      pehele to seedha seedha "s" mei add hote jaenge codes(16bytes wal nhi..chote wale..like a=101)
      fir jaise hi"s>8" hoga, tab use ek byte mei convert kr denge using in+=(char)binToDec(s.substr(0,8));
      agr aise (char) bina use kre karenge to har ek char 8bits lega store hokr kyo har ek char will be stored by its
      ASCII value and it takes 8bits per char.

      to isliye 8chars ko bunch up karenge,use decimal mei convert karenge,fir wo decimal ko char mei convert kr denge,
      to itne saare characters wo 8 bits ke andr aa jaenge
*/

      
      inFile.get(c);
      while(!inFile.eof()){
        s+=arr[c]->code;
        while(s.length()>8){
          in+=(char)binToDec(s.substr(0,8));
          s=s.substr(8);
        }
        inFile.get(c);
      }
/* fir bachi kuchi bits mei 0 append krkr unhe 8 bits ka bana denge aur ekdam end mei
      no of 0 jo add kra hai wo daal denge for decoding
      */
       
       int count=8-s.length();
       if(s.length()<8){
        s.append(count,'0');

       }
       in+=(char)binToDec(s);
       in+=(char)count;

       outFile.write(in.c_str(),in.size());
       inFile.close();
       outFile.close();

  }

   void huffman::getTree(){
      inFile.open(inFileName,ios::in | ios::binary);
      unsigned char size; //no. of diff variables in text which we stored at the beginning of encoded file
      inFile.read(reinterpret_cast<char*>(&size),1);
      // converts the location of size to char* from unsigned char*

      root=new node();
      for(int i=0;i<size;i++){
         char ch;
         unsigned char huffcode[16];
         inFile.read(&ch,1);
         inFile.read(reinterpret_cast<char*>(huffcode),16);

        string huffcodeStr="";
        for(int i=0;i<16;i++){
          huffcodeStr+=decToBin(huffcode[i]);
        }
        //00000000,000......,00000001,10100000
        //                            |_______|
        //                 ye poora as a number store hota(binary mein)...
        //          to ab isko wapis binary mei convert krna pdega,i.e., 
        //          todna padega use poora fir huffcodestr mei store karenge...
        //   huffcodeStr=[0000......000110100000]
       
       int j=0;
       while(huffcodeStr[j]=='0'){
        j++;
       }
       huffcodeStr=huffcodeStr.substr(j+1); //removed padding and wo '1' jiske baad se huffman code store hua
       buildTree(ch,huffcodeStr);

      }
      inFile.close();
 }

   void huffman::saveDecodedFile(){
    inFile.open(inFileName,ios::in | ios::binary);
    outFile.open(outFileName,ios::out);
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size),1);


    inFile.seekg(-1,ios::end); //moves the get pointer 1 before the end of file
    char count0;
    inFile.read(&count0,1);
    inFile.seekg(1 + 17 * size, ios::beg); //skipped to where encoded text is there
      
      vector<unsigned char> text;
      unsigned char temptext;
      inFile.read(reinterpret_cast<char*>(&temptext), 1);
      while(!inFile.eof()){
        text.push_back(temptext);
        inFile.read(reinterpret_cast<char*>(&temptext), 1);
      }

        

      node* curr=root;
      
      string path;
      for(int i=0;i<text.size()-1;i++){
        //i<text.size()-1 isliye coz last byte mei count of padded 0 stored hai
        path=decToBin(text[i]);
        if(i==text.size()-2){
          path=path.substr(0,8-count0);
        }

        for(int j=0;j<path.size();j++){
          if(path[j]=='0'){
              curr=curr->left;
          }
          else {
            curr=curr->right;
          }

          if(curr->left==NULL && curr->right==NULL){
            outFile.put(curr->data);
            curr=root;
          }
        }
        


      }
      inFile.close();
      outFile.close();
  }

  void huffman::compress(){
   createMinHeap();
    createTree();
    createCodes();
    saveEncodedFile();
  }

  void huffman::decompress(){
    getTree();
    saveDecodedFile();
  }  
