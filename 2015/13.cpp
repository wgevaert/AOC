#include <iostream>
#include <string>

#define UNINITIALISED 666

int main() {
	 const int peeps_nr=8;
     static int happiness[peeps_nr][peeps_nr];
     int a,b,choice,space_cnt,sign,dummy,max=0,left=peeps_nr-1,right=peeps_nr,total=0;
     int table[2*peeps_nr];
     for(int i=0;i<2*peeps_nr;i++)table[i]=UNINITIALISED;
     static bool seated[peeps_nr];
     bool at_right;
     std::string line;
     while (true) {
		 std::getline(std::cin, line);
		 if (line.size() ==0)
		     break;
		 a = line[0]-'A';
		 int i=line.size()-1;
		 while(line[i]>'Z'||line[i]<'A')i--;
		 b=line[i]-'A';
		 if (a==12)a=7;
		 if (b==12)b=7;
		 i=0;
		 space_cnt=0;
		 while (space_cnt<2) {
			 if(line[i]==' ')space_cnt++;
			 i++;
		 }
		 if (line[i]=='l')
		    sign = -1;
		 else if(line[i]=='g')
		    sign = 1;
		 else std::cout<<"WHAT IS "<<line[i]<<' '<<static_cast<int>(line[i])<<" DOING HERE AT "<<i<<std::endl;
		 while(line[i]!=' ')i++;
		 i++;
		 dummy=0;
		 while(line[i]>='0'&&line[i]<='9'){
			 dummy=10*dummy+line[i]-'0';
			 i++;
		 }
		 dummy *=sign;
		 happiness[a][b]=happiness[b][a]+=dummy;
	 }
	 for (int i=0;i<peeps_nr;i++)for(int j=0;j<peeps_nr;j++)if(i==j)continue;else if(happiness[i][j]>max){max=happiness[a][b];a=i;b=j;}
	 table[left]=a;table[right]=b;
	 seated[a]=seated[b]=true;
	 for(int seated_nr=2;seated_nr < peeps_nr;seated_nr++) {
    	 int max =-10000;
	     choice =666;
    	 for (int i=0;i<peeps_nr;i++) {
	    	 if (seated[i])
		         continue;
    		 if (happiness[i][a] > max) {
	    		 at_right = false;
		    	 choice=i;
			     max = happiness[i][a];
    		 }
	    	 if (happiness[i][b] > max) {
		    	 at_right = true;
			     choice=i;
    			 max = happiness[i][b];
	    	 }
		 }
		seated[choice]=true;
		 if (at_right) {
			 b=choice;
			 right++;
			 table[right]=b;
		 } else {
			 a=choice;
			 left--;
			 table[left]=a;
		 }
	 }
	 int j=2*peeps_nr;
	 max=1000;
	 for(int i=0;i<2*peeps_nr;i++)if (table[i]!=666){if (i<j)j=i; int cur_hap = happiness[table[i]][table[(i+1-j)%peeps_nr + j]]; if (cur_hap < max) max=cur_hap; std::cout<<table[i]<<"-("<<cur_hap<<")-";total +=cur_hap;}
	 std::cout<<"\n\n"<<total<<'-'<<max<<std::endl;
     return 0;
}
