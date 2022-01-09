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
bool cmp(const pair<string,int> &p1,const pair<string,int> &p2)//Ҫ�ó�������Ȼ�������
{
return p1.second>p2.second;
}
pthread_mutex_t f_mutex;//�����ź���
FILE *fp1,*fp2;//�ļ�ָ��
int countt=0;//��ǰ��������
map<string,int> word_count;//map����
map<string,int> ::iterator iter;//map������
int main( )
{       int p_num;
	printf("������С˵ƪ��: ");
	scanf("%d",&p_num);
        int* filename=(int *)malloc(sizeof(int)*(p_num+1));
	pthread_t tid[100];//�߳�id����
	pthread_mutex_init(&f_mutex,NULL);//��ʼ���ٽ���
	for(int i=1; i<=p_num; i++)
        {       filename[i]=i;
		while(pthread_create(&tid[i],NULL,count,&filename[i])!=0);//���δ����߳�
	}
        for(int i=1; i<=p_num; i++)
		pthread_join(tid[i],NULL);//���������߳�,�����߳���Ҫ���̵߳Ľ��
	printf("total number of words:%d\n",countt);
	pthread_mutex_destroy(&f_mutex);
	//ͳ��top10�ȴ� 
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
void *count(void *filename)  //�������
{   int now=*(int *)filename;
	char frename[20];
	sprintf(frename,"%d.txt",now);
	FILE *fp=NULL;
	char ch;
	fp=fopen(frename,"r");
	string word="";
	ch=fgetc(fp);//�����ĵ�ǰ�ַ�
	char prevc='\0';//���������ǰһ���ַ�
	while(ch!=EOF) { //�����ļ�ĩβ
		if(!isalpha(ch)&&isalpha(prevc)) {//���ǰһ���ַ��Ƕ���ǰ�ַ����ǣ���˵��һ�����ʽ�����
                        pthread_mutex_lock(&f_mutex);
                        
			countt++;
			pthread_mutex_unlock(&f_mutex);
			
			iter=word_count.find(word);//iterΪword��map�е�λ��
			if(iter!=word_count.end()) {
				word_count[word]+=1;//��ǰ����Ƶ�ʼ�һ
				word="";
			} else {
				word_count.insert(pair<string,int>(word,1));//�����ȫ�µ��ʼ���map
				
			
                                    word="";
                               
			}
		}
		else if(isalpha(ch)&&isalpha(prevc))//��ǰ���ʻ�û�ж���
			word+=ch;
                else     if(isalpha(ch)&&!isalpha(prevc))//��ǰ���ʻ�û�ж���
			word+=ch;


		prevc=ch;     
                ch=fgetc(fp);//���Ŷ���һ���ַ�
           
	}
	fclose(fp);  
}
	

