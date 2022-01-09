#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <map>
#include <ctype.h>
using namespace std; 
void *count(void *filename);
bool cmp(const pair<string,int> &p1,const pair<string,int> &p2)//要用常数，不然编译错误
{
return p1.second>p2.second;
}
pthread_mutex_t f_mutex;//互斥信号量
FILE *fp1,*fp2;//文件指针
int countt=0;//当前单词总数
map<string,int> word_count;//map对象
map<string,int> ::iterator iter;//map迭代器
int main( )
{       int p_num;
	printf("请输入小说篇数: ");
	scanf("%d",&p_num);
        int* filename=(int *)malloc(sizeof(int)*(p_num+1));
	pthread_t tid[100];//线程id数组
	pthread_mutex_init(&f_mutex,NULL);//初始化临界区
	for(int i=1; i<=p_num; i++)
        {       filename[i]=i;
		while(pthread_create(&tid[i],NULL,count,&filename[i])!=0);//依次创建线程
	}
        for(int i=1; i<=p_num; i++)
		pthread_join(tid[i],NULL);//依次启动线程,且主线程需要子线程的结果
	printf("total number of words:%d\n",countt);
	pthread_mutex_destroy(&f_mutex);
	//统计top10热词 
	vector<pair<string,int> > arr;
	for (map<string,int>::iterator it=word_count.begin(); it!=word_count.end(); ++it) {
		//cout<<it->first<<'\t'<<it->second<<endl;
		arr.push_back(make_pair(it->first,it->second));
	}
	sort(arr.begin(),arr.end(),cmp);
        int number=0; 
	for (vector<pair<string,int> >::iterator it=arr.begin(); it!=arr.end(); ++it) {
		cout<<it->first<<'\t'<<it->second<<endl;
                number++;
                   if(number==11) break;
	}

	return 0;
}
void *count(void *filename)  //计算过程
{   int now=*(int *)filename;
	char frename[20];
	sprintf(frename,"%d.txt",now);
	FILE *fp=NULL;
	char ch;
	fp=fopen(frename,"r");
	string word="";
	ch=fgetc(fp);//读到的当前字符
	char prevc='\0';//定义读到的前一个字符
	while(ch!=EOF) { //读到文件末尾
		if(!isalpha(ch)&&isalpha(prevc)) {//如果前一个字符是而当前字符不是，则说明一个单词结束了
                        pthread_mutex_lock(&f_mutex);
                        
			countt++;
			pthread_mutex_unlock(&f_mutex);
			
			iter=word_count.find(word);//iter为word在map中的位置
			if(iter!=word_count.end()) {
				word_count[word]+=1;//当前单词频率加一
				word="";
			} else {
				word_count.insert(pair<string,int>(word,1));//将这个全新单词加入map
				
			
                                    word="";
                               
			}
		}
		else if(isalpha(ch)&&isalpha(prevc))//当前单词还没有读完
			word+=ch;
                else     if(isalpha(ch)&&!isalpha(prevc))//当前单词还没有读完
			word+=ch;


		prevc=ch;     
                ch=fgetc(fp);//接着读下一个字符
           
	}
	fclose(fp);  
}
	

