#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<windows.h>
#include<iomanip>
using namespace std;
struct node
{
	node* left;
	node* right;
	int info;
	float freq;
	char code[12];
};
struct node1
{
	int info;
	float freq;
	int pos;
};
void func(char *str,char *str1,char track)
{
	int i=0;
	while(*(str1+i)!='\0')
	{
		*(str+i)=*(str1+i);
		i++;
	}
	str[i]=track;
	str[i+1]='\0';
}
typedef struct node* nodeptr;
typedef struct node1* nodeptr1;
nodeptr root;
nodeptr1 root1;
int main()
{
	system("color 5f");
	int i,j;
	int n,temp=0,bpp=0;
 	int bmpsize=0,bmpdataoff=0;
	int width,height;
	cout<<"\n\n\n\n\n\n\n\n\n\n\n";
	cout<<setw(100)<<"WELCOME TO \n\n";
	cout<<setw(100)<<"COMPRESSION\n\n\n\n\n\n";
	cout<<"\t\t\t";
	system("pause");
	system("cls");
	FILE *p;
	p=fopen("lenagray.bmp","rb");
	if(p==NULL)
	{
		cout<<"the file doesn't exist"<<"\n";
		exit(1);
	}
	else
	{
		cout<<"\t\t\t\t\t\tTHE DETAILS OF THE IMAGE\n\n\n\n";
 		n=2;
		//size of bmp file
		fseek(p,n,SEEK_SET);
		fread(&bmpsize,4,1,p);
		cout<<"\n";
		cout<<"\t\t\t\tsize of bmp file\n";
		cout<<"\t\t\t\t"<<bmpsize;
		n=10;
		//location where pixel array starts
		fseek(p,n,SEEK_SET);
		fread(&bmpdataoff,4,1,p);
		cout<<"\n";
		cout<<"\t\t\t\tthe location where pixel starts\n";
		cout<<"\t\t\t\t"<<bmpdataoff;
		//getting width
		fseek(p,18,SEEK_SET);
		fread(&width,4,1,p);
		cout<<"\n";
		cout<<"\t\t\t\tthe width of the image\n";
		cout<<"\t\t\t\t"<<width;
		//getting height
		fseek(p,22,SEEK_SET);
		fread(&height,4,1,p);
		cout<<"\n";
		cout<<"\t\t\t\tthe height of the image\n";
		cout<<"\t\t\t\t"<<height;
  		//bits per pixel
		fseek(p,2,SEEK_CUR);
		fread(&bpp,2,1,p);
			cout<<"\n";
		cout<<"\t\t\t\tbits per pixel\n";
		cout<<"\t\t\t\t"<<bpp;
 		//n to start of pixel data
		fseek(p,bmpdataoff,SEEK_SET);
		//image array
		int **image=(int **)malloc(height*sizeof(int*));
		for(i=0;i<height;i++)
		{
			image[i]=(int*)malloc(width*sizeof(int));
		}
		cout<<"\n";
		cout<<"\t\t\t\tthe no.of pixels\n";
		cout<<"\t\t\t\t"<<height*width;
		//bmp file into image array
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				fread(&temp,3,1,p);
				temp=temp&0xFF;
				image[i][j]=temp;
			}
		}
		cout<<"\n\n";

		/*for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				cout<<image[i][j]<<" ";
			}
			cout<<"\n\n";
		}*/
		int hist[256];
		for(i=0;i<256;i++)
		{
			hist[i]=0;
		}
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				hist[image[i][j]]+=1;
			}
		}
		system("pause");
		system("cls");
		cout<<"\t\t\t\t\t\tTHE HISTOGRAM\n\n\n";
		for(i=0;i<256;i++)
		{
			cout<<i<<"->"<<hist[i]<<"\n";
		}
		//no of nodes
		int nodes=0;
		for(i=0;i<256;i++)
		{
			if(hist[i]!=0)
			{
				nodes+=1;
			}
		}
		cout<<"\nthe no.of nodes having non-zero probabilty\n";
		cout<<"\n"<<nodes;
		int total_nodes=2*nodes-1;
		root=(nodeptr)malloc(sizeof(struct node)*total_nodes);
		root1=(nodeptr1)malloc(sizeof(struct node1)*total_nodes);
		j=0;
		int tot=height*width;
		float temp;
		for(i=0;i<256;i++)
		{
			if(hist[i]!=0)
			{
				//initializing pixel value
				root[j].info=i;
				root1[j].info=i;
				//initializing probability value
				temp=(float)hist[i]/(float)tot;
				root[j].freq=temp;
				root1[j].freq=temp;
				//initializing left and right child as NULL
				root[j].left=NULL;
				root[j].right=NULL;
				//storing pos of node in node1
				root1[j].pos=j;
				//making code to \0
				root[i].code[0]='\0';
				j++;
			}
		}
		/*for(i=0;i<nodes;i++)
		{
			cout<<"\n";
			cout<<root[i].freq<<"  "<<root1[i].freq<<"\n";
			cout<<root[i].info<<"               "<<root1[i].pos<<"\n";
			cout<<"\n";
		}*/
		//sorting node1
		struct node1 t;
		float small;
		for(i=0;i<nodes;i++)
		{
			for(j=i+1;j<nodes;j++)
			{
				if(root1[i].freq<root1[j].freq)
				{
					t=root1[i];
					root1[i]=root1[j];
					root1[j]=t;
				}
			}
		}
		//the sorted node1
		/*cout<<"the sorted node1\n";
		for(i=0;i<nodes;i++)
		{
			cout<<root1[i].freq<<"\n";
			cout<<root1[i].pos<<"\n";
			cout<<"\n";
		}*/
		//huffman tree
		float sumfreq;
		int suminfo;
		int info;
		int n=0,k;
		int nodes1=nodes;
		while(n<nodes-1)
    	{
        	sumfreq = root1[nodes-n-1].freq+root1[nodes-n-2].freq;
        	suminfo = root1[nodes-n-1].info+root1[nodes-n-2].info;
       		root[nodes1].info = suminfo;
        	root[nodes1].freq = sumfreq;
        	root[nodes1].left = &root[root1[nodes-n-2].pos];
       		root[nodes1].right = &root[root1[nodes-n-1].pos];
        	root[nodes1].code[0] = '\0';
        	i=0;
        	while(sumfreq<=root1[i].freq)
              i++;
        	for(k=nodes;k>=0;k--)
        	{
            	if (k==i)
            	{
               		root1[k].info = suminfo;
               		root1[k].freq = sumfreq;
                	root1[k].pos = nodes1;
            	}
            	else if(k>i)
            	{
                	root1[k] = root1[k-1];
				}
        	}
        	n+=1;
       		nodes1+=1;
   		}
 		/*for(i=0;i<nodes1;i++)
		{
			cout<<"\n";
			cout<<root[i].freq<<"\n";
			cout<<root[i].info<<"\n";
			cout<<i<<"\n";
			cout<<"\n";
		}
		cout<<"the nodes1\n";
		cout<<"\n\n\n\n\n\n\n";
		cout<<nodes1;*/
		//assigning values to codes
		//assigning 0->left and 1->right
		char left='0';
		char right='1';
		for(i=nodes1-1;i>=nodes;i--)
		{
			if(root[i].left!=NULL)
			{
				func(root[i].left->code,root[i].code,left);
			}
			if(root[i].right!=NULL)
			{
				func(root[i].right->code,root[i].code,right);
			}
		}
		system("pause");
		system("cls");
		cout<<"\t\t\t\t\t\tCODE WORDS\n\n";
		for(i=0;i<nodes;i++)
		{
			cout<<root[i].info<<"     "<<root[i].code<<"\n";
		}
		cout<<"\n\n\n";
		cout<<"\t\t\t\tno.of bytes\n";
		int tot1=0;
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				for(int k=0;k<nodes;k++)
				{
					if(image[i][j]==root[k].info)
					{
						tot1+=strlen(root[k].code);
					}
				}
			}
		}
		cout<<"\n"<<(tot1/8)*3;
		//writng into a file
		ofstream f;
		f.open("huff.txt");
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				for(k=0;k<nodes;k++)
				{
					if(image[i][j]==root[k].info)
					{
						f<<root[k].code;
					}
				}
			}
		}
	}
	return 0;
}
