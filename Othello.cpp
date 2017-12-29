#include <iostream>
#include <cstring> 
using namespace std;
int board[15][15]; 
int me,opp;//玩家执子 
bool turn;
const int dir_r[8]={-1,-1,-1,0,0,1,1,1};//r,c方向上的变化量 
const int dir_c[8]={-1,0,1,-1,1,-1,0,1};
int dirrr,dircc;//每一次固定方向的固定变化量 
const int depth=6;
int s[8][8]={{100,-8,15,5,5,15,-8,100},
             {-8,-80,1,1,1,1,-80,-8},
			 {15,1,3,2,2,3,1,15},
			 {5,1,2,1,1,2,1,5},
             {5,1,2,1,1,2,1,5},
			 {15,1,3,2,2,3,1,15},
			 {-8,-80,1,1,1,1,-80,-8},
			 {100,-8,15,5,5,15,-8,100}};
			 
int dijibu;//记录当前的步数 
int pass;
 
struct node{
 int cboard[9][9];
 int score;
}qipan[depth+1][750000];//定义结构体node，包括棋局状态和该状态评分  

void end_game(){
	int count_me=0,count_opp=0;
	for(int i=1;i<=8;i++)
     for(int j=1;j<=8;j++){
       if(board[i][j]==me)       count_me++;
       else if(board[i][j]==opp) count_opp++;
     }
    if(count_me>count_opp) cout<<"you win"<<endl;
    else if(count_me==count_opp) cout<<"equal"<<endl;
    else cout<<"you lose"<<endl;
    cout<<"me:"<<count_me<<" "<<"AI:"<<count_opp;
} 


void printboard(){//打印当前棋盘
	dijibu++;
	cout<<"step: "<<dijibu<<endl;
    cout<<"   1  2  3  4  5  6  7  8"<<endl<<endl;
    for(int i=1;i<=8;i++){
 	cout<<i<<" |";
    for(int j=1;j<=8;j++) {
    	if(board[i][j]==1) cout<<"B"<<" |";
        else if(board[i][j]==2) cout<<"W"<<" |";	
        else cout<<"  |";
    }
    cout<<endl<<"  —————————————"<<endl;
    cout<<endl;
  }
 cout<<"___________________________"<<endl<<endl;
}


bool check_empty_board(int count1,int count2){
    for(int i=1;i<=8;i++) for(int j=1;j<=8;j++) if(qipan[count1][count2].cboard[i][j]!=0) return false;
    return true;
}


bool dfs_checklegal(int r,int c,int count,char role){//看能不能下在当前位置，被checklegal调用
   if(r<=0 || r>=9 || c<=0 || c>=9) return false;
   r=r+dirrr,c=c+dircc;
   if(board[r][c]==0) return false;
   else if(role=='p' && board[r][c]==opp){
   	count++;
    dfs_checklegal(r,c,count,'p');
   }
   else if(role=='c' && board[r][c]==me){
    count++;
    dfs_checklegal(r,c,count,'c');
   }
   else if(count>=1) return true;
   else return false;
}



bool checklegal(int r, int c,char role){ //i-->r && j-->c  判断该位置是否合法 
       if( board[r][c]!=0 ) return false; 
   	   else{
            for(int i=0;i<8;i++){
   				dirrr=dir_r[i];
   				dircc=dir_c[i];
   				if(dfs_checklegal(r,c,0,role)) return true; //判断在x=dirrr、y=dircc方向是否合法 	
  			}
  	   return false;
	 } 
}

int have_or_not(char role){
	for(int i=1;i<=8;i++) for(int j=1;j<=8;j++) if(checklegal(i,j,role)) return 1;
    return 0;
} 

bool board_dfs(int r,int c,int js,int count1,int count2,char role){
 		if(r<=0 || r>=9 || c<=0 || c>=9) return false;
 		r=r+dirrr,c=c+dircc;
	    if(qipan[count1][count2].cboard[r][c]==0) return false;
 		else if( role=='p' && qipan[count1][count2].cboard[r][c]==opp){
   			 js++;
   			 board_dfs(r,c,js,count1,count2,role);
 			}
 		else if( role=='c' && qipan[count1][count2].cboard[r][c]==me){
 			 js++;
             board_dfs(r,c,js,count1,count2,role);
 			}
		else if(js>=1) return true;
 		else return false;
}


bool board_endchecklegal(int r, int c,int count1,int count2,char role){//判断玩家想下的位置是否合法 
 		if(qipan[count1][count2].cboard[r][c]!=0) return false;
 		else{
  			for(int i=0;i<8;i++){
   					dirrr=dir_r[i];
  					dircc=dir_c[i];
   					if(board_dfs(r,c,0,count1,count2,role)) return true;//判断在x=dirrr、y=dircc方向是否合法 
                  }
  			return false;
 		} 
}


void turn_dfs(int r, int c,int js,char role){//翻棋
 
 pass=0;
 if( role=='p' && r>=1 && r<=8 && c>=1 && c<=8 && board[r+dirrr][c+dircc]!=0 && board[r+dirrr][c+dircc]==opp){
  js++;
  board[r+dirrr][c+dircc]=me;
  turn_dfs(r+dirrr,c+dircc,js,role);
  if(pass==0) board[r+dirrr][c+dircc]=opp;//回溯 
 }
 else if(role=='c' && r>=1 && r<=8 && c>=1 && c<=8 && board[r+dirrr][c+dircc]!=0 && board[r+dirrr][c+dircc]==me){
 	js++;
    board[r+dirrr][c+dircc]=opp;
    turn_dfs(r+dirrr,c+dircc,js,role);
    if(pass==0) board[r+dirrr][c+dircc]=me; 
 }
 else if(  (role=='p' && board[r+dirrr][c+dircc]==me && js>=1)||(role=='c' && board[r+dirrr][c+dircc]==opp && js>=1)) pass=1; 
}


void turn_chess(int r, int c,char role){//唯一区别在于调用函数不同 
 for(int i=0;i<8;i++){
  dirrr=dir_r[i];
  dircc=dir_c[i];
  turn_dfs(r,c,0,role);
 }
}


void AIfdfs(int r, int c,int js,int count1,int count2,int turn2){//AI模拟翻转 
 pass=0;
 if(turn2==0){
  if(r>=1 && r<=8 && c>=1 && c<=8 && qipan[count1][count2].cboard[r+dirrr][c+dircc]!=0 && qipan[count1][count2].cboard[r+dirrr][c+dircc]!=opp){
   js++;
   qipan[count1][count2].cboard[r+dirrr][c+dircc]=opp;
   AIfdfs(r+dirrr,c+dircc,js,count1,count2,turn2);
   if(pass==0) qipan[count1][count2].cboard[r+dirrr][c+dircc]=me;//变回去 
  }
  else if(qipan[count1][count2].cboard[r+dirrr][c+dircc]==opp && js>=1) pass=1; 
 }
 else{
  if(r>=1 && r<=8 && c>=1 && c<=8 && qipan[count1][count2].cboard[r+dirrr][c+dircc]!=0 && qipan[count1][count2].cboard[r+dirrr][c+dircc]!=me){
   js++;
   qipan[count1][count2].cboard[r+dirrr][c+dircc]=me;
   AIfdfs(r+dirrr,c+dircc,js,count1,count2,turn2);
   if(pass==0) qipan[count1][count2].cboard[r+dirrr][c+dircc]=opp;//变回去 
  }
  else if(qipan[count1][count2].cboard[r+dirrr][c+dircc]==me && js>=1) pass=1; 
 } 
}


void AIfturn(int r, int c,int count1,int count2,int turn2){
 for(int i=0;i<8;i++){
  dirrr=dir_r[i];
  dircc=dir_c[i];
  AIfdfs(r,c,0,count1,count2,turn2);
 }
}


int evaluate(int count1,int count2,int turn3){
 int sum=0;
 for(int i=1;i<=8;i++) 
  for(int j=1;j<=8;j++){
   if(qipan[count1][count2].cboard[i][j]==opp) sum=sum+s[i-1][j-1]; 
   if(turn3==0 && checklegal(i,j,'p')) sum=sum-5;
   else if(turn3==1 && checklegal(i,j,'c')) sum=sum-5; 
 }
 qipan[count1][count2].score=sum;
 return sum;
}

int minmax(int depth,int count1,int count2,int turn2){//待搜索深度，第count1层中的第count2状态，该turn2下 
         //返回count1，count2棋盘的评分

 int m=count2;
 if(depth==0) {
  return evaluate(count1,count2,turn2);//
 }
 
 else{  
  for(int i=1;i<=8;i++){
   for(int j=1;j<=8;j++){ 
    if(turn2==0 && board_endchecklegal(i,j,count1,m,'c')){ //以电脑为视角  0,0是有值的 
        
     qipan[count1][count2].score=-99999;  
     for(int o=1;o<=8;o++) 
      for(int w=1;w<=8;w++) 
       qipan[count1+1][count2].cboard[o][w]=qipan[count1][m].cboard[o][w];
              //将其上一个状态赋值给子状态 
        qipan[count1+1][count2].cboard[i][j]=opp;
        AIfturn(i,j,count1+1,count2,0);//在该棋局把该翻的翻了 
        
             
        qipan[count1][count2].score = max(qipan[count1][count2].score,minmax(depth-1,count1+1,count2,1));
      count2++;//每做完一次循环count2加一次，代表同搜索深度的不同状态 
    }
    
    else if(turn2==1 && board_endchecklegal(i,j,count1,m,'p')){
    qipan[count1][count2].score=100000;
    for(int o=1;o<=8;o++) 
     for(int w=1;w<=8;w++) 
      qipan[count1+1][count2].cboard[o][w]=qipan[count1][m].cboard[o][w];  
      qipan[count1+1][count2].cboard[i][j]=me;
      AIfturn(i,j,count1+1,count2,1);     
      qipan[count1][count2].score = min(qipan[count1][count2].score,minmax(depth-1,count1+1,count2,0));       
      count2++;   
    }
   } 
  }
  

  if(turn2==1){
   int min=10000;
   for(int l=0;l<=count2-1;l++){
    if(qipan[count1][l].score<min) min=qipan[count1][l].score;
   }
    return min;
  }
  else{
   int max=-10000;
   for(int l=0;l<=count2-1;l++){
    if(qipan[count1][l].score>max) max=qipan[count1][l].score;
   }
    return max;
   
  } 
 }
}

int main(){  

 int row,col;
 int chave=1,phave=1;//终局判断用的变量，与其他无关 
 board[4][4]=2;
 board[4][5]=1;
 board[5][4]=1;
 board[5][5]=2;
 
 cout<<"Please enter the colour you have chosen(1=black and 2=white):"<<endl; cin>>me;
 if(me==1) { opp=2;turn=1;} else {opp=1; turn=0; }
 printboard();
 
 while(1){ 
  	if(turn==1){//该人下
      	 phave=have_or_not('p');
      	 if(chave==0 && phave==0) { end_game();return 0; }//双方无子可下 
      	 if(phave==1){
        	    cout<<"Please enter row & col:"<<endl; 
        	    do{ cin>>row>>col;} while(checklegal(row,col,'p')==false);
       	     board[row][col]=me;
       	     turn_chess(row,col,'p');        
      	      printboard();  } 
    	   turn=0;
  	}
 	 if(turn==0){//该AI下 
  		 chave=have_or_not('c');
   	     if(chave==0 && phave==0) { end_game();return 0; }//双方无子可下 
  		 if(chave==1){
    	 for(int i=0;i<6;i++) for(int j=0;j<750000;j++) memset(qipan[i][j].cboard,0,sizeof(qipan[i][j].cboard));
    	 for(int i=1;i<=8;i++) for(int j=1;j<=8;j++) qipan[0][0].cboard[i][j]=board[i][j];//将目前棋盘状态设为根节点
   		 minmax(depth,0,0,0);
   		 int max=-1000,n=0;//找下个状态里score最大的,n为下一个状态里第n个棋局可能         
     	 for(int j=0;j<750000;j++){
        	 if( check_empty_board(1,j))  break;
      		 else if(qipan[1][j].score>max){
       				max=qipan[1][j].score;
      			    n=j;
      			}
     	} 
   		 for(int i=1;i<=8;i++) for(int j=1;j<=8;j++) board[i][j]=qipan[1][n].cboard[i][j];//将这个可能赋值给当前棋盘，代表电脑选择这么下 
         printboard();  
     } 
         turn=1;
   }
 }
 return 0;
}

