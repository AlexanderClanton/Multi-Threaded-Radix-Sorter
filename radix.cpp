#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include <netdb.h>
#include "radix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
static void radixSort(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, long unsigned int digit);
void radixSort(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, long unsigned int digit){
    if (begin == end)
        return;
    std::string hold;
    // first skip short numbers
    auto e = begin;
    for (auto p = begin; p != end; ++p){
        hold=std::to_string(*p);
	if (hold.size() <= digit)
        {
            //if (p != e)
                std::swap(*p, *e);
            e++;
        }
    }
    if (e == end){
        return;
    }
    for (char d = '0'; d <= '9'; ++d)
    {
        auto s = e;
        for (auto p = e; p != end; ++p){
            hold=std::to_string(*p);
	    if (hold.at(digit) == d)
            {
                if (p != e)
                    std::swap(*p, *e);
                e++;
            }
	}
        radixSort(s, e, digit+1);
    }
}





RadixServer::RadixServer(const int port, const unsigned int cores) {
     
    //std::cout<<"we here server"<<std::endl;
    //int port=atoi(argv[1]);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
   // if(sockfd<0){
//	    exit(-1);
  //  }
    
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(port);
   bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    listen(sockfd,5);
    struct sockaddr_in client_addr;
        socklen_t len=sizeof(client_addr);
        int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&len);
        //if(newsockfd<0)exit(-1);
    int n=0;
    unsigned int zero=0;
    unsigned int hold=0;

    unsigned int held=0;
   
   while((n=recv(newsockfd,&hold,sizeof(hold),0))>0) {
        //if(n==0){
           //break;
	//}
	auto buck=std::vector<std::vector<unsigned int>>(std::vector<std::vector<unsigned int>>());
        for(int a=0; a<9; a++){
            buck.push_back(std::vector<unsigned int>());
	}
	std::vector<std::thread> cont;
	
	hold=ntohl(hold);

	std::vector<unsigned int> hodl;
	hodl.push_back(hold);
	    while(hold!=0){
	        n=recv(newsockfd,&hold,sizeof(hold),0);
		//if(n<0)exit(-1);
		hold=ntohl(hold);
		//std::cout<<"server"<<std::endl;
		//std::cout<<hold<<std::endl;
		if(hold!=0)hodl.push_back(hold);
	    }
	    

        unsigned int pox=0;
         
	 unsigned pox3=0;
      
	     
	   
	     

      for(unsigned int b=0; b<hodl.size(); b++){
         pox=hodl[b];
	 pox3=pox;
	  while(pox>=10){
                pox/=10;
	     }
 
	      
	      
	 if(pox==1){
            
	          buck[0].push_back(pox3);
	      
          }
	  else if(pox==2){
             
	         buck[1].push_back(pox3);
	   
          }
          else if(pox==3){
              
	      
                 
                 buck[2].push_back(pox3);
	   
          }
	 else if(pox==4){
              
                 buck[3].push_back(pox3);
	      
          }
	  else if(pox==5){
              
                 
                 buck[4].push_back(pox3);
	     
	  }
	  else if(pox==6){
              
                 
                 buck[5].push_back(pox3);
	      
          }
	  else if(pox==7){
             
             
                 buck[6].push_back(pox3);
	    
          }
	
	  else if(pox==8){
              
              
                 buck[7].push_back(pox3);
	      
          }
	  else{
              
               
                 buck[8].push_back(pox3);
	      
          }



      }	       

	   	auto lambaRadV=[&buck](unsigned int start){
                
                    radixSort(buck[start].begin(), buck[start].end(), 0);
                    //std::cout<<start<<x<<end<<std::endl;
                

	    };

            unsigned int f=0;
	    unsigned int coreZ=cores%9;
	    
            while(f<coreZ){
               //if(f>=9)break;
	    

	       std::thread t1(lambaRadV,f);
	       cont.push_back(std::move(t1));
	       f+=1;
	       
	       
            
	    }
	    for (unsigned int mft=0; mft<cont.size(); mft++){
		    
                   cont.at(mft).join();
		
             
            }
	    for(unsigned int g=coreZ; g<9; g++){
                radixSort(buck[g].begin(),buck[g].end(),0);

           }
            
	//}
	

	for(unsigned int v=0; v<9; v++){
            for(unsigned int m=0; m<buck[v].size(); m++){
                held=buck[v][m];
		held=htonl(held);
		n=write(newsockfd,&held,sizeof(held));
		//if(n<0)exit(-1);


            }


	}
	zero=htonl(zero);
	n=write(newsockfd,&zero,sizeof(zero));
	//if(n<0)exit(-1);
	//close(newsockfd);	

	
    }
    close(newsockfd);
   close(sockfd);


}

void RadixClient::msd(const char *hostname,const int port,std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) 
{
    //std::cout<<"WE here client?"<<std::endl; 
    char word[sizeof(hostname)+1];
    for(size_t x=0; x<=sizeof(hostname); x++){
        word[x]=hostname[x];

    }
   // std::cout<<word<<std::endl;
    // your client implementation goes here :)
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
   // if(sockfd<0) exit(-1);
    
    struct hostent *server=gethostbyname(word);
    //if(server==NULL)exit(-1);
   // std::cout<<"connect issue"<<std::endl;
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
   
    serv_addr.sin_port=htons(port);
    //std::cout<<"before connect"<<std::endl;
    connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    
    
    unsigned int held=0;
    std::string hold;
    unsigned int zero=0;
    
    
     auto x=lists.size();
    for(x=0; x<lists.size(); x++){
        
	    
	    
	    
	auto& y=lists[x].get();
	auto z=y.size();
            for(unsigned int r=0; r<z; r++){
                held=y[r];
		held=htonl(held);
		//std::cout<<"written"<<std::endl;
		//std::cout<<held<<std::endl;
		write(sockfd,&held,sizeof(held));
	     }
	  zero=htonl(zero);
	 write(sockfd,&zero,sizeof(zero));
	//if(n<0)exit(-1);
	y.clear();
	unsigned int hold=1;
	while(hold!=0){
               read(sockfd,&hold,sizeof(hold));

	       hold=ntohl(hold);
	       //if(n<0)exit(-1);
	       if(hold!=0)y.push_back(hold);

	}
	



    }
    close(sockfd);



}
